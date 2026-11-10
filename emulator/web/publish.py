import argparse
import gzip
from hashlib import sha256
from pathlib import Path
import shutil
from zipfile import ZIP_DEFLATED, ZipFile


parser = argparse.ArgumentParser()
parser.add_argument("--runtime", type=Path, required=True)
parser.add_argument("--output", type=Path, required=True)
parser.add_argument("--stock", type=Path, help="Staged browser stock image and watch frames")
parser.add_argument("--presets", type=Path, help="Prepared browser firmware presets")
args = parser.parse_args()

web = Path(__file__).resolve().parent
target = args.output / "emulator" / "watch-pro-2"
scripts = ("watch.js", "style.css", "face.mjs", "catalog.mjs", "firmware-worker.js", "speaker.mjs")
root = web.parents[1]
tools = (
    ("src", ("cmf_watch_utils/emu/image.py", "cmf_watch_utils/emu/profiles.py")),
    ("tooling/src", ("fwkit/aota.py", "fwkit/sdfs.py")),
)
runtime_files = (
    ("qemu-system-arm", "qemu-system-arm.js"),
    ("qemu-system-arm.wasm", "qemu-system-arm.wasm"),
    ("qemu-system-arm.worker.js", "qemu-system-arm.worker.js"),
)
digest = sha256()
stock_files = []
preset_files = []
if args.presets:
    preset_files = [args.presets / "display-demo.bin"]
    bad_apple = [args.presets / name for name in ("bad-apple-app.bin", "bad-apple-sdfs.bin")]
    if any(path.exists() for path in bad_apple):
        preset_files.extend(bad_apple)
if args.stock:
    stock_files = [args.stock / "fixtures" / name for name in (
        "app.bin", "compat.bin", "nand-defaults.bin", "nand-id.bin", "nor.bin", "wfc-index.txt",
    )] + [args.stock / "frames" / f"{color}.png" for color in (
        "dark-grey", "light-grey", "blue", "orange",
    )]
for path in (
    *(web / name for name in scripts),
    *(args.runtime / name for name, _ in runtime_files),
    *(root / base / name for base, files in tools for name in files),
    *stock_files,
    *preset_files,
):
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
asset_path = f"assets/{digest.hexdigest()[:16]}"
assets = target / asset_path
runtime = assets / "runtime"
runtime.mkdir(parents=True, exist_ok=True)
for name in scripts:
    shutil.copyfile(web / name, assets / name)
for name in ("isolation-worker.js", "favicon.svg", "og-emulator-screenshot.png"):
    shutil.copyfile(web / name, target / name)
html = (web / "index.html").read_text(encoding="utf-8")
body = ["data-hosted"]
if args.stock:
    body.append("data-stock-ready")
if preset_files:
    body.append("data-demo-ready")
if len(preset_files) == 3:
    body.append("data-bad-apple-ready")
html = html.replace("<body>", f"<body {' '.join(body)}>", 1)
for name in ("watch.js", "style.css"):
    html = html.replace(f'"./{name}"', f'"./{asset_path}/{name}"', 1)
(target / "index.html").write_text(html, encoding="utf-8")
for source, name in runtime_files:
    shutil.copyfile(args.runtime / source, runtime / name)
if preset_files:
    (assets / "presets").mkdir(exist_ok=True)
    for source in preset_files:
        shutil.copyfile(source, assets / "presets" / source.name)
for source in stock_files:
    destination = assets / source.relative_to(args.stock)
    destination.parent.mkdir(parents=True, exist_ok=True)
    if source.name == "nand-defaults.bin":
        with source.open("rb") as data, destination.with_suffix(".bin.gz").open("wb") as compressed:
            with gzip.GzipFile(fileobj=compressed, mode="wb", compresslevel=6, mtime=0) as output:
                shutil.copyfileobj(data, output)
    else:
        shutil.copyfile(source, destination)

with ZipFile(assets / "firmware-tools.zip", "w", compression=ZIP_DEFLATED) as archive:
    for package in ("cmf_watch_utils", "cmf_watch_utils/emu", "fwkit"):
        archive.writestr(f"{package}/__init__.py", "")
    for base, files in tools:
        for name in files:
            archive.write(root / base / name, name)
