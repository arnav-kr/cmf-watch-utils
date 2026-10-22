---
title: Methodology
description: How the formats and protocol were recovered, and how to reproduce or extend the work.
---

## Approach

1. **Reversible experiments.** The same bytes are parsed and rebuilt before any change is made.
2. **Separate hypotheses from facts.** A decompiler comment is a hypothesis until a round-trip,
   a live reply, or a boot proves it.
3. **Application capture.** The official app is the reference client for frame layout,
   keys, retries and ordering.
4. **Rollback.** The image being replaced is saved, and the diff kept as small as possible.
5. **Personal data.** MAC addresses, serial numbers, pairing secrets and authkeys are stripped
   from this repository.

## Static analysis

The app image (`app.bin`, base `0x10000000`, Thumb-2) was loaded into Ghidra and analysed with
the scripts in `tooling/ghidra_scripts/`. The scripts dump functions, callers, string xrefs,
immediates and selected decompilations to text so findings can be diffed and reviewed.

Conventions:

- Base address: `0x10000000`; file offset = `addr - 0x10000000`.
- Function boundaries and the ACTH header are documented in
  [app.bin internals](/watch-utils/devices/cmf-watch-pro-2/internals/appbin/).
- The BLE dispatcher, OTA state machine and UI message queue were traced by cross-referencing
  command constants (`0x9052`, `0x9040`, `0xA042`, ...) and queue calls.

## Dynamic analysis

- BLE traffic was captured and replayed against the watch with a Python/Bleak client.
- Shell/AT commands were used for identity, battery and sensor checks.
- The OTA flow was validated with a dry run (`--stop-after-a040`) before transferring data.
- A successful full flash was accepted only after the watch rebooted and the new animation
  played on the UI.

## Format recovery

For each container:

1. The magic, header fields and table are located.
2. The smallest shipped object is decoded.
3. It is re-encoded and compared **byte-for-byte** with the original.
4. Only then is a field edited or a payload replaced.
5. The outer container CRC, entry CRCs and partition checksums are verified.

The regression suite (`tooling/tests/selftest.py`) encodes this discipline. It requires a
legitimate firmware image supplied locally; the image is not redistributed here.

## Cross-checking

The public [`freethinkel/fmc`](https://github.com/freethinkel/fmc) project (AGPL-3.0) was used
as an independent reference for the frame header, AES/CRC rules, `.wfdl` layout and the
watch-face upload handshake. Where older assumptions differed, live-verified FMC behaviour took
precedence. The watch-face uploader is a behavioural port, which is why this repository is
AGPL-3.0.

## Reproducing a result

- Offline: the selftest is run against a locally held firmware image, followed by
  `fwkit info/verify`.
- BLE read-only: `ota-state`, `info` or shell commands.
- BLE write: a dry run precedes the write, and the old partition/image is retained.
- Firmware OTA: the [AOTA OTA flow](/watch-utils/devices/cmf-watch-pro-2/protocol/ota/) is followed exactly; INIT2
  is not reordered and the end acknowledgement is not skipped.
