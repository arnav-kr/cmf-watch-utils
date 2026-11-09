---
title: BLE tools
description: Reference for cmf_watch.py and ota_client.py, the host-side tools that talk to the watch.
---

The existing stock scripts use [Bleak](https://github.com/hbldh/bleak). Physical tests ran
on Windows. For the separate Actions SDK protocol, see [Actions SDK OTA](/tooling/actions-ota/).
For OTA over a simulated radio, see [Emulator CLI](/tooling/emulator/).

```bash
uv run python tools/cmf_watch.py --help
uv run python tools/ota_client.py --help
```

## cmf_watch.py

### Read-only commands

| Command | Purpose |
|---|---|
| `scan` | List nearby BLE advertisements |
| `at <command>` | Send an AT command over the shell channel and print the reply |
| `info` | Version, serial, name, PID, battery, temperature, accelerometer |
| `sync` | Observe the notification stream for a while |
| `workouts` | Dump the workout/sport configuration tables |
| `specs` | Device specification strings |
| `wf-dials` | List installed watch faces |
| `ota-state` | Observe the OTA characteristic without writing firmware data |

### Write commands

| Command | Purpose |
|---|---|
| `ota-init` | Send the OTA init frame and observe the reply |
| `pair` | Full pairing exchange; writes the local auth state |
| `wf-flash --file <wfdl>` | Install a watch face (`--dry-run` prints the plan only) |
| `wf-delete --id <id> --file <wfdl>` | Remove an installed face |
| `ota-flash --image <bin>` | Legacy OTA path used during the earlier experiments |
| `ota-push`, `ota-arm`, `ota-dtry`, `ota-resume`, `ota-run` | Experiment variants used while the OTA flow was being mapped |

### Local state

`tools/.watch_state.json` holds the pairing secret and authkey. It is gitignored. A stale
authkey still produces an `A052` reply after INIT1, so `A052` alone does not prove the session
is usable; the data phase only starts with a fresh pairing.

## ota_client.py

The dedicated AOTA OTA client.

| Flag | Purpose |
|---|---|
| `--image <file>` | Firmware image to serve (defaults to the stock image under `firmware/`) |
| `--version <hex>` | 4-byte version argument of INIT2, for example `0100005a` |
| `--stop-after-a040` | Stop once the watch acknowledges INIT2; no data is sent |
| `--dry-run` | Connect, authenticate and stop after INIT1 acknowledgement. No firmware bytes sent. |
| `--pace <s>` | Delay between write batches |
| `--pipeline <n>` | Frames written concurrently per batch (default 4) |
| `--idle-timeout <s>` | Abort after a period without requests (default 120 s) |
| `-v` | Per-request logging |

The client sends INIT1 on the command characteristic, INIT2 on the OTA characteristic, serves
the ranges the watch requests, and acknowledges the end of the transfer. The device-specific
details of that sequence are documented in the device section.

## Read-only sessions

Use `scan`, `info`, `ota-state` and known read-only AT queries for diagnosis.
Pairing and OTA negotiation use live protocol commands and can change device state.
In particular, `--dry-run` and `--stop-after-a040` on the stock client are live BLE operations.
