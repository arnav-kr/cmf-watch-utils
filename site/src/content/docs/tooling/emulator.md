---
title: Emulator CLI
description: Set up the Watch Pro 2 QEMU preview, select a profile, and test firmware boot, virtual OTA and modeled recovery.
---

The emulator runs Watch Pro 2 firmware against a QEMU model of the Actions ATS3089C. It can boot selected stock and custom images, capture panel frames and audio, and exercise OTA with virtual flash. The CLI can run these tests without a watch, a Bluetooth adapter, or a phone.

## Profiles

| Profile | Stock baseline | Raw NAND in the fixture directory |
|---|---|---|
| `wp2-stock` (default) | Watch Pro 2 JX402, `1.0.0.73`, language already selected | `nand-defaults.bin` |
| `wp2-onboarding` | Same watch and firmware, before language selection | `nand-onboarding.bin` |

Both profiles use machine `ats3089c-wp2`. They share `nor.bin` and `nand-id.bin`. Profiles select a verified baseline and storage layout. Use `--app`, `--ota-image` or `--nor` to select custom firmware. Other watches and stock versions need their own verified profiles.

## 1. Get the runtime

Follow [Setup](/getting-started/setup/) through the CLI check. Package preflight can run
at that point without any additional software. For a boot, get these two matching
archives from the [release page](https://github.com/arnav-kr/cmf-watch-utils/releases)
when available, or follow the [source build guide](https://github.com/arnav-kr/cmf-watch-utils/blob/main/emulator/README.md).
Building QEMU from source also needs a compiler toolchain and several GB of disk space.

- The QEMU archive for your OS and CPU. It contains `qemu-system-arm` (or
  `qemu-system-arm.exe`) with the `ats3089c-wp2` machine. Standard QEMU lacks this machine.
- The shared `cmf-emulator-wp2-rom` archive. It contains `compat.bin`, the NAND
  provisioner (`provision-nand.elf` and `provision-nand.bin`), and the staged OTA reader
  (`read-ota-temp.elf` and `read-ota-temp.bin`). Older bundles may lack the provisioner.

For the examples, put the contents of both archives in `runtime/` under the checkout.
Install host libraries for the build you picked:

| Host | Runtime | Optional viewer |
|---|---|---|
| Windows x64 | In the MSYS2 UCRT64 shell, install `mingw-w64-ucrt-x86_64-glib2`, `mingw-w64-ucrt-x86_64-pixman`, `mingw-w64-ucrt-x86_64-dtc`, `mingw-w64-ucrt-x86_64-libwinpthread` and `mingw-w64-ucrt-x86_64-zlib` with `pacman -S --needed`. Add the UCRT64 `bin` directory to PATH or pass it as `--runtime-dir`. | Install `mingw-w64-ucrt-x86_64-tk` or supply another Tcl/Tk `wish.exe`. |
| Ubuntu 22.04 x64 | Install `libglib2.0-0`, `libpixman-1-0`, `libfdt1` and `libpulse0` for the PulseAudio build. | Install `tk`. |
| macOS ARM64 | For the Homebrew build, install `glib`, `pixman` and `dtc`. | Use Tcl/Tk with `wish` available on PATH. |

Firmware integration ran on Windows. Linux and macOS are build targets with unverified
runtime integration. Check package names and library versions if you use another distro
or a different build. Android, root access, Nothing X and a BLE adapter are unnecessary
for normal emulator use.

Check the runtime before creating any fixtures:

```text
uv run cmf-watch-utils emu doctor --qemu runtime/qemu-system-arm --rom runtime/compat.bin
```

On Windows, use `runtime/qemu-system-arm.exe` and add `--runtime-dir C:/msys64/ucrt64/bin`
if UCRT64 is outside PATH. A missing viewer is fine for a headless run.

## 2. Prepare virtual flash

If you already have a verified fixture set, skip to step 3 or use `emu import-fixtures`
to copy it into one directory. Otherwise get these inputs:

1. The exact `JX402_V1.0.0.73.bin` from the [setup guide](/getting-started/setup/).
2. The pinned SDK [NAND ID file](https://raw.githubusercontent.com/lvgl/lv_port_actions_technology/26f51e584940d5b141cd1d09d2629d69b595f579/action_technology_sdk/zephyr/tools/prebuilt/leopard/common/bin/nand_id_all.bin).
   Save it as `runtime/nand_id_all.bin`. The CLI checks its hash. You only need this one SDK file.
3. An Arm-capable GDB. On Windows use a bare-metal Arm GNU Toolchain distribution that
   includes `arm-none-eabi-gdb.exe`. On Ubuntu you can install `gdb-multiarch` and pass
   `--gdb gdb-multiarch`. Verify the executable with `--version`. The debugger connects
   to QEMU on loopback and runs the provisioner through the stock NAND driver.

Allow at least 250 MiB of free space when preparing one profile. Each finished NAND
image takes about 132 MiB. Run the following from the checkout. On Windows use
`runtime/qemu-system-arm.exe` and add, for example, `--runtime-dir C:/msys64/ucrt64/bin`
to commands that launch QEMU.

```text
uv run cmf-watch-utils emu prepare-fixtures --fixtures fixtures --stock dumps/binaries/firmware/JX402_V1.0.0.73.bin --nand-id runtime/nand_id_all.bin --qemu runtime/qemu-system-arm --rom runtime/compat.bin --provisioner runtime --gdb arm-none-eabi-gdb
```

The command prints the fixture directory and the location of the provisioning logs.
It creates a 4 MiB `nor.bin`, a 790-byte `nand-id.bin`, and the profile's
138,412,032-byte NAND file. Boot and recovery slots in NOR remain erased. The
prepared NAND uses stock resource archives and a synthetic empty `res_b`.
To create the onboarding NAND as well, run the same command with `--profile wp2-onboarding`.
The example directories `runtime/`, `fixtures/`, `local/` and `runs/` are ignored by Git.

## 3. Boot stock

```text
uv run cmf-watch-utils emu profiles
uv run cmf-watch-utils emu doctor --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin --headless --output runs/stock
```

For onboarding, select `--profile wp2-onboarding` when launching. Each run needs a new output directory. A successful headless run writes `report.json` and `frame.ppm`. Omit `--headless` for the viewer with mouse touch gestures, crown controls and simulated sensor inputs. For audio use `--headless --wav` or select a backend included in the interactive build with `--audio`.

Use `--app local/app.bin --sdfs local/sdfs.bin` for raw custom images. Use `--ota-image local/update.bin` to validate and stage a supported update's app and SDFS directly in a working NOR copy. That boot test skips OTA transport and install logic. Use `--nor` to boot an already staged or recovered virtual NOR.

## Firmware preflight and loopback OTA

```bash
uv run cmf-watch-utils emu preflight --image local/update.bin
uv run cmf-watch-utils emu ota --help
uv run cmf-watch-utils emu recover --help
```

Preflight checks the supported app/SDFS package shape, board, manifest and CRCs, chunk bounds, ACTH checksums and partition sizes. It needs no QEMU binary or radio. Resource-heavy factory containers can contain additional partitions outside this verifier's supported shape. Use [fwkit](/tooling/fwkit/) to inspect those containers.

Virtual OTA is optional. Run an isolated H4 controller before starting QEMU. The
Windows test used `netsimd` from the Android Emulator package as a standalone
process. A phone, Android virtual device, and Nothing X sign-in are unnecessary.
For example, run `netsimd --instance 70 --hci-port 17310 --no-cli-ui --no-web-ui` in a separate
terminal and use that port for both QEMU and the client.

For a virtual transfer, launch the guest with `--hci-port` set to the isolated H4 listener. Run `emu ota --protocol stock|actions --hci-port PORT --image FILE` on the same simulated radio. Select the protocol the guest exposes and use `--name` if the advertisement differs from the default `cmf`. The default stops after negotiation. `--transfer` serves firmware bytes to the virtual guest.

### Prepare stock virtual pairing

A newly created NOR has no pairing secret. Start a fresh stock guest with both
ports below. In another terminal, run the second command. It creates a random
synthetic secret inside the guest, exports a new virtual NOR and closes QEMU.

```text
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin --headless --hci-port 17310 --qmp-port 17320 --seconds 120 --output runs/pairing
uv run cmf-watch-utils emu pair-fixture --hci-port 17310 --qmp-port 17320 --output local/paired-nor.bin
```

Keep the paired NOR local. If Netsim stops after QEMU closes, start it again on the same
port. Boot the next guest from the paired NOR so the app loads its new pairing state.
Actions OTA uses its own protocol and skips stock pairing.

### Stock reset and staged install

Start an isolated H4 controller on port `17310`. Run the commands below in order, in separate
terminals for the guest, recovery supervisor and transfer. `local/paired-nor.bin` is a virtual
stock NOR with a synthetic guest pairing secret. The second command waits for the reset, so start
it before running the third.

```bash
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin --nor local/paired-nor.bin --headless --allow-reboot --ota-snapshot --hci-port 17310 --qmp-port 17320 --seconds 450 --output runs/stage
uv run cmf-watch-utils emu recover --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin --reader runtime --gdb arm-none-eabi-gdb --qmp-port 17320 --pending-nand runs/stage/nand-staged.bin --output runs/install
uv run cmf-watch-utils emu ota --protocol stock --hci-port 17310 --image local/stock-protocol-update.bin --transfer --output runs/stock-transfer.json
```

When recovery succeeds, boot `runs/install/nor.bin` with `emu run --nor` in a fresh output
directory. The recorded end-to-end run used a QEMU-only Bad Apple package with an empty
AGPS trailer.

For a broken-app rescue, supply a broken **virtual** NOR and both modeled inputs:

```bash
uv run cmf-watch-utils emu recover --fixtures fixtures --nor local/broken-nor.bin --crown-held --charger-connected --output runs/rescue
```

Boot `runs/rescue/nor.bin` to verify the restored app reaches the panel.

## If setup stops

| Symptom | Check |
|---|---|
| Stock hash mismatch or a tiny firmware file | Fetch the reference Git LFS object or use the direct download. A text pointer cannot be used. |
| `ats3089c-wp2` missing | Point `--qemu` at the project's executable for your host. |
| QEMU exits at startup | Run `emu doctor` and check the host libraries. On Windows, confirm `--runtime-dir` points to UCRT64's `bin`. |
| Provisioning fails | Check the numbered QEMU, UART and GDB logs in the printed work directory. Verify the ROM bundle, free space and Arm-capable GDB. |
| No viewer | Try `--headless` first, then install Tcl/Tk or pass the path to `wish` with `--wish`. |
| No matching BLE advertisement | Check that both commands use the same H4 port and that `--name` matches the guest's advertisement. |
| Output already exists | Choose a new output file or run directory so previous results stay intact. |

## Verified scope

- Stock `1.0.0.73`, stock onboarding, Bad Apple and Tilt Mouse rendered frames. Bad Apple produced nonzero PCM in a captured WAV.
- Stock-protocol OTA reached END and rebooted the guest. The modeled install read TEMP from the resulting NAND snapshot through the stock NAND driver and booted the installed Bad Apple image.
- Actions OTA negotiated with the Bad Apple guest, served the rollback package and received image validation success. That result covers transfer and validation.
- Crown + charger rescue restored stock from a deliberately broken app in virtual NOR. This runs in the host recovery model and uses known-good stock app and SDFS data.
- A separate Nothing X lab synced simulated heart rate and SpO₂ using a local binding fixture. The standard CLI provides the emulator and loopback OTA. Public account binding and live cloud sync are outside that verification.

The compatibility ROM starts the application with a modeled handoff. Bootloader execution, real crown/charger power-on behavior, physical flash timing and failure modes remain outside this verification. See [Recovery & boot modes](/devices/cmf-watch-pro-2/internals/recovery/).
