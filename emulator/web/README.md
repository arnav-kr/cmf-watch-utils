# Watch Pro 2 in the browser

Experimental browser port on `wasm-watch-pro-2`. It runs stock 1.0.0.73 using
our existing ATS3089C model and compatibility ROM. Firmware is supplied locally;
the browser runtime does not contain stock firmware or artwork.

Stock boot, touchscreen taps and swipes, crown presses, and crown rotation have
been exercised in Chromium. This is a display-and-input preview, not a replacement
for hardware testing. The CPU uses QEMU's TCI interpreter because the upstream
Wasm JIT produced guest faults on this Cortex-M workload.

## Requirements

- A recent desktop browser with WebAssembly threads and SharedArrayBuffer support.
  Chromium is the tested browser; other browsers have not been verified.
- Enough memory for a 768 MiB Wasm heap, virtual flash and browser overhead.
  Allow at least 1.5 GiB free RAM for this prototype.
- This repository's Python environment: `uv sync`.
- Prepared stock fixtures and the matching 64 KiB compatibility ROM from the
  native emulator setup. A phone, Bluetooth adapter and real watch are unnecessary.
- The experimental browser build artifact. Native QEMU executables cannot be
  used as the browser runtime.

## Prepare a local session

From the repository root, stage your existing stock fixtures:

```text
uv run python emulator/web/stage.py --fixtures fixtures --rom runtime/compat.bin --output web-local
```

The output directory must be new. Staging validates the stock application and
copies the browser page and virtual storage. For correct rendering of the local
stock watch faces, add `--wfc-dir PATH` pointing to the extracted stock WFC files
named `*_local*.wfc`. This generates texture-format metadata from those files;
it does not modify the stock application. Without it, some watch-face graphics
are incorrectly decoded. The ordinary stock app list does not need that index.

Download `wp2-wasm-experimental` from the **Experimental browser emulator**
workflow. Copy `qemu-system-arm.wasm` and `qemu-system-arm.worker.js` into
`web-local/runtime/`. Copy `qemu-system-arm` into that directory and rename it
`qemu-system-arm.js`.

Start the local server:

```text
uv run python emulator/web/serve.py web-local
```

Open `http://127.0.0.1:8090/` and select **Start emulation**. The server binds to
loopback and sends the cross-origin isolation headers needed by Wasm workers.
Opening the HTML directly as a file will not work. A first boot can take tens
of seconds; the boot log shows guest progress.

## Controls

- Click or drag on the round display to tap or swipe.
- Press the round crown button, or focus the display and press Space.
- Use the crown arrows, the keyboard arrows, or scroll over the display to rotate.
- **Show frame** toggles the surrounding bezel. The expand button enlarges the
  device view. Neither changes the guest's 466 x 466 display resolution.
- **Stop & reset** reloads the page. Guest storage changes are discarded.
- Expand **Boot log** to inspect output or save it. Add `?debug` to the URL for
  verbose GPU/MMIO diagnostics; this increases logging and memory use.

## Current limits

The port reuses the native model, including its approximations and stock-specific
helpers. A successful browser boot does not verify the real boot ROM, physical
recovery, electrical behavior or freedom from crashes. TCI execution is slower
than native QEMU and does not reproduce device performance or power consumption.

Sound, browser OTA, Nothing X pairing, persistent storage and other watch models
are not implemented in this browser milestone. The stock firmware and resource
files stay outside Git and runner artifacts. No website deployment is included.
The proposed route remains `/emulator/watch-pro-2/`, listed under `/emulator/`.

## Development checks

The runner builds the pinned QEMU-Wasm revision with the shared hardware model.
The vector renderer has a host-side regression check:

```text
cc -Wall -Wextra -Werror emulator/tests/vg_path.c -lm -o vg-path-test
./vg-path-test
node --check emulator/web/watch.js
```

For a browser regression check, boot stock, open the app list with the crown,
scroll it using both touch and crown rotation, and tap a stock app. Repeat after
reset. Keep the runtime revision, boot output and screenshots with the results.
A loading screen or a canvas with no guest output is not a successful boot.
