"""Build and package the Watch Pro 2 emulator with one host-native recipe."""

import argparse
import hashlib
import json
import os
import platform
import shutil
import socket
import struct
import subprocess
import sys
import tarfile
import tempfile
import time
from pathlib import Path
from urllib.request import urlopen

ROOT = Path(__file__).resolve().parent
QEMU = "qemu-9.2.0"
QEMU_SHA256 = "f859f0bc65e1f533d040bbe8c92bcfecee5af2c921a6687c652fb44d089bd894"
SDK_COMMIT = "26f51e584940d5b141cd1d09d2629d69b595f579"
QEMU_BUILD_WHEELS = (
    ("meson-1.5.0-py3-none-any.whl", "c4/cd/47e45d3abada2e1edb9e2ca9334be186d2e7f97a01b09b5b82799c4d7bd3",
     "52b34f4903b882df52ad0d533146d4b992c018ea77399f825579737672ae7b20"),
    ("pycotap-1.3.1-py3-none-any.whl", "36/f4/77b5a41d53ae3c530b204ed9a35137171b20f8ee0f337e2ec4993d6026a5",
     "1c3a25b3ff89e48f4e00f1f71dbbc1642b4f65c65d416524d07e73492fff25ea"),
)
JPEG = "application/bt_watch/simulator/LVGL.Simulator/framework_porting/jpeg"
ROM_SDK = (
    "framework/display/compression/lz4/lz4.c",
    "framework/display/compression/lz4/lz4.h",
    "zephyr/framework/display/sw_math.c",
    "zephyr/framework/include/display/sw_math.h",
)


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def run(*args, cwd=None):
    return subprocess.run(args, cwd=cwd, check=True)


def fetch(url, target, expected=None):
    if not target.is_file():
        pending = target.with_suffix(target.suffix + ".part")
        try:
            with urlopen(url, timeout=90) as response, pending.open("wb") as output:
                shutil.copyfileobj(response, output)
            if expected and digest(pending) != expected:
                raise ValueError(f"Download checksum mismatch: {url}")
            pending.replace(target)
        finally:
            pending.unlink(missing_ok=True)
    if expected and digest(target) != expected:
        raise ValueError(f"Checksum mismatch: {target}")
    return target


def sdk_file(root, path):
    if root:
        source = root / path
        if not source.is_file():
            raise ValueError(f"Missing Actions SDK source: {source}")
        return source
    raise ValueError("An Actions SDK source directory is required")


def prepare_sdk(work, component):
    root = work / f"actions-{SDK_COMMIT}"
    files = []
    if component in ("all", "qemu"):
        files += [f"{JPEG}/{name}" for name in ("tjpgd.c", "tjpgd.h", "tjpgdcnf.h")]
    if component in ("all", "rom"):
        files += ROM_SDK
    for name in files:
        target = root / name
        target.parent.mkdir(parents=True, exist_ok=True)
        fetch(f"https://raw.githubusercontent.com/lvgl/lv_port_actions_technology/"
              f"{SDK_COMMIT}/action_technology_sdk/{name}", target)
    return root


def append_once(path, marker, content):
    if marker not in path.read_text(encoding="utf-8"):
        with path.open("a", encoding="utf-8", newline="\n") as stream:
            stream.write(content)


def write_if_changed(path, contents):
    if not path.is_file() or path.read_bytes() != contents:
        path.write_bytes(contents)


def texture_index(faces):
    if faces is None:
        return b"/* No optional WFC fixture index. */\n"
    sys.path.insert(0, str(ROOT.parent / "tooling/src"))
    from fwkit import lz4, wfdl

    textures = set()
    for face in faces.glob("*_local*.wfc"):
        for entry in wfdl.parse(face.read_bytes()).resources:
            if entry.cf == 5:
                pixels = lz4.decompress(entry.data, entry.raw_size)
                if len(pixels) != entry.w * entry.h * 3:
                    raise ValueError(f"Invalid WFC texture in {face}")
                textures.add((entry.w, entry.h, hashlib.sha256(pixels).hexdigest()))
    if not textures:
        raise ValueError(f"No matching WFC textures in {faces}")
    return "".join(f'    {{{w}, {h}, "{sha}"}},\n' for w, h, sha in sorted(textures)).encode()


