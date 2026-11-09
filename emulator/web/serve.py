import argparse
import hashlib
import lzma
import struct
import xml.etree.ElementTree as ET
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

from cmf_watch_utils.emu.image import verify_package
from cmf_watch_utils.emu.prepare import STOCK_SHA256
from cmf_watch_utils.emu.profiles import WP2

from cmf_watch_utils.emu.watchface import MAX_FACE, face_patch


class Handler(SimpleHTTPRequestHandler):
    extensions_map = {**SimpleHTTPRequestHandler.extensions_map, ".wasm": "application/wasm"}

    def do_POST(self):
        if self.path not in ("/watch-face", "/firmware"):
            self.send_error(404)
            return
        origin = self.headers.get("Origin")
        port = self.server.server_port
        if origin not in (f"http://127.0.0.1:{port}", f"http://localhost:{port}"):
            self.send_error(403, "Expected a request from the local emulator.")
            return
        if self.headers.get("Content-Type") != "application/octet-stream":
            self.send_error(415)
            return
        try:
            length = int(self.headers.get("Content-Length", "0"))
            limit = MAX_FACE if self.path == "/watch-face" else 64 * 1024**2
            if not 0 < length <= limit:
                raise ValueError("File exceeds the supported size.")
            self.connection.settimeout(15)
            payload = self.rfile.read(length)
            if len(payload) != length:
                raise ValueError("Incomplete file.")
            if self.path == "/firmware":
                if hashlib.sha256(payload).hexdigest() == STOCK_SHA256:
                    self.send_response(204)
                    self.end_headers()
                    return
                app, resources, _ = verify_package(payload, WP2)
                patch = struct.pack("<I", len(app)) + app + resources
            else:
                nand = Path(self.directory, "fixtures", "nand-defaults.bin").read_bytes()
                patch = face_patch(nand, payload)
        except (ValueError, IndexError, RecursionError, TimeoutError, struct.error, lzma.LZMAError, ET.ParseError) as error:
            self.send_error(400, str(error))
            return
        except OSError:
            self.send_error(500, "Could not read the prepared virtual NAND.")
            return
        self.send_response(200)
        self.send_header("Content-Type", "application/octet-stream")
        self.send_header("Content-Length", str(len(patch)))
        self.end_headers()
        self.wfile.write(patch)

    def end_headers(self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        self.send_header("Cache-Control", "no-store")
        super().end_headers()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("directory")
    parser.add_argument("--port", type=int, default=8090)
    args = parser.parse_args()
    server = ThreadingHTTPServer(("127.0.0.1", args.port), partial(Handler, directory=args.directory))
    print(f"Browser prototype: http://127.0.0.1:{args.port}", flush=True)
    server.serve_forever()
