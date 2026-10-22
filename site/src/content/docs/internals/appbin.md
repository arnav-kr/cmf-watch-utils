---
title: app.bin internals
description: Vector table, ACTH header, address map, OTA/UI/BLE functions and Ghidra pitfalls.
---

# app.bin internals — CMF Watch Pro 2 (0.73)

Reverse-engineering notes for the application image that lives inside the
`TEMP.bin` partition of `JX402_V1.0.0.73.bin`. All addresses are virtual
(`load_base + file_offset`).

Companion docs: `aota.md` (container/patch formats), `protocol.md`
(wire protocol + AT registry), `watchface.md` (WFManager, `.wfdl`,
`.ajs`).

## 1. Identity and extraction

| item | value |
|---|---|
| file | `out/app_073.bin` (extracted from nested AOTA in `TEMP.bin`) |
| size | `0x25A080` = 2,465,920 B |
| crc32 | `0x1AA1663D` (== nested `ota.xml` `<checksum>`) |
| load base | **`0x10000000`** |
| entry (reset) | `0x10105C59` |
| header | `ACTHHTCA` (8 B magic), header size `0x200` |

`TEMP.bin` is itself a complete AOTA archive whose only entries are
`ota.xml` (at `0x400`, `0x3B8` B), `app.bin` (at `0x800`, **stored raw**,
`file_size == orig_size`) and `sdfs.bin` (at `0x25AA00`, `0x5FC0` B, also raw).

Load-base check: the Zephyr banner string sits at file `0x2515C0`
(`0x102515C0`) and its pointer is stored at file `0x19C48C` as
`0x102515C0`. The exported 0.57 image uses the same base, so
`addr = 0x10000000 + file_offset`.

The 0.57 image (`out/app_057.bin`, `0x255080`, crc32 `0x78AE6370`, entry
`0x10102EC9`) is kept only for diffing; do not use its addresses.

## 2. Vector table (file `0x200`)

| idx | handler |
|---|---|
| 0 (SP) | `0x2FFB6740` |
| 1 Reset | `0x10105C59` |
| 2 NMI | `0x101BF2EB` |
| 3–6 HardFault/MemManage/BusFault/UsageFault | `0x10105C2D` |
| 11 SVCall | `0x1010556D` |
| 12 DebugMon | `0x10105C2D` |
| 14 PendSV | `0x101054D1` |
| 15 SysTick | `0x100F9461` |
| 16–79 IRQ | `0x10105615` (shared default) |

Entries from index 80 on are not vector-table entries.

## 3. ACTH header

| off | value (0.73) | meaning |
|---|---|---|
| `0x00` | `ACTHHTCA` | magic |
| `0x08` | `0x10105C59` | entry |
| `0x0C` | `0x62` (98) | unknown |
| `0x14` | `0x10105C59` | entry again |
| `0x18` | `0x259C3C` | payload size − `0xC` |
| `0x1C` | `0xDCF84B50` | unknown checksum (not crc32/sum32) |
| `0x20` | `0x7929C5D6` | unknown checksum (not crc32/sum32) |
| `0x24` | `0x200` | header size |

The image ends with a small `sdfs.bin` (128 B, `sdfs.txt` +
`ZEPHYR_TOOLS=D:\ProgramData\Actions\ZEPHYR_SDK`). It is not validated by
the updater.

## 4. Static analysis (Ghidra 12.1, Thumb LE)

Project `proj073` (`wp2`), import at base `0x10000000`
with `-processor ARM:LE:32:Cortex -loader BinaryLoader`. The pre-script
`MarkVectors.java` seeds the 80 vector entries before analysis.

| metric | value |
|---|---|
| functions | 8,869 |
| code bytes | 1,297,446 |
| highest code end | `0x10254FAC` |
| strings | 9,296 |
| funcs referencing strings | 2,883 |
| renamed from `__FILE__` | 510 |
| decompiled | 70 |

Analysis initially left a ~9 KB hole at `0x100E88AE–0x100EAC84`
(the `GETVERSION`/`GETGPSVERSION` handlers). `DisasRange.java`
force-disassembles it and re-analysis recovers +927 functions.

Outputs: `out/functions.txt`, `out/strings.txt`, `out/blocks.txt`,
`out/function_map.tsv`, `out/decomp/*.c`, `out/string_xrefs.txt`.

### Subsystem counts (by source path evidence)

