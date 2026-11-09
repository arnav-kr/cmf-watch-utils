---
title: Quick start
description: The shortest safe path from clone to a read-only BLE check and an offline firmware parse.
---

## Inspect an image offline

Complete [Setup](/getting-started/setup/) and fetch the stock image first.

```text
uv run fwkit info dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.0.73.bin
```

The inner contents of the image are unpacked for exploration:

```bash
uv run fwkit unpack dumps/binaries/firmware/JX402_V1.0.0.73.bin work/aota
```

## BLE, read-only

```bash
uv run python tools/cmf_watch.py scan
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

The `--dry-run` flag is removed only after the validation passes. See
[Dial editing](/devices/cmf-watch-pro-2/guides/dial-editing/).

## Emulator and OTA checks

```bash
uv run cmf-watch-utils emu profiles
uv run cmf-watch-utils emu preflight --image local/update.bin
uv run cmf-watch-utils ota actions --image local/actions-update.bin
```

These are offline commands. Continue with the [Emulator CLI](/tooling/emulator/) guide
for a virtual boot or OTA transfer. Before any physical OTA, review the
[stock flash guide](/devices/cmf-watch-pro-2/guides/ota-flash/) or
[Actions OTA reference](/tooling/actions-ota/) for the service your firmware exposes.
On the stock client, `--stop-after-a040` connects, authenticates and sends live OTA
commands before stopping at the acknowledgement.
