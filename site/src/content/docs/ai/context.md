---
title: Agent context
description: A compact, factual briefing for AI coding agents working in this repository.
---

## Mission

Maintain and extend open tooling/documentation for the **CMF Watch Pro 2** (`JX402`, Actions
ATS3089C, firmware `1.0.0.73`). The project parses and rebuilds AOTA/SDFS/RES/STY/WFDL/AJS
containers and talks to the watch over BLE (auth, shell/AT, watch faces, firmware OTA).

## Hard rules

1. **Never flash, upload, delete or change device settings without explicit user approval.**
   Read-only BLE commands are fine.
2. Never use `Interceptor.attach` on Dart AOT (`libapp.so`); it crashes the target.
3. Never blanket-kill Python processes; stop only the specific process you started.
4. Never commit firmware binaries, vendor resources, animation/watch-face assets, or any
   personal data: MAC, serial, secret, authkeys, session keys, logs with device ids.
5. Do not reorder the OTA sequence. INIT2 goes to the OTA characteristic; the end ack is
   required.
6. Do not add comments to code unless asked. Match existing style.

## Repository map

```text
tooling/src/fwkit/      image/AOTA, SDFS, RES, STY, AJS, WFDL, pictures, LZ4
tooling/tests/          selftest.py (needs a local firmware image)
tooling/ghidra_scripts/ Ghidra analysis helpers
tools/cmf_watch.py      BLE CLI: scan/pair/info/AT/dials/OTA
tools/ota_client.py     full AOTA OTA client
tools/spp_ota.py        SPP/RFCOMM experiments
tools/rfcomm.py         RFCOMM helper
tools/maputil.py        address/offset helpers
site/                   Astro Starlight documentation
dumps/                  manifests and redacted notes (no binaries)
```

## Environment

- Python **3.12+**, uv workspace at the repo root.
- `uv sync`; run the CLI as `uv run fwkit ...` and tools as
  `uv run python tools/cmf_watch.py ...`.
- Docs: `cd site; pnpm install; pnpm dev` / `pnpm build`.
- Host tested on Windows 11; Bleak is cross-platform but MTU/behaviour may differ.

## Established facts

- AOTA: 0x400-byte header, 32-byte entries, LZMA chunks, nested partitions.
- SDFS: `sdfs.bin`, 0x20-byte header, 32-byte entries, 0x40 alignment, `sum32` checksums.
- `res_e.bin`: fixed **58,777,088 B**; stock uses 58,776,014 B.
- App: `app.bin` at base `0x10000000`, Thumb-2, ACTH header.
- BLE: MTU 23; frames are AES-CBC, fixed IV, CRC32-LE tail.
- Auth: `authkey = sha256(r1||r2||secret)[:16]`,
  `session = sha256(nonce||authkey)[:16]`.
- OTA: `0x9052` → `A052 01`; `0x9040` **on `...ff01`** → `A040`; watch pulls `A042`;
  answer `0x9042`; at `A041` send `0x9041 A5`.
- Do **not** send INIT2 on `CMD_W` first with a valid session; it can reboot the watch.
- Watch-face update: delete + append is the reliable path.
- `.ajs`: 240×240 JPEG frames, fps at `+0x07` (0 => 25); `9.ajs` is sport type 22, 8 s.
- Full Bad Apple (~18.75 MiB) does not fit the ~3.85 MiB free after replacing `9.ajs`.
- No proven universal recovery; bootloader is not in AOTA.

## Known unknowns

- ACTH checksum fields `+0x1C`/`+0x20` and `+0x0C = 0x62`.
- `.sty` type-2 `+0x18` font/style encoding.
- `album.cfg`, `.wfdl +0x17`, several SPP opcodes.
- `0xA055` capacity byte vs. actual dial slots; side-loaded dial ids are not listed.
- UART recovery path is unverified.

## How to verify changes

```bash
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
uv run fwkit info   firmware/JX402_V1.0.0.73.bin
uv run python tooling/tests/selftest.py --firmware firmware/JX402_V1.0.0.73.bin
cd site; pnpm build
```

For format work, always prove a byte-identical round-trip before editing. For protocol work,
use a dry run (`--stop-after-a040`) and do not write to the watch unless approved.

## PII policy

Placeholder MAC: `AA:BB:CC:DD:EE:FF`. Remove real MAC/serial/secret/authkey/GPS/notification
data from logs, screenshots, issues and commits. `tools/.watch_state.json` is gitignored and
must never be published.
