<p align="center">
  <img src="./logo.svg" alt="CMF Watch Utils" width="120" height="120" />
</p>

<h1 align="center">CMF Watch Utils</h1>

<p align="center">
  Tooling and documentation for <b>CMF Watch</b> and other smartwatches — firmware container
  toolkit, BLE tooling and per-device reverse-engineering notes.
</p>

<p align="center">
  <a href="https://cmf.arnv.dev/"><img alt="Docs" src="https://img.shields.io/badge/docs-cmf.arnv.dev-FB471F?style=for-the-badge&logo=readthedocs&logoColor=white" /></a>
  <a href="https://cmf.arnv.dev/devices/cmf-watch-pro-2/"><img alt="CMF Watch Pro 2" src="https://img.shields.io/badge/device-CMF%20Watch%20Pro%202-black?style=for-the-badge" /></a>
  <a href="https://github.com/arnav-kr/cmf-watch-utils/actions"><img alt="CI" src="https://github.com/arnav-kr/cmf-watch-utils/actions/workflows/ci.yml/badge.svg" /></a>
  <a href="https://github.com/arnav-kr/cmf-watch-utils/actions"><img alt="Pages" src="https://github.com/arnav-kr/cmf-watch-utils/actions/workflows/pages.yml/badge.svg" /></a>
  <a href="LICENSE"><img alt="AGPL-3.0" src="https://img.shields.io/badge/license-AGPL--3.0-blue?style=for-the-badge" /></a>
  <a href="https://github.com/arnav-kr/cmf-watch-utils"><img alt="Stars" src="https://img.shields.io/github/stars/arnav-kr/cmf-watch-utils?style=for-the-badge" /></a>
  <a href="https://github.com/arnav-kr/cmf-watch-utils"><img alt="Forks" src="https://img.shields.io/github/forks/arnav-kr/cmf-watch-utils?style=for-the-badge" /></a>
</p>

<p align="center">
  <a href="https://cmf.arnv.dev/"><strong>Read the documentation</strong></a> ·
  <a href="https://cmf.arnv.dev/llms.txt"><code>llms.txt</code></a> ·
  <a href="https://github.com/arnav-kr">arnav-kr</a> ·
  <a href="https://arnv.dev">arnv.dev</a>
</p>

---

