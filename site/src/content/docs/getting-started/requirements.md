---
title: Requirements
description: Minimum dependencies by task, from offline inspection to a virtual or physical OTA.
---

Start with Git, uv and Python 3.12 or later. Install additional tools only for the
task you are doing.

| Task | Additional requirements |
|---|---|
| List profiles or read command help | None |
| Inspect a firmware file or preflight an update | The local firmware file. Git LFS is optional if you use a direct download. |
| Create emulator fixtures | Project QEMU, compatibility ROM bundle with the provisioner, Arm-capable GDB, exact stock package and pinned NAND ID table |
| Boot firmware in QEMU | Project QEMU, compatibility ROM, host runtime libraries and prepared virtual flash fixtures |
| Open the emulator window | Tcl/Tk `wish` in addition to the QEMU boot requirements |
| Run virtual BLE OTA | Local H4 controller such as Netsim. Stock CMF OTA also needs a synthetic pairing fixture. |
| Read staged TEMP from raw virtual NAND | Arm-capable GDB and reader files from the ROM bundle |
| Talk to a physical watch | A Watch Pro 2 and a Bluetooth LE adapter supported by Bleak |
| Build the documentation | Node.js 22.22 or later and pnpm 12 |

The [emulator guide](/emulator/) lists OS-specific runtime packages and
shows how to create the fixtures. A basic boot needs no Android emulator, Nothing X
account, root access, or Bluetooth adapter.

## Physical watch

The reference device is the CMF Watch Pro 2, board JX402, running stock `1.0.0.73`.
Firmware builds and BLE behavior can differ. The tested physical host was Windows.
Pairing state in `tools/.watch_state.json` is private.

A watch connected to Nothing X may be unavailable to the host. See
[BLE tools](/tooling/ble-tools/) for a read-only first check and
[Limitations & risks](/devices/cmf-watch-pro-2/limitations/) before any live update.
