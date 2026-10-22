---
title: Hardware notes
description: SoC, RAM, flash, display, UART shell, factory mode and recovery notes.
---

# Hardware, app model and factory mode

Findings for `JX402_V1.0.0.73` (CMF Watch Pro 2).  Addresses are for the
0.73 `app.bin` (`0x10000000` base).

## 1. SoC / memory

| item | finding | confidence |
|---|---|---|
| SoC | Actions ATS3089C, Zephyr RTOS, Thumb-2 | high |
| core | Cortex-M class; exact core and clock not extracted | medium |
| SRAM | `0x2FF00000..0x30000000` (~1 MB); reset SP `0x2FFB6740` | high |
| PSRAM | window at `0x38000000`; refs up to `0x38FFF10A` (~16 MB address space), globals use low offsets | medium |
| flash | raw OTA partitions total ~81.4 MB, so >=64 MB; likely 128 MB SPI NAND, exact chip not proven | medium |
| storage | SDFS on NAND; `/NAND:A` UI, `/NAND:C` user dials, `/NAND:E` animations, `/NAND:K` boot assets | high |

The app image is `0x25A080` (2,465,920 B).  RAM globals at `0x3800_xxxx`
and `0x3830_xxxx` are **not** in `app.bin`; the image is XIP/loaded at
`0x10000000`.

## 2. Display / graphics

| item | finding |
|---|---|
| panel | 410x502 AMOLED, CO5300 controller |
| refresh | 55 Hz |
| UI toolkit | LVGL 8.x (`lvgl/src/extra`, `lv_api_map.h`) |
| 2D GPU | VeriSilicon **VGLite** |
| media | hardware JPEG decoder (Allwinner `ezJPEG`-style) |
| other | display engine (DE), GPU clock control |

The UI is a compiled LVGL app; `.sty` only supplies scene geometry, text
indices, picture indices and colours.

## 3. App model

- Monolithic C/Zephyr `app.bin`; no dynamic loader, no Lua/JS/ELF/plugin
  engine in the image.
- Apps are **views** in a registry of 24-byte entries starting near file
  offset `0x4b8`:
  `[name_ptr, 0, create_fn, 0, id_word, 0x01d201d2]`.
  `id_word` low 16 bits is the view id (`0x4e` about, `0x87` factory).
- No user-installable apps.  A new screen needs a `.sty` scene plus an
  app-side scene-id reference.  New behaviour needs a firmware patch or
  a rebuild with the vendor SDK, not an installable package.
- Zephyr shell is present over `UART_0`: `devmem`, `kheap`, `dumpbuf`,
  `nand logic read/write`, `reboot`, `cpuload`.  Physical UART pads are
  required; none are connected on this host.

## 4. Hidden factory mode

Trigger: **Settings -> About -> five rapid taps on the version row**.
`FUN_100bf9bc` increments a counter when `FUN_101cb8a2(...) < 0x14d`
(333 ms); after the fifth tap (`counter > 4`) it calls
`FUN_101898bc(0x87, 0)`.

Menu (`factory_test_view`, id `0x87`):

- system comprehensive / semi-finished / finished-product tests
- LCD, drawline, touch, sensor level/tilt, light-leak 1/2
- BIO, charge/NTC, battery voltage, MIC, key encoder, motor, speaker
- light perception, GPS test
- DUT test, AP mode, test record, device QR code
- shipping mode, clear-user-data-and-power-off, exit factory mode

Labels are inline UTF-8 in `app.bin` around `0x21a9fb..0x21ad97`; they are
not in `bt_watch.<lang>`.  Menu builder `FUN_10094a48`, click handler
`FUN_10094800`, key handler `FUN_1009408c`.

There are **no Wi-Fi/lwIP/TCP strings** in the app, so factory "AP mode"
is not a Wi-Fi access point; it is likely a BLE/RF or PCBA test path.
Shipping mode and clear-data can wipe settings; both are destructive and
are only invoked deliberately.

## 5. JPEG animation playback

`.ajs` files are `/NAND:E/N.ajs`, 240x240 JPEG frames.  The prelude byte
at `+0x07` is the frame rate (0 => 25).  `simple_video_9` creates
`lv_timer_2(cb, 1000/fps, ...)`, so shipped files play at 25 fps.  All 19
are sport warm-up animations driven by the `0x101ED708` table; they are
not menu entries (see `watchface.md` for the index-to-sport map).

`res_e.bin` is 58,777,088 B and the 19 files occupy 58,776,014 B (1,074 B
free).  A 3:30 clip at 25 fps is 5,250 frames; at ~4 KB/frame that is
~21 MB, so a Bad Apple build either replaces existing animations or
repacks `res_e` without them.  A 202-frame PoC (`out/badapple_poc.ajs`,
756,479 B, byte-identical extract/repack) projects to **18.75 MiB** for the
full clip.  The offline candidate `out/JX402_V1.0.0.73_badapple9.bin`
replaces `9.ajs` (sport type 22, 8 s warm-up) and has not been flashed.
See `watchface.md` section 5 and `fwkit.ajs`.