def integrate(source, sdk, faces):
    arm = source / "hw/arm"
    mkvenv = source / "python/scripts/mkvenv.py"
    original = mkvenv.read_bytes()
    fixed = original.replace(b'f"file://{str(wheels_dir)}"', b"Path(wheels_dir).resolve().as_uri()")
    write_if_changed(mkvenv, fixed)
    jpeg = arm / "wp2-jpeg"
    jpeg.mkdir(exist_ok=True)
    for path in (ROOT / "hw").iterdir():
        write_if_changed(arm / path.name, path.read_bytes())
    for name in ("tjpgd.c", "tjpgd.h", "tjpgdcnf.h"):
        write_if_changed(jpeg / name, sdk_file(sdk, f"{JPEG}/{name}").read_bytes())
    write_if_changed(arm / "wp2_wfc_index.h", texture_index(faces))
    append_once(arm / "meson.build", "files('ats3089c.c'",
                "\narm_ss.add(when: 'CONFIG_ATS3089C', "
                "if_true: files('ats3089c.c', 'wp2-jpeg/tjpgd.c'))\n")
    append_once(arm / "Kconfig", "config ATS3089C",
                "\nconfig ATS3089C\n    bool\n    default y\n"
                "    depends on TCG && ARM\n    select ARM_V7M\n")
    write_if_changed(source / "configs/devices/arm-softmmu/wp2.mak", b"CONFIG_ATS3089C=y\n")


def extract_qemu(archive, work):
    source = work / QEMU
    if not source.exists():
        with tempfile.TemporaryDirectory(dir=work) as scratch, tarfile.open(archive) as contents:
            def filter_member(member, destination):
                if member.name == f"{QEMU}/roms/edk2/EmulatorPkg/Unix/Host/X11IncludeHack":
                    return None
                return tarfile.data_filter(member, destination)

            contents.extractall(scratch, filter=filter_member)
            (Path(scratch) / QEMU).replace(source)
    return source


def smoke(binary, work):
    with tempfile.TemporaryDirectory(dir=work) as scratch, socket.socket() as listener:
        scratch = Path(scratch)
        flash = bytearray(0x400000)
        struct.pack_into("<II4HI", flash, 0x24200, 0x2ff70000, 0x10000209,
                         0x202a, 0x4901, 0x6008, 0xe7fe, 0x2ff60400)
        (scratch / "nor.bin").write_bytes(flash)
        (scratch / "nand-id.bin").write_bytes(bytes(790))
        (scratch / "rom.bin").write_bytes(bytes(0x10000))
        listener.bind(("127.0.0.1", 0))
        listener.listen(1)
        listener.settimeout(12)
        env = {key: value for key, value in os.environ.items()
               if not key.upper().startswith("QEMU_WP2_")}
        env["QEMU_WP2_NOR"] = str(scratch / "nor.bin")
        process = subprocess.Popen([
            str(binary), "-M", "ats3089c-wp2", "-bios", str(scratch / "rom.bin"),
            "-display", "none", "-serial", "null", "-monitor", "none", "-S",
            "-qmp", f"tcp:127.0.0.1:{listener.getsockname()[1]}",
        ], env=env)
        try:
            connection, _ = listener.accept()
            with connection, connection.makefile("rwb", buffering=0) as stream:
                connection.settimeout(6)
                json.loads(stream.readline())

                def request(command, **arguments):
                    stream.write(json.dumps({"execute": command, "arguments": arguments}).encode() + b"\n")
                    while True:
                        line = stream.readline()
                        if not line:
                            raise EOFError("QEMU closed the monitor")
                        reply = json.loads(line)
                        if "error" in reply:
                            raise RuntimeError(reply["error"])
                        if "return" in reply:
                            return reply["return"]

                request("qmp_capabilities")
                request("cont")
                deadline = time.monotonic() + 5
                while time.monotonic() < deadline:
                    result = request("human-monitor-command", **{"command-line": "xp /1wx 0x2ff60400"})
                    if "0x0000002a" in result:
                        break
                    time.sleep(0.05)
                else:
                    raise RuntimeError("Synthetic ROM handoff did not execute the ARM guest")
                try:
                    request("quit")
                except (EOFError, ConnectionResetError, BrokenPipeError):
                    pass
            process.wait(timeout=6)
            if process.returncode != 0:
                raise RuntimeError(f"Synthetic boot exited with {process.returncode}")
        finally:
            if process.poll() is None:
                process.kill()
                process.wait()
    print("Watch Pro 2 synthetic boot passed")


