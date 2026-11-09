import argparse
import json
from pathlib import Path
import shutil

from cmf_watch_utils.emu.profiles import PROFILES


parser = argparse.ArgumentParser()
parser.add_argument("--fixtures", type=Path, required=True)
parser.add_argument("--rom", type=Path, required=True)
parser.add_argument("--output", type=Path, required=True)
args = parser.parse_args()
profile = PROFILES["wp2-stock"]
nor, nand, nand_id = profile.fixtures(args.fixtures)
rom = args.rom.read_bytes()
if len(rom) != 65536:
    raise ValueError("Expected a 64 KiB compatibility ROM")
args.output.mkdir(parents=True, exist_ok=False)
web = Path(__file__).resolve().parent
for name in ("index.html", "watch.js"):
    shutil.copyfile(web / name, args.output / name)
fixtures = args.output / "fixtures"
fixtures.mkdir()
for path, name in ((nor, "nor.bin"), (nand, "nand-defaults.bin"), (nand_id, "nand-id.bin")):
    shutil.copyfile(path, fixtures / name)
(fixtures / "compat.bin").write_bytes(rom)
(fixtures / "app.bin").write_bytes(profile.stock_app(nor.read_bytes()))
(fixtures / "manifest.json").write_text(json.dumps({"files": [
    "nor.bin", "nand-defaults.bin", "nand-id.bin", "compat.bin", "app.bin"]}), encoding="utf-8")
print(args.output.resolve())
