---
title: Requirements
description: Hardware, firmware, host tools and permissions needed before trying anything.
---

## Hardware

- **CMF Watch Pro 2** (board `JX402`, Actions ATS3089C).
- Firmware verified: **`1.0.0.73`**. Other builds may differ and require a diff first.
- A Bluetooth LE adapter supported by [Bleak](https://github.com/hbldh/bleak).
- The watch is disconnected from the phone/Nothing X app, so that it advertises to the host.
- For flashing: a charged watch and a host that remains awake for 10–30 minutes.

## Host software

- **Python 3.12+**.
- **uv** for the Python workspace.
- Git.
- Optional: Ghidra 12.x for static analysis, Node 20+ and pnpm for the documentation site.
- Optional: a UART adapter for investigating the physical debug shell (not required).

Windows notes:

- Bleak uses the Windows BLE stack. The watch often stays at **MTU 23**; that is normal and
  still works, only slower.
- Pairing state is stored in `tools/.watch_state.json`, which is gitignored and never shared.

## Externally supplied files

This repository does **not** include firmware images, vendor resources, watch-face packages or
copyrighted animation frames. They are obtained legally for the device in use and placed
locally:

```text
firmware/JX402_V1.0.0.73.bin
watchfaces/           # optional .wfdl packages
```

Both paths are gitignored.

## Permissions

- Testing the watch in use requires permission.
- Write and flash commands are limited to hardware that is owned or explicitly authorised.
- The [Limitations & risks](/watch-utils/devices/cmf-watch-pro-2/limitations/) page covers the
  risks that apply before the first write.
