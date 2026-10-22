---
title: Picture codecs
description: JPEG, raw LZ4, RGB888/RGB565 and all verified picture decoders.
---

# WP2 pictures: `bt_watch.res`, `logo.res` (LZ4 / JPEG)

Status: **fully reversed and verified against all 2,192 entries of
`bt_watch.res`** (2026-10). Every entry decodes; every `.sty` type-4
node's `w/h/bpp` matches the decoded image (0 mismatches).

This document supersedes the "unknown codec" notes in `aota.md` §RES.

---

## 1. Entry layout inside a `RES\x19` file

The RES table (see `aota.md`) gives, for entry `i`:

```
u16 len          # bytes of the stored payload, INCLUDING the 4-byte w/h prefix
u8  type         # 0x26 | 0x05 | 0x0E  (pictures);  text tables use their own type
char name[9]     # NUL padded, e.g. "PIC332"
u32 end          # absolute end offset of the payload (last entry's is NOT stored)
```

The payload of entry `i` starts at **`end[i-1]`** (the previous entry's
`end`, or the pool offset `hdr[0x10]` for `i == 1`) — **not** at
`end - len`. Entries are padded to a **4-byte boundary**, so
`end[i] - end[i-1] == align4(len[i])` and there may be 0–3 filler bytes
after the payload. This is exactly what the firmware does
(`FUN_10187088` builds `p[i] = prev_end + 4`, `q[i] = len - 4`).

Payload:

```
+0x00  u16 w
+0x02  u16 h
+0x04  body ...        # format depends on type
```

The firmware **skips the 4-byte `w,h` prefix** (`+4`, `len-4`) and hands
`body` to one of two decoders (`FUN_101878e4`):

```c
if (*(u32*)body == 'JPEG')  jpeg_decode(body, len-4, dst, 1, w, 0,0, w, h);
else                        (*_DAT_00007090)(body, dst, len-4, dst_capacity);
```

`_DAT_00007090` is a RAM function pointer with the exact signature of
`LZ4_decompress_safe(src, dst, compressedSize, dstCapacity)` — and it is
LZ4: raw block, no frame header, no size prefix.

---

## 2. Type table

| type | name | body format | output size | count in `bt_watch.res` |
|---|---|---|---|---|
| `0x26` (38) | `pic_jpeg` | `"JPEG"` + 18-byte header + baseline JFIF | `w*h*3` | 358 |
| `0x05` (5) | `pic_lz4_888` | raw **LZ4 block** | `w*h*3` (RGB888) | 910 |
| `0x0E` (14) | `pic_lz4_565` | raw **LZ4 block** | `w*h*2` (RGB565 LE) | 924 |

So the RES `type` byte is the *pixel format*, not the codec family:
both 5 and 14 are LZ4; only the bytes-per-pixel differs. This matches
the `.sty` node's `bpp` field (2 ↔ type 0x0E, 3 ↔ type 0x05).

### 2.1 JPEG (`0x26`)

```
+0x00 u16 w
+0x02 u16 h
+0x04 "JPEG"            (4A 50 45 47)
+0x08 u16 width
+0x0A u16 height
+0x0C u16 tile_width
+0x0E u16 tile_height
+0x10 u16 tile_count N   (1 for all 358 shipped JPEGs)
+0x12 N * (u16 offset, u16 length)   offsets from "JPEG"
+...     baseline JFIF (FF D8 FF E0 … FF D9)
```

For one tile the table is `(18, payload_len)` and the JPEG starts at
body+0x16; `payload_len` is `len-22` (4-byte w/h prefix + 18-byte tile
header).  This is the same tile descriptor used by `.ajs`; the player
decodes every tile and composites a grid.

### 2.2 LZ4 (`0x05`, `0x0E`)

```
+0x00 u16 w
+0x02 u16 h
+0x04 LZ4 block, raw (no magic, no 4-byte size, no frame checksum)
```

The decompressed length is **exactly `w*h*bpp`** (bpp 3 for type 5,
2 for type 0x0E). The decoder is given a destination buffer of that
size by the caller; the LZ4 block must not overrun it.

The vendor uses **LZ4 HC level 12**.  `lz4.block.compress(data,
store_size=False, mode="high_compression", compression=12)` reproduces
every one of the 1,834 shipped LZ4 bodies **byte-for-byte**;
`fwkit.lz4.compress` uses it (with a pure-Python fallback).

Many images have a body that begins `1f 00 01 00` — that is *not* a
header. It is a perfectly ordinary LZ4 token stream that expands to a
run of the same byte (token `0x1f` = 1 literal + 19-byte match at
offset 1), i.e. a flat colour region at the top-left. It is not stripped.

Pixel layout (verified by rendering, see §5):

* RGB888: `R,G,B` per pixel, top-down, row-major, no padding between rows.
* RGB565: little-endian `u16`, `RRRRRGGGGGGBBBBB`, top-down.
* No alpha channel; transparency comes from the `.sty` node / blit rules.

