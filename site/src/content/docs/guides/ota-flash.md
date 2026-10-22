---
title: Flash an AOTA image
description: A careful, checkpointed procedure for a full firmware update over BLE.
---

This is the procedure that worked on a `1.0.0.73` / `JX402` unit. Read
[AOTA OTA flow](/cmf-watch-utils/protocol/ota/) and
[Limitations & risks](/cmf-watch-utils/overview/limitations/) first.

## 0. Prepare rollback

```bash
mkdir -p backups
cp firmware/JX402_V1.0.0.73.bin backups/stock.bin
uv run fwkit verify backups/stock.bin
uv run fwkit verify firmware/custom.bin
```

Keep the stock image somewhere outside the watch. There is no guaranteed BLE unbrick.

## 1. Charge and disconnect

- Charge the watch; do not start below ~50%.
- Disconnect it from the phone app and turn off phone Bluetooth.
- Keep the host awake and close to the watch. Disable sleep.

## 2. Refresh pairing state

If the watch was reset or the factory menu cleared app data, the old authkey is stale.

```bash
uv run python tools/cmf_watch.py pair
```

Check the session before any write:

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

If you see `A052` but no `A040`, stop and fix pairing. If the link drops right after INIT2,
verify the client is writing `0x9040` to `...ff01`, not `CMD_W`.

## 4. Full transfer

```bash
uv run python tools/ota_client.py \
  --image firmware/custom.bin \
  --version 0100005a \
  -v
```

What is normal:

- The watch requests ranges; offsets can jump.
- Progress is relative to the whole image and may finish well under 100%.
- `A041 01` can arrive before the zero-length `A042`. The client sends `0x9041 A5`
  immediately.
- The watch reboots when the update is committed.

Do not unplug, close the lid, or start another BLE client during the transfer.

## 5. Verify

After the watch reboots:

```bash
uv run python tools/cmf_watch.py info
uv run python tools/cmf_watch.py ota-state --duration 5
```

Then check the thing you changed (animation, scene, string, picture). If the watch does not
come back, do not start another flash; see [Recovery](/cmf-watch-utils/internals/recovery/).

## Interpreting the summary

A successful run can end at 70–75% of the file because the watch only requests changed
partitions. The important signals are:

- `A041` and the end ack were sent.
- Frames were served with no long idle gap.
- The watch rebooted and answers `info` on the new image.

## If it fails

- **Disconnect during data:** wait for reboot, check `info`, then retry from pairing.
- **`A041` but no commit:** ensure `0x9041` payload `A5` was actually written; retry the
  end sequence only after reconnecting and re-authenticating.
- **`no frames` after a few requests:** the watch aborted or rebooted. Inspect the log tail;
  do not loop full transfers.