bt 461 · ui 361 · kernel 264 · fs 119 · ota 115 · power 77 · audio 62 ·
sensor 39

## 5. Key symbols (0.73)

### Startup / version

| address | what |
|---|---|
| `0x10105C59` | reset |
| `0x100F7374` | prints Firmware / res / System version |
| `0x1005E728` | "JX APP Firmware Version" printf |
| `0x10054CF4` | `system_app_launch` (logs `ota_fg`, `res_chk`, `ver_chk`) |
| `0x10054DBC` | `system_app_launch_init` |
| `0x1019C478` | Zephyr boot banner |

`GETVERSION` / `GETGPSVERSION` handlers (found after recovering the 9 KB
hole; both take `(ctx, has_data)` and reply through `FUN_100E895C`):

| address | what |
|---|---|
| `0x100EA50C` | `GETVERSION` → `GETVERSION:%s,OK` / `GETVERSION,FAIL`; version text via `ux2sys_api_7(0x11, buf)` |
| `0x100E8BD0` | `GETGPSVERSION` → `GETGPSVERSION:%d,OK` / `...FAIL`; value via `thunk_FUN_101978C0()` |
| `0x100E895C` | response dispatcher: transport `0` → UART/print (`FUN_101BA194`), `1` → BLE (`FUN_100CB9E0`) |

The command registry that dispatches these lives in the data region around
`0x101F4D40` (Thumb pointers with bit 0 set, e.g. `0x100E7265`,
`0x100E7F79`) and holds `(name, handler)` descriptors, including
`0x100EA50D` next to `0x1022925F` ("GETVERSION"). The exact struct layout
is still undecoded.

### wewear (phone protocol)

| address | what |
|---|---|
| `0x100CBF84` | read thread; logs `head_id/ability_id/cmd_id/length_id` |
| `0x100CC0C8` | connect |
| `0x100CC2D4` | `wewear_ble_send_data` |
| `0x100CC51C` | `wewear_app_init` |
| `0x100EC400` | dispatcher (`invalid head_id`, `unkown cmd_id`) |
| `0x100EB12C` | `wewear_d2h_file_end` |
| `0x100D343C` | `REQ_VERSION` handler |
| `0x100D2D5C` | `sync_watch_face_info` |
| `0x100CD324` | `updata_send` progress |
| `0x101B60F0` | `UpdataRecordData` |
| `0x100EC5B8` | `wewear_ftp_app_init` |

Frame header field names are confirmed by the log string at `0x10220D73`:
`head_id, ability_id, cmd_id, length_id`.

### Watch-face manager

| address | what |
|---|---|
| `0x100DC3C0` | `WFManager_Parser` |
| `0x100DC4EC` | `_WFManager_DelFile` |
| `0x100DC82C` | `WFManager_Unload` |
| `0x100DCEF4` | `WFManager_UnoadThumbResource` |
| `0x100DCFEC` | `WFManager_DelRubbish` |
| `0x100DD17C` | `WFManager_Load_ByIdx` |
| `0x100DD37C` | `WFManager_Reload` |
| `0x100DD3E4` | `WFManager_SetCurrIdxForUi` |
| `0x100DD5AC` | `WFManager_Update` |
| `0x100DE020` | `WFManager_Init` |
| `0x100DE7A8` | `WFManager_ReplaceWatchFaceForBle` |
| `0x100DE80C` | `WFManager_DelIdxForUi` |
| `0x102243F1` | string `WFManager_SetWatchFaceListForBle` (no direct xref) |
| `0x1009A0A4` | `album_wf_layout_create` |
| `0x1009A384/A4F0/A890/AB18` | album styles 1–4 |
| `0x100B15DC` | logs `wf_idx: %d, %d` |

### OTA / upgrade

| address | what |
|---|---|
| `0x10058438/0x10058508/0x10058544` | `OTA_UPG_FLAG` users |
| `0x100585BC` | `OTA_UPG_TYPE` |
| `0x1006B3A8/0x1006B3F8/0x1006B444` | `ota_view_init/_deinit/_safemode_set` |
| `0x100CDCAC` | "ota app" (msgbox) |
| `0x101566F0` | `ota_backend_bt_init` |
| `0x10157278` | update OTA state |
| `0x10157E84`, `0x10158390` | `ota file_cnt` |
| `0x101581E8` | `ota_rx` thread |
| `0x10158390` | `handle upgrade` |
| `0x10159264` | OTA image progress |
| `0x1015A164` | `ota_cfg` default init |
| `0x1015A1F0` | `ota_cfg` file deinit |
| `0x1015A238/0x1015A314` | `ota_manifest_cfg_files_update` |
| `0x1015A66C/0x1015A6D0` | `OTA_INFO` |
| `0x1015A6FC` | "ota.ota" |
| `0x10244963` | string `wewear_ota_cmd_d2h_end` |