---

## 3. Firmware code path

| address | role |
|---|---|
| `FUN_10187088` | opens `.sty`, `.res`, `.enG`; parses `RES\x19` headers; reads the 16-byte entries (from **0x10**, so the first 4 bytes of each "entry" are the previous `end`) and builds `off[]` (`prev_end+4`) and `len[]` (`len-4`) arrays at `ctx+0x34/0x38` and the second file's at `ctx+0x70/0x74` |
| `FUN_101878e4` | type-4 loader: binary-searches `off[]/len[]` by picture index, seeks + reads, JPEG/LZ4 dispatch, cache/refcount |
| `FUN_100FFCC4` | JPEG wrapper (`ezJPEG`-style decode into `w*h*3`) |
| `*0x7090` | `LZ4_decompress_safe` (RAM pointer, set at boot) |
| `FUN_101072f0` / `FUN_10107238` | seek / read on the RES file handle |

The picture index used by the `.sty` node is **1-based**; node
`pic_index == 1` is `PIC1`.

---

## 4. `logo.res` (boot logo, in `sdfs_k.bin`)

`RES\x19`, 3 entries, all **type `0x0E`** (LZ4 RGB565), pool at `0x40`:

| # | w×h | stored len |
|---|---|---|
| 1 | 222×132 | 6,278 |
| 2 | 216×84 | 3,671 |
| 3 | 220×148 | 5,106 |

Same decoder; `logo.sty` (1 scene, 466×466) references them.

---

## 5. Verified round-trip / samples

* All 2,192 `bt_watch.res` entries decode without error.
* Decoded sizes: 358 × JPEG (`len == 22 + payload_len`), 910 × `w*h*3`,
  924 × `w*h*2`.
* Node cross-check: 1,342 node-referenced pictures — `w/h` and `bpp`
  match in 100 % of cases.
* Rendered samples (host PNG; JPEG decode uses Pillow):
  `out/pic333.png` = 72×72 gears (the Settings icon; the label patched
  to `PWNED!!!`), `out/pic335.png` = 120×120 trash-can button,
  `out/pic332.png` = 466×466 yellow-on-blue test frame (RGB888).

---

## 6. Editing pictures (recipes)

* **Replace with same size class:** the body is encoded directly.
  * type 5 → `fwkit.lz4.compress(img_rgb888)`.
  * type 0x0E → `fwkit.lz4.compress(rgb565_le)`.
  * These produce the vendor's HC-12 stream byte-for-byte.
  * The `w,h` in the 4-byte prefix stays identical to the `.sty` node,
    or the node is updated as well.
* **Size changes are fine** — the RES table stores absolute `end`
  offsets and `len`; the whole file is rebuilt with `fwkit.res.build`
  (bytes after the edited entry shift, which the table absorbs).
* **JPEG:** any baseline JFIF of the same `w×h` works; the tile table's
  offset/length entries are updated, or the header is regenerated with
  `picres.encode_jpeg_body(w, h, rgb, quality=85)`.  After `"JPEG"` is the
  same 14-byte tile header used by `.ajs` (count at +0x10, table at +0x12).
* The slice is `prev_end .. prev_end+len` (gap padding); `end-len` is never
  used.

`fwkit` implements all of it (`fwkit/picres.py`, `fwkit/lz4.py`,
`fwkit/png.py`):

```powershell
$fw = "fwkit"
& $fw pic-list   out\bt_watch.res
& $fw pic-decode out\bt_watch.res PIC335 icon.png
& $fw pic-set    out\bt_watch.res PIC335 icon.png -o res_new.bin
& $fw edit-pic   IMG res.bin bt_watch.res PIC335 icon.png out.bin
```

`parse()`/`build()` of `bt_watch.res` and `logo.res` are byte-identical.
Verified end-to-end: replacing `logo.res` PIC1 with an inverted PNG and
rebuilding the whole image (4.5 s) passes `fwkit verify`, and the picture
decodes back correctly. `pic-set` now re-compresses with LZ4 HC 12, so an
unchanged image reproduces the vendor body exactly; changed pixels still
get a valid block and the RES table absorbs any size change.

---

## 7. Pitfalls

1. Entry start is `end[i-1]`, not `end[i]-len[i]` (4-byte padding).
2. The 4-byte `w,h` prefix is **inside** `len` but is skipped by the
   loader; `body` is not a standalone file.
3. `1f 00 01 00` at the start of many LZ4 bodies is data, not magic.
4. Type byte encodes bpp (5 ⇒ 3, 0x0E ⇒ 2); a mismatched node `bpp`
   makes the UI read past/short of the buffer.
5. The last RES entry has no stored `end`; it ends at EOF.
6. `bt_watch.res` lives inside `res.bin`, which is itself LZMA-chunked —
   decompression (`img.decompressed("res.bin")`) precedes `sdfs.parse`.
7. JPEG payload length must equal `len-22`; the firmware validates it.
