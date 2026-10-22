---
title: Quick start
description: The shortest safe path from clone to a read-only BLE check and an offline firmware parse.
---

## Offline

```bash
uv sync
uv run fwkit info firmware/JX402_V1.0.0.73.bin
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
```

Extract the inside of the image for exploration:

```bash
uv run fwkit unpack firmware/JX402_V1.0.0.73.bin work/aota
```

## BLE, read-only

```bash
uv run python tools/cmf_watch.py scan
uv run python tools/cmf_watch.py pair          # once, or after a reset
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

## Watch faces

```bash
uv run fwkit wf-info watchfaces/wf1.bin --tree
uv run fwkit wf-res  watchfaces/wf1.bin
uv run fwkit wf-unpack watchfaces/wf1.bin work/wf1
uv run fwkit wf-set-res watchfaces/wf1.bin 0 icon.png -o work/wf1_new.bin
uv run python tools/cmf_watch.py wf-flash --file work/wf1_new.bin --dry-run
```

Only drop `--dry-run` after the validation passes. See [Dial editing](/cmf-watch-utils/guides/dial-editing/).

## Firmware OTA

Do not start here. Use the [step-by-step OTA guide](/cmf-watch-utils/guides/ota-flash/) and read
[AOTA OTA flow](/cmf-watch-utils/protocol/ota/) first. A dry run is available:

```bash
uv run python tools/ota_client.py --stop-after-a040 --version 0100005a -v
```

That stops after the watch acknowledges `0xA040` and sends no data.
