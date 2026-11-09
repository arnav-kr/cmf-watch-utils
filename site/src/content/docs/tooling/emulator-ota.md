---
title: Virtual updates and recovery
description: Transfer a firmware update between virtual devices and check what happens after reboot.
---

Start with the [first-boot walkthrough](/tooling/emulator/). This guide uses two
local programs. QEMU runs the virtual watch. Netsim gives it a simulated
Bluetooth radio so the OTA client can talk to it without a physical adapter.

## 1. Start the simulated radio

Find `netsimd` in the Android Emulator package. You can run this executable
without creating an Android device or signing in to Nothing X. Open a new
terminal and run:

```text
netsimd --instance 70 --hci-port 17310 --no-cli-ui --no-web-ui
```

Leave that terminal open. Use port `17310` in both QEMU and the OTA client.
If Netsim exits when the guest closes, start it again before the next test.

## 2. Choose an update protocol

- **Stock CMF OTA** uses the original watch app. It needs a synthetic pairing
  secret before it will accept an OTA request.
- **Actions OTA** works with a guest app exposing the Actions SDK OTA service.
  It uses a standalone AOTA file without a CMF trailer and skips stock
  pairing. See [Actions SDK OTA](/tooling/actions-ota/).

Both protocols default to negotiation. The guest responds to the setup
commands, and no firmware bytes are sent. Add `--transfer` to send the
file to the virtual watch.

## 3. Pair a stock guest for testing

Run the first command in the checkout. In a second terminal run the second
command. It gives the guest a random test secret, saves its virtual storage
to `local/paired-nor.bin`, and closes QEMU.

These examples use the same paths as the first-boot walkthrough. On Windows,
use `runtime/qemu-system-arm.exe` and add
`--runtime-dir C:/msys64/ucrt64/bin` when launching QEMU.

```text
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin --headless --hci-port 17310 --qmp-port 17320 --seconds 120 --output runs/pairing
uv run cmf-watch-utils emu pair-fixture --hci-port 17310 --qmp-port 17320 --output local/paired-nor.bin
```

Port `17320` is QEMU's local control connection. The CLI uses it to save
the virtual storage. Keep the exported file local.

## 4. Transfer, reboot, and boot the result

Run these commands in order, each in a separate terminal. Give the first
command time to start, then start the recovery command before transferring.
Replace `local/stock-protocol-update.bin` with a compatible app/SDFS update.

```text
uv run cmf-watch-utils emu run --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin --nor local/paired-nor.bin --headless --allow-reboot --ota-snapshot --hci-port 17310 --qmp-port 17320 --seconds 450 --output runs/stage
uv run cmf-watch-utils emu recover --fixtures fixtures --qemu runtime/qemu-system-arm --rom runtime/compat.bin --reader runtime --gdb arm-none-eabi-gdb --qmp-port 17320 --pending-nand runs/stage/nand-staged.bin --output runs/install
uv run cmf-watch-utils emu ota --protocol stock --hci-port 17310 --image local/stock-protocol-update.bin --transfer --output runs/stock-transfer.json
```

The first command starts stock and saves its simulated NAND when stock asks
to reboot. The second waits for that reboot, reads the staged update through
the stock NAND driver, and writes a new `runs/install/nor.bin`. The third
sends the update. Boot the new file with `emu run --nor runs/install/nor.bin`
and a fresh output directory. A panel frame confirms that the installed
app started and drew something.

## What the result means

- In the stock test, the guest accepted the update, rebooted, and started
  Bad Apple after the emulator installed the staged package. The recorded test
  used a QEMU-only package with an empty AGPS trailer.
- In the Actions test, the guest accepted the image after transfer.
  Post-install boot is a separate check.
- Crown + charger rescue is a separate model. Given a deliberately broken
  virtual app, `emu recover --fixtures fixtures --nor local/broken-nor.bin --crown-held --charger-connected --output runs/rescue`
  restores stock app and resource data. Boot the resulting `nor.bin` to
  check the restore.

The emulator uses a replacement boot handoff. These tests do not establish
whether a real watch will enter recovery with a broken app or recover after
power loss or damage to its boot partitions. See
[Recovery & boot modes](/devices/cmf-watch-pro-2/internals/recovery/).
