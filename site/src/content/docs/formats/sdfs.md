---
title: SDFS filesystem
description: The simple named-file filesystem used inside res_e and other AOTA partitions.
---

SDFS is the filesystem used by the resource partitions. It is deliberately small: a header, a
fixed-size entry table, then aligned file bodies.

## Layout

```text
0x00  magic "sdfs.bin"
0x08  u32 zero
0x0C  u32 file count
0x10  u32 total image size
0x14  u32 zero
0x18  u32 checksum of the entry table
0x1C  u32 checksum of the body
0x20  entries, 32 bytes each:
        name[12] | u32 offset | u32 size | u32 0 | u32 0 | u32 sum32(data)
```

File offsets are absolute inside the image and aligned to `0x40`. The checksum is a 32-bit
word sum (`sum32`), not CRC32: the data is read as little-endian `u32` values, summed modulo
2^32; trailing bytes are ignored.

## Tools

```bash
uv run fwkit sdfs-list    work/res_e.bin
uv run fwkit sdfs-extract work/res_e.bin work/res_e
uv run fwkit sdfs-pack    work/res_e work/res_e_new.bin
```

Replace one file inside an AOTA partition without unpacking everything:

```bash
uv run fwkit edit-sdfs firmware/JX402_V1.0.0.73.bin res_e.bin 9.ajs work/9.ajs out/custom.bin
```

## Fixed partition length

`res_e.bin` is a fixed **58,777,088-byte** partition. Replacing a file can change the natural
SDFS size, so `fwkit.sdfs.pad_to(img, size)` rebuilds the filesystem and pads it back to the
original partition length, refreshing `total_size` and the body checksum.

Use this whenever the partition is embedded in an AOTA image. Do not shrink the partition
unless you also understand how the bootloader maps it.

## Capacity

The stock `res_e` uses 58,776,014 of 58,777,088 bytes. There is almost no headroom. Plan
replacements around the size of the file you remove, not the free bytes reported by a naive
directory listing.
