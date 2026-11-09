from contextlib import contextmanager
from importlib.resources import as_file, files
from pathlib import Path
import json
import os
import re
import shutil
import socket
import subprocess
import time

from .image import require, verify_package


def executable(value, env=None):
    resolved = shutil.which(str(value), path=env.get("PATH") if env is not None else None)
    if resolved is None:
        raise ValueError(f"Executable not found: {value}")
    return str(Path(resolved).resolve())


def runtime_env(args, model, **values):
    env = {key: value for key, value in os.environ.items()
           if not key.upper().startswith(model.env_prefix)}
    if args.runtime_dir:
        env["PATH"] = str(args.runtime_dir) + os.pathsep + env.get("PATH", "")
        for variable, name in (("TCL_LIBRARY", "tcl8.6"), ("TK_LIBRARY", "tk8.6")):
            library = args.runtime_dir.parent / "lib" / name
            if library.is_dir():
                env[variable] = str(library)
    env.update({model.env_prefix + key: str(value) for key, value in values.items()})
    return env


def command(args, model, output, env):
    require(args.rom.is_file(), f"Compatibility ROM not found: {args.rom}")
    argv = [executable(args.qemu, env), "-M", model.machine, "-bios", str(args.rom),
            "-display", "none", "-monitor", "none", "-serial", f"file:{output / 'uart0.log'}",
            "-serial", "null", "-serial", "null", "-icount",
            "shift=3,align=off" if args.headless or not args.audio else "shift=3,align=on"]
    if args.hci_port:
        argv += ["-chardev", f"socket,id=wp2-hci,host=127.0.0.1,port={args.hci_port}"]
    if not args.allow_reboot:
        argv.append("-no-reboot")
    return argv


@contextmanager
def child(argv, env, log):
    with log.open("wb") as output:
        process = subprocess.Popen(argv, env=env, stdout=output, stderr=output,
                                   creationflags=getattr(subprocess, "CREATE_NO_WINDOW", 0))
        try:
            yield process
        finally:
            if process.poll() is None:
                process.terminate()
                try:
                    process.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    process.kill()
                    process.wait()


class Qmp:
    def __init__(self, stream):
        self.stream = stream
        greeting = self.receive()
        require("QMP" in greeting, "Invalid QMP greeting")
        self.request("qmp_capabilities")

    def receive(self):
        line = self.stream.readline()
        if not line:
            raise EOFError("QEMU disconnected")
        return json.loads(line)

    def request(self, name, **args):
        self.stream.write(json.dumps({"execute": name, "arguments": args}).encode() + b"\n")
        while True:
            reply = self.receive()
            if "error" in reply:
                raise RuntimeError(reply["error"])
            if "return" in reply:
                return reply["return"]


def headless(args, argv, env, output):
    with socket.socket() as listener:
        listener.bind(("127.0.0.1", 0))
        listener.listen(1)
        listener.settimeout(0.25)
        argv += ["-qmp", f"tcp:127.0.0.1:{listener.getsockname()[1]}", "-S"]
        with child(argv, env, output / "qemu.log") as process:
            deadline = time.monotonic() + 15
            while True:
                try:
                    conn, _ = listener.accept()
                    break
                except TimeoutError:
                    require(process.poll() is None and time.monotonic() < deadline,
                            f"QEMU did not connect to the monitor. See {output / 'qemu.log'} "
                            "and check the host runtime libraries.")
            with conn, conn.makefile("rwb", buffering=0) as stream:
                conn.settimeout(8)
                qmp = Qmp(stream)
                qmp.request("cont")
                deadline = time.monotonic() + args.seconds
                while process.poll() is None and time.monotonic() < deadline:
                    time.sleep(0.1)
                require(process.poll() is None, "QEMU stopped during the boot observation")
                qmp.request("stop")
                registers = qmp.request("human-monitor-command", **{"command-line": "info registers"})
                qmp.request("quit")
                require(process.wait(timeout=8) == 0, "QEMU exited with an error")
    return registers


def monitored(args, argv, env, output):
    with child(argv, env, output / "qemu.log") as process:
        try:
            status = process.wait(timeout=args.seconds)
        except subprocess.TimeoutExpired as error:
            raise RuntimeError("The external QMP session did not finish before --seconds elapsed") from error
        require(status == 0, f"QEMU exited with status {status}")


def live(args, argv, env, output, version, stock, title):
    default_wish = "wish.exe" if os.name == "nt" else "wish"
    wish = args.wish or shutil.which(default_wish, path=env.get("PATH"))
    require(wish is not None, "Tk wish not found; provide --wish or use --headless")
    with as_file(files(__package__).joinpath("watch_window.tcl")) as script:
        viewer = [executable(wish, env), str(script), str(output), "stock" if stock else "custom", version, title]
        with child(argv, env, output / "qemu.log") as vm, child(viewer, env, output / "viewer.log") as gui:
            while vm.poll() is None and gui.poll() is None:
                time.sleep(0.2)
            if vm.poll() is not None:
                (output / "stopped").touch()
                raise RuntimeError(f"QEMU stopped; see {output / 'qemu.log'}")
            require(gui.returncode == 0, f"Viewer failed; see {output / 'viewer.log'}")


