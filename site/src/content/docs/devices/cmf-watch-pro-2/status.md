---
title: What works
description: Verified functionality, hardware-tested results and known-good paths.
---

This page separates **verified on hardware** from **offline verified** and **theory**. The read-only
paths are the safest starting point.

## Status summary

| Area | Status | Evidence |
|---|---|---|
| Read shell/AT commands over BLE | Verified | `GETVERSION`, `GETPID`, `GETBATLV`, `GETSN`, `GETGSENSOR` |
| BLE authentication and session keys | Verified | Pairing, nonce exchange, AES-CBC session key |
| AOTA container parse / rebuild | Verified offline | Byte-identical rebuild of the stock image |
| SDFS partition parse / pack / pad | Verified offline | Every partition round-trips, including padded `res_e` |
| RES picture and string pools | Verified offline | All shipped entries decode; edit round-trips |
| STY scene bundles | Verified offline | Parse/build; add/delete/compact scene tests |
| WFDL watch faces | Verified offline + on-watch upload | Build and upload/delete |
| AJS animations | Verified offline + on-watch | Extract/repack byte-identical; custom animation played |
| Full AOTA firmware OTA over BLE | Verified on hardware | Changed partition transferred and committed; watch booted |
| App binary patching | Verified offline | Scene-id literal rewrite and header rebuild |

## Firmware and container work

- **AOTA** is a 0x400-byte header, a 32-byte entry table, LZMA chunks, and nested partitions
  (`TEMP.bin`, `res.bin`, `fonts.bin`, `res_e.bin`, `sdfs_k.bin`). The toolkit verifies the
  header CRC, every entry CRC, and can rebuild the image with changed partitions.
- **SDFS** (`res_e`, `res_e`-style images) is a simple named-file filesystem with fixed
  partition length. `fwkit.sdfs.pad_to()` preserves the original partition size while
  refreshing `total_size` and the data checksum.
- The stock image was rebuilt byte-for-byte to validate the parser/writer prior to any edit.

## Watch faces and UI

- `.wfdl` packages can be parsed into a resource tree, edited (including image replacement),
  rebuilt and uploaded over BLE.
- The upload path is `0x8052` → `0x9075` → watch pulls with `0xA064` → `0x9064` chunks →
  `0xA065` → `0x9065`. Delete + append is the reliable update path.
- `.sty` scene bundles are mapped enough to read the scene table, walk the node graph, add a
  scene with fresh node IDs, delete a scene and compact records. A new scene still needs an
  app-side scene-id reference if the scene is to be reachable from the UI.
- `.res` picture codecs are fully decoded for the shipped content: JPEG and raw LZ4 with
  RGB888/RGB565 variants.

## Animations and the Bad Apple proof of concept

- `.ajs` files are 240×240 JPEG frame sequences at 25 fps by default, with a small record
  prelude and a tile table. `9.ajs` is the 8-second sport warm-up animation (sport type 22).
- A 202-frame / 8.1-second PoC was built, extracted back byte-identically, packed into
  `res_e.bin`, wrapped in a valid AOTA image, flashed over BLE, and **confirmed playing on the
  watch UI**.
- Full Bad Apple is ~5,250 frames / ~18.75 MiB, which does **not** fit in the free space left
  by replacing only `9.ajs`. See [Limitations](/devices/cmf-watch-pro-2/limitations/).

## BLE and OTA

- BLE authentication, shell/AT commands, watch-face upload and the AOTA OTA state machine are
  all usable from Python.
- The critical OTA detail: **`0x9040` (INIT2) is sent on the OTA characteristic `...ff01`**, and
  not first on the command characteristic. Sending it on `CMD_W` can reboot the watch before the
  data phase starts.
- At the end, `0xA041` is answered with `0x9041` and payload `A5`. Skipping the end ack leaves
  the transfer uncommitted in testing.
- A full 54 MB candidate flashed successfully; the watch reported `A041 01`, accepted the end
  ack, rebooted and booted the new resource partition.

## What the toolkit is not

This is not a universal unlock, not a bootloader exploit, and not a way to read arbitrary NAND.
It works with the normal app/bootloader interfaces that the watch already exposes. Encrypted
or unimplemented parts remain out of reach without a NAND dump or a patched app.
