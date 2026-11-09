import argparse
import asyncio
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import xml.etree.ElementTree as ET
import lzma

from . import actions
from .emu import image, prepare, recovery, vm
from .emu.profiles import PROFILES


def absolute(value):
    return Path(value).expanduser().resolve()


def positive(value):
    result = float(value)
    if not 0 < result < float("inf"):
        raise argparse.ArgumentTypeError("expected a finite positive number")
    return result


def delay(value):
    result = float(value)
    if not 0 <= result < float("inf"):
        raise argparse.ArgumentTypeError("expected a finite nonnegative number")
    return result


def port(value):
    result = int(value)
    if not 1 <= result <= 65535:
        raise argparse.ArgumentTypeError("expected a TCP port from 1 to 65535")
    return result


def emit(result, output=None, display=None):
    if output:
        output.parent.mkdir(parents=True, exist_ok=True)
        with output.open("x", encoding="utf-8") as handle:
            handle.write(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result if display is None else display, indent=2))


def cmd_profiles(args):
    emit([{"profile": profile.name, "model": profile.model.name, "title": profile.model.title,
           "board": profile.model.board, "machine": profile.model.machine,
           "stock_version": profile.stock_version, "nand_fixture": profile.nand_filename}
          for profile in PROFILES.values()])


def cmd_fixtures(args):
    profile = PROFILES[args.profile]
    for source, size in ((args.nor, profile.model.nor_bytes), (args.nand, profile.model.nand_bytes),
                         (args.nand_id, profile.model.nand_id_bytes)):
        image.require(source.is_file() and source.stat().st_size == size, f"Expected {size} bytes at {source}")
    profile.stock_app(args.nor.read_bytes())
    args.fixtures.mkdir(parents=True, exist_ok=True)
    targets = [(args.nor, "nor.bin"), (args.nand_id, "nand-id.bin"), (args.nand, profile.nand_filename)]
    for source, name in targets:
        target = args.fixtures / name
        if target.exists():
            with source.open("rb") as given, target.open("rb") as installed:
                image.require(hashlib.file_digest(given, "sha256").digest() ==
                              hashlib.file_digest(installed, "sha256").digest(), f"Fixture already exists: {target}")
    for source, name in targets:
        target = args.fixtures / name
        if not target.exists():
            with source.open("rb") as given, target.open("xb") as installed:
                try:
                    shutil.copyfileobj(given, installed)
                    installed.flush()
                except BaseException:
                    installed.close()
                    target.unlink(missing_ok=True)
                    raise
    emit({"profile": profile.name, "fixtures": str(args.fixtures)})


def cmd_doctor(args):
    profile = PROFILES[args.profile]
    env = vm.runtime_env(args, profile.model)
    qemu = vm.executable(args.qemu, env)
    process = subprocess.run([qemu, "-M", "help"], capture_output=True,
                             text=True, env=env, timeout=10)
    image.require(process.returncode == 0, f"QEMU could not start (exit {process.returncode}). "
                  f"Check its host runtime libraries and --runtime-dir. {process.stderr.strip()}")
    image.require(any(line.split()[:1] == [profile.model.machine] for line in process.stdout.splitlines()),
                  f"QEMU does not include {profile.model.machine}")
    image.require(args.rom.is_file(), f"Compatibility ROM not found: {args.rom}")
    if args.fixtures:
        profile.fixtures(args.fixtures)
    emit({"profile": profile.name, "machine": profile.model.machine,
          "fixtures": str(args.fixtures) if args.fixtures else None, "qemu": qemu, "rom": str(args.rom),
          "viewer_available": bool(shutil.which(args.wish or "wish", path=env.get("PATH")))})


def cmd_prepare(args):
    emit(prepare.prepare(args, PROFILES[args.profile]))


def cmd_pair_fixture(args):
    from .emu.pairing import pair_fixture

    emit(pair_fixture(args, PROFILES[args.profile]))


def cmd_preflight(args):
    emit(image.report(args.image.read_bytes(), PROFILES[args.profile].model), args.output)


def cmd_run(args):
    emit(vm.run(args, PROFILES[args.profile]))


def cmd_recover(args):
    result = recovery.recover(args, PROFILES[args.profile])
    emit(result, args.output / "recovery.json")


