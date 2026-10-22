---
title: Recovery & boot modes
description: What is known about recovery, safe mode, OTA flags and the limits of unbricking.
---

## Honest summary

There is **no confirmed, button-only recovery path** for every failure mode on this watch.
The app contains safe-mode and launch/retry logic, and OTA flags persist across reboots, but we
have not deliberately bricked a unit to prove a universal recovery sequence.

Do not treat the notes below as a guarantee.

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
and pairing state on our unit. That is why a stale authkey can produce `A052` but no `A040`;
re-pairing restores the OTA path.

Treat factory-menu options as destructive unless you know exactly what they do.

## UART / physical debug

The app contains a UART shell (`nand logic read/write`, `devboot`, `reboot`, `cpuload`) on
`UART_0`. Reaching it requires physical access to the debug pads and a USB-UART adapter. It is
the most promising route for a real unbrick or a NAND dump, but we have not connected it.

## Practical precautions

1. Keep a known-good full image and the exact one you flash.
2. Verify every CRC before flashing.
3. Do not interrupt power or the BLE link during the data phase.
4. After a failed flash, wait for the watch to reboot and check `info` before retrying.
5. If the app never returns, stop repeating OTA attempts; investigate UART/hardware recovery.
6. Do not flash a bootloader image or a different board's firmware.

## Bootloop warning

Other CMF/Pro devices have community reports of bootloop modes that are not recoverable over
BLE. We have not validated a recovery combo for the Pro 2. If your unit is critical, do not
experiment with full-firmware writes.
