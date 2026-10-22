---
title: CMF Watch Pro 2
description: Device profile, verified capabilities and page map for the CMF Watch Pro 2 (JX402, Actions ATS3089C).
---

The CMF Watch Pro 2 is the first device covered by this project. It runs the Actions
ATS3089C platform on board `JX402`, ships as firmware `1.0.0.73` and exposes a BLE command
service, a watch-face upload channel and an AOTA firmware update path.

## Profile

| Property | Value |
|---|---|
| Model | CMF Watch Pro 2 |
| Board | `jx402_01_3089c` |
| SoC | Actions ATS3089C (Cortex-M based, Thumb-2) |
| Firmware | `1.0.0.73` (AOTA version string `1.00_2509131556`) |
| Application | `app.bin`, base address `0x10000000`, ACTH header |
| Display | 240×240 |
| Storage | `/NAND:E` resource partition, fixed 58,777,088 bytes |
| BLE | MTU 23 observed; payload frames are AES-CBC encrypted with a CRC32-LE tail |

## Verified capabilities

| Area | Result |
|---|---|
| Shell/AT channel | Status, identity, sensor and factory commands answered |
| BLE authentication | Pairing, nonce exchange and session key derivation working |
| AOTA parse and rebuild | Byte-identical rebuild of the stock image |
| SDFS, RES, STY, WFDL, AJS | Parse, edit and rebuild with byte-identical round-trips |
| Watch-face upload | Install and delete over BLE, verified on hardware |
| AJS animation replacement | Custom animation played on the watch UI |
| Full AOTA firmware OTA | Image transferred, committed and booted on the watch |

The detailed result table, with the evidence behind each entry, is on the
[status page](/watch-utils/devices/cmf-watch-pro-2/status/).

## Page map

**Protocol**

- [BLE protocol](/watch-utils/devices/cmf-watch-pro-2/protocol/ble/) — frames, keys, characteristic layout, notification streams
- [AOTA OTA flow](/watch-utils/devices/cmf-watch-pro-2/protocol/ota/) — the verified update sequence
- [Shell / AT channel](/watch-utils/devices/cmf-watch-pro-2/protocol/shell/) — command family and pairing secret retrieval

**Formats**

- [AOTA container](/watch-utils/devices/cmf-watch-pro-2/formats/aota/)
- [SDFS filesystem](/watch-utils/devices/cmf-watch-pro-2/formats/sdfs/)
- [Watch faces & WFDL](/watch-utils/devices/cmf-watch-pro-2/formats/watchface/)
- [UI style bundles](/watch-utils/devices/cmf-watch-pro-2/formats/style/)
- [Picture codecs](/watch-utils/devices/cmf-watch-pro-2/formats/pictures/)
- [AJS animations](/watch-utils/devices/cmf-watch-pro-2/formats/ajs/)

**Internals**

- [Hardware](/watch-utils/devices/cmf-watch-pro-2/internals/hardware/)
- [app.bin internals](/watch-utils/devices/cmf-watch-pro-2/internals/appbin/)
- [Recovery & boot modes](/watch-utils/devices/cmf-watch-pro-2/internals/recovery/)

**Guides**

- [Firmware OTA flash](/watch-utils/devices/cmf-watch-pro-2/guides/ota-flash/)
- [Dial editing & upload](/watch-utils/devices/cmf-watch-pro-2/guides/dial-editing/)
- [Bad Apple case study](/watch-utils/devices/cmf-watch-pro-2/guides/bad-apple/)
- [Ghidra workflow](/watch-utils/devices/cmf-watch-pro-2/guides/ghidra/)

## Boundaries

The constants, offsets and BLE behaviour documented here belong to this firmware build.
Other models and other builds are covered by separate sections; see the
[roadmap](/watch-utils/about/roadmap/).

Risks specific to this device — flashing, storage ceilings, unresolved formats — are on the
[limitations page](/watch-utils/devices/cmf-watch-pro-2/limitations/).
