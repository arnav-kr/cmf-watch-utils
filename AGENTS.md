# AGENTS.md

Guidance for AI coding agents (and humans) working in this repository.

## Mission

Maintain and extend open tooling and documentation for the **CMF Watch Pro 2**
(`JX402`, Actions ATS3089C, firmware `1.0.0.73`): parse/rebuild AOTA, SDFS, RES, STY, WFDL and
AJS containers, and talk to the watch over BLE (auth, shell/AT, watch faces, firmware OTA).

## Hard rules

1. **Never flash, upload, delete or change device settings without explicit user approval.**
   Read-only BLE commands (`scan`, `info`, `at`, `ota-state`) are fine.
2. Never use `Interceptor.attach` on Dart AOT (`libapp.so`); it crashes the target.
3. Never blanket-kill Python processes; stop only the specific process you started.
4. Never commit firmware binaries, vendor resources, watch-face or animation media, or personal
   data: MAC, serial, secret, authkeys, session keys, GPS tracks, notification contents.
5. Never reorder the OTA sequence. INIT2 goes to the OTA characteristic; the end ack is
   mandatory.
6. Do not add code comments unless asked. Match the surrounding style.
7. PowerShell on Windows has no `&&`; chain with `; if ($?) { ... }`. There is no `rg`, `head` or
   `tail` — use `Select-String` and `Select-Object`.

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

- Python 3.12+, uv workspace at the repo root: `uv sync`.
- CLI: `uv run fwkit <cmd>`; tools: `uv run python tools/cmf_watch.py <cmd>`.
- Docs: `cd site && pnpm install && pnpm dev` (build: `pnpm build`).
- Tested on Windows 11 with Bleak. Other hosts are untested; MTU behaviour may differ.

## Established facts

- AOTA: 0x400-byte header, 32-byte entries at 0x200, LZMA chunk streams, nested partitions
  (`TEMP.bin`, `res.bin`, `fonts.bin`, `res_e.bin`, `sdfs_k.bin`).
- SDFS: `sdfs.bin` magic, 0x20-byte header, 32-byte entries, 0x40 alignment, `sum32`
  checksums (not CRC32).
- `res_e.bin` partition length is fixed at **58,777,088 B**; stock usage is 58,776,014 B.
- App: `app.bin` at base `0x10000000`, Thumb-2, ACTH header.
- BLE: MTU 23; payload frames are AES-CBC (fixed IV) with a CRC32-LE tail.
- Auth: `authkey = sha256(r1||r2||secret)[:16]`, `session = sha256(nonce||authkey)[:16]`.
- OTA: `0x9052` -> `A052 01`; `0x9040` **on `...ff01`** -> `A040`; watch pulls `A042`; answer
  `0x9042`; on `A041` send `0x9041` with payload `A5`.
- Watch faces: delete + append is the reliable update path.
- `.ajs`: 240x240 JPEG frames, fps byte `+0x07` (0 => 25); `9.ajs` is sport type 22, 8 s.
- Full Bad Apple (~18.75 MiB) does not fit the ~3.85 MiB free after replacing `9.ajs`.
- The bootloader is not part of the AOTA image; there is no proven universal recovery.

## Known unknowns

- ACTH checksum fields `+0x1C` / `+0x20` and `+0x0C = 0x62`.
- `.sty` type-2 `+0x18` font/style encoding.
- `album.cfg`, `.wfdl` header byte `+0x17`, several SPP opcodes.
- `0xA055` capacity byte vs. actual slots; side-loaded dial ids are not listed.
- UART recovery path is unverified.

## Working rules for changes

1. Format work: prove a byte-identical round-trip before editing anything.
2. Protocol work: add a dry-run path first; never write to the watch without approval.
3. Keep `fwkit` CLI output stable; follow the existing `cmd_*` + argparse pattern.
4. Documentation: frontmatter needs `title` and `description`; internal links use site-relative
   paths. Run `pnpm build` in `site/` and fix broken links.
5. Verify with:

```bash
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
uv run python tooling/tests/selftest.py firmware/JX402_V1.0.0.73.bin
cd site && pnpm build
```

## PII policy

Use the placeholder MAC `AA:BB:CC:DD:EE:FF`. Strip real MAC/serial/secret/authkey/session key
data, GPS tracks and notification contents from logs, screenshots, issues and commits.
`tools/.watch_state.json` is gitignored and must never be published.
