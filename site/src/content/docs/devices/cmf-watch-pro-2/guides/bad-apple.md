---
title: Bad Apple case study
description: How a 202-frame animation was built, packed into res_e, flashed and verified on hardware.
---

This is the end-to-end proof that the format and OTA work is correct. It is documented as a
case study; **no Bad Apple frames or prebuilt firmware are distributed here**.

## Why `9.ajs`

`9.ajs` is the 8-second sport warm-up animation for sport type 22. It is small enough to
replace without rebuilding the whole resource set:

| | Stock `9.ajs` | PoC |
|---|---|---|
| Frames | 1,124 | 202 |
| FPS | 25 | 25 |
| Duration | ~45 s | ~8.1 s |
| Size | 4,795,064 B | 756,479 B |
| Video | 240×240 | 240×240 |

The PoC is silent, 240×240, baseline JPEG, and fits in a single-tile-per-frame layout.

## Build outline

1. Extract `res_e.bin` from the stock AOTA image.
2. Extract `9.ajs`, confirm the record layout and fps.
3. Encode 202 frames at 240×240/25 fps as baseline JPEG.
4. Repack with `fwkit ajs-repack` so the prelude and tile table are correct.
5. Verify: extract the new `.ajs` again and compare frame bytes.
6. Replace `9.ajs` in `res_e.bin` with `fwkit edit-sdfs` (or rebuild SDFS and
   `pad_to` the original 58,777,088-byte partition).
7. Rebuild the AOTA image with `fwkit pack`; `fwkit verify` must pass.
8. Flash over BLE using the [OTA guide](/devices/cmf-watch-pro-2/guides/ota-flash/).

## Flash result

- The image was ~54 MB; the watch requested ~38.08 MB (70.4%) because unchanged partitions
  were skipped.
- The watch reported `0xA041 01`; the client sent `0x9041 A5`.
- It rebooted, booted the new image and **played the animation in the UI**.
- A second short transfer (1.63 MB, 3.0%) completed after the first had already staged most
  data; it also ended with the commit ack.

## Capacity ceiling

Full Bad Apple at 5,250 frames is ~19.7 MB (~18.75 MiB). After replacing `9.ajs` with the PoC,
`res_e` has only ~3.85 MiB free. Options:

- deleting other warm-up `.ajs` files to reclaim space;
- lowering resolution/fps or using stronger JPEG compression;
- splitting the clip and swapping files (not supported by the stock UI);
- accepting a shorter excerpt.

`res_e` total is 58,777,088 B; stock usage is 58,776,014 B, so headroom is essentially zero
unless content is removed.

## Legal note

Bad Apple is copyrighted. Frames, a built `.ajs` and a firmware image containing them are not
published. The method is described, the tooling is shipped, and the media is supplied by the
user.
