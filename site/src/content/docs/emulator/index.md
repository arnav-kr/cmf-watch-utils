---
title: Watch Pro 2 emulator
description: What the QEMU preview supports and where to start.
---

The emulator runs CMF Watch Pro 2 firmware on your computer. You can use
the watch face and menus, try a custom app, or test a firmware update
without a physical watch.

## Browser preview

[Open the Watch Pro 2 browser emulator](/emulator/watch-pro-2/) in a recent
desktop version of Chrome or Edge. Use the touchscreen and crown, change
brightness, or simulate battery, motion, GPS and optical sensor readings.

1. Follow [Installation](/emulator/installation/) and the
   [Prepare the storage](/emulator/first-boot/#prepare-the-storage) section to
   create the three stock storage files.
2. From the repository root, prepare them for the browser. Use an output
   folder that does not exist yet.

   ```sh
   uv run python emulator/web/stage.py --fixtures fixtures --rom runtime/compat.bin --output web-local
   ```

3. Open the browser emulator, select the `web-local/fixtures` folder under
   **Boot images**, then press **Play**. Give stock firmware time to reach the
   watch face. **Watch log** shows its progress.

The browser reads the selected files on your computer and remembers the folder
for future boots. **Hard reset to stock** discards the running session and
boots stock again from the remembered files.

Open **FMC watch faces** to select a face by preview, title, or creator. The
emulator installs the selected face into its virtual storage at boot. You can
also load a local watch-face file. Choose **Custom firmware** to boot a local
Watch Pro 2 AOTA package. The **Bad Apple** preset accepts your built package,
or the application and companion SDFS files. For Bluetooth OTA and recovery
tests, use desktop QEMU.

## Desktop emulator

| Page | What you will do |
|---|---|
| [Installation](/emulator/installation/) | Install the command line tools, download the QEMU and ROM archives, and check the host libraries. |
| [First boot](/emulator/first-boot/) | Create the virtual storage from the reference firmware, boot stock, and open the watch window. |
| [OTA and recovery](/emulator/ota-recovery/) | Transfer an update over simulated Bluetooth and check the installed app after reboot. |

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
