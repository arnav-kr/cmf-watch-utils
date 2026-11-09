---
title: Watch Pro 2 emulator
description: Run Watch Pro 2 firmware in your browser or use desktop QEMU for virtual Bluetooth OTA and recovery tests.
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
Stock starts at the highest manual brightness in the watch's own settings.
The browser brightness slider adjusts the rendered display separately.
The virtual speaker is on by default. If your browser blocks autoplay, tap or
click the emulator to start audio. Use **Watch sound** under **Simulation →
Display** to mute it. If the virtual watch runs slowly, audio stretches to
keep pace while preserving pitch.
**Battery level** sets the reported percentage in stock firmware. With a
custom app, the slider changes the simulated battery voltage and the app
decides how to report it.

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
separately under **Raw application files**.

Select **Bad Apple** from **Boot preset** to run the bundled demo.
For an example of a custom Zephyr application, see the
[Bad Apple source](https://github.com/arnav-kr/firmware-wp2-badapple) and
[case study](/devices/cmf-watch-pro-2/guides/bad-apple/).
For Bluetooth OTA and recovery tests, use desktop QEMU.

On phones and devices that report limited memory, the browser limits display
updates to about 30 fps to reduce work. The watch firmware still runs in software,
so boot times and animation speed depend on the host device.
The bundled Bad Apple preset uses a clock setting tuned for playback. Use a
custom firmware boot or desktop QEMU for tests that depend on guest timing.

### Base firmware files

The browser includes the stock files needed to boot. Use **Advanced boot
files** when your test needs a different starting image. The emulator uses
its bundled version for each file you leave out.

| File | What it does |
|---|---|
| `app.bin` | Stock application used when booting stock. A custom app is selected through the main firmware inputs. |
| `nor.bin` | The small flash image containing boot parameters and application partitions. Custom app and resource files are placed in this image. |
| `nand-defaults.bin` | The larger flash image containing resources and settings. **Virtual NAND** supplies a replacement for this file. |
| `compat.bin` | The emulator's compatibility ROM, which provides the services the watch app expects at boot. |
| `nand-id.bin` | The chip identification table used by the NAND driver. |
| `wfc-index.txt` | Texture information used to render stock watch faces. |

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
