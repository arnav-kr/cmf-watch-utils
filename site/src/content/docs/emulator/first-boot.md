---
title: First boot
description: Create the watch's virtual storage, boot stock firmware, and open the viewer.
---

Complete [Installation](/emulator/installation/) first. The commands
below use Windows paths. On Linux and macOS, use the executable names and
debugger options from the installation guide.

## Prepare the storage

QEMU needs files that stand in for the watch's storage. They contain the app,
its fonts and images, and the language setting. The CLI uses the reference
firmware to create these files on your computer.

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

The command prints where it saved the storage files and logs. The
`fixtures` folder should contain `nor.bin`, `nand-id.bin` and
`nand-defaults.bin`. Keep them for future runs. If you already have these
three files from a previous setup, reuse them and go to [Headless boot](#headless-boot).

## Headless boot

First run without opening a window. This checks that the virtual watch starts
and draws something:

```text
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm.exe --rom runtime/compat.bin --runtime-dir C:/msys64/ucrt64/bin --headless --output runs/first-boot
```

Look for `"panel_frame_captured": true` in the output. QEMU saves a screenshot
as `runs/first-boot/frame.ppm` and its result as `report.json`.

## Open the viewer

Run again with a new output folder:

```text
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm.exe --rom runtime/compat.bin --runtime-dir C:/msys64/ucrt64/bin --output runs/first-window
```

You should see a window titled **CMF Watch Pro 2**. For the first-run language
screen, repeat the storage preparation with `--profile wp2-onboarding`,
then add that flag to your boot command. Every boot command needs a new output
folder. Use a different name to keep older results.

Windowed runs select an available speaker backend automatically. If you want
to inspect audio without using your speakers, add `--wav`; it writes
`audio.wav` to the output folder. Use `--audio none` for a silent run.

## Custom firmware

Use `--app local/app.bin` to start an app you built. Add `--sdfs local/sdfs.bin`
if it needs a matching resource archive. Use `--ota-image local/update.bin`
to extract the app and resources from a supported update and boot them directly.
That direct boot checks whether the app starts and renders. Use
[OTA and recovery](/emulator/ota-recovery/) to test transfer and installation.

```text
uv run cmf-watch-utils emu preflight --image local/update.bin
```

Preflight means checking a package without running it. The command checks the
target board, checksums, and whether the app and its resources fit. It only
needs the Python workspace and the update file. Some factory packages include
extra partitions. Use [fwkit](/tooling/fwkit/) to inspect those.

## Troubleshooting

| What you see | What to check |
|---|---|
| Firmware hash mismatch or a tiny firmware file | Repeat step 2 in [Setup](/getting-started/setup/) to download the full reference image. |
| `ats3089c-wp2` missing | Point `--qemu` at the executable from the preview archive. |
| QEMU exits before boot | Rerun `emu doctor`. On Windows, check that `--runtime-dir` points to MSYS2's UCRT64 `bin` folder. |
| Storage preparation fails | Check free space, the debugger path, and the numbered GDB, QEMU and UART logs in the reported folder. |
| Headless works but the window fails | Install Tcl/Tk and pass `--wish` with the path to its executable if it is outside PATH. |
