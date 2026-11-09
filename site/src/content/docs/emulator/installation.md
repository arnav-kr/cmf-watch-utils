---
title: Installation
description: Install the CMF Watch Utils CLI, the QEMU runtime, and the host libraries.
---

QEMU runs the watch on your computer. The compatibility ROM in the shared
archive starts the firmware. Both are required before you can create the
watch's storage or boot the emulator.

## Workspace

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

## Runtime archives

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

## Host dependencies

### Windows

Install MSYS2 and open its **UCRT64** terminal. Run this command there,
then switch back to PowerShell:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-glib2 mingw-w64-ucrt-x86_64-pixman mingw-w64-ucrt-x86_64-dtc mingw-w64-ucrt-x86_64-libwinpthread mingw-w64-ucrt-x86_64-zlib mingw-w64-ucrt-x86_64-tk
```

The last package provides `wish.exe`, which opens the watch window. If you
installed MSYS2 somewhere other than `C:/msys64`, use that path in the check below.

### Arch Linux

Install the runtime libraries, viewer and Arm debugger from the official repositories:

```bash
sudo pacman -Syu --needed git git-lfs uv glib2 pixman dtc libpulse tk arm-none-eabi-gdb
```

`uv` manages the Python workspace. GLib, Pixman and `dtc` provide QEMU's
native libraries; `libpulse` provides its audio client library. Tcl/Tk supplies
the `wish` viewer. [Arm GDB](https://archlinux.org/packages/extra/x86_64/arm-none-eabi-gdb/)
is used to prepare the virtual storage.

Use the Linux x64 archive and the shared ROM archive. From the checkout:

```bash
mkdir -p runtime
tar -xf cmf-emulator-linux-x64.tar.gz -C runtime --strip-components 1
tar -xf cmf-emulator-wp2-rom.tar.gz -C runtime --strip-components 1
uv sync --locked
uv run cmf-watch-utils emu doctor --qemu runtime/qemu-system-arm --rom runtime/compat.bin
arm-none-eabi-gdb --version
```

In [First boot](/emulator/first-boot/), use
`--qemu runtime/qemu-system-arm`, omit `--runtime-dir C:/msys64/ucrt64/bin`,
and keep `--gdb arm-none-eabi-gdb`. The viewer needs a graphical desktop;
on Wayland, it also needs XWayland (`xorg-xwayland`). Audio playback needs a
running PulseAudio server or PipeWire's PulseAudio compatibility service.

The Linux archive is built and smoke-tested on Ubuntu 22.04. Arch has not
had an end-to-end firmware boot test. If a rolling library update prevents
the archive from loading, follow the [Arch source build instructions](https://github.com/arnav-kr/cmf-watch-utils/blob/main/emulator/README.md#arch-linux).
The distribution's standard QEMU package does not include this watch model.

### Ubuntu and macOS

| Host | Packages | Command differences |
|---|---|---|
| Ubuntu 22.04 x64 | Install `libglib2.0-0 libpixman-1-0 libfdt1 libpulse0 tk gdb-multiarch` with apt. | Remove `.exe` and `--runtime-dir`. Use `--gdb gdb-multiarch` when preparing storage. |
| macOS ARM64 | Install `glib pixman dtc` with Homebrew. Install Tcl/Tk for the viewer and an Arm-capable GDB to prepare storage. | Remove `.exe` and `--runtime-dir`. Put `wish` on PATH or pass its path with `--wish`. Pass the debugger path with `--gdb`. |

## Verify the installation

Run this check from the checkout. Apply the command differences above on Linux and macOS:

```text
uv run cmf-watch-utils emu doctor --qemu runtime/qemu-system-arm.exe --rom runtime/compat.bin --runtime-dir C:/msys64/ucrt64/bin
```

Look for `"machine": "ats3089c-wp2"` and `"viewer_available": true`. If only the
viewer is missing, you can still run the headless check in [First boot](/emulator/first-boot/).

Next, [prepare the watch's storage and boot stock firmware](/emulator/first-boot/).
