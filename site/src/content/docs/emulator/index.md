---
title: Watch Pro 2 emulator
description: What the QEMU preview supports and where to start.
---

The emulator runs CMF Watch Pro 2 firmware on your computer. You can use
the watch face and menus, try a custom app, or test a firmware update
without a physical watch.

## Browser preview

[Open the Watch Pro 2 browser emulator](/emulator/watch-pro-2/) in a recent
desktop version of Chrome or Edge. Stock firmware starts automatically.
The first boot downloads the virtual storage and can take a moment. **Watch log**
shows the guest's progress. Use the touchscreen and crown, change brightness,
or simulate battery, motion, GPS and optical sensor readings.

Choose **Display test** from **Boot preset** to run a bundled custom firmware.
Press the crown to change the pattern’s color. **Hard reset to stock** discards
the running session and boots stock again. Under **Custom firmware**, **Advanced
boot files** lets you supply a different virtual NAND image or override the
prepared base files.
Use the camera button to save the circular display as a transparent PNG.
Turn on **Include frame in screenshots** under **Display** to include the case.

Open **FMC watch faces** to browse the full Watch Pro 2 catalog. Search by
title or creator and sort by newest, most downloaded, or most flashed. Choose a
face, then select **Apply watch face** to install it at boot. Applying restarts
a running watch. To use a local face file, stop the watch, select the file
under **Watch face**, and start it again. To try your own build, select **Custom
firmware**, then select a Watch Pro 2 AOTA
under **Firmware package**, or select the application and companion SDFS
separately under **Raw application files**. The **Bad Apple** preset accepts
both formats. For Bluetooth OTA and recovery
tests, use desktop QEMU.

## Desktop emulator

| Page | What you will do |
|---|---|
| [Installation](/emulator/installation/) | Install the command line tools, download the QEMU and ROM archives, and check the host libraries. |
| [First boot](/emulator/first-boot/) | Create the virtual storage from the reference firmware, boot stock, and open the watch window. |
| [OTA and recovery](/emulator/ota-recovery/) | Transfer an update over simulated Bluetooth and check the installed app after reboot. |

Run `uv run cmf-watch-utils emu faces --search "LCD" --sort downloads` to
browse FMC from the desktop CLI. Copy the face's `id` and add
`--fmc-face ID` to your stock `emu run` command. For a file on your computer,
use `--watch-face FILE`. Both replace Activity Mood for that run.

## Profiles

| Profile | Where the watch starts |
|---|---|
| `wp2-stock` | Stock firmware `1.0.0.73`, after language selection. Start here. |
| `wp2-onboarding` | The same firmware, at first-time setup. |

Use the Watch Pro 2 profiles only for board JX402. They share the same
virtual hardware and use different starting storage.

## Scope

The preview has booted stock, Bad Apple, and Tilt Mouse on Windows. Its
virtual recovery can restore a broken app in QEMU. Real-watch bootloader
behavior, electrical faults, and recovery after power loss remain outside
that test. See [Recovery & boot modes](/devices/cmf-watch-pro-2/internals/recovery/).
