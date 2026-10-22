---
title: AOTA container
description: Outer AOTA image, XML, LZMA chunks, TEMP, SDFS and rebuild recipes.
---

# AOTA firmware container — format and patching

Everything here was verified byte-for-byte against
`JX402_V1.0.0.73.bin` (56,903,752 B) with `fwkit`.

## 1. Outer image

```
0x000  header (0x400)
0x200  partition table (32-byte entries)
0x400  OTA XML
...    partitions (0x200-aligned)
...    AGPS blob (header[0x14], 0x200-aligned)
```

Header:

| off | meaning |
|---|---|
| `0x00` | magic `ACTHHTCA` |
| `0x04` | `crc32(file[0x08:0x400])` |
| `0x08` | build/version string (32 B area) |
| `0x0C` | partition count |
| `0x14` | AGPS offset |
| `0x18` | unknown (preserved as-is) |
| `0x40` | version string (16 B) |
| `0x60` | board name (16 B) |

Table entry (32 B): `name[16] | offset(u32) | size(u32) | 0(u32) | crc32(u32)`.
`crc32` is computed over the **stored** bytes of that partition.

Known partitions: `TEMP.bin`, `res.bin`, `fonts.bin`, `res_e.bin`,
`sdfs_k.bin`.

## 2. OTA XML

`<file_size>` = stored size, `<orig_size>` = decompressed size,
`<checksum>` = `crc32(decompressed)`.

**Exception — TEMP.bin**: `file_size == orig_size == stored size` and
`checksum = crc32(stored)`. TEMP is not a normal compressed partition;
it is a nested AOTA container stored with the chunk header but with
compression effectively bypassed (see below).

## 3. Chunked storage

Non-TEMP partitions are stored as a sequence of chunks:

```
"LZMA" | u32 hdr_len=16 | u32 comp_size | u32 uncomp_size | XZ stream
```

- resource partitions: chunk size **0x8000**
- `TEMP.bin`: chunk size **0x200000**
- `lzma.compress(piece, format=FORMAT_XZ, preset=6)` reproduces the
  original sizes exactly.

## 4. Nested AOTA inside TEMP.bin

`TEMP.bin` decompresses to a complete AOTA image (same header/table/XML
layout) containing:

| entry | offset | size | storage |
|---|---|---|---|
| `ota.xml` | 0x400 | 0x3B8 | raw |
| `app.bin` | 0x800 | 0x25A080 | **raw** (`file_size == orig_size`) |
| `sdfs.bin` | 0x25AA00 | 0x5FC0 | raw |

Do **not** LZMA-compress `app.bin` when rebuilding the nested image —
the updater accepts it raw and the original is raw.

## 5. SDFS filesystem

Used inside `res.bin`, `sdfs_k.bin`, and the small tail blob of
`app.bin`.

```
0x00 "sdfs.bin"
0x08 u32 0
0x0C u32 count
0x10 u32 total_size
0x14 u32 0
0x18 u32 sum32(table)   // entries region
0x1C u32 sum32(data)    // file data region
0x20 entries[count] (32 B each)
```

Entry: `name[12] | offset(u32) | size(u32) | 0 | 0 | hash(u32)`.
`hash = sum32(file data)` where `sum32` is the sum of little-endian u32
words mod 2^32 (trailing bytes treated as a partial word). Data starts
at `align(0x20 + count*32, 0x40)`; files are `0x40`-aligned.

## 6. RES resource files

Header (0x14 B):

| off | meaning |
|---|---|
| `0x00` | magic `RES\x19` |
| `0x04` | flags/subtype; **low 16 == entry_count** |
| `0x08` | 0x200 |
| `0x0C` | 0 (strings) / 0x20000 (pictures) |
| `0x10` | pool offset — **always `0x10 + count*16`** |

Entries at `0x14`, 16 B each:
`u16 len | u8 type | name[9] | u32 end_offset`.

The payload is `blob[end-len : end]` and **includes the trailing NUL**
for strings. Names are NUL-terminated within the 9-byte field.

**Overlap quirk:** the pool starts at `0x10 + count*16`, i.e. 4 bytes
before the natural end of the table (`0x14 + count*16`). The **last
entry's `end_offset` is therefore not stored** — it would sit inside the
pool. Its end is implicitly EOF. `fwkit` reproduces this exactly and
round-trips `bt_watch.zht` / `bt_watch.eng` byte-identically.

### Picture resources (`bt_watch.res`, 6.6 MB, `0xC0000890`)

