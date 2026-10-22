# dumps/

Artifacts published with the project: firmware manifests, downloadable images and redacted
notes.

## Contents

| Path | Description |
|---|---|
| `manifests/firmware-1.0.0.73.json` | Sizes, SHA-256 hashes, header fields and per-entry CRC status of the stock image, plus the published binaries |
| `binaries/firmware/JX402_V1.0.0.73.bin` | Stock factory firmware image for the CMF Watch Pro 2 (`JX402`, firmware 1.0.0.73) |
| `binaries/firmware/JX402_V1.0.0.57.bin` | Rollback target, firmware 1.0.0.57, rebuilt from unpacked parts |
| `binaries/firmware/JX402_V1.0.0.73_badapple9.bin` | Rebuilt image containing the 202-frame animation proof of concept |
| `binaries/app/app_073.bin` | `app.bin` extracted from the `TEMP` partition of the 1.0.0.73 image |
| `binaries/app/app_057.bin` | `app.bin` extracted from the `TEMP` partition of the 1.0.0.57 image |
| `analysis/` | Function list, function map, strings, string xrefs, block list, 231 decompiled listings, Ghidra run logs |
| `logs/` | OTA client logs of the successful and failed flashes, with device identifiers replaced |
| `traces/` | Captured notification frames used to map the protocol |
| `samples/` | A STY bundle, decoded picture samples, one WFDL watch-face package |
| `ota-flash-run.md` | Redacted transcript shape of a successful AOTA flash |

## Storage

The binaries are stored with **Git LFS**. Cloning requires LFS to be installed:

```bash
git lfs install
git clone https://github.com/arnav-kr/watch-utils.git
```

Fetching a single artifact without the full history:

```bash
git lfs pull --include="dumps/binaries/app/app_073.bin"
```

## Verification

```bash
sha256sum dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.0.73.bin
```

The manifest records the expected hash of each file.

## Redistribution notice

These images are vendor firmware and vendor resources. They are published for research and
interoperability on hardware that is owned. The proof-of-concept image additionally contains
third-party animation frames.

Before making the repository public, either remove `dumps/binaries/` or confirm that
redistribution rights exist. The rest of the project — tooling, documentation and manifests —
carries no such restriction.
