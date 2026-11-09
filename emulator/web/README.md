# Watch Pro 2 browser prototype

Experimental work on the `wasm-watch-pro-2` branch. A successful compilation
alone does not establish a working stock boot or input handling.

The prototype compiles the existing ATS3089C hardware model against the pinned
`ktock/qemu-wasm` revision in the workflow. It uses the same compatibility ROM,
stock application and prepared flash as the native emulator. Firmware is kept
local and is not included in runner artifacts.

## Local files

Prepare stock storage using the native emulator CLI first. Then, from the
repository root:

```text
uv run python emulator/web/stage.py --fixtures fixtures --rom runtime/compat.bin --output web-local
```

The output directory must be new. It contains copies of the virtual storage
and the browser page. Download the experimental workflow artifact, copy its
`qemu-system-arm.wasm` and `qemu-system-arm.worker.js` into `web-local/runtime/`,
and copy `qemu-system-arm` there as `qemu-system-arm.js`.

```text
uv run python emulator/web/serve.py web-local
```

Open `http://127.0.0.1:8090/` and select **Start stock firmware**. The server
binds only to loopback and supplies the isolation headers required for workers
and shared memory. Firmware files are read from that same local server.

Touch and crown events use the model's existing input format. The canvas reads
the model's 466×466 RGB display output. This first prototype uses an in-memory
filesystem; changes made by the guest disappear on reload. Sound, browser OTA,
Nothing X and persistent storage are outside this first milestone.

## Verification

Confirm the stock screen appears, a tap and swipe change the stock UI, the
crown button changes screens, and rotation scrolls a stock list. Keep boot logs
and screenshots with the exact runtime revision used. A page displaying only
its controls or a black canvas is not a successful emulator boot.

The proposed public route is `/emulator/watch-pro-2/`, with `/emulator/` listing
available models. The prototype does not change or deploy those routes.