2192 entries, names `PIC1 … PIC2192`, **three payload types**
(an earlier note that all were JPEG was wrong). Every entry is
`u16 w | u16 h | body`; the firmware skips the 4-byte prefix and picks
the decoder from `body`:

| type | count | body | codec | output |
|---|---|---|---|---|
| `0x26` (38) | 358 | `"JPEG"` + 18-B hdr | baseline JFIF | `w*h*3` |
| `0x05` (5) | 910 | raw **LZ4 block** | no frame/size header | `w*h*3` RGB888 |
| `0x0E` (14) | 924 | raw **LZ4 block** | no frame/size header | `w*h*2` RGB565 LE |

`0x26` payload (verified, `len == 22 + payload_len`):

```
+0x00 u16 width      +0x08 u16 width    +0x10 u16 tile_count (1)
+0x02 u16 height     +0x0A u16 height   +0x12 u16 tile_offset (18)
+0x04 "JPEG"         +0x0C u16 tile_w   +0x14 u16 tile_size (len-22)
                     +0x0E u16 tile_h   +0x16 baseline JPEG (FF D8 …)
```

The type byte is effectively the **pixel format** (5 ⇒ 3 bpp, 0x0E ⇒ 2
bpp), matching the `.sty` node's `bpp`. The frequent `1f 00 01 00` at the
start of LZ4 bodies is stream data (a flat run), **not** a magic.

Full details, verified decode of all 2,192 entries, and edit recipes:
**`pictures.md`**.

`.sty` image nodes point into this file by **1-based PIC index**
(`node[0xC]`: 0x1F…0x890 = PIC31…PIC2192).

## 7. Patch recipes

### Resource string (verified)

1. unpack `res.bin` → SDFS → `bt_watch.<lang>` → RES.
2. replace the string (same length limit: `len` field).
3. rebuild RES, rebuild SDFS (recompute sums/hashes), repack `res.bin`
   (`0x8000` chunks), update XML `<file_size>/<orig_size>/<checksum>`,
   rebuild outer header + table CRC.

`fwkit edit-res IMG PART RESFILE KEY VALUE OUT`

### app.bin byte patch (verified)

1. `aota.parse(TEMP.bin)` → nested XML + parts.
2. patch `app.bin` at `addr - 0x10000000` (same length).
3. nested XML: `file_size = orig_size = len(app)`, `checksum = crc32(app)`.
4. `aota.build(...)` → nested image (header CRC recomputed).
5. outer TEMP: stored = `pack_chunks(nested, 0x200000)`.
6. outer XML TEMP: `file_size = orig_size = len(stored)`,
   `checksum = crc32(stored)`.
7. rebuild outer image.

`fwkit edit-app IMG ADDR HEX OUT`

## 8. Pitfalls

- **`unpacked/app.bin` can be stale.** The real 0.73 app lives in the
  nested AOTA; always extract it from the image you are patching
  (`Image.app_image()`).
- **Two different app sizes exist**: 0.57 = `0x255080`,
  0.73 = `0x25A080`. Their code addresses differ.
- **TEMP XML is special** — using normal XML rules makes the updater
  reject the image.
- **TEMP chunk size is 0x200000**, resources are 0x8000. Mixing them
  changes every compressed size and can break the header length check.
- **Every checksum must be recomputed**: outer header CRC, per-entry
  stored CRC, XML checksums, SDFS sums/hashes. `fwkit` does all of them.
- **RES entry `len` is per-entry**, so strings may grow or shrink; the
  pool is rebuilt and all `end_offset`s shift. The one invariant is the
  last entry (its end is EOF) and `pool == 0x10 + count*16`.
- Pictures use three formats (see `pictures.md`); a size change requires
  rebuilding the picture table. Payload start is `end[i-1]` with 4-byte
  padding, **not** `end-len`. `bt_watch.res` is ~6.6 MB, so a full rebuild
  is cheap; re-compress with raw LZ4 (no frame, no size prefix).
- Resizing any partition changes its stored/chunked size; update the XML
  `file_size`/`orig_size`/`checksum` for that partition (the SDK does).
- **AGPS trailer** must be copied verbatim; it has no signature.
- **Header 0x18 and app ACTH 0x1C/0x20 are not validated** — preserve
  original values.
- **Flashing is incremental** and slow (~44 KB/s, ~2 min for a resource
  patch). Use `--pipeline 1`; do not interrupt power during the
  `TEMP`/`res` write phase.
- **`AT+GETVERSION` is a compile-time constant**, not read from XML —
  changing the XML version does not change the reported version.
- The device boots an image whose `app.bin` got a 7-byte code patch
  (verified: resource patch booted; code patch not yet flashed).
