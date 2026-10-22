---
title: Dial editing & upload
description: Unpack a WFDL watch face, replace resources, rebuild and install it over BLE.
---

## Inspect

```bash
uv run fwkit wf-info  watchfaces/wf1.bin --tree
uv run fwkit wf-res   watchfaces/wf1.bin
uv run fwkit wf-unpack watchfaces/wf1.bin work/wf1
```

`wf-unpack` writes `header.bin`, `tree.bin`, `resources/` (decoded PNGs) and raw LZ4 blocks
unless `--no-png` is used.

## Edit

Replace a single resource by index:

```bash
uv run fwkit wf-set-res watchfaces/wf1.bin 0 icon.png -o work/wf1_new.bin
```

Or rebuild from a directory of `NNN_*.png` replacements:

```bash
uv run fwkit wf-build work/wf1 work/wf1_new.bin --resources work/new_pngs --name "My Dial"
```

The toolkit keeps the original compression format unless you force `--cf`. Keep images inside
the original dimensions; the watch does not scale arbitrary layouts.

## Install

List current dials and slots first:

```bash
uv run python tools/cmf_watch.py wf-dials
```

Dry run:

```bash
uv run python tools/cmf_watch.py wf-flash --file work/wf1_new.bin --dry-run
```

Append to a free slot:

```bash
uv run python tools/cmf_watch.py wf-flash --file work/wf1_new.bin
```

Replace an existing dial by id:

```bash
uv run python tools/cmf_watch.py wf-flash --file work/wf1_new.bin --old-id 0x12345678
```

The reliable update path observed on hardware is **delete + append**:

```bash
uv run python tools/cmf_watch.py wf-delete --id 0x12345678 --file work/wf1_new.bin
uv run python tools/cmf_watch.py wf-flash  --file work/wf1_new.bin
```

## Known quirks

- The upload handshake is `0x8052` → `0x9075` → watch pulls with `0xA064` →
  `0x9064` chunks → `0xA065` → `0x9065`.
- A finish code of `0x0A` means stored but not activated: all slots full, or `--old-id` is
  stale.
- `0xA055` byte 3 read `7` on our unit while the public FMC client hardcodes capacity `6`.
  Side-loaded dials are not reported in the `wf-dials` list, so keep your own id map.
- The `.wfdl` header byte `+0x17` is not fully understood; preserve it when hand-editing.
