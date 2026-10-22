---
title: Style bundles (.sty)
description: Scene tables, node graphs, RES pools and loader behaviour.
---

# UI style bundles (`.sty`, `.res`, `.font`)

The watch UI is **not** LVGL-XML - it is a small compiled scene format in
`/NAND:A/bt_watch.sty`, paired at runtime with three other files:

| file | role |
|---|---|
| `/NAND:A/bt_watch.sty` | scene table + node tree (this doc) |
| `/NAND:A/bt_watch.res` | picture pool, `RES\x19` (see `pictures.md`) |
| `/NAND:A/bt_watch.<lang>` | string pool, `RES\x19` |
| `/NAND:K/logo.sty` | boot/power-off bundle (same format; 204 B, **1** scene 466x466, id `0x3632CB4E`, no scene data) |

## 0. Ids are opaque, not name hashes

Scene and node ids are **builder-assigned 32-bit values**, not hashes of
the source names:

- there is **no 32-bit hash function** in `app.bin` (no FNV/SDBM/DJB2
  constants; the loader only compares ids);
- node ids often come in **sequential runs** - 481 of 2,382 top-level ids
  sit in 74 runs of 3 or more (up to 10, e.g. `0x4D2F8D80..0x4D2F8D87`),
  which is an allocator, not a hash;
- ~30M candidates (fnv1a/fnv1/sdbm/djb2/java/bkdr/crc32 + md5/sha1/sha256
  truncations, utf8/utf16le/utf16be, common prefixes/suffixes) matched
  **none** of the 181 scene ids and none of the real node ids (17
  coincidental node hits only).

That is why literal pools in `app.bin` contain opaque constants such as
`0x5D695BF2` - they are scene ids read from the code, not pointers.
**When authoring, use any unused u32 and never renumber an id that
`app.bin` references.**

## 1. Loading path (0.73)

```
FUN_10184804(scene_id, out, sty_path, res_path, lang_path)
  └─ FUN_10184674(sty, res, lang, 1, 0)      // cache by path
       └─ FUN_10187088(sty, res, lang)       // main loader, 0xA8 ctx
            ├─ opens sty   → ctx+0x00  (main)
            ├─ opens res   → ctx+0x2C  (RES\x19, picture pool)
            └─ opens lang  → ctx+0x58  (RES\x19, strings)

FUN_10186440(ctx, scene_id)                  // scene lookup (0x24 records)
FUN_101848d4(ctx, ids[], out, sizes, n)      // load N nodes by id
  └─ FUN_10187d90(node_list, count, id)
FUN_101869e8(ctx, scene, &index, &offset)    // walk a scene's items
FUN_101866f8(ctx, scene, node_id)            // find one item by id
FUN_101878e4(ctx, node, refs)                // image decoder
FUN_10186dd4(ctx, node)                      // text loader
```

`FUN_10185134` / `FUN_10184f78` build the LVGL view tree from a scene
(item -> widget, recursive for containers). `FUN_10187088` also has the
alternative **RES album** branch (`"RES"` magic + `WFRes_AlbumReadPic`),
used for pictures, not for `.sty` itself.

## 2. Container

```
0x00  u32 scene_count          (181 in 0.73)
0x04  u32 0
0x08  scene[scene_count]       36 B each
...   (0x08 + count*36)        node base  (0x197C in 0.73)
...                            node records / scene blobs
```

Scene record (36 B):

| off | meaning |
|---|---|
| `0x00` | u32 type = 0 |
| `0x04` | u32 `0x01F6019A` = **u16 410 x u16 502** (design canvas) |
| `0x08` | u32 0 |
| `0x0C` | u8 alpha = 255, then 3 zero bytes |
| `0x10` | u32 **top-level item count** |
| `0x14` | u32 **first item offset** (relative to the node base; 0 = empty) |
| `0x18` | u32 **scene id** |
| `0x1C` | u32 `0x00010000` |
| `0x20` | u32 0 |

The 410x502 constant appears in every scene; it is the source design
canvas, not the 466x466 round display.

## 3. Node records

```text
+0x00 u32 type        1 = list/grid, 2 = text, 3 = container, 4 = image
+0x04 u32 id
+0x08 u32 next        offset of the next record, RELATIVE to the node base
                      (0 = end of chain / file)
+0x0C ...             type-specific
```

A record's size is `next - offset`. `next` jumps over a container's whole
subtree, so the root chain is a depth-first serialization. Containers
(types 1 and 3) hold their children **inline from `+0x24`**; children use
the same `next` field, and the last child points at the parent's `next`.
Maximum depth seen is 2 (children are only text/image nodes).

0.73 stats: **5,009 records** = 3,020 top-level + 1,989 nested, 2,587
unique ids. Type counts: image 2,630; text 2,305; container 68; list 6.
Records are NOT all 12 bytes: text = 36 B (`0x24`), image = 40 B (`0x28`),
containers = `0x24 + children`.

### Type 4 - image (40 B)

