import argparse
from hashlib import sha256
from pathlib import Path
import shutil
from zipfile import ZIP_DEFLATED, ZipFile


parser = argparse.ArgumentParser()
parser.add_argument("--runtime", type=Path, required=True)
parser.add_argument("--output", type=Path, required=True)
args = parser.parse_args()

web = Path(__file__).resolve().parent
target = args.output / "emulator" / "watch-pro-2"
scripts = ("watch.js", "style.css", "face.mjs", "catalog.mjs", "storage.mjs", "firmware-worker.js")
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
for path in (
    *(web / name for name in scripts),
    *(args.runtime / name for name, _ in runtime_files),
    *(root / base / name for base, files in tools for name in files),
):
    digest.update(path.read_bytes())
asset_path = f"assets/{digest.hexdigest()[:16]}"
assets = target / asset_path
runtime = assets / "runtime"
runtime.mkdir(parents=True, exist_ok=True)
for name in scripts:
    shutil.copyfile(web / name, assets / name)
shutil.copyfile(web / "isolation-worker.js", target / "isolation-worker.js")
html = (web / "index.html").read_text(encoding="utf-8")
html = html.replace("<body>", "<body data-hosted>", 1)
for name in ("watch.js", "style.css"):
    html = html.replace(f'"./{name}"', f'"./{asset_path}/{name}"', 1)
(target / "index.html").write_text(html, encoding="utf-8")
for source, name in runtime_files:
    shutil.copyfile(args.runtime / source, runtime / name)

with ZipFile(assets / "firmware-tools.zip", "w", compression=ZIP_DEFLATED) as archive:
    for package in ("cmf_watch_utils", "cmf_watch_utils/emu", "fwkit"):
        archive.writestr(f"{package}/__init__.py", "")
    for base, files in tools:
        for name in files:
            archive.write(root / base / name, name)
