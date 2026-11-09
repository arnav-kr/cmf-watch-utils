---
title: Recovery & boot modes
description: What is known about recovery, safe mode, OTA flags and the limits of unbricking.
---

## Physical recovery

There is **no confirmed, button-only recovery path** for every failure mode on this watch.
The app contains safe-mode and launch/retry logic, and OTA flags persist across reboots, but no
unit has been deliberately bricked to prove a universal recovery sequence.

An Actions OTA loading screen confirms the app exposed that update service in
the tested state. It does not establish where boot-time recovery runs or whether
the service survives a broken app.

## What QEMU recovery verifies

| Scenario | What the test verified |
|---|---|
| Stock OTA reset and install | Stock reported success and the host sent END. QMP observed the guest OTA reset and retained reboot reason. The recovery supervisor read logical TEMP from the captured virtual NAND using the stock NAND driver, validated app/SDFS data, retired the pending record and wrote a separate virtual NOR. The resulting Bad Apple app booted to the panel. |
| Crown + charger with a broken app | Given both modeled inputs, the host restored the stock app and SDFS from the verified local fixture while leaving other virtual NOR partitions in place apart from retiring pending OTA records. The restored stock image booted. |
| Actions OTA | The guest returned image validation success. Installation and post-reset boot were verified separately for the stock OTA path. |

The guest starts through a compatibility ROM. The real bootloader and recovery
partitions were not read back or executed in this flow. The model cannot verify
the physical power-on selector, recovery BLE when the app is broken, or recovery
after corruption in the bootloader, storage or power path. Use
[OTA and recovery](/tooling/emulator-ota/) for the fixture and command requirements.

## Evidence in the firmware

The app references:

- `system_app_launch` at `0x10054CF4`
- safe mode at `0x10054C94`
- launch init at `0x10054DBC`
- checks for `/NAND:C/A/B/E/K` and `_res_file_check`
- persistent OTA flags `OTA_UPG_FLAG` (`0x10058508`) and `OTA_UPG_TYPE` (`0x100585BC`)

This suggests a software retry/safe-mode path if an update fails validation. It does not prove
that a corrupt bootloader or an interrupted NAND write can be recovered without hardware.

## Factory test menu

The watch has a hidden factory/test menu. Entering its shutdown/reset option cleared app data
and pairing state on the tested unit. That is why a stale authkey can produce `A052` but no
`A040`; re-pairing restores the OTA path.

Factory-menu options are destructive unless their exact effect is known.

## UART / physical debug

The app contains a UART shell (`nand logic read/write`, `devboot`, `reboot`, `cpuload`) on
`UART_0`. Reaching it requires physical access to the debug pads and a USB-UART adapter. It is
the most promising route for a real unbrick or a NAND dump, but the shell has not been
connected.

## Practical precautions

1. A known-good full image and the exact image to be flashed are kept.
2. Every CRC is verified before flashing.
3. Interrupting power or the BLE link during the data phase aborts the transfer.
4. After a failed flash, the watch reboots and `info` is checked before a retry.
5. If the app never returns, OTA attempts are not repeated; UART/hardware recovery is
   investigated instead.
6. Bootloader images and firmware for a different board are not flashed.

## Bootloop warning

Other CMF/Pro devices have community reports of bootloop modes that are not recoverable over
BLE. No recovery combo has been validated for the Pro 2. Critical units are not exposed to
experimental full-firmware writes.