```
+0x0C u32 picture index (1-based into bt_watch.res)
+0x10 u16 x | u16 y
+0x14 u16 w | u16 h
+0x18 u16 bytes_per_pixel (2 = RGB565, 3 = RGB888)
+0x1A u16 format/alpha (0 or 1, sometimes 7)
+0x1C u32 offset of the picture BODY inside bt_watch.res
+0x20 u32 body length (0 for type-0x05 pictures)
```

`+0x1C`/`+0x20` are a build-time mirror of the `.res` layout: PIC333 is
at `0x47EBC` (body), len 1646; PIC1260 at `0x230D10`, len 2847; PIC332
(type 5) at `0x45C20` with the length field left 0. The runtime decoder
seeks through the RES entry table, not these fields, so they can go stale
when `.res` is rebuilt. Keep them in sync for other firmware.

`FUN_101878e4` allocates `w*h*bpp`, caches by `(ctx, id)` with a refcount
and decodes from `ctx+0x2C` (the RES file); `FUN_10186f50` is the pool
allocator/splitter.

### Type 2 - text (36 B)

```
+0x0C u32 text index into bt_watch.<lang>
+0x10 u16 x | u16 y        (0,0 = auto / laid out by parent)
+0x14 u16 w | u16 h        (0,0 = auto)
+0x18 u32 style/format      (24 distinct values: 32..548; packed, not fully
                              identified - high bits 0x100/0x200 are common,
                              low bits vary by label; likely font/alignment)
+0x1C u32 colour 1         (0 or 0x00FFFFFF)
+0x20 u32 colour 2         (0 or 0x00FFFFFF)
```

`FUN_10186dd4` reads the language file (`ctx+0x58`):

1. 16-byte header; **byte `+9` must be 2** - this is the `0x0200` field
   of a `RES\x19` file, so language files are ordinary RES pools.
2. seek `text_index * 16`, read a 16-B record `{u32 offset, u16 len, ...}`
3. seek `offset`, read `len + 1` bytes (NUL-terminated string).

So text nodes hold **no strings** - only an index into `bt_watch.<lang>`.
Translating the UI therefore only needs the `bt_watch.<lang>` files.

### Type 3 - container/page (68, size `0x24 + children`)

```
+0x0C u32 0
+0x10 u16 canvas w | u16 canvas h     (410x502)
+0x14 u32 0
+0x18 u32 alpha (255)
+0x1C u32 child count
+0x20 u32 first child offset (== self + 0x24)
```

Used for pages and repeated groups; e.g. scene 125's list has 6 image
children, scene 90 has 12 containers of 10 children each. Verified: for
all 68, walking `count` children from `+0x24` ends exactly at the parent's
`next`.

### Type 1 - list/grid (6, size `0x24 + children`)

```
+0x0C u16 x | u16 y
+0x10 u16 w | u16 h
+0x14 u16 gap | u16 columns
+0x18 u32 child count
+0x1C u32 first child offset (== self + 0x24)
+0x20 u32 background colour (BGRA, e.g. 0xBC93BCB0)
```

All 6 verified the same way. Scene 125 is a 220x220, gap 16, 2-column
list of 6 animation frames (`PIC1534..PIC1539`, all sharing the list id);
scenes 126 has three such lists.

## 4. Scene -> items

Scene `+0x14` points at the first item, `+0x10` is the number of items;
walk `next` from item to item. There are **no opcodes**: the first word
of every item is its node type.

Example (scene 88, single 204x96 image):

```
scene 88 id=5BBE3E12 410x502 group=1 alpha=255 items=1 data=0x1D63C
  image id=F9C614C6 pic=1260 204x96 bpp=2 res=0x230D10+0xB1F
```

```
$ fwkit sty-tree out\bt_watch.sty 125
scene 125 id=66977E99 410x502 group=1 alpha=255 items=1 data=0x2329C
  list id=79C22D30 xy=0,0 wh=220x220 cell=16/2 kids=6
    image id=79C22D30 pic=1534 220x220 ... res=0x33A49C+0x1528
    image id=79C22D30 pic=1535 ...            # 6 frames
```

`sty-info` reports `records`, `top-level + nested` and unique ids;
`sty-nodes` lists all 5,009 (nested ones are indented).

## 5. Authoring notes

- **Ids**: any unused u32; duplicates are normal (the same id can be
  reused for variants and inside a list). Keep the ids `app.bin` loads
  (e.g. `0x5D695BF2` for the pairing-failure dialog, builtin dial ids at
  `0x101F3D54`).
- **In-place edits** (picture index, text index, x/y, colours, res
  offset/length) keep all sizes and offsets, so `fwkit`'s patch-in-place
  `build()` stays byte-identical outside the edited bytes.
- **Adding a scene** (`sty.add_scene`, CLI `sty-add-scene`) copies a
  scene's whole top-level chain and appends it after the existing node
  block.  Growing the table moves the node base by 36, but since every
  offset is **relative to that base**, the old block is copied verbatim;
  only the appended copy's `next`/container-child offsets are re-emitted
  (base-relative, with the root terminator kept at `0`).  The new scene
  record is a copy with `count`, `data_offset` and `id` replaced.
