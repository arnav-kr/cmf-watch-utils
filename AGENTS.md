# AGENTS.md

Guidance for AI coding agents (and humans) working in this repository.

## Mission

Maintain and extend CMF Watch Utils, the open tooling and documentation set for CMF Watch and other smartwatches:
parse and rebuild the firmware containers, talk to a device over BLE (authentication, shell
and AT commands, watch-face upload, firmware OTA), and document the findings per device.

## Hard rules

1. **No device writes without explicit approval from the repository owner.** Read-only BLE
   commands (`scan`, `info`, `at`, `ota-state`) are always acceptable.
2. No `Interceptor.attach` on Dart AOT (`libapp.so`); it crashes the target.
3. No blanket killing of Python processes; only the specific process that was started.
4. No firmware binaries, vendor resources, watch-face or animation media, or personal data in
   commits — except the published dumps under `dumps/binaries/`, which are tracked with Git
   LFS. Personal data means MAC address, serial number, pairing secret, authkey, session key,
   GPS tracks and notification contents.
5. The OTA sequence is not to be reordered: INIT2 goes to the OTA characteristic, and the end
   acknowledgement is mandatory.
6. Comments in code are added only on request. The surrounding style is matched.
7. Windows PowerShell has no `&&`; commands are chained with `; if ($?) { ... }`. There is no
   `rg`, `head` or `tail`; `Select-String` and `Select-Object` are used instead.

## Repository map

```text
tooling/src/fwkit/      image/AOTA, SDFS, RES, STY, AJS, WFDL, pictures, LZ4
tooling/tests/          selftest.py (needs a firmware image)
tooling/ghidra_scripts/ Ghidra analysis helpers
tools/cmf_watch.py      BLE CLI: scan/pair/info/AT/dials/OTA
tools/ota_client.py     full AOTA OTA client
tools/spp_ota.py        SPP/RFCOMM experiments
tools/rfcomm.py         RFCOMM helper
tools/maputil.py        address/offset helpers
site/                   Astro Starlight documentation
site/src/content/docs/  documentation content, organised per device
dumps/                  manifests, downloadable images, redacted notes
```

Documentation layout:

```text
docs/index.mdx               project home and device matrix
docs/about/                  project-wide documentation
docs/getting-started/        host setup and first steps
docs/tooling/                fwkit and BLE tool references
docs/devices/<model>/        protocol, formats, internals, guides for one device
docs/ai/                     agent-facing briefings
docs/dumps/                  manifests and downloadable images
```

## Environment

- Python 3.12+, uv workspace at the repository root: `uv sync`.
- CLI: `uv run fwkit <cmd>`; tools: `uv run python tools/cmf_watch.py <cmd>`.
- Documentation: `cd site && pnpm install && pnpm dev`; build: `pnpm build`.
- Tested on Windows 11 with Bleak. Other hosts are untested; MTU behaviour may differ.
- Binaries under `dumps/binaries/` are Git LFS objects; `git lfs install` is required before
  cloning or pulling them.

## Established facts for the CMF Watch Pro 2

- AOTA: 0x400-byte header, 32-byte entries at 0x200, LZMA chunk streams, nested partitions
  (`TEMP.bin`, `res.bin`, `fonts.bin`, `res_e.bin`, `sdfs_k.bin`).
- SDFS: `sdfs.bin` magic, 0x20-byte header, 32-byte entries, 0x40 alignment, `sum32`
  checksums (not CRC32).
- `res_e.bin` partition length is fixed at **58,777,088 B**; stock usage is 58,776,014 B.
- App: `app.bin` at base `0x10000000`, Thumb-2, ACTH header.
- BLE: MTU 23; payload frames are AES-CBC (fixed IV) with a CRC32-LE tail.
- Authentication: `authkey = sha256(r1||r2||secret)[:16]`,
  `session = sha256(nonce||authkey)[:16]`.
- OTA: `0x9052` → `A052 01`; `0x9040` **on `...ff01`** → `A040`; the watch pulls `A042`;
  the answer is `0x9042`; on `A041` the end is `0x9041` with payload `A5`.
- Watch faces: delete and append is the reliable update path.
- `.ajs`: 240×240 JPEG frames, fps byte `+0x07` (0 means 25); `9.ajs` is sport type 22, 8 s.
- Full Bad Apple (~18.75 MiB) exceeds the ~3.85 MiB free after replacing `9.ajs`.
- The bootloader is not part of the AOTA image; no universal recovery has been proven.

These values are specific to firmware `1.0.0.73` on board `JX402`. Other devices and builds
are re-derived rather than assumed, and are documented in their own section.

## Known unknowns

- ACTH checksum fields `+0x1C` / `+0x20` and `+0x0C = 0x62`.
- `.sty` type-2 `+0x18` font and style encoding.
- `album.cfg`, the `.wfdl` header byte `+0x17`, several SPP opcodes.
- `0xA055` capacity byte against the actual slot count; side-loaded dial ids are not listed.
- The UART recovery path is unverified.

## Working rules for changes

1. Format work proves a byte-identical round-trip before any edit.
2. Protocol work adds a dry-run path first; device writes require approval.
3. The `fwkit` CLI output stays stable; the existing `cmd_*` plus argparse pattern is
   followed.
4. Documentation pages carry `title` and `description` frontmatter, and internal links use
   `/<page>/`. `pnpm build` in `site/` must pass with no broken links.
5. A new device gets its own section under `docs/devices/<model>/`, its own sidebar group and
   its own entry in the device matrix and the header switcher. Shared tooling stays
   device-agnostic.

## Verification

```bash
uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run python tooling/tests/selftest.py dumps/binaries/firmware/JX402_V1.0.0.73.bin
cd site && pnpm build
```

## PII policy

The placeholder MAC is `AA:BB:CC:DD:EE:FF`. Real MAC addresses, serial numbers, pairing
secrets, authkeys, session keys, GPS tracks and notification contents are stripped from logs,
screenshots, issues and commits. `tools/.watch_state.json` is gitignored and never published.
