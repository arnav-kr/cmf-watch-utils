---
title: Watch Pro 2 emulator
description: What the QEMU preview supports and where to start.
---

The emulator runs CMF Watch Pro 2 firmware on your computer. You can use
the watch face and menus, try a custom app, or test a firmware update
without a physical watch.

## Start here

| Page | What you will do |
|---|---|
| [Installation](/tooling/emulator/installation/) | Install the command line tools, download the QEMU and ROM archives, and check the host libraries. |
| [First boot](/tooling/emulator/first-boot/) | Create the virtual storage from the reference firmware, boot stock, and open the watch window. |
| [OTA and recovery](/tooling/emulator-ota/) | Transfer an update over simulated Bluetooth and check the installed app after reboot. |

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