async def ota(args, data, log):
    if args.command == "ota":
        if not args.address:
            return {"result": "offline_validated"}
        return {"result": await actions.bluetooth(args, data, log)}
    from .emu.hci import virtual_gatt

    async with virtual_gatt(args.hci_port, args.name) as client:
        if args.protocol == "actions":
            return {"result": await actions.exchange(client, args, data, log)}
        from .emu import stock_ota

        return await stock_ota.transfer(args, client, data)


def cmd_ota(args):
    if args.command == "ota":
        image.require(not args.flash or args.address, "Physical transfer requires an explicit --address")
    if args.output:
        image.require(not args.output.exists(), f"Output already exists: {args.output}")
    data = args.image.read_bytes()
    profile = PROFILES[args.profile]
    result = image.report(data, profile.model)
    files, _, _ = image.container(data, profile.model)
    image.require(set(files) == {"TEMP.bin"}, "OTA transport requires an outer TEMP container")
    image.require(args.protocol != "actions" or not result["agps_trailer_bytes"],
                  "Actions transport expects a standalone AOTA without a CMF trailer")
    result.update(protocol=args.protocol, target=("virtual" if args.command == "emu" else
                  "physical" if args.address else "offline"), events=[])

    def log(event, **values):
        if args.output:
            result["events"].append({"event": event, **values})

    try:
        result.update(asyncio.run(ota(args, data, log)))
        return 0 if result["result"] != "unconfirmed" else 1
    except (OSError, ValueError, RuntimeError, EOFError, KeyError) as error:
        result.update(result="failed", error_type=type(error).__name__)
        raise
    finally:
        summary = {key: value for key, value in result.items() if key != "events"}
        if args.output:
            summary["report"] = str(args.output)
        emit(result, args.output, summary)


def select_profile(parser):
    parser.add_argument("--profile", choices=PROFILES, default="wp2-stock")


def fixtures(parser):
    select_profile(parser)
    parser.add_argument("--fixtures", type=absolute, required=True)


def runtime(parser, rom_required=True):
    parser.add_argument("--qemu", default="qemu-system-arm")
    parser.add_argument("--rom", type=absolute, required=rom_required)
    parser.add_argument("--runtime-dir", type=absolute)


def ota_args(parser, default_delay=0):
    select_profile(parser)
    parser.add_argument("--image", type=absolute, required=True)
    parser.add_argument("--timeout", type=positive, default=30)
    parser.add_argument("--packet-delay", type=delay, default=default_delay)
    parser.add_argument("--output", type=absolute)