> Unofficial interoperability project. Not affiliated with or endorsed by Nothing / CMF.
> Firmware modification can permanently damage a device — read the
> [limitations page](https://cmf.arnv.dev/devices/cmf-watch-pro-2/limitations/) before flashing
> anything.

## What it does

| Area | Result |
|---|---|
| AOTA / SDFS / RES / STY / WFDL / AJS parse and rebuild | verified offline, byte-identical round-trips |
| BLE authentication, shell/AT, watch-face upload | verified on hardware |
| Full AOTA firmware OTA over BLE | verified on hardware, the watch rebooted into the new image |
| Custom animation flashed and played | verified on hardware, 202-frame proof of concept |
| Firmware `1.0.0.57` rollback image | published, rebuilt from extracted parts, all CRCs verify |

## Install

```bash
git clone https://github.com/arnav-kr/cmf-watch-utils.git
cd cmf-watch-utils
uv sync
```

Or in one line:

```bash
curl -fsSL https://raw.githubusercontent.com/arnav-kr/cmf-watch-utils/main/scripts/install.sh | bash
```

```powershell
irm https://raw.githubusercontent.com/arnav-kr/cmf-watch-utils/main/scripts/install.ps1 | iex
```

The scripts clone the repository, install the workspace with `uv` and pull the published
firmware images with Git LFS.

## Contents

- [What it does](#what-it-does)
- [Install](#install)
- [Use](#use)
- [Downloads](#downloads)
- [Layout](#layout)
- [Documentation](#documentation)
- [The three OTA rules that decide the outcome](#the-three-ota-rules-that-decide-the-outcome)
- [Testing](#testing)
- [Attribution](#attribution)
- [License](#license)

## Use

```bash
# offline: parse and verify a firmware image
uv run fwkit info   dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run fwkit unpack dumps/binaries/firmware/JX402_V1.0.0.73.bin work/aota

# BLE, read-only first
uv run python tools/cmf_watch.py scan
uv run python tools/cmf_watch.py pair
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

## Downloads

All images are stored with Git LFS. `git lfs install` is required before cloning.

| File | Size | SHA-256 (first 16) | Description |
|---|---:|---|---|
| [`JX402_V1.0.0.73.bin`](https://media.githubusercontent.com/media/arnav-kr/cmf-watch-utils/main/dumps/binaries/firmware/JX402_V1.0.0.73.bin) | 56.9 MB | `30903fc8ce6db4f8` | Stock factory firmware, `1.0.0.73` |
| [`JX402_V1.0.0.57.bin`](https://media.githubusercontent.com/media/arnav-kr/cmf-watch-utils/main/dumps/binaries/firmware/JX402_V1.0.0.57.bin) | 56.1 MB | `cd4574f1f9fa3110` | Rollback target, `1.0.0.57` |
| [`JX402_V1.0.0.73_badapple9.bin`](https://media.githubusercontent.com/media/arnav-kr/cmf-watch-utils/main/dumps/binaries/firmware/JX402_V1.0.0.73_badapple9.bin) | 54.1 MB | `4933e593d14bef6f` | Rebuilt image with the animation proof of concept |
| [`app_073.bin`](https://media.githubusercontent.com/media/arnav-kr/cmf-watch-utils/main/dumps/binaries/app/app_073.bin) | 2.4 MB | `30e7978c39d8fb79` | `app.bin` from the `1.0.0.73` image |
| [`app_057.bin`](https://media.githubusercontent.com/media/arnav-kr/cmf-watch-utils/main/dumps/binaries/app/app_057.bin) | 2.4 MB | `26fbe34c10343949` | `app.bin` from the `1.0.0.57` image |

Manifests with full hashes and per-entry CRC status:
[`firmware-1.0.0.73.json`](dumps/manifests/firmware-1.0.0.73.json) ·
[`firmware-1.0.0.57.json`](dumps/manifests/firmware-1.0.0.57.json)

## Layout

```text
tooling/src/fwkit/       format toolkit (AOTA, SDFS, RES, STY, AJS, WFDL, pictures, LZ4)
tooling/tests/           selftest.py, needs a firmware image
tooling/ghidra_scripts/  Ghidra analysis helpers
tooling/redact.py        removes device identifiers from logs before publishing
tools/cmf_watch.py       BLE CLI: scan / pair / info / AT / dials / OTA
tools/ota_client.py      full AOTA OTA client
tools/spp_ota.py         SPP / RFCOMM experiments
tools/rfcomm.py          RFCOMM helper
tools/maputil.py         address and offset helpers
tools/legacy/            the original standalone unpack, decompress and xref scripts
tools/experiments/       one-off probes that established the pairing and OTA behaviour
site/                    Astro Starlight documentation site
dumps/                   firmware images, analysis output, logs, traces and samples
```

The documentation is organised per device, so new models can be added without duplicating the
tooling:

```text
site/src/content/docs/
  index.mdx               project home and device matrix
  about/                  project-wide documentation
  getting-started/        host setup and first steps
  tooling/                fwkit and BLE tool references
  devices/<model>/        protocol, formats, internals and guides for one device
  ai/                     agent-facing briefings
  dumps/                  manifests and downloadable images
```

## Documentation

```bash
cd site
pnpm install
pnpm dev
```

The site is served from the repository root, so the local preview is at
<http://localhost:4321/>. The published site is <https://cmf.arnv.dev/>.

Key pages:

- [CMF Watch Pro 2](https://cmf.arnv.dev/devices/cmf-watch-pro-2/) — device profile and page map
- [Verified status](https://cmf.arnv.dev/devices/cmf-watch-pro-2/status/)
- [AOTA OTA flow](https://cmf.arnv.dev/devices/cmf-watch-pro-2/protocol/ota/)
- [Flash guide](https://cmf.arnv.dev/devices/cmf-watch-pro-2/guides/ota-flash/)
- [Bad Apple case study](https://cmf.arnv.dev/devices/cmf-watch-pro-2/guides/bad-apple/)
- [Dumps and downloads](https://cmf.arnv.dev/dumps/)
- [Roadmap and devices](https://cmf.arnv.dev/about/roadmap/)
- [Agent context](https://cmf.arnv.dev/ai/context/)

For AI agents: [`/llms.txt`](https://cmf.arnv.dev/llms.txt) lists every page, and
[`/llms-full.txt`](https://cmf.arnv.dev/llms-full.txt) contains the full text.

## The three OTA rules that decide the outcome

1. INIT2 (`0x9040`) is written to the **OTA characteristic** `...ff01`. Sending it on `CMD_W`
   with a valid session can reboot the watch and drop the link.
2. The watch pulls data; the exact ranges it requests are served. Offsets are not monotonic.
3. On `0xA041`, `0x9041` with payload `A5` is sent. Without that end acknowledgement the
   update stays staged and is never committed.

## Testing

```bash
uv run python tooling/tests/selftest.py dumps/binaries/firmware/JX402_V1.0.0.73.bin
cd site && pnpm build
```

## Attribution

- [arnav-kr](https://github.com/arnav-kr) — [arnv.dev](https://arnv.dev)
- The watch-face upload behaviour was informed by the AGPL-3.0
  [`freethinkel/fmc`](https://github.com/freethinkel/fmc) project.
- Bad Apple is a copyrighted work; no animation frames are published in the documentation.
  The proof-of-concept image contains them, so the redistribution terms of that work apply to
  that file.

<p align="center">
  <a href="https://github.com/arnav-kr/cmf-watch-utils/graphs/contributors">
    <img alt="Contributors" src="https://img.shields.io/github/contributors/arnav-kr/cmf-watch-utils?style=for-the-badge" />
  </a>
</p>

## License

AGPL-3.0. See [`LICENSE`](LICENSE) and
[Legal & attribution](https://cmf.arnv.dev/about/legal/).

<p align="center">
  <img src="./og-banner.png" alt="CMF Watch Utils" width="640" />
</p>
