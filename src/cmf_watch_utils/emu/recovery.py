import hashlib
import shutil
import socket
import subprocess
import time

from .image import inner_payload, require
from .vm import Qmp, child, executable, runtime_env


def nvram_crc(data):
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ (0x8c if crc & 1 else 0)
    return crc


def pending_records(nor):
    marker = b"REC_OTA_FLAG\0yes\0"
    for segment in (0x3e6000, 0x3e7000):
        header = nor[segment:segment + 16]
        if (header[:5] != b"NVSR\xff" or header[9:12] != b"\x10\0\x10" or
                nvram_crc(header[6:]) != header[5]):
            continue
        for offset in range(segment + 16, segment + 4096 - 7 - len(marker), 16):
            header = nor[offset:offset + 8]
            if (header[:2] == b"I\xff" and header[5:8] == b"\x0d\x04\0" and
                    nor[offset + 8:offset + 8 + len(marker)] == marker and
                    nvram_crc(header[3:] + marker) == header[2]):
                yield offset


def capture_reset(port, target, size, timeout):
    with socket.create_connection(("127.0.0.1", port), timeout=10) as conn:
        conn.settimeout(timeout)
        with conn.makefile("rwb", buffering=0) as stream:
            qmp = Qmp(stream)
            while True:
                event = qmp.receive()
                if (event.get("event") in ("RESET", "SHUTDOWN") and
                        event.get("data", {}).get("reason") == "guest-reset"):
                    break
            qmp.request("stop")
            reason = qmp.request("human-monitor-command", **{"command-line": "xp /1wx 0x4000c03c"})
            require("0x42520002" in reason, "Guest reset was not an OTA reboot")
            qmp.request("pmemsave", val=0x11000000, size=size, filename=str(target))
            qmp.request("quit")


def read_temp(args, profile, nor, nand, nand_id, output):
    model = profile.model
    require(args.rom and args.reader and args.gdb, "Raw NAND requires --rom, --reader and --gdb")
    require(nand.is_file() and nand.stat().st_size == model.nand_bytes, "Invalid staged raw NAND")
    elf = args.reader / "read-ota-temp.elf"
    code = args.reader / "read-ota-temp.bin"
    require(elf.is_file() and code.is_file(), "NAND reader ELF and BIN are required")
    kernel = output / "stock-app.bin"
    kernel.write_bytes(profile.stock_app(nor.read_bytes()))
    reader_nor = output / "reader-nor.bin"
    shutil.copyfile(nor, reader_nor)
    shutil.copyfile(nand_id, output / "nand-id.bin")
    placeholder = output / "placeholder.bin"
    placeholder.write_bytes(bytes(1))
    target = output / "fw0_temp.bin"
    env = runtime_env(args, model, NOR=reader_nor, NAND=nand,
                      NAND_EXPORT=output / "unused-nand-export.bin",
                      NAND_PROGRAM=code, NAND_STAGING=placeholder)
    with socket.socket() as port:
        port.bind(("127.0.0.1", 0))
        address = f"127.0.0.1:{port.getsockname()[1]}"
    argv = [executable(args.qemu, env), "-M", model.machine, "-kernel", str(kernel),
            "-bios", str(args.rom), "-display", "none", "-monitor", "none",
            "-serial", f"file:{output / 'reader-uart.log'}", "-serial", "null", "-serial", "null",
            "-icount", "shift=3,align=off", "-S", "-gdb", f"tcp:{address}", "-no-reboot"]
    gdb = [executable(args.gdb, env), "-batch", str(elf)]
    for line in (f"target remote {address}", "break *0x100fa6f8", "continue",
                 "set $pc = read_ota_temp", "break *stage_ready", "continue",
                 'printf "TEMP read status: %d\n", $r0',
                 f"dump binary memory {target.name} 0x20010000 {0x20010000 + model.temp.size:#x}"):
        gdb += ["-ex", line]
    with child(argv, env, output / "reader-qemu.log") as process, (output / "reader-gdb.log").open("wb") as log:
        time.sleep(0.7)
        require(process.poll() is None, "QEMU exited before NAND reader could attach")
        subprocess.run(gdb, cwd=output, env=env, stdout=log, stderr=log, check=True, timeout=100)
    require("TEMP read status: 0" in (output / "reader-gdb.log").read_text(errors="replace")
            and target.is_file() and target.stat().st_size == model.temp.size, "Incomplete TEMP read")
    return target


def recover(args, profile):
    model = profile.model
    stock, _, nand_id = profile.fixtures(args.fixtures)
    rescue = args.crown_held and args.charger_connected
    require(rescue or not (args.crown_held or args.charger_connected), "Rescue requires both crown and charger")
    require(rescue != bool(args.pending_temp or args.pending_nand),
            "Choose either crown + charger rescue or a pending OTA")
    require(not args.qmp_port or args.pending_nand, "--qmp-port requires --pending-nand")
    output = args.output
    output.mkdir(parents=True, exist_ok=False)
    source = args.nor
    if args.qmp_port:
        source = output / "nor-staged.bin"
        capture_reset(args.qmp_port, source, model.nor_bytes, args.reset_timeout)
    nor = bytearray(source.read_bytes())
    require(len(nor) == model.nor_bytes, "Invalid virtual NOR size")
    pending = list(pending_records(nor))
    if rescue:
        base = stock.read_bytes()
        app, resources = profile.stock_app(base), model.sdfs.read(base)
        mode = "modeled_crown_charger_rescue"
    else:
        require(pending, "No valid pending OTA marker in virtual NOR")
        temp = args.pending_temp or read_temp(args, profile, source, args.pending_nand, nand_id, output)
        require(0 < temp.stat().st_size <= model.temp.size, "Invalid staged TEMP size")
        app, resources = inner_payload(temp.read_bytes(), model, allow_padding=True)
        mode = "modeled_pending_ota_install"
    for offset in pending:
        nor[offset + 1] = 0x5a
    model.app.stage(nor, app)
    model.sdfs.stage(nor, resources)
    shutil.copyfile(nand_id, output / "nand-id.bin")
    (output / "nor.bin").write_bytes(nor)
    return {"mode": mode, "app_sha256": hashlib.sha256(app).hexdigest(),
            "sdfs_sha256": hashlib.sha256(resources).hexdigest(),
            "pending_records_cleared": len(pending), "nor": str(output / "nor.bin")}
