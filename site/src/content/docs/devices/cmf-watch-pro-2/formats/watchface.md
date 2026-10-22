---
title: Watch faces and animations
description: NAND layout, WFDL packages, album.cfg, .ajs animations and warm-up mapping.
---

# Watch-face subsystem

Reverse-engineered from `WFManager.c`, `WfmPara.c`, `WFRes.c` and the
string/address map of the 0.73 app. File formats below are the ones the
firmware actually reads; field meanings marked *(?)` are inferred.

## 1. NAND layout

| mount | partition | contents |
|---|---|---|
| `/NAND:A` | `res.bin` | `bt_watch.res`, `bt_watch.sty`, `bt_watch.<lang>`, fonts |
| `/NAND:C` | (user area, not in OTA image) | watch-faces (`albumN.wfdl`), `album.cfg`, user fonts |
| `/NAND:E` | `res_e.bin` | `1.ajs … 19.ajs` animations (SDFS) |
| `/NAND:K` | `sdfs_k.bin` | `logo.res`, `logo.sty`, sounds, `admusic.dsp` |

`res_e.bin` is a normal SDFS container with 19 entries named
`N.ajs`; total 58,777,088 B.

## 2. Watch-face package (`*.wfdl` = FMC `.bin`)

Produced by the phone app, written to `/NAND:C/`, parsed on-watch by
`WFManager_Parser` (`0x100DC3C0`). Format cross-checked byte-for-byte
against FMC's working editor/flasher (`freethinkel/fmc`,
`docs/cmf-protocol.md`, `format/bin.ts`).

```
0x00   36 B  header
0x24   1 B   root tag 0x20
0x25   u16   TLV tree length (LE)
0x27   ...   TLV tree
       ...   resources: [u32 hdr][u32 size][data] ...
