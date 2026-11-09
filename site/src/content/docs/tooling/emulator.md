---
title: Run the Watch Pro 2 emulator
description: Start from a clean checkout and boot the stock Watch Pro 2 on your computer.
---

This walkthrough opens the stock Watch Pro 2 in a window. You can click and
drag on the watch screen and use the crown controls beside it. A physical watch,
a phone, and Bluetooth are unnecessary for this walkthrough.

## 1. Install the command line tools

Follow [Setup](/getting-started/setup/) through step 2. That installs the CLI
and puts the reference firmware at
`dumps/binaries/firmware/JX402_V1.0.0.73.bin`. Keep your terminal open in the
`cmf-watch-utils` folder. Run:

```text
uv run cmf-watch-utils emu profiles
```

You should see `wp2-stock` and `wp2-onboarding`. Use `wp2-stock` for your first
run. It starts after the language selection screen. The second profile starts
at first-time setup.

## 2. Get the program that runs the virtual watch

Download **two** archives from the same [preview release](https://github.com/arnav-kr/cmf-watch-utils/releases):

- The archive named `cmf-emulator-windows-x64.tar.gz`, `cmf-emulator-linux-x64.tar.gz`,
  or `cmf-emulator-macos-arm64.tar.gz` for your computer. It contains the project's
  QEMU executable. This build understands the Watch Pro 2 hardware.
- `cmf-emulator-wp2-rom.tar.gz`. It contains `compat.bin`, which starts the
  virtual watch, and the small programs used to prepare and read its storage.

On Windows, move the downloads into the checkout. Then run these commands in
PowerShell to put the contents of both archives in one folder:

```powershell
New-Item -ItemType Directory -Force runtime | Out-Null
tar -xf cmf-emulator-windows-x64.tar.gz -C runtime --strip-components 1
tar -xf cmf-emulator-wp2-rom.tar.gz -C runtime --strip-components 1
Get-ChildItem runtime
```

Check that you see `qemu-system-arm.exe`, `compat.bin` and `provision-nand.elf`.
On Linux or macOS, use `mkdir -p runtime` and substitute your host's archive in
the first `tar` command. The QEMU executable on those hosts has no `.exe` suffix.
If a release archive is unavailable, see the [source build instructions](https://github.com/arnav-kr/cmf-watch-utils/blob/main/emulator/README.md).

**Install the libraries QEMU needs.** On Windows, install MSYS2 and open its
**UCRT64** terminal. Run this there, then switch back to PowerShell:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-glib2 mingw-w64-ucrt-x86_64-pixman mingw-w64-ucrt-x86_64-dtc mingw-w64-ucrt-x86_64-libwinpthread mingw-w64-ucrt-x86_64-zlib mingw-w64-ucrt-x86_64-tk
```

The last package provides `wish.exe`, which opens the watch window. If you
installed MSYS2 somewhere other than `C:/msys64`, change the path below. Check
the runtime from PowerShell:

```text
uv run cmf-watch-utils emu doctor --qemu runtime/qemu-system-arm.exe --rom runtime/compat.bin --runtime-dir C:/msys64/ucrt64/bin
```

Look for `"machine": "ats3089c-wp2"` and `"viewer_available": true`. If only the
viewer is missing, you can still run the headless check in step 4.

| On this host | Install these packages | Changes to the Windows commands |
|---|---|---|
| Ubuntu 22.04 x64 | `libglib2.0-0 libpixman-1-0 libfdt1 libpulse0 tk gdb-multiarch` with apt | Remove `.exe` and `--runtime-dir`. Use `--gdb gdb-multiarch`. |
| macOS ARM64 | `glib pixman dtc` with Homebrew, Tcl/Tk with `wish` on PATH, and an Arm-capable GDB | Remove `.exe` and `--runtime-dir`. Set `--gdb` to your debugger's path. |

## 3. Give the watch a local disk

QEMU needs files that stand in for the watch's storage. They contain the app,
its fonts and images, and the language setting. The program in step 2 uses
the stock firmware to create them on your computer.

1. Download the pinned SDK [NAND ID file](https://raw.githubusercontent.com/lvgl/lv_port_actions_technology/26f51e584940d5b141cd1d09d2629d69b595f579/action_technology_sdk/zephyr/tools/prebuilt/leopard/common/bin/nand_id_all.bin)
   and save it as `runtime/nand_id_all.bin`. It tells the stock driver which
   kind of flash chip the emulator presents. You only need that one SDK file.
2. Install an Arm debugger. On Windows, use an Arm GNU Toolchain distribution
   containing `arm-none-eabi-gdb.exe`. Add its `bin` folder to PATH or pass
   the full path in quotes after `--gdb`. Run `arm-none-eabi-gdb --version` to
   check it. The debugger is used here to start the storage setup inside QEMU.
3. Leave at least 250 MiB free on the drive. Run this from the checkout:

```text
uv run cmf-watch-utils emu prepare-fixtures --fixtures fixtures --stock dumps/binaries/firmware/JX402_V1.0.0.73.bin --nand-id runtime/nand_id_all.bin --qemu runtime/qemu-system-arm.exe --rom runtime/compat.bin --runtime-dir C:/msys64/ucrt64/bin --provisioner runtime --gdb arm-none-eabi-gdb
```

Success prints a JSON result with the paths to the fixtures and logs. The
`fixtures` folder should contain `nor.bin`, `nand-id.bin` and
`nand-defaults.bin`. Keep them for future runs. If you already have these
three files from a previous setup, you can reuse them and skip step 3.

## 4. Boot the watch

First run without opening a window. This checks that the virtual watch starts
and draws something:

```text
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm.exe --rom runtime/compat.bin --runtime-dir C:/msys64/ucrt64/bin --headless --output runs/first-boot
```

Look for `"panel_frame_captured": true` in the output. QEMU saves a screenshot
as `runs/first-boot/frame.ppm` and its result as `report.json`. Now open the
window:

```text
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm.exe --rom runtime/compat.bin --runtime-dir C:/msys64/ucrt64/bin --output runs/first-window
```

You should see a window titled **CMF Watch Pro 2**. For the first-run language
screen, prepare a second disk by repeating step 3 with `--profile wp2-onboarding`,
then add that flag to your boot command. Every boot command needs a new output
folder. Use a different name to keep older results.

## If a step fails

| What you see | What to check |
|---|---|
| Firmware hash mismatch or a tiny firmware file | Repeat step 2 in [Setup](/getting-started/setup/) to download the full reference image. |
| `ats3089c-wp2` missing | Point `--qemu` at the executable from the preview archive. |
| QEMU exits before boot | Rerun `emu doctor`. On Windows, check that `--runtime-dir` points to MSYS2's UCRT64 `bin` folder. |
| Storage preparation fails | Check free space, the debugger path, and the numbered GDB, QEMU and UART logs in the reported folder. |
| Headless works but the window fails | Install Tcl/Tk and pass `--wish` with the path to its executable if it is outside PATH. |

## Test a custom app

Use `--app local/app.bin` to start an app you built. Add `--sdfs local/sdfs.bin`
if it needs a matching resource archive. Use `--ota-image local/update.bin`
to extract the app and resources from a supported update and boot them directly.
That direct boot checks the result you would run after an update. The next guide
covers the Bluetooth transfer and the install after reboot.

```text
uv run cmf-watch-utils emu preflight --image local/update.bin
```

Preflight means checking a package without running it. The command checks the
target board, checksums, and whether the app and its resources fit. It only
needs the Python workspace and the update file. Some factory packages include
extra partitions. Use [fwkit](/tooling/fwkit/) to inspect those.

## Test OTA and recovery

Continue with [Virtual updates and recovery](/tooling/emulator-ota/) once the stock
boot works. That guide shows how to transfer an update inside QEMU and what the
result can tell you about the candidate. The physical bootloader remains outside
the test.