def build_qemu(args, sdk, output):
    work = args.work_dir
    archive = args.qemu_archive
    if archive:
        if digest(archive) != QEMU_SHA256:
            raise ValueError("Unexpected QEMU source archive checksum")
    else:
        archive = fetch(f"https://download.qemu.org/{QEMU}.tar.xz",
                        work / f"{QEMU}.tar.xz", QEMU_SHA256)
    source = extract_qemu(archive, work)
    integrate(source, sdk, args.wfc_dir)
    build = work / "qemu-build"
    build.mkdir(exist_ok=True)
    library_paths = []
    if sys.platform == "darwin" and (brew := shutil.which("brew")):
        dtc = Path(subprocess.check_output([brew, "--prefix", "dtc"], text=True).strip())
        if (dtc / "include/libfdt.h").is_file():
            library_paths = [f"--extra-cflags=-I{dtc / 'include'}",
                             f"--extra-ldflags=-L{dtc / 'lib'}"]
    configure = [
        "--target-list=arm-softmmu", "--with-devices-arm=wp2", "--enable-tcg",
        "--without-default-features", "--without-default-devices",
        "--enable-fdt=system", "--disable-werror", "--disable-docs",
        "--disable-tools", "--disable-guest-agent", "--disable-plugins",
        *[f"--enable-{backend}" for backend in args.audio], *library_paths, *args.configure_arg,
    ]
    stamp = build / "recipe.json"
    recipe = json.dumps(configure)
    if (build / "build.ninja").is_file() and (not stamp.is_file() or stamp.read_text() != recipe):
        raise ValueError("Configure options changed. Select a fresh --work-dir.")
    if not (build / "build.ninja").is_file():
        wheels = source / "python/wheels"
        wheels.mkdir(exist_ok=True)
        for filename, location, checksum in QEMU_BUILD_WHEELS:
            fetch(f"https://files.pythonhosted.org/packages/{location}/{filename}", wheels / filename, checksum)
        bash = os.environ.get("WP2_BUILD_BASH") or shutil.which("bash")
        if not bash:
            raise ValueError("QEMU configure requires Bash. On Windows use the MSYS2 UCRT64 shell.")
        run(bash, "../" + QEMU + "/configure", *configure, cwd=build)
        stamp.write_text(recipe, encoding="utf-8")
    binary = "qemu-system-arm" + (".exe" if os.name == "nt" else "")
    run("ninja", "-j", str(args.jobs), binary, cwd=build)
    shutil.copy2(build / binary, output / binary)
    run(str(output / binary), "--version")
    machines = subprocess.check_output([str(output / binary), "-machine", "help"], text=True)
    if "ats3089c-wp2" not in machines:
        raise ValueError("Built QEMU is missing the Watch Pro 2 machine")
    smoke(output / binary, work)
    for license_file in ("COPYING", "COPYING.LIB", "LICENSE"):
        shutil.copy2(source / license_file, output / f"qemu-{license_file}")
    if args.source_bundle:
        with tarfile.open(args.output_dir / "cmf-emulator-qemu-source.tar.gz", "w:gz") as bundle:
            bundle.add(source, arcname=QEMU,
                       filter=lambda member: None if member.name == f"{QEMU}/python/wheels" else member)


