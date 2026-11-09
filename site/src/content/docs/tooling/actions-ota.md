---
title: Actions SDK OTA
description: Select the Actions BLE OTA transport, validate packages offline, and interpret virtual and physical test results.
---

CMF Watch Utils supports two BLE OTA protocols for Watch Pro 2 firmware. The stock CMF client uses authenticated CMF frames and serves ranges requested by the stock app. The Actions SDK client uses its own service, packet framing and negotiation. Select the client based on the BLE service exposed by the running firmware.

| Path | CLI | Transport |
|---|---|---|
| Stock CMF | `uv run python tools/ota_client.py` | Stock BLE command and OTA characteristics. See [AOTA OTA flow](/devices/cmf-watch-pro-2/protocol/ota/). |
| Actions SDK | `uv run cmf-watch-utils ota actions` | Local package checks by default. An explicit address selects physical BLE. |
| Emulator | `uv run cmf-watch-utils emu ota --protocol stock` or `--protocol actions` | Loopback H4 only. See [OTA and recovery](/tooling/emulator-ota/). |

## Offline validation

```bash
uv run cmf-watch-utils ota actions --image local/actions-update.bin --output runs/actions-offline.json
```

With no address this checks the package and reports `offline_validated` without scanning or connecting. The client expects a standalone app/SDFS AOTA with an outer `TEMP.bin` and no CMF AGPS trailer. A stock download or an image that passed `fwkit verify` can still be incompatible with this transport.

## Negotiation and transfer

| Role | UUID |
|---|---|
| Service | `e49a25f8-f69a-11e8-8eb2-f2801f1b9fd1` |
| Acknowledged write | `e49a25e0-f69a-11e8-8eb2-f2801f1b9fd1` |
| Notify | `e49a28e1-f69a-11e8-8eb2-f2801f1b9fd1` |

On hardware, `--address` subscribes and negotiates, which can show the update screen even when no image bytes are sent. Treat negotiation as an OTA state change. Physical transfer requires both an explicit address and `--flash`. In QEMU, use `emu ota --protocol actions` and opt into data transfer with `--transfer`.

The host requests an upgrade and negotiates the unit size. During transfer it serves the peer's requested offsets using acknowledged writes. It accepts a success result after serving data and receiving the peer's validation response.

| Result | Evidence |
|---|---|
| `offline_validated` | Local package checks passed. |
| `negotiated_only` | The peer accepted negotiation. No image bytes were sent. |
| `watch_reported_image_valid` | The peer accepted the transferred image. Confirm installation separately by checking the reboot and running firmware. |

The integrated client negotiated with the Bad Apple QEMU guest at a 230-byte unit size and completed a transfer of the rollback image through validation. The integrated client's physical transfer and boot outcome remain unverified. The Actions service and its update screen alone do not establish an app-independent recovery mode. See [Recovery & boot modes](/devices/cmf-watch-pro-2/internals/recovery/).