### UI / framework

| address | what |
|---|---|
| `0x100D7488/0x100D77A8/0x100D7854` | `jx_msgbox_creater` variants |
| `0x100D587C` | `ui_view_creater` |
| `0x1018F4A0` | `lvgl_virtual_display_create` |

## 6. Patching recipe

`app.bin` is raw inside the nested AOTA (no LZMA). To patch one or more
contiguous bytes of code/data at virtual address `addr`:

1. `aota.parse(TEMP.bin)` → nested header, XML, parts.
2. Patch `app.bin` at `addr - 0x10000000` (same-length only).
3. Set nested XML for `app.bin`: `file_size = orig_size = len(app)`,
   `checksum = crc32(app)`.
4. Rebuild nested AOTA (`aota.build`, 0x200-aligned parts, AGPS empty) —
   this recomputes the header CRC.
5. Store back into TEMP: `pack_chunks(nested, 0x200000)` and set outer XML
   for `TEMP.bin` to `file_size = orig_size = len(stored)`,
   `checksum = crc32(stored)` (special case for TEMP).
6. Rebuild outer AOTA (res.bin etc. untouched).

The SDK implements this as `Image.edit_app(addr, patch)` /
`fwkit edit-app IMG ADDR HEX OUT`.

Verified: patching 7 bytes at `0x102515C4` ("Booting" → "Patched")
produces an image that passes `fwkit verify`, changes exactly 7 bytes of
`app.bin`, and updates the nested checksum to `crc32(new_app)`.

Caveats:
- Fixed-length patches only; growing code requires relocating data and is
  not supported.
- The device does not appear to re-validate `app.bin` at boot (the 0.57
  resource patch booted), but this has not been tested with a code patch.
- `0x1C`/`0x20` in the ACTH header are not verified by the updater.

## 7. AT command registry

71 AT commands are dispatched from a 20-byte-record table at
`0x101F4DA4` (names at `0x10229250`, handlers Thumb pointers).
`GETVERSION = 0x100EA50C`, `GETGPSVERSION = 0x100E8BD0`; replies go
through the transport switch `0x100E895C` (0 = UART, 1 = BLE).
Full list in `protocol.md`.

## 8. RE pitfalls (Ghidra)

- **The project directory must exist** before `analyzeHeadless` runs.
- The `.bat` wrapper ends in `pause`; always redirect `< nul`.
- `-processor ARM:LE:32:Cortex` means **Thumb little-endian** — correct
  for this image.
- **Analysis skips unreferenced code.** A ~9 KB region at
  `0x100E88AE–0x100EAC84` (all AT handlers) produced no functions until
  `DisasRange.java` force-disassembled it. Always diff function coverage
  against the code range before trusting a symbol map.
- Pointers to strings appear in **literal pools** at the end of the
  *previous* function; a hit does not mean the containing function is
  the handler. Use `getFunctionContaining(reference)` after analysis,
  not a raw pointer scan.
- `0x101F4DA4`-style tables are only referenced indirectly; the code
  consumer may load a base address from a different data structure.
- The 0.57 and 0.73 images share the base but not addresses — always
  check the entry point (`0x10102EC9` vs `0x10105C59`) before using a
  symbol.

## 9. Open questions

- Meaning of ACTH `0x1C`/`0x20`, and of `0x0C = 0x62`.
- `album.cfg` field offsets (166-byte struct); the `.wfdl` body is the
  FMC `.bin` TLV format (see `watchface.md`).
- Whether the running firmware checksums `app.bin` at runtime.
- `.ajs` is mapped: prelude `+0x07` is fps (0 => 25), record `+0x0E` is the
  tile count, and `+0x1A` is the first tile's JPEG length (see
  `watchface.md` §5).
- `.sty` ids are opaque builder-assigned values; scene/node tree is mapped
  (see `style.md`); type 2 `+0x18` font/style encoding remains open.
- RES `0x05`/`0x0E` bitmap codecs are mapped (LZ4 HC 12; see
  `pictures.md`).
