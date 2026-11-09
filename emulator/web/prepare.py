import importlib.util
from pathlib import Path
import sys


root = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("native_build", root / "build.py")
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
source = Path(sys.argv[1]).resolve()
sdk = native.prepare_sdk(source.parent / "wasm-sdk", "qemu")
native.integrate(source, sdk, None)
dockerfile = source / "Dockerfile"
text = dockerfile.read_text()
text = text.replace("curl -Ls https://zlib.net/zlib-$ZLIB_VERSION.tar.xz | tar xJC",
                    "curl -fLs https://zlib.net/fossils/zlib-$ZLIB_VERSION.tar.gz | tar xzC")
dockerfile.write_text(text)