- The clone **reuses the source node ids** by default.  That is legal
  (the same id already appears in many scenes), but a loader cache may
  hand a widget the earlier node.  Pass `fresh_ids=True` (CLI
  `--fresh-node-ids`) to allocate a new unused u32 for every cloned node,
  including nested children.
- A new scene is only displayed if the app knows its id.  Either patch a
  literal in `app.bin` (`app-scene-refs` finds it, `app-patch-scene`
  retargets it, or `edit-app` patches a known address) or reuse an id the
  app already requests.  `add_scene` refuses to create a duplicate scene
  id unless `--force` is given.
- **Deleting a scene** (`sty.remove_scene`, CLI `sty-del-scene`) drops the
  36-byte table entry.  The default rebuilds every surviving scene and
  drops unreachable records (`sty.compact`); `--keep-orphans` only removes
  the entry and shifts the node block, leaving dead records behind.  The
  compactor copies shared records once per scene instead of deduplicating,
  so output is valid but not always minimal.
- Type-1/3 children live *inside* the parent's byte range; a naive
  "walk the root chain" parser silently loses 1,989 nodes (fixed in
  `fwkit.sty` 0.73, which now parses them).

## 6. Pitfalls

- **Do not trust `0x0C` as the table start.** `0x0C` is inside scene 0;
  the table starts at `0x08`. Reading from `0x0C` yields 410x502 as "w/h"
  and wrong ids.
- Node `next` and scene `data_offset` are **relative to the node base**,
  while picture `pic_index` is 1-based and the image `+0x1C` offset is
  absolute in the `.res` file. Mixing these up silently produces garbage.
- **The same node id can be reused for different variants** (e.g.
  `0xDEFBE0D9` appears 3x showing PIC333, PIC487 and PIC564). Retarget
  with `--from-pic` (or `all_matches=True`), never blindly by id.
- Image nodes store geometry that must agree with the picture payload's
  own `u16 w | u16 h` prefix (see `pictures.md`). A resized sprite must
  be fixed in both the `.sty` node and the RES entry.
- Text nodes index `bt_watch.<lang>`; an index past the table makes the
  loader log `read style file error` and return `-1`, and the widget
  silently falls back to no text.
- `.sty` has no checksum/signature of its own - it lives in `res.bin`
  (SDFS hash) inside the AOTA image, so the AOTA/SDFS rules in
  `aota.md` apply.
- `logo.sty` (in `sdfs_k.bin`) uses the same container; patching boot
  artwork requires rebuilding `sdfs_k.bin` + the outer AOTA.

## 7. Tooling (`fwkit`)

```powershell
$fw = "fwkit"
& $fw sty-info   out\bt_watch.sty --scenes
& $fw sty-nodes  out\bt_watch.sty --type 4
& $fw sty-tree   out\bt_watch.sty 125
& $fw sty-add-scene out\bt_watch.sty 125 0x76543210 -o out.sty --fresh-node-ids
& $fw sty-del-scene out.sty 0x76543210 -o trimmed.sty
& $fw sty-set-pic out\bt_watch.sty 0xDEFBE0D9 332 -o out.sty --from-pic 333
& $fw app-scene-refs out\app_073.bin 0x5D695BF2
& $fw app-patch-scene out\app_073.bin 0x5D695BF2 0x5D695BF3 -o out\app_new.bin
& $fw edit-sty-pic IMG res.bin bt_watch.sty 0xDEFBE0D9 332 out.bin --from-pic 333
```

Python API (`fwkit.sty`):

```python
import fwkit.sty as sty
s = sty.parse(open("bt_watch.sty", "rb").read())   # .build() is byte-identical
s.node(0xDEFBE0D9)          # -> Node(type, id, xy, wh, children, ...)
s.scene(125)                # -> Scene(width, height, alpha, group, id, count)
s.scene_nodes(125)          # -> top-level Node list
n.children                  # nested list/container children (depth 2)
out = sty.set_pic_index(blob, 0xDEFBE0D9, 332, from_pic=333)
out = sty.add_scene(blob, 125, 0x76543210)          # append a cloned scene
out = sty.add_scene(blob, 125, 0x76543210, fresh_ids=True)
out = sty.add_scene(blob, 125, 0x76543210, index=0)  # or insert in the table
out = sty.remove_scene(blob, 0x76543210)             # delete + compact
out = sty.remove_scene(blob, 125, compact_nodes=False)  # keep orphan records
out = sty.compact(blob)                              # GC without deleting
```

Node records are patched **in place** (only changed bytes are written), so
`.sty` edits never shift scene offsets or the node chain. Verified: a
one-node retarget changes exactly the 1 byte of `pic_index` (333 -> 332)
in a 10.8 s full-image rebuild, `fwkit verify` ok. Round-trip is
byte-identical for `bt_watch.sty` and `logo.sty`; `selftest.py` checks it.
