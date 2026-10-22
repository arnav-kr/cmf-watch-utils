# watch-utils

Unofficial tooling and documentation for CMF Watch and other smartwatches. The project parses,
rebuilds and patches the firmware containers used by these devices, and talks to a watch over
BLE: pairing, the shell/AT channel, watch-face upload and full AOTA firmware updates.

The first supported device is the **CMF Watch Pro 2** (`JX402`, Actions ATS3089C, firmware
`1.0.0.73`). Device-specific research lives in its own section, so additional models can be
added without duplicating the tooling.

> Not affiliated with or endorsed by Nothing / CMF. Firmware modification can permanently
> damage a device. The device section documents the risks: see
> [`site/src/content/docs/devices/cmf-watch-pro-2/limitations.md`](site/src/content/docs/devices/cmf-watch-pro-2/limitations.md).

## Status

| Area | Result |
|---|---|
| AOTA / SDFS / RES / STY / WFDL / AJS parse and rebuild | verified offline, byte-identical round-trips |
| BLE authentication, shell/AT, watch-face upload | verified on hardware |
| Full AOTA firmware OTA over BLE | verified on hardware, the watch rebooted into the new image |
| Custom animation flashed and played | verified on hardware, 202-frame proof of concept |

## Layout

```text
tooling/src/fwkit/       format toolkit (AOTA, SDFS, RES, STY, AJS, WFDL, pictures, LZ4)
tooling/tests/           selftest.py, needs a firmware image
tooling/ghidra_scripts/  Ghidra analysis helpers
tools/cmf_watch.py       BLE CLI: scan / pair / info / AT / dials / OTA
tools/ota_client.py      full AOTA OTA client
tools/spp_ota.py         SPP / RFCOMM experiments
tools/rfcomm.py          RFCOMM helper
tools/maputil.py         address and offset helpers
site/                    Astro Starlight documentation site
dumps/                   manifests, downloadable firmware images, redacted notes
```

Documentation is organised per device:

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

## Install

```bash
uv sync
```

The workspace installs the `fwkit` CLI and the dependencies of the BLE tools.

## Use

```bash
# offline
uv run fwkit info   dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.0.73.bin
uv run fwkit unpack dumps/binaries/firmware/JX402_V1.0.0.73.bin work/aota

# BLE, read-only
uv run python tools/cmf_watch.py scan
uv run python tools/cmf_watch.py pair
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

## Dumps

Firmware images are published under `dumps/binaries/` and stored with Git LFS:

```bash
git lfs install
git lfs pull
```

The images are vendor firmware and vendor resources; the proof-of-concept image additionally
contains third-party animation frames. Redistribution rights are the responsibility of
whoever republishes them, and `dumps/binaries/` is the directory to remove before a public
release.

## Documentation

```bash
cd site
pnpm install
pnpm dev
```

The rendered site is published at <https://arnav.is-a.dev/watch-utils/>.

Key pages:

- [Project home](site/src/content/docs/index.mdx)
- [CMF Watch Pro 2](site/src/content/docs/devices/cmf-watch-pro-2/index.md)
- [Verified status](site/src/content/docs/devices/cmf-watch-pro-2/status.md)
- [AOTA OTA flow](site/src/content/docs/devices/cmf-watch-pro-2/protocol/ota.md)
- [Flash guide](site/src/content/docs/devices/cmf-watch-pro-2/guides/ota-flash.md)
- [Bad Apple case study](site/src/content/docs/devices/cmf-watch-pro-2/guides/bad-apple.md)
- [Roadmap and devices](site/src/content/docs/about/roadmap.md)
- [Agent context](site/src/content/docs/ai/context.md)

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

## License

AGPL-3.0. See [`LICENSE`](LICENSE) and
[Legal & attribution](site/src/content/docs/about/legal.md).
