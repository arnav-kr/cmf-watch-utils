---
title: Requirements
description: Hardware, firmware, host tools and permissions needed before trying anything.
---

## Hardware

- **CMF Watch Pro 2** (board `JX402`, Actions ATS3089C).
- Firmware verified: **`1.0.0.73`**. Other builds may differ; diff first.
- A Bluetooth LE adapter supported by [Bleak](https://github.com/hbldh/bleak).
- The watch must be disconnected from the phone/Nothing X app so it advertises to your host.
- For flashing: a charged watch and a host that will stay awake for 10–30 minutes.

## Host software

- **Python 3.12+**.
- **uv** for the Python workspace.
- Git.
- Optional: Ghidra 12.x for static analysis, Node 20+ and pnpm for the documentation site.
- Optional: a UART adapter if you want to investigate the physical debug shell (not required).

Windows notes:

- Bleak uses the Windows BLE stack. The watch often stays at **MTU 23**; that is normal and
  still works, just slower.
- Pairing state is stored in `tools/.watch_state.json`, which is gitignored. Do not share it.

## What you must supply

This repository does **not** include firmware images, vendor resources, watch-face packages or
copyrighted animation frames. Obtain them legally for your own device, then place them locally:

```text
firmware/JX402_V1.0.0.73.bin
watchfaces/           # optional .wfdl packages
```

Both paths are gitignored.

## Permissions

- You need permission to test the watch you are using.
- Do not run write/flash commands on someone else's device.
- Read [Limitations & risks](/cmf-watch-utils/overview/limitations/) before the first write.