def parser():
    top = argparse.ArgumentParser(prog="cmf-watch-utils")
    commands = top.add_subparsers(dest="command", required=True)
    emu = commands.add_parser("emu", help="Run and test firmware on virtual hardware")
    emu_commands = emu.add_subparsers(dest="emu_command", required=True)
    emu_commands.add_parser("profiles", help="List supported profiles").set_defaults(handler=cmd_profiles)
    setup = emu_commands.add_parser("import-fixtures", help="Import local virtual flash fixtures")
    fixtures(setup)
    setup.add_argument("--nor", type=absolute, required=True)
    setup.add_argument("--nand", type=absolute, required=True)
    setup.add_argument("--nand-id", type=absolute, required=True)
    setup.set_defaults(handler=cmd_fixtures)
    provision = emu_commands.add_parser("prepare-fixtures", help="Create local virtual flash from stock firmware")
    fixtures(provision)
    runtime(provision)
    provision.add_argument("--stock", type=absolute, required=True)
    provision.add_argument("--nand-id", type=absolute, required=True)
    provision.add_argument("--provisioner", type=absolute, required=True, help="Directory containing provision-nand.elf and .bin")
    provision.add_argument("--gdb", default="arm-none-eabi-gdb", help="Arm-capable GDB executable")
    provision.set_defaults(handler=cmd_prepare)
    doctor = emu_commands.add_parser("doctor", help="Check local runtime and fixture paths")
    select_profile(doctor)
    doctor.add_argument("--fixtures", type=absolute, help="Also check prepared virtual flash")
    runtime(doctor)
    doctor.add_argument("--wish")
    doctor.set_defaults(handler=cmd_doctor)
    preflight = emu_commands.add_parser("preflight", help="Validate an app/SDFS OTA package offline")
    select_profile(preflight)
    preflight.add_argument("--image", type=absolute, required=True)
    preflight.add_argument("--output", type=absolute)
    preflight.set_defaults(handler=cmd_preflight)
    run = emu_commands.add_parser("run", help="Launch the interactive viewer or a bounded boot test")
    fixtures(run)
    runtime(run)
    source = run.add_mutually_exclusive_group()
    source.add_argument("--app", type=absolute)
    source.add_argument("--ota-image", type=absolute)
    source.add_argument("--nor", type=absolute)
    run.add_argument("--nand", type=absolute)
    run.add_argument("--sdfs", type=absolute)
    run.add_argument("--wish")
    run.add_argument("--output", type=absolute, required=True, help="New run directory")
    run.add_argument("--headless", action="store_true")
    run.add_argument("--seconds", type=positive, default=12, help="Headless observation or external QMP timeout")
    run.add_argument("--audio", choices=("dsound", "coreaudio", "pa", "alsa"))
    run.add_argument("--wav", action="store_true")
    run.add_argument("--charger-connected", action="store_true")
    run.add_argument("--allow-reboot", action="store_true")
    run.add_argument("--hci-port", type=port)
    run.add_argument("--qmp-port", type=port)
    run.add_argument("--nand-export", action="store_true")
    run.add_argument("--ota-snapshot", action="store_true", help="Save virtual NAND on an OTA reboot")
    run.set_defaults(handler=cmd_run)
    virtual = emu_commands.add_parser("ota", help="Negotiate or transfer over loopback H4")
    ota_args(virtual, default_delay=0.008)
    virtual.add_argument("--protocol", choices=("stock", "actions"), required=True)
    virtual.add_argument("--hci-port", type=port, required=True)
    virtual.add_argument("--name", default="cmf", help="Guest advertisement name substring")
    virtual.add_argument("--version", default="0100005a", help="Four-byte stock INIT2 version in hex")
    virtual.add_argument("--transfer", dest="flash", action="store_true", help="Send bytes to the virtual watch")
    virtual.set_defaults(handler=cmd_ota)
    pairing = emu_commands.add_parser("pair-fixture", help="Export stock virtual NOR with a synthetic pairing secret")
    select_profile(pairing)
    pairing.add_argument("--hci-port", type=port, required=True)
    pairing.add_argument("--qmp-port", type=port, required=True)
    pairing.add_argument("--name", default="cmf", help="Guest advertisement name substring")
    pairing.add_argument("--output", type=absolute, required=True, help="New virtual NOR file")
    pairing.set_defaults(handler=cmd_pair_fixture)
    recover = emu_commands.add_parser("recover", help="Install staged TEMP or rescue a broken virtual app")
    fixtures(recover)
    runtime(recover, rom_required=False)
    source = recover.add_mutually_exclusive_group(required=True)
    source.add_argument("--nor", type=absolute)
    source.add_argument("--qmp-port", type=port, help="Capture NOR when QEMU signals an OTA reset")
    pending = recover.add_mutually_exclusive_group()
    pending.add_argument("--pending-temp", type=absolute)
    pending.add_argument("--pending-nand", type=absolute)
    recover.add_argument("--reader", type=absolute)
    recover.add_argument("--gdb")
    recover.add_argument("--reset-timeout", type=positive, default=360)
    recover.add_argument("--crown-held", action="store_true")
    recover.add_argument("--charger-connected", action="store_true")
    recover.add_argument("--output", type=absolute, required=True)
    recover.set_defaults(handler=cmd_recover)
    physical = commands.add_parser("ota", help="Firmware OTA clients")
    actions_cli = physical.add_subparsers(dest="protocol", required=True).add_parser(
        "actions", help="Actions SDK OTA (offline validation unless a BLE address is supplied)")
    ota_args(actions_cli)
    actions_cli.add_argument("--address")
    actions_cli.add_argument("--scan-timeout", type=positive, default=15)
    actions_cli.add_argument("--connect-timeout", type=positive, default=45)
    actions_cli.add_argument("--flash", action="store_true", help="Send bytes to the physical watch")
    actions_cli.set_defaults(handler=cmd_ota)
    return top


def main(argv=None):
    args = parser().parse_args(argv)
    try:
        return args.handler(args) or 0
    except KeyboardInterrupt:
        return 130
    except (OSError, ValueError, RuntimeError, EOFError, KeyError, struct.error, ET.ParseError,
            lzma.LZMAError, subprocess.SubprocessError) as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
