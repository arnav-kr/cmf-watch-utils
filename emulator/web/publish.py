import argparse
from pathlib import Path
import shutil


parser = argparse.ArgumentParser()
parser.add_argument("--runtime", type=Path, required=True)
parser.add_argument("--output", type=Path, required=True)
args = parser.parse_args()

web = Path(__file__).resolve().parent
target = args.output / "emulator" / "watch-pro-2"
runtime = target / "runtime"
runtime.mkdir(parents=True, exist_ok=True)
for name in ("watch.js", "style.css", "isolation-worker.js"):
    shutil.copyfile(web / name, target / name)
html = (web / "index.html").read_text(encoding="utf-8")
(target / "index.html").write_text(html.replace("<body>", "<body data-hosted>", 1), encoding="utf-8")
for source, name in (
    ("qemu-system-arm", "qemu-system-arm.js"),
    ("qemu-system-arm.wasm", "qemu-system-arm.wasm"),
    ("qemu-system-arm.worker.js", "qemu-system-arm.worker.js"),
):
    shutil.copyfile(args.runtime / source, runtime / name)