end-36 36 B  footer = byte-identical copy of the header
```

Header:

| off | meaning | verified on |
|---|---|---|
| `0x00` | CRC32 **raw** (poly 0xEDB88320, init 0, *no* final XOR) of `file[0x04 : 0x27+tree_len]` — i.e. header bytes 4..0x23 **plus** the root tag, u16 tree length and the whole TLV tree | wf1/wf2/wf3 |
| `0x04` | u32 `1` (dial type / format version) | wf1/wf2/wf3 |
| `0x08` | 15-byte NUL-padded dial name (`"Metric"`, `"Type"`, `"Classical"`) | wf1/wf2/wf3 |
| `0x17` | u8 unknown — `0x0E`/`0x0A`/`0x08` in the samples (preview/style id?) | wf1/wf2/wf3 |
| `0x18` | u32 = `file_size − 36`, i.e. offset of the 36-byte footer (tree + resources) | wf1/wf2/wf3 |
| `0x1C` | u32 resources-section length | wf1/wf2/wf3 |
| `0x20` | CRC32 **raw** of the resources section | wf1/wf2/wf3 |
| `0x24` | root tag `0x20` | wf1/wf2/wf3 |
| `0x25` | u16 TLV tree length | wf1/wf2/wf3 |

`0x24 + 3 + tree_len` = start of the resources; `0x1C` bytes later the
36-byte footer begins; the footer at `file_size − 36` is a byte-identical
copy of `header[0:36]` (checked on all three samples).

The parser reads 0x24, then 3 bytes (`0x20` + `u16` tree length), and
computes `0x27 + tree_len` as the TLV section size. Byte `+0x17` is the
name-field tail it logs.

TLV nodes: `[tag u8][len u16][value]`.

| tag | meaning |
|---|---|
| `0x20` | root wrapper |
| `0x21` / `0x22` | normal / always-on screen |
| `0x01` | struct: `x u16, y u16, meta[14]` + optional image ref |
| `0x02` | visibility bind |
| `0x05` | pivot `flag, x u16, y u16` |
| `0x08` | preview struct (`prefix[5]` + ref) |
| `0x30` | image (static or pick-by-value) |
| `0x60` | number (digit strip, `fmt 0x40` sibling) |
| `0x70` | hand (rotating image) |
| `0x80`/`0x81` | progress ring, procedural / image-clipped |
| `0x5A`/`0x5B` | arc spec for the ring |
| `0x48`/`0x68` | frame / group (auto-layout; skippable) |
| `0x86` | 64-byte display name |
| `0x28` | preview |

`meta[14]`: `w u16, h u16, …, id at [9], sub [10], max u24 at [11..13]`.
`meta[7] == 4` marks the widget accent-tintable at render time. `id` is
the live-data source (0x01 hour, 0x0B minute, 0x19 steps, 0x1A HR, …).

Resources: `[u32 hdr][u32 size][LZ4 block]`, `hdr = (cf & 0x1F) | (w << 10) | (h << 21)`,
`size` u32 LE. Pixel formats: **4 = RGB565** (`w*h*2`), **5 = RGB565 + 8-bit alpha**
(`w*h*3`), **13 = 4-bit alpha** (`w*h/2`), 14 = 8-bit alpha, 24 = BGRA8888,
28 = ETC2 RGBA8 (Pro 3), 1 = raw JPEG. Resource data is LZ4-compressed
(`wfm lz4 dec failed`) with **no frame header and no size prefix**; `cf=1`
JPEG is decode-only. All **180** resources across the three samples
LZ4-decode to exactly `w*h*2`, `w*h*3` or `w*h/2`.

All three real samples have `cf=4`, `cf=5` and `cf=13` resources; the
decompressed size always equals `w*h*bpp` for the format above.

Canvas is 466×466; the round look is artwork, not a clip. An image side
above ~480 px can exhaust the watch's RAM while the face loads.

### 2.1 Real sample packages (pulled from the app cache)

`/data/data/com.nothing.smartcenter/app_flutter/dial/market/dial_file/*_WatchFace.bin`
are the `*.wfdl` packages the app uploads (same format, no wrapper).
Copies: `watchfaces\wf{1,2,3}.bin`.

| file | size | name | tree len | resources | cf counts | top tags |
|---|---|---|---|---|---|---|
| `wf1.bin` | 112,751 | `Metric` | 4,911 | 118 | 4×3, 5×115 | `0x30`×46, `0x60`×34, `0x68`×26, `0x70`×26, `0x02`×21, `0x85`×3, `0x28`×2 |
| `wf2.bin` | 52,123 | `Type` | 935 | 36 | 4×2, 5×11, 13×23 | `0x30`×13, `0x60`×5, `0x28`×2, `0x21`/`0x22`/`0x86`×1 |
| `wf3.bin` | 40,603 | `Classical` | 511 | 26 | 4×4, 5×22 | `0x70`×5, `0x30`×3, `0x28`×2, `0x21`/`0x22`/`0x68`/`0x60`/`0x86`×1 |

For all three: `resources_end == file_size − 36` exactly, and
`header[0:36] == footer`.

`album.cfg` (also in `/NAND:C/`, 166 bytes = 0xA6) is the **current album
dial config**; `WfmPara_LoadAlbum` (`0x100DED80`) reads exactly 0xA6
bytes and validates:

| off | type | meaning |
|---|---|---|
| `0x6C` | u32 | magic `0xAC67BD45` |
| `0x71` | u8  | style count — must be **non-zero** |
| `0x72` | u16 | dial id |
| `0x74` | u16 | x |
| `0x76` | u16 | y |
| `0x78` | u16 | colour (`RGB565`) |

Invalid/missing file → defaults `x=0x60`, `y=0xB0`, `colour=0xFFFF`, and
the manager logs `no album.cfg file,skip`. Bytes `0x00..0x6B` are not
decoded yet (0x6C = 108 = 3×36 or 6×18 — most likely per-style layout
records). The runtime copy uses a different layout: magic at `+0x00`,
`+0x04` u8, `+0x05` u8, `+0x06` u16 id, `+0x08` x, `+0x0A` y, `+0x0C`
colour (this is what the `0x9065` handler logs).

Related paths (from the same code region): `%salbum%d.wfdl`, `%salbum0.cfdl`,
`%saigc0.cfdl` with prefix `/NAND:/`. A built-in 0xA2-byte default
`album.cfg` template is referenced at `0x3804BF4B` (an XIP/flash address,
**not** inside `app.bin`) and is copied into the 166-byte buffer before
parsing — it cannot be extracted from the OTA image.

### 2.2 Builder / compiler rules

These are the rules a writer follows; `fwkit/wfdl.py` implements them
and reproduces `wf1/wf2/wf3` **byte-identically**.

- Tree = `0x20` + `u16(len(body))` + `body`, where `body` is the
  concatenation of the screen TLVs. Node = `[tag u8][len u16][payload]`.
- Container payloads are the concatenation of child TLVs; they are kept
  **verbatim** when re-serialising (the parser also exposes them as
  `Node.children` for inspection). Leaves `0x02 0x40 0x48 0x5A 0x5B 0x5F`
  are never descended into.
- Fixed-leaf widget payloads: `0x01` struct = `x i16, y i16, meta[14]`
  (18 B) then the ref tail at `+18` (payload must be ≥ 25 B); `0x08`
  preview struct = `prefix[5]` then the ref tail at `+5` (payload ≥ 12 B).
  Callers that ignore the tail still render fine.
- Image refs are **absolute file offsets**, i.e.
  `36 + (3 + tree_len) + Σ(8 + stored_size)` of the preceding resources.
  - kind `0x01`: `[0x01][count u16][offset u32 × count]`
  - kind `0x61` / `0x71`: `[kind][count u16][first offset u32][block u16 × count]`
    where `block = stored_size + 8` and following frames are found by
    walking consecutive blocks (only the first offset is stored).
- Because stored sizes are baked into the tree, **resizing any resource
  requires rewriting every ref tail after it**. Re-emitting an untouched
  package reuses the source tree byte-for-byte — vendor files contain
  oddities (e.g. a last block size that disagrees with the real size) that
  a from-scratch serialiser would silently "fix".
- Header/footer: `0x00 = crc32raw(header[4:36] + tree)`,
  `0x18 = total − 36`, `0x1C = len(resources)`,
  `0x20 = crc32raw(resources)`, footer = `header[0:36]`.
- Pixel payloads are **raw LZ4 blocks** (no size prefix): re-encoding
  uses a plain LZ4 block compressor; the decoder needs the size implied by
  `cf/w/h`, so it only has to be *valid*, not byte-identical.
- `cf=5` layout is interleaved per pixel: `RGB565 LE` at `raw[i*3]`,
  alpha byte at `raw[i*3+2]`. `cf=13` packs two pixels per byte with the
  **high nibble for the even pixel** and `alpha = nibble * 17`.
- Widget `x`/`y` are signed 16-bit. Hand value ids:
  `0x0A` hour, `0x0E` minute, `0x72` / `0x12` second.

`fwkit` usage:

```python
from fwkit import wfdl
w = wfdl.parse(open("wf1.bin", "rb").read())
w.set_png(0, open("background.png", "rb").read())   # keeps cf, resizes
open("wf1_new.bin", "wb").write(w.build())
```

CLI: `wf-info`, `wf-res`, `wf-tree`, `wf-unpack`, `wf-set-res`, `wf-build`.

## 3. WFManager state (`0x100DD17C` + globals)

```
+0x00  u8   flags           bit3/bit4 = slot A/B active
+0x08  u8   count           number of dials
+0x09  u8   current index
+0x0A  u8   (?) 
+0x0B  u8   type            1 = album/built-in (no file)
+0x0C  u32  current entry value
+0x10  entry[count]         stride 0x10
        +0x00 u32 value
        +0x04 u8  type
        +0x06 char name[10] (or index into dial table)
+0x30  ptr  parsed dial buffer (from WFManager_Parser)
+0x44  ptr  LVGL object, slot A
+0x48  ptr  LVGL object, slot B
+0x4C  u8   flags
```

`WFManager_Load_ByIdx` (`0x100DD17C`):

- `type == 1` → `album_wf_layout_create(ui, ctx, &entry.name)`
  (built-in album dial, no file I/O).
- otherwise → open file, `WFManager_Parser`, create LVGL object,
  position at `0x1D2` (x?), set flag bit.

`WFManager_Update` (`0x100DD5AC`) drives the carousel/selection and
calls `album_wf_layout_create` for album styles.

## 4. BLE replace flow

`WFManager_ReplaceWatchFaceForBle` (`0x100DE7A8`):

1. takes a mutex, logs `(param_1, param_2)`,
2. stores `+0xAC = param_1`, `+0xBC = param_2`, `+0x98 = 3`,
3. calls `FUN_100DC648()` (reload/parse),
4. allocates 0x10 bytes and schedules the UI update.

The transfer itself is done by the `wewear` file-transfer commands
(`wewear_new_h2d_wf_start`, `wewear_d2h_file_end`, …) which write the
received data to `/NAND:C/` and then invoke the replace above.

### 4.1 Flashing from the host (verified)

`watools/cmf_watch.py` drives the whole flow over BLE (`protocol.md`
§4.1 for the frames): it authenticates with the stored authkey, reads
`0xA055`, and streams the file the watch pulls in `0xC00` chunks.
Appending uses `--old-id 0`. Finish `01` = activated, `0x0A` = stored but
not activated (unknown id, `new_wf_id == old_wf_id`, or all slots full).
An 89 KB append returned `finish: 01`, and the LZ4-fixed rebuild
(`uptime_inv3`, 87,899 B) was confirmed **rendering on the watch** — the
end-of-block fix in §6 is validated on hardware.

Slot management is delete + append, not replace:

- `wf-delete --id <installed> --file <any .wfdl>` sends a normal init2
  with `new_wf_id = 0`. The firmware's manager enters its delete branch,
  removes the entry from `album.cfg` and the file from `/NAND:C`, and
  replies `finish: 01`. Verified on hardware; a following append reused
  the freed slot.
- Replace (`old_wf_id != 0`, `new_wf_id` different) is unreliable — one
  returned `01` but left the dial unaddressable under both ids. `0x0A`
  with a non-zero `old_wf_id` means that id is not installed.
- A dial can't replace itself (`new == old` → `0x0A`); re-flashing the
  same name without deleting first is refused.
- Side-loaded ids are not in `0xA055`; the host keeps its own list of
  appended ids.

## 5. `.ajs` animation files (`/NAND:E`)

A `.ajs` is **not** a plain concatenation of JPEGs: each frame is preceded
by a tile descriptor and the chain is driven by a length field, not marker
scanning.  There are 8 prelude bytes, then repeated records:

```
0x00  u32 0
0x04  bytes 00 00 00 <fps>    (0.73 files: 0x19 = 25 fps; 0 => 25)
0x07  u8   fps               <-- not a version field

record at p (first record p = 0x08):
p+0x00  u16  size            bytes after this field
p+0x02  "JPEG"               Allwinner tile descriptor magic
p+0x06  u16  width           (240)
p+0x08  u16  height          (240)
p+0x0A  u16  tile_width      (240)
p+0x0C  u16  tile_height     (240)
p+0x0E  u16  tile_count N    (1 in every shipped file)
p+0x10  N * (u16 offset, u16 length)   offsets from the "JPEG" magic
p+...   tile JPEG streams    (each FF D8 … FF D9)
next record p' = p + 2 + size
```

For one tile the table is `(18, jpeg_len)`, the JPEG starts at
`p+0x14` (file `0x1C` for the first frame), and
`size = 14 + 4*N + sum(tile lengths)` (= `18 + jpeg_len` for `N=1`).
File `+0x1A` is the first tile's JPEG length.  There is no trailing record:
the last frame ends at EOF.

Tile offsets are relative to the descriptor magic, so a multi-tile frame is
a normal table; the player computes `cols = ceil(width/tile_width)`,
`rows = ceil(height/tile_height)` and blits every tile at
`(col*tile_width, row*tile_height)`, clipped to the canvas.

All 19 files are **sport warm-up animations**, not menu entries.  The
app table at `0x101ED708` (file `0x1ED708`) has 19 16-byte records:
`[sport_type, seconds, category, ajs_index]`.

| `.ajs` | sport type | seconds | category |
|---|---|---|---|
| 1 | 14 | 20 | 9 |
| 2 | 15 | 20 | 9 |
| 3 | 16 | 30 | 9 |
| 4 | 17 | 30 | 9 |
| 5 | 18 | 30 | 9 |
| 6 | 19 | 30 | 9 |
| 7 | 20 | 20 | 9 |
| 8 | 21 | 20 | 9 |
| 9 | 22 | 8 | 11 |
| 10 | 23 | 30 | 9 |
| 11 | 24 | 20 | 9 |
| 12 | 25 | 30 | 9 |
| 13 | 26 | 8 | 11 |
| 14 | 27 | 8 | 11 |
| 15 | 28 | 6 | 11 |
| 16 | 29 | 6 | 11 |
| 17 | 30 | 6 | 11 |
| 18 | 31 | 20 | 11 |
| 19 | 32 | 20 | 9 |

Replacing `N.ajs` only changes the warm-up preview/animation for that
sport; it does not add a dial or menu item.  `9.ajs` (sport type 22,
8-second warm-up) is the closest duration match for the 8.1-second PoC.

Player (`simple_video_8` = open/parse, `simple_video_7` = timer tick,
`simple_video_6` = rewind):

1. 0x16 bytes are read from offset 0, `"JPEG"` is required at `+0x0A`,
   width/height are parsed, and the fps byte is taken from prelude `+0x07`;
2. the seek goes to `0x0A` (descriptor start) and `size + 2` bytes are read;
3. every tile is decoded at `buffer + offset` with `length` bytes via the
   Allwinner JPEG decoder (`FUN_100ffcc4` → `FUN_100ffa70`);
4. the next `size` comes from the two bytes at `buffer[size]`.  At EOF the
   short read yields 0, and `simple_video_6` seeks back to `0x08` to loop.

`simple_video_9` creates `lv_timer_2(cb, 1000 / fps, ...)`; fps 0 is
coerced to 25.  Observed frame counts: 1.ajs = 500, 8.ajs = 500,
9.ajs = 1124, 16.ajs = 1397.  Frames are ~1.5–4.9 KB each; all 19 shipped
files use 240×240 with a single 240×240 tile, and they fill `res_e.bin`
(58,776,014 of 58,777,088 bytes; only 1,074 bytes free).

Unlike `bt_watch.res` pictures, which have a 22-byte
header with `"JPEG"` at `+0x04` and the same 14-byte tile descriptor after
it, `.ajs` stores the descriptor after a 2-byte `size` field.

Tools: `ajs-info`, `ajs-extract`, `ajs-repack`, `ajs-set-frame`
(`fwkit.ajs.build/set_frame/extract`).  At 25 fps a 3:30 clip needs 5,250
frames; at ~4 KB each that is ~21 MB, so a Bad Apple test replaces one or
more existing `.ajs` files (or repacks `res_e.bin` without them) rather
than appending.

Proof of concept: `out/badapple_poc.ajs` is a 202-frame (8.1 s), 240x240,
25 fps clip made from archive.org `TouhouBadApple` at 45 s, cropped to
square and encoded `-q:v 4`.  It is 756,479 B (3,745 B/frame); extract ->
repack is byte-identical.  A full 5,250-frame clip extrapolates to
**18.75 MiB**.  Preview: `out/badapple_poc_sheet.png`.

Offline install candidate: `out/JX402_V1.0.0.73_badapple9.bin` is based
on `JX402_V1.0.0.73_pwned.bin` (the running resource-patched image), so
**only `res_e.bin` differs**.  It replaces `9.ajs` (sport type 22,
8-second warm-up) with the PoC and keeps `res_e` at its original
58,777,088-byte size.  `out/res_e_badapple9.bin` is the padded SDFS
partition.  Verified: AOTA header and all entries, SDFS total/hashes, the
other 18 files byte-identical, and all other partitions byte-identical to
the pwned image.  **Not flashed.**

## 6. Pitfalls

- Watch-faces live on `/NAND:C`, which is **not part of the OTA image**;
  changing them needs BLE upload or on-device file access.
- `WFManager_Parser` trusts the tree length (u16 at +0x25): a wrong
  value causes a short read and a failed load. The 36-byte footer must
  be a byte-identical copy of the header (the phone-side flasher also
  checks the name at `+0x08` equals the name at `len-28`).
- Files are kept ≤ 32 MB and every image ≤ ~480 px per side; the
  firmware decodes all bitmaps at load and an oversized one boot-loops
  the watch. `wfdl.Watchface.build` enforces **480 px per side** and
  **32 MiB per package**, and `wf-set-res`/`wf-build` abort before
  writing; the API bypasses the check with `build(validate=False)` only
  when deliberate.
- A widget's image-ref list must reference **consecutive** resource
  indices; the compiler enforces it and the firmware relies on it.
- The tree stores absolute resource offsets and stored sizes. Swapping a
  bitmap for one of a different size without rewriting the ref tails makes
  the watch read the middle of the resource section — the dial loads but
  shows garbage/black. The rebuild goes through `fwkit`/`wfdl.build`, never
  by hand.
- LZ4 blocks must obey the end-of-block rule: the last sequence is
  literals (≥5 bytes for inputs ≥13), and no match may start within the
  last 12 bytes or extend into the last 5. The watch's
  `LZ4_decompress_safe` fails the whole resource otherwise and the dial
  renders **pitch black**, even though the lenient decoder and the
  uploader's CRC checks accept it. Fixed in `fwkit.lz4.compress`
  (matches now stop at `n-5`); `tests/selftest.py` asserts it for both
  the vendor blocks and freshly compressed ones. Verified on hardware:
  the pre-fix rebuilds (`uptime_rebuild`, `uptime_all_inverted`) rendered
  black; `uptime_inv3` (87,899 B) renders correctly.
- Editing an existing dial (swap a resource, rename) is safe; synthesising
  a new widget graph is not — the tree must be cloned from a real
  package.
- `new_wf_id` is read signed: ids ≥ 0x80000000 are rejected. Capacity is
  6 dials.
- Only 6 slots: an append to a full watch is accepted at init2 and only
  refused at finish (`0x0A`, "stored but not activated").
- `album.cfg` is 166 bytes and validated by magic `0xAC67BD45` at `+0x6C`
  plus a non-zero style count at `+0x71`; a malformed file makes the
  manager fall back to "no album" (`no album.cfg file,skip`).
- `.ajs` frames are baseline JPEG wrapped in a tile descriptor.  When
  re-encoding, the record `size` (`14 + 4*N + sum(tile lengths)`) and
  every tile length are updated; a stale length makes the player read the
  next descriptor from the middle of a frame.  Shipped files are 240×240
  with one 240×240 tile, but the format supports a tile grid.
- Watch-face IDs 1..6 are special-cased in `WfmCheck_CheckFast`
  (built-in dials).
