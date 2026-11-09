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
rr = source / "accel/tcg/tcg-accel-ops-rr.c"
text = rr.read_text()
entry = "static void *rr_cpu_thread_fn(void *arg)\n{"
if text.count(entry) != 1:
    raise ValueError("Unexpected round-robin CPU entry point")
text = text.replace(entry,
    '#if defined(EMSCRIPTEN) && !defined(CONFIG_TCG_INTERPRETER)\n'
    '#include "../../tcg/wasm32.h"\n#endif\n' + entry +
    '\n#if defined(EMSCRIPTEN) && !defined(CONFIG_TCG_INTERPRETER)\n'
    '    init_wasm32();\n#endif')
rr.write_text(text)
dockerfile = source / "Dockerfile"
text = dockerfile.read_text()
text = text.replace("curl -Ls https://zlib.net/zlib-$ZLIB_VERSION.tar.xz | tar xJC",
                    "curl -fLs https://zlib.net/fossils/zlib-$ZLIB_VERSION.tar.gz | tar xzC")
dockerfile.write_text(text)
