---
title: Limitations & risks
description: Hard boundaries, unresolved formats, recovery uncertainty and safe operating rules.
---

## Read this before flashing

1. **A failed full OTA can leave the watch unbootable.** We have not proven a button-only or
   BLE-only recovery path for every failure. If the app or bootloader never comes back, the
   realistic recovery options are a NAND dump/programmer or vendor service.
2. **The bootloader is not part of the AOTA image.** The OTA updates app-visible partitions
   (`TEMP`, `res`, `fonts`, `res_e`, `sdfs_k`); it does not replace the bootloader.
3. **Keep the watch charged.** A 54 MB image took roughly 10–30 minutes on our unit at BLE
   MTU 23. A weak battery is an avoidable risk.
4. **Do not interrupt the transfer.** Disconnecting during the data phase aborts the staging;
   the watch may reboot back to the old image, but do not rely on that.
5. **Factory menus can clear app data.** Entering a shutdown/reset option from the factory test
   menu cleared pairing state on our unit. A stale authkey still produced an `A052` reply but
   no `A040`; re-pairing fixed it. See [Shell / AT channel](/cmf-watch-utils/protocol/shell/).

## Storage ceiling

`/NAND:E` (`res_e.bin`) is a fixed **58,777,088-byte** partition.

- Stock content uses 58,776,014 bytes, leaving only **1,074 bytes** free.
- Replacing only `9.ajs` (4,795,064 bytes) with the 756,479-byte PoC leaves about
  **3.85 MiB** free.
- Full Bad Apple is ~19.7 MB (~18.75 MiB), so it requires deleting other warm-up animations,
  reducing resolution/fps/frame count, or splitting the clip.

## Protocol and access limitations

- **No arbitrary file read.** The shell and BLE command set expose status/control operations,
  not a general NAND read. `album.cfg` and user watch faces in `/NAND:C` cannot be pulled
  without a dump or a patched app.
- **No unlock or privilege escalation.** The tools use the normal authenticated app protocol.
- **Pairing state matters.** The authkey is derived from the pairing exchange and the watch
  secret. After a reset/re-pair, old state produces misleading acknowledgements.
- **Encryption is fixed for this generation.** Frame payloads use AES-CBC with a fixed IV and
  a session key derived from the nonce and authkey. Other firmware may change this.

## Unresolved formats and internals

- ACTH header checksum bytes (`+0x1C`, `+0x20`) and `+0x0C = 0x62` are not fully explained.
- `.sty` type-2 `+0x18` font/style selector encoding is unresolved.
- `album.cfg` contents and the `.wfdl` header byte `+0x17` are not fully mapped.
- The six AT group callbacks at `0x101F4D70` are not fully identified.
- SPP opcodes `0x905E`/`0x9060`/`0x9063` semantics are only partly understood.
- `0xA055` byte 3 reads `7` on our unit while the public FMC client hardcodes capacity `6`;
  side-loaded dials are not reported in the list.

## Tested scope

- **Watch:** CMF Watch Pro 2, board `JX402`, SoC Actions ATS3089C, firmware `1.0.0.73`.
- **Host:** Windows 11 with Python 3.12 and Bleak. Bleak is cross-platform, but macOS/Linux BLE
  behaviour and MTU negotiation were not validated by us.
- **Do not assume** these addresses, UUIDs, keys or layouts apply to the Pro 3, a different
  firmware build, or a regional variant. Diff before writing.

## Ethical and legal limits

- Do not use this to bypass account, payment, or device-ownership protections.
- Do not redistribute stock firmware, vendor resources, or copyrighted animation frames unless
  you have the right to do so.
- The code is AGPL-3.0 because part of the BLE/watcher behaviour is a port of the AGPL-3.0
  `freethinkel/fmc` project. See [Legal & attribution](/cmf-watch-utils/overview/legal/).
