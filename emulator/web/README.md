# Watch Pro 2 in the browser

The [browser preview](https://cmf.arnv.dev/emulator/watch-pro-2/) runs stock 1.0.0.73 using
our existing ATS3089C model and compatibility ROM. The hosted preview downloads
stock storage generated from the published firmware and includes the four
Watch Pro 2 frame renders. Local builds can use their own prepared fixtures.

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

Open `http://127.0.0.1:8090/` to start stock automatically. The server binds to
loopback and sends the cross-origin isolation headers needed by Wasm workers.
Opening the HTML directly as a file will not work. A first boot can take tens
of seconds; the watch log shows guest progress.

You can select individual files from `web-local/fixtures` under
**Stock firmware override**. The bundled stock files fill in any you leave out.
The hosted preview accepts local firmware packages,
application files and watch faces. Open **FMC watch faces** to browse community
faces by name or author and sort by newest, downloads, or flashes. The same
picker is available when serving local stock files. Select **Bad Apple** to
provide your own built firmware. Under
**Display**, you can choose a built-in frame color or replace its artwork with
an image file.

To preview the same versioned web bundle that Pages publishes, run
`uv run python emulator/web/publish.py --runtime WASM_BUILD --stock web-local --output web-preview`
and serve `web-preview`. Open `http://127.0.0.1:8090/emulator/watch-pro-2/`.
For this layout, stage with `--stock-image` and `--frames-dir` to include the
stock texture index and frame renders.

## Controls

- Click or drag on the round display to tap or swipe.
- Press the round crown button, or focus the display and press Space.
- Use the crown arrows, the keyboard arrows, or scroll over the display to rotate.
- **Show frame** toggles the surrounding bezel. The expand button enlarges the
  device view. Neither changes the guest's 466 x 466 display resolution.
- The camera button saves the circular display as a transparent PNG. Turn on
  **Include frame in screenshots** to include the selected watch frame.
- **Stop & reset** reloads the page. Guest storage changes are discarded.
- Open **Watch log** to inspect recent guest UART output or save the full captured UART log. Add `?debug` to the URL for
  verbose GPU/MMIO diagnostics; this increases logging and memory use.

## Current limits

The port reuses the native model, including its approximations and stock-specific
helpers. A successful browser boot does not verify the real boot ROM, physical
recovery, electrical behavior or freedom from crashes. TCI execution is slower
than native QEMU and does not reproduce device performance or power consumption.

Sound, browser OTA, Nothing X pairing, persistent storage and other watch models
are not implemented in this browser milestone. The Pages build generates its
stock files from the public firmware package and keeps binaries out of Git.

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

With the local Python server running, select a single Watch Pro 2 face
(`.wfdl`, `.wfc`, or `.bin`) under **Boot images â†’ Watch face**, then press Play.
The import replaces Activity Mood for that stock session. If the face needs
more room, the importer uses the Prismatic Time slot to keep the original
Activity Mood available. The browser changes its in-memory copy of the prepared
NAND. Use **Hard reset to stock** to start over.

For a custom firmware session, open **Boot images** and select its Watch Pro 2 ACTH `app.bin` under
**Application image** and its matching `sdfs.bin`, if provided, under
**Application resources**. Start emulation to boot the staged virtual NOR.

Use **Firmware package** to boot a validated Watch Pro 2 AOTA package that
contains an application and companion resources. The verified stock 1.0.0.73
package uses the matching stock fixtures. Select the package and press Play. If a guest fault or runtime exit appears in the notification strip,
**Boot stock** starts a fresh session from the supplied stock fixtures.
**Battery target** maps percentage to an approximate voltage. The guest
firmware calculates its own displayed percentage.
Use **Simulation → Display → Brightness** to dim the virtual screen. At
100%, the screen uses the brightness set by the guest firmware. Desktop
QEMU offers the same slider under **Advanced**.

Drag the miniature watch to change orientation, or use the arrow keys for
smaller adjustments. Select its X/Y/Z readout to edit the three acceleration
values directly. **Reset simulation** restores the default sensor and power
settings. **Hard reset to stock** discards the running session and selected
files, then starts a fresh stock session.

Open **Optical sensors** to set the heart rate and blood oxygen shown by
the stock measurement apps. Open **GPS** to send a stationary fix at a
latitude and longitude in decimal degrees. The guest must start its GPS
receiver, for example when starting an outdoor workout.

Pass `--frames-dir` to the staging script to include the four Nothing X
front renders. Select **Watch color** in the browser to switch frames while
the emulator runs.