def run(args, profile):
    model = profile.model
    base, default_nand, nand_id = profile.fixtures(args.fixtures)
    nand = args.nand or default_nand
    require(nand.is_file() and nand.stat().st_size == model.nand_bytes, "Invalid raw NAND")
    require(not args.sdfs or args.app, "--sdfs requires --app")
    require(not args.ota_snapshot or args.allow_reboot, "--ota-snapshot requires --allow-reboot")
    require(not args.wav or args.headless, "--wav requires --headless")
    require(not args.audio or not args.headless, "--audio is for live runs; use --wav for headless")
    stock = not (args.app or args.ota_image or args.nor)
    data = bytearray((args.nor or base).read_bytes())
    require(len(data) == model.nor_bytes, "Invalid virtual NOR size")
    if args.ota_image:
        app, resources, _ = verify_package(args.ota_image.read_bytes(), model)
        model.app.stage(data, app)
        model.sdfs.stage(data, resources)
    elif args.app:
        app = args.app.read_bytes()
        model.validate_app(app)
        model.app.stage(data, app)
        if args.sdfs:
            model.sdfs.stage(data, args.sdfs.read_bytes())
    elif stock:
        app = profile.stock_app(data)
    else:
        app = model.app.read(data)
        model.validate_app(app)
    stamp = app[64:128].split(b"\0", 1)[0]
    version = profile.stock_version if stock else (
        stamp.decode("ascii") if re.fullmatch(rb"[A-Za-z0-9._+-]{1,63}", stamp) else "Unknown version")
    output = args.output
    output.mkdir(parents=True, exist_ok=False)
    nor = output / "nor.bin"
    nor.write_bytes(data)
    shutil.copyfile(nand_id, output / "nand-id.bin")
    (output / "input.txt").touch()
    env = runtime_env(args, model, NOR=nor, NAND=nand, FRAME=output / "frame.ppm",
                      INPUT=output / "input.txt", UNPLUGGED=int(not args.charger_connected),
                      HOST_RTC=1, EXPLORATORY=0)
    argv = command(args, model, output, env)
    if not args.headless:
        env[model.env_prefix + "LIVE_FRAME"] = "1"
        env[model.env_prefix + "LIVE_SENSORS"] = "1"
        if stock and profile.live_health:
            env[model.env_prefix + "LIVE_HEALTH"] = "1"
            env[model.env_prefix + "STOCK_AWAKE"] = "1"
            kernel = output / "app.bin"
            kernel.write_bytes(app)
            argv += ["-kernel", str(kernel)]
    if args.qmp_port:
        argv += ["-qmp", f"tcp:127.0.0.1:{args.qmp_port},server=on,wait=off"]
    if args.nand_export:
        env[model.env_prefix + "NAND_EXPORT"] = str(output / "nand-export.bin")
    if args.ota_snapshot:
        env[model.env_prefix + "OTA_NAND_SNAPSHOT"] = str(output / "nand-staged.bin")
    if args.wav or args.audio:
        env[model.env_prefix + "AUDIODEV"] = "wp2-audio"
        driver = f"wav,id=wp2-audio,path={str(output / 'audio.wav').replace(',', ',,')}" if args.wav else f"{args.audio},id=wp2-audio"
        argv += ["-audiodev", driver]

    report = {"profile": profile.name, "version": version, "bootloader": "compatibility_rom"}
    try:
        if args.headless:
            if args.qmp_port:
                monitored(args, argv, env, output)
            else:
                report["registers"] = headless(args, argv, env, output)
            uart = (output / "uart0.log").read_text(errors="replace")
            log = (output / "qemu.log").read_text(errors="replace")
            require("ZEPHYR FATAL ERROR" not in uart and "unsupported API" not in log
                    and "stopped on unimplemented" not in log, "Guest hit a fatal or unsupported path")
            if not args.allow_reboot:
                require(uart.count("*** Booting Zephyr OS") <= 1, "Guest restarted during the boot observation")
            if args.ota_snapshot:
                require((output / "nand-staged.bin").is_file(), "Guest did not export OTA NAND on reset")
            require((output / "frame.ppm").is_file(), "Guest did not render a panel frame")
        else:
            print(f"Run directory {output}", flush=True)
            live(args, argv, env, output, version, stock, model.title)
        report["outcome"] = "completed"
    except (OSError, ValueError, RuntimeError, EOFError, subprocess.SubprocessError) as error:
        report.update(outcome="failed", error=str(error))
        raise
    finally:
        report["panel_frame_captured"] = (output / "frame.ppm").is_file()
        (output / "report.json").write_text(json.dumps(report, indent=2), encoding="utf-8")
    return report
