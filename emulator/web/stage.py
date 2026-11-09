import argparse
import hashlib
import json
from pathlib import Path
import shutil

from cmf_watch_utils.emu.profiles import PROFILES


parser = argparse.ArgumentParser()
parser.add_argument("--fixtures", type=Path, required=True)
parser.add_argument("--rom", type=Path, required=True)
parser.add_argument("--output", type=Path, required=True)
parser.add_argument("--wfc-dir", type=Path)
parser.add_argument("--frames-dir", type=Path, help="Directory with the four Nothing X front renders")
args = parser.parse_args()
profile = PROFILES["wp2-stock"]
nor, nand, nand_id = profile.fixtures(args.fixtures)
rom = args.rom.read_bytes()
if len(rom) != 65536:
    raise ValueError("Expected a 64 KiB compatibility ROM")
frame_names = ("dark-grey.png", "light-grey.png", "blue.png", "orange.png")
if args.frames_dir and any(not (args.frames_dir / name).is_file() for name in frame_names):
    parser.error("--frames-dir needs dark-grey.png, light-grey.png, blue.png, and orange.png")
args.output.mkdir(parents=True, exist_ok=False)
if args.frames_dir:
    frames = args.output / "frames"
    frames.mkdir()
    for name in frame_names:
        shutil.copyfile(args.frames_dir / name, frames / name)
web = Path(__file__).resolve().parent
for name in ("index.html", "watch.js", "style.css", "face.mjs", "catalog.mjs", "storage.mjs"):
    shutil.copyfile(web / name, args.output / name)
fixtures = args.output / "fixtures"
fixtures.mkdir()
for path, name in ((nor, "nor.bin"), (nand, "nand-defaults.bin"), (nand_id, "nand-id.bin")):
    shutil.copyfile(path, fixtures / name)
(fixtures / "compat.bin").write_bytes(rom)
(fixtures / "app.bin").write_bytes(profile.stock_app(nor.read_bytes()))
files = ["nor.bin", "nand-defaults.bin", "nand-id.bin", "compat.bin", "app.bin"]
if args.wfc_dir:
    from fwkit import lz4, wfdl

    textures = set()
    for face in args.wfc_dir.glob("*_local*.wfc"):
        for entry in wfdl.parse(face.read_bytes()).resources:
            if entry.cf == 5:
                pixels = lz4.decompress(entry.data, entry.raw_size)
                if len(pixels) != entry.w * entry.h * 3:
                    raise ValueError(f"Invalid WFC texture in {face}")
                textures.add(f"{entry.w}:{entry.h}:{hashlib.sha256(pixels).hexdigest()}")
    if not textures:
        raise ValueError("No packed WFC textures found")
    (fixtures / "wfc-index.txt").write_text("\n".join(sorted(textures)) + "\n", encoding="ascii", newline="\n")
    files.append("wfc-index.txt")
(fixtures / "manifest.json").write_text(json.dumps({"files": files}), encoding="utf-8")
print(args.output.resolve())