def build_rom(args, sdk, output):
    compiler = args.arm_prefix + "gcc"
    objcopy = args.arm_prefix + "objcopy"
    common = ["-mcpu=cortex-m33", "-mthumb", "-Os", "-ffunction-sections", "-fdata-sections"]
    elf = args.work_dir / "compat.elf"
    raw = args.work_dir / "compat-offset.bin"
    run(compiler, *common, "-mfloat-abi=soft", "-DNDEBUG", "-nostartfiles",
        "--specs=nosys.specs", "-T", str(ROOT / "rom/compat.ld"), "-Wl,--gc-sections",
        "-I", str(sdk / "zephyr/framework/include"), str(ROOT / "rom/compat.c"),
        str(ROOT / "rom/format.c"), str(sdk_file(sdk, ROM_SDK[0])),
        str(sdk_file(sdk, ROM_SDK[2])), "-o", str(elf))
    run(objcopy, "-O", "binary", "--gap-fill=0", "--pad-to=0x10000", str(elf), str(raw))
    rom = bytes(0x140) + raw.read_bytes()
    if len(rom) != 0x10000:
        raise ValueError("Unexpected compatibility ROM size")
    (output / "compat.bin").write_bytes(rom)
    reader = args.work_dir / "read-ota-temp.elf"
    run(compiler, *common, "-ffreestanding", "-nostdlib", "-Wl,-Ttext=0x20000000",
        "-Wl,-e,read_ota_temp", str(ROOT / "rom/read_ota_temp.c"), "-o", str(reader))
    shutil.copy2(reader, output / reader.name)
    run(objcopy, "-O", "binary", "-j", ".text", str(reader), str(output / "read-ota-temp.bin"))
    provisioner = args.work_dir / "provision-nand.elf"
    run(compiler, *common, "-ffreestanding", "-nostdlib", "-Wl,-Ttext=0x20000000",
        "-Wl,--section-start=.ARM.exidx=0x20000800", "-Wl,-e,provision",
        str(ROOT / "rom/provision_nand.c"), "-o", str(provisioner))
    shutil.copy2(provisioner, output / provisioner.name)
    run(objcopy, "-O", "binary", "-j", ".text", str(provisioner), str(output / "provision-nand.bin"))
    if args.source_bundle:
        with tarfile.open(args.output_dir / "cmf-emulator-rom-source.tar.gz", "w:gz") as bundle:
            bundle.add(ROOT / "rom", arcname="rom")
            for path in ROM_SDK:
                bundle.add(sdk_file(sdk, path), arcname=f"sdk/{path}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--component", choices=("all", "qemu", "rom"), default="all")
    parser.add_argument("--work-dir", type=Path, default=ROOT / "build")
    parser.add_argument("--output-dir", type=Path, default=ROOT / "dist")
    parser.add_argument("--sdk-root", type=Path, help="Local action_technology_sdk directory")
    parser.add_argument("--qemu-archive", type=Path, help="Local pinned QEMU source archive")
    parser.add_argument("--wfc-dir", type=Path, help="Optional local stock watch-face files")
    parser.add_argument("--audio", action="append", choices=("dsound", "coreaudio", "pa", "alsa"), default=[])
    parser.add_argument("--configure-arg", action="append", default=[])
    parser.add_argument("--arm-prefix", default="arm-none-eabi-")
    parser.add_argument("--jobs", type=int, default=os.cpu_count() or 1)
    parser.add_argument("--source-bundle", action="store_true")
    args = parser.parse_args()
    for attr in ("work_dir", "output_dir", "sdk_root", "qemu_archive", "wfc_dir"):
        path = getattr(args, attr)
        if path is not None:
            setattr(args, attr, path.resolve())
    if args.source_bundle and args.wfc_dir:
        parser.error("Source bundles must exclude the local WFC index")
    args.work_dir.mkdir(parents=True, exist_ok=True)
    args.output_dir.mkdir(parents=True, exist_ok=True)
    sdk = args.sdk_root or prepare_sdk(args.work_dir, args.component)
    machine = platform.machine().lower()
    arch = {"amd64": "x64", "x86_64": "x64", "aarch64": "arm64"}.get(machine, machine)
    host_os = {"darwin": "macos"}.get(platform.system().lower(), platform.system().lower())
    host = f"{host_os}-{arch}"
    name = f"cmf-emulator-{host}" if args.component != "rom" else "cmf-emulator-wp2-rom"
    output = args.work_dir / f"{name}-{args.component}"
    output.mkdir(exist_ok=True)
    if args.component in ("all", "qemu"):
        build_qemu(args, sdk, output)
    if args.component in ("all", "rom"):
        build_rom(args, sdk, output)
    shutil.copy2(ROOT / "README.md", output / "README.md")
    (output / "build.json").write_text(json.dumps({
        "component": args.component, "host": host, "machine": "ats3089c-wp2",
        "qemu": QEMU, "qemu_source_sha256": QEMU_SHA256,
        "sdk": SDK_COMMIT if not args.sdk_root else "local override",
        "audio": args.audio, "configure_overrides": args.configure_arg,
        "local_wfc_index": args.wfc_dir is not None,
    }, indent=2) + "\n", encoding="utf-8")
    archive = args.output_dir / (name + ".tar.gz")
    with tarfile.open(archive, "w:gz") as bundle:
        for path in sorted(output.iterdir()):
            bundle.add(path, arcname=f"{name}/{path.name}")
    print(archive)


if __name__ == "__main__":
    main()
