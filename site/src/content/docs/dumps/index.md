---
title: Data index
description: Manifests, hashes and redacted notes published with this project.
---

This directory contains **metadata only**: hashes, sizes, structural manifests and redacted
notes. No firmware, vendor resources, watch-face packages or animation frames are included —
see [Legal & attribution](/cmf-watch-utils/overview/legal/).

## Manifests

| File | Description |
|---|---|
| [`manifests/firmware-1.0.0.73.json`](https://github.com/arnav-kr/cmf-watch-utils/blob/main/dumps/manifests/firmware-1.0.0.73.json) | SHA-256, header fields and per-entry sizes/CRCs of the stock `1.0.0.73` image |

The manifest lets you verify that your own legally obtained image matches the structure this
project was written against:

```bash
uv run fwkit info firmware/JX402_V1.0.0.73.bin
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
```

## Structural facts

Stock `1.00_2509131556` / board `jx402_01_3089c`, image size 56,903,752 bytes:

| Entry | Offset | Stored | Raw | Notes |
|---|---|---|---|---|
| `ota.xml` | 0x400 | 1,832 | – | version/board metadata |
| `TEMP.bin` | 0xC00 | 1,317,084 | 2,492,928 | contains `app.bin` |
| `res.bin` | 0x142600 | 6,978,004 | 10,137,792 | RES string/picture pools |
| `fonts.bin` | 0x7EA000 | 7,813,200 | 9,513,344 | fonts |
| `res_e.bin` | 0xF5DA00 | 39,237,168 | 58,777,088 | SDFS with `.ajs` animations |
| `sdfs_k.bin` | 0x34C9200 | 310,872 | 524,096 | system data |

## Redacted notes

- [`ota-flash-run.md`](https://github.com/arnav-kr/cmf-watch-utils/blob/main/dumps/ota-flash-run.md) —
  the shape of a successful flash log with device identifiers removed.

## Adding your own dumps

If you publish a dump, remove MAC, serial, secret, authkeys, session keys, GPS tracks,
notification contents and any copyrighted media first. Hashes are the most useful artifact and
carry no personal data.
