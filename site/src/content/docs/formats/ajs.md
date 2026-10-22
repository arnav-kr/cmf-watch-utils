---
title: AJS animations
description: The JPEG-frame animation format used for sport warm-ups, including tile tables and capacity math.
---

`.ajs` files live in `/NAND:E` inside `res_e.bin` and are played by the sport warm-up view. They
are not menu entries or watch faces.

## Layout

```text
0x00  magic/header
0x07  u8 fps (0 => 25)
0x08  records, one per frame:
        u16 size? / u16 tile_count?
        frame header / tile table
        JPEG tile stream(s)
```

The shipped files are 240×240, 25 fps, usually one tile per frame. Multi-tile frames use a
tile table with `(offset, length)` pairs; offsets are relative to the descriptor magic.

Each record ends immediately before the next record; the last record ends at EOF. JPEG streams
are ordinary baseline JPEG (`FF D8 ... FF D9`).

## Tools

```bash
uv run fwkit ajs-info    work/res_e/9.ajs --frames
uv run fwkit ajs-extract work/res_e/9.ajs work/ajs9
uv run fwkit ajs-repack  work/ajs9 -o work/custom.ajs --fps 25
uv run fwkit ajs-set-frame work/custom.ajs 0 new_frame.jpg -o work/custom2.ajs
```

`ajs-extract` writes `NNNN.jpg` (or `NNNN_TT.jpg` for multi-tile frames). A matching repack is
byte-identical for the shipped files.

## Warm-up mapping

The app table at `0x101ED708` has 19 16-byte records: `[sport_type, seconds, category, ajs_index]`.
All 19 `.ajs` files are warm-up animations. `9.ajs` is sport type **22**, an **8-second** warm-up.
See [Watch faces & animations](/cmf-watch-utils/formats/watchface/) for the full table.

## Capacity

The 202-frame PoC is 756,479 bytes (~3.7 KB/frame). Scaling to 5,250 frames gives ~19.7 MB
(~18.75 MiB). `res_e` has only ~3.85 MiB free after replacing `9.ajs`, so a full clip needs
more deletions or a smaller encode.

## Pitfalls

- Keep 240×240 and a sane fps; the player assumes the frame rate from `+0x07` (0 means 25).
- Do not add trailing bytes after the last record; the last frame ends at EOF.
- Repack with the toolkit rather than concatenating JPEGs; the record prelude and tile table
  must match what the player expects.
- The Bad Apple frames are copyrighted; no animation assets are included in this repository.
