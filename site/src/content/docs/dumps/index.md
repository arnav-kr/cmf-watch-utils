---
title: Data index
description: Manifests, downloadable firmware images and redacted notes published with this project.
---

This section holds the artifacts behind the research: firmware images, manifests with hashes
and structure, and redacted notes.

## Downloadable images

| File | Size | SHA-256 (first 16) | Description |
|---|---|---|---|
| `dumps/binaries/firmware/JX402_V1.0.0.73.bin` | 56,903,752 | `30903fc8ce6db4f8` | Stock factory image, CMF Watch Pro 2 (`JX402`), firmware 1.0.0.73 |
| `dumps/binaries/firmware/JX402_V1.0.0.73_badapple9.bin` | 54,117,960 | `4933e593d14bef6f` | Rebuilt image with the 202-frame animation proof of concept |
| `dumps/binaries/app/app_073.bin` | 2,465,920 | `30e7978c39d8fb79` | `app.bin` extracted from the `TEMP` partition of the stock image |

The images are stored with Git LFS:

```bash
git lfs install
git clone https://github.com/arnav-kr/watch-utils.git
git lfs pull --include="dumps/binaries/app/app_073.bin"
```

The proof-of-concept image contains third-party animation frames. Redistribution rights for
vendor firmware and for those frames are the responsibility of whoever redistributes them;
before the repository is made public, `dumps/binaries/` is removed or the rights are
confirmed.

## Manifests

[`manifests/firmware-1.0.0.73.json`](https://github.com/arnav-kr/watch-utils/blob/main/dumps/manifests/firmware-1.0.0.73.json)
records the size, SHA-256, header fields and per-entry CRC status of the stock image, plus the
published binaries. It serves as the reference for checking that a locally obtained image
matches the structure the tooling was written against:

```bash
uv run fwkit info dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.0.73.bin
```

## Structural facts

Stock image `1.00_2509131556`, board `jx402_01_3089c`, 56,903,752 bytes:

| Entry | Offset | Stored | Raw | Notes |
|---|---|---|---|---|
| `ota.xml` | 0x400 | 1,832 | – | version and board metadata |
| `TEMP.bin` | 0xC00 | 1,317,084 | 2,492,928 | contains `app.bin` |
| `res.bin` | 0x142600 | 6,978,004 | 10,137,792 | RES string and picture pools |
| `fonts.bin` | 0x7EA000 | 7,813,200 | 9,513,344 | fonts |
| `res_e.bin` | 0xF5DA00 | 39,237,168 | 58,777,088 | SDFS with the `.ajs` animations |
| `sdfs_k.bin` | 0x34C9200 | 310,872 | 524,096 | system data |

Every partition is derivable from the image:

```bash
uv run fwkit unpack dumps/binaries/firmware/JX402_V1.0.0.73.bin work/aota
```

## Redacted notes

- [`ota-flash-run.md`](https://github.com/arnav-kr/watch-utils/blob/main/dumps/ota-flash-run.md) —
  the shape of a successful flash log with device identifiers removed.

## Personal data

Published artifacts contain no MAC address, serial number, pairing secret, authkey, session
key, GPS track or notification content. The manifests carry hashes only.
