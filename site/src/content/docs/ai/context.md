---
title: Agent context
description: A compact, factual briefing for AI coding agents working in this repository.
---

## Mission

The scope of the project is open tooling and documentation for the **CMF Watch Pro 2**
(`JX402`, Actions ATS3089C, firmware `1.0.0.73`). The project parses and rebuilds
AOTA/SDFS/RES/STY/WFDL/AJS containers and talks to the watch over BLE (auth, shell/AT,
watch faces, firmware OTA).

## Hard rules

1. **No device writes without explicit approval from the repository owner.** Read-only BLE
   commands are unconstrained.
2. `Interceptor.attach` on Dart AOT (`libapp.so`) crashes the target and is therefore excluded.
3. Blanket termination of Python processes is excluded; termination applies only to the specific
   process that was started.
4. Commits exclude firmware binaries, vendor resources, animation/watch-face assets and any
   personal data: MAC, serial, secret, authkeys, session keys, logs with device ids.
5. The OTA sequence is not reordered. INIT2 goes to the OTA characteristic; the end ack is
   required.
6. Comments are added to code only on request. The surrounding style is matched.

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
- `uv sync`; the CLI runs as `uv run fwkit ...` and tools as
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
- Sending INIT2 on `CMD_W` first with a valid session can reboot the watch.
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

## Verification

```bash
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
uv run fwkit info   firmware/JX402_V1.0.0.73.bin
uv run python tooling/tests/selftest.py --firmware firmware/JX402_V1.0.0.73.bin
cd site; pnpm build
```

Format work proves a byte-identical round-trip before any edit. Protocol work uses a dry run
(`--stop-after-a040`); writes to the watch require approval.

## PII policy

The placeholder MAC is `AA:BB:CC:DD:EE:FF`. Real MAC/serial/secret/authkey/GPS/notification
data is absent from logs, screenshots, issues and commits. `tools/.watch_state.json` is
gitignored and never published.
