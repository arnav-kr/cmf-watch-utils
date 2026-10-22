---
title: Setup
description: Install the Python workspace, place firmware locally and pair the watch.
---

## 1. Clone and install

```bash
git clone https://github.com/arnav-kr/cmf-watch-utils.git
cd cmf-watch-utils
uv sync
```

This creates a virtual environment with the `fwkit` CLI and the BLE tool dependencies.
Check it:

```bash
uv run fwkit --help
uv run python tools/cmf_watch.py --help
```

## 2. Place your firmware locally

```text
firmware/JX402_V1.0.0.73.bin
```

The file is gitignored. Confirm it parses before editing or flashing:

```bash
uv run fwkit info firmware/JX402_V1.0.0.73.bin
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
```

## 3. Free the watch from the phone

Disconnect the watch in the Nothing X / CMF app, or turn off phone Bluetooth. The watch should
advertise as `CMF Watch Pro 2-XXXX`. Find it:

```bash
uv run python tools/cmf_watch.py scan
```

## 4. Pair / create local auth state

```bash
uv run python tools/cmf_watch.py pair
```

This performs the full pairing exchange and writes `tools/.watch_state.json`. It contains the
watch secret and authkey — **never commit or share it**. If the watch is factory reset or you
use the factory-menu shutdown/reset option, re-pair; a stale authkey can still get an `A052`
reply but will not enter the OTA data phase.

## 5. Read-only check

```bash
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

If version, battery and PID look right, the BLE path is working. Do not start with a flash.

## Documentation site

Only needed if you are editing the docs:

```bash
cd site
pnpm install
pnpm dev
```
