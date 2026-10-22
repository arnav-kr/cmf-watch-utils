---
title: Methodology
description: How the formats and protocol were recovered, and how to reproduce or extend the work.
---

## Principles

1. **Prefer reversible experiments.** Parse and rebuild the same bytes before changing anything.
2. **Separate hypotheses from facts.** A decompiler comment is a hypothesis until a round-trip,
   a live reply, or a boot proves it.
3. **Capture the app, not guesses.** The official app is the reference client for frame layout,
   keys, retries and ordering.
4. **Keep a rollback.** Save the image you are replacing and make the smallest possible diff.
5. **Do not publish personal data.** MAC addresses, serial numbers, pairing secrets and authkeys
   are stripped from this repository.

## Static analysis

The app image (`app.bin`, base `0x10000000`, Thumb-2) was loaded into Ghidra and analysed with
the scripts in `tooling/ghidra_scripts/`. The scripts dump functions, callers, string xrefs,
immediates and selected decompilations to text so findings can be diffed and reviewed.

Useful conventions:

- Base address: `0x10000000`; file offset = `addr - 0x10000000`.
- Function boundaries and the ACTH header are documented in
  [app.bin internals](/cmf-watch-utils/internals/appbin/).
- The BLE dispatcher, OTA state machine and UI message queue were traced by cross-referencing
  command constants (`0x9052`, `0x9040`, `0xA042`, ...) and queue calls.

## Dynamic analysis

- BLE traffic was captured and replayed against the watch with a Python/Bleak client.
- Shell/AT commands were used for identity, battery and sensor checks.
- The OTA flow was validated with a dry run (`--stop-after-a040`) before transferring data.
- A successful full flash was only accepted after the watch rebooted and the new animation
  played on the UI.

## Format recovery

For each container:

1. Locate the magic, header fields and table.
2. Decode the smallest shipped object.
3. Re-encode it and compare **byte-for-byte** with the original.
4. Only then edit a field or replace a payload.
5. Verify the outer container CRC, entry CRCs and partition checksums.

The regression suite (`tooling/tests/selftest.py`) encodes this discipline. It requires a
legitimate firmware image that you supply locally; the image is not redistributed here.

## Cross-checking

The public [`freethinkel/fmc`](https://github.com/freethinkel/fmc) project (AGPL-3.0) was used
as an independent reference for the frame header, AES/CRC rules, `.wfdl` layout and the
watch-face upload handshake. Where our older assumptions differed, live-verified FMC behaviour
won. Our watch-face uploader is a behavioural port, which is why this repository is AGPL-3.0.

## Reproducing a result

- Offline: run the selftest against your own firmware image, then use `fwkit info/verify`.
- BLE read-only: use `ota-state`, `info` or shell commands.
- BLE write: use a dry run first and keep the old partition/image.
- Firmware OTA: follow [AOTA OTA flow](/cmf-watch-utils/protocol/ota/) exactly; do not reorder
  INIT2 or skip the end acknowledgement.
