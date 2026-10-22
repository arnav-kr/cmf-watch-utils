---
title: Flash an AOTA image
description: A careful, checkpointed procedure for a full firmware update over BLE.
---

This is the procedure that worked on a `1.0.0.73` / `JX402` unit. The prerequisites are
[AOTA OTA flow](/watch-utils/devices/cmf-watch-pro-2/protocol/ota/) and
[Limitations & risks](/watch-utils/devices/cmf-watch-pro-2/limitations/).

## 0. Rollback preparation

```bash
mkdir -p backups
cp firmware/JX402_V1.0.0.73.bin backups/stock.bin
uv run fwkit verify backups/stock.bin
uv run fwkit verify firmware/custom.bin
```

The stock image is kept somewhere outside the watch. There is no guaranteed BLE unbrick.

## 1. Charging and disconnection

- The watch is charged; the procedure does not start below ~50%.
- The watch is disconnected from the phone app and phone Bluetooth is turned off.
- The host stays awake and close to the watch, with sleep disabled.

## 2. Pairing state

If the watch was reset or the factory menu cleared app data, the old authkey is stale.

```bash
uv run python tools/cmf_watch.py pair
```

The session is checked before any write:

```bash
uv run python tools/cmf_watch.py info
```

## 3. Dry run to `A040`

```bash
uv run python tools/ota_client.py --stop-after-a040 --version 0100005a -v
```

Required checkpoints:

- `A052 01` after INIT1.
- `A040 01` immediately after INIT2.
- No immediate disconnect.

An `A052` without a following `A040` means the pairing state is not yet correct. A link that
drops right after INIT2 points to the client writing `0x9040` to `...ff01` instead of
`CMD_W`.

## 4. Full transfer

```bash
uv run python tools/ota_client.py \
  --image firmware/custom.bin \
  --version 0100005a \
  -v
```

Expected behaviour:

- The watch requests ranges; offsets can jump.
- Progress is relative to the whole image and may finish well under 100%.
- `A041 01` can arrive before the zero-length `A042`. The client sends `0x9041 A5`
  immediately.
- The watch reboots when the update is committed.

The transfer is left undisturbed: power stays connected, the lid stays open and no other BLE
client is started.

## 5. Verification

After the watch reboots:

```bash
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

The changed item (animation, scene, string, picture) is then checked. A watch that does not
come back is not reflashed; see
[Recovery](/watch-utils/devices/cmf-watch-pro-2/internals/recovery/).

## Interpreting the summary

A successful run can end at 70–75% of the file because the watch only requests changed
partitions. The important signals are:

- `A041` and the end ack were sent.
- Frames were served with no long idle gap.
- The watch rebooted and answers `info` on the new image.

## Failure modes

- **Disconnect during data:** the watch is allowed to reboot, `info` is checked, and the retry
  starts from pairing.
- **`A041` but no commit:** the `0x9041` payload `A5` is confirmed written; the end sequence
  is retried only after reconnecting and re-authenticating.
- **`no frames` after a few requests:** the watch aborted or rebooted. The log tail is
  inspected; repeated full transfers are avoided.
