---
title: Setup
description: Install the Python workspace, place firmware locally and pair the watch.
---

## 1. Installation

```bash
git clone https://github.com/arnav-kr/cmf-cmf-watch-utils.git
cd watch-utils
uv sync
```

This creates a virtual environment with the `fwkit` CLI and the BLE tool dependencies. The
installation is verified with:

```bash
uv run fwkit --help
uv run python tools/cmf_watch.py --help
```

## 2. Firmware image

The stock image for this device is published under `dumps/binaries/`:

```bash
git lfs pull --include="dumps/binaries/firmware/JX402_V1.0.0.73.bin"
```

A local copy is used for editing, and the `firmware/` directory is gitignored:

```text
firmware/JX402_V1.0.0.73.bin
```

The image is parsed and verified before any edit or flash:

```bash
uv run fwkit info firmware/JX402_V1.0.0.73.bin
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
```

## 3. Watch advertising

With the watch disconnected in the Nothing X / CMF app, or phone Bluetooth turned off, the
watch advertises as `CMF Watch Pro 2-XXXX`. The advertised device is located with:

```bash
uv run python tools/cmf_watch.py scan
```

## 4. Pairing and local auth state

```bash
uv run python tools/cmf_watch.py pair
```

The command performs the full pairing exchange and writes `tools/.watch_state.json`. The file
holds the watch secret and authkey — **it is never committed or shared**. After a factory
reset, or after the factory-menu shutdown/reset option is used, the watch is re-paired; a
stale authkey can still get an `A052` reply but will not enter the OTA data phase.

## 5. Read-only check

```bash
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

Correct version, battery and PID values indicate a working BLE path. Firmware flashing is a
later step.

## Documentation site

Required only for documentation work:

```bash
cd site
pnpm install
pnpm dev
```
