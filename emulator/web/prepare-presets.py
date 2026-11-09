import argparse
from pathlib import Path
import subprocess

from cmf_watch_utils.emu.image import verify_package
from cmf_watch_utils.emu.profiles import WP2


parser = argparse.ArgumentParser()
parser.add_argument("--output", type=Path, required=True)
parser.add_argument("--arm-prefix", default="arm-none-eabi-")
parser.add_argument("--bad-apple", type=Path, help="Build of Bad Apple to include in this deployment")
args = parser.parse_args()
args.output.mkdir(parents=True)
web = Path(__file__).resolve().parent
elf = args.output / "display-demo.elf"
demo = args.output / "display-demo.bin"
subprocess.run([
    args.arm_prefix + "gcc", "-mcpu=cortex-m33", "-mthumb", "-Os", "-ffreestanding",
    "-fno-unwind-tables", "-fno-asynchronous-unwind-tables", "-nostdlib",
    "-Wl,--build-id=none", "-T", str(web / "display-demo.ld"),
    str(web / "display-demo.c"), "-o", str(elf),
], check=True)
subprocess.run([args.arm_prefix + "objcopy", "-O", "binary", str(elf), str(demo)], check=True)
app = b"ACTHHTCA".ljust(0x200, b"\0") + demo.read_bytes()
WP2.validate_app(app)
demo.write_bytes(app)
if args.bad_apple:
    app, resources, _ = verify_package(args.bad_apple.read_bytes(), WP2)
    (args.output / "bad-apple-app.bin").write_bytes(app)
    (args.output / "bad-apple-sdfs.bin").write_bytes(resources)
