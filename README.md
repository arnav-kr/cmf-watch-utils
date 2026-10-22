# cmf-watch-utils

Unofficial tooling and documentation for the **CMF Watch Pro 2** (board `JX402`, Actions
ATS3089C, firmware `1.0.0.73`).

This project parses, rebuilds and patches the watch's firmware containers, and talks to the
watch over BLE: pairing, the shell/AT channel, watch-face upload and full AOTA firmware
updates.

> Not affiliated with or endorsed by Nothing / CMF. Firmware modification can permanently
> damage a device. Read [`site/src/content/docs/overview/limitations.md`](site/src/content/docs/overview/limitations.md)
> before writing anything to hardware.

## What works

| Area | Status |
|---|---|
| AOTA / SDFS / RES / STY / WFDL / AJS parse + rebuild | verified offline (byte-identical round-trips) |
| BLE auth, shell/AT, watch-face upload | verified on hardware |
| Full AOTA firmware OTA over BLE | verified on hardware (watch rebooted into the new image) |
| Custom animation flashed and played | verified on hardware (202-frame proof of concept) |

## Layout

```text
tooling/src/fwkit/       format toolkit (AOTA, SDFS, RES, STY, AJS, WFDL, pictures, LZ4)
tooling/tests/           selftest.py (needs a local firmware image)
tooling/ghidra_scripts/  Ghidra analysis helpers
tools/cmf_watch.py       BLE CLI: scan / pair / info / AT / dials / OTA
tools/ota_client.py      full AOTA OTA client
tools/spp_ota.py         SPP / RFCOMM experiments
tools/rfcomm.py          RFCOMM helper
tools/maputil.py         address and offset helpers
site/                    Astro Starlight documentation site
dumps/                   manifests, hashes and redacted notes (no binaries)
```

## Install

```bash
uv sync
```

## Use

```bash
# offline
uv run fwkit info   firmware/JX402_V1.0.0.73.bin
uv run fwkit verify firmware/JX402_V1.0.0.73.bin
uv run fwkit unpack firmware/JX402_V1.0.0.73.bin work/aota

# BLE (read-only first)
uv run python tools/cmf_watch.py scan
uv run python tools/cmf_watch.py pair
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

Firmware images are **not** included. Put your own legally obtained image in `firmware/`
(gitignored).

## Documentation

```bash
cd site
pnpm install
pnpm dev
```

The rendered site is published at <https://arnav-kr.github.io/cmf-watch-utils/>.

Key pages:

- [What works](site/src/content/docs/overview/achievements.md)
- [Limitations & risks](site/src/content/docs/overview/limitations.md)
- [AOTA OTA flow](site/src/content/docs/protocol/ota.md)
- [Flash guide](site/src/content/docs/guides/ota-flash.md)
- [Bad Apple case study](site/src/content/docs/guides/bad-apple.md)
- [Agent context](site/src/content/docs/ai/context.md)

## The three OTA rules that matter

1. Send INIT2 (`0x9040`) on the **OTA characteristic** `...ff01`. Sending it on `CMD_W` with a
   valid session can reboot the watch and drop the link.
2. The watch pulls data; serve exactly the ranges it asks for. Offsets are not monotonic.
3. On `0xA041`, send `0x9041` with payload `A5`. Without it the update stays staged and is
   never committed.

## Testing

```bash
uv run python tooling/tests/selftest.py firmware/JX402_V1.0.0.73.bin
```

## License

AGPL-3.0. See [`LICENSE`](LICENSE) and
[Legal & attribution](site/src/content/docs/overview/legal.md).
