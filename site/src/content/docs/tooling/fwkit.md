---
title: fwkit CLI
description: Command reference for the offline firmware container toolkit.
---

`fwkit` is the offline half of the project: it parses the firmware containers, rebuilds them
byte-for-byte, and applies targeted edits. It never talks to a watch.

```bash
uv run fwkit --help
```

## AOTA container

| Command | Purpose |
|---|---|
| `fwkit info <image>` | Header, entry table, per-entry sizes and CRC status |
| `fwkit verify <image>` | Header CRC and every entry CRC |
| `fwkit unpack <image> <dir>` | Header, `ota.xml`, AGPS trailer and each partition |
| `fwkit pack <dir> <image>` | Rebuild an image from an unpacked directory |

## SDFS filesystem

| Command | Purpose |
|---|---|
| `fwkit sdfs-list <image>` | File table of an SDFS partition |
| `fwkit sdfs-extract <image> <dir>` | Extract all files |
| `fwkit sdfs-pack <dir> <image>` | Rebuild from a directory |
| `fwkit edit-sdfs <image> <partition> <name> <input> <out>` | Replace one file inside a partition of an AOTA image |

`fwkit.sdfs.pad_to(image, size)` rebuilds a filesystem and pads it back to a fixed partition
length while refreshing `total_size` and the body checksum. Partitions embedded in an AOTA
image keep their original length that way.

## RES strings and pictures

| Command | Purpose |
|---|---|
| `fwkit res-list <resfile>` | Strings in a RES table |
| `fwkit res-set <resfile> <key> <value>` | Replace one string |
| `fwkit pic-list <resfile>` | Pictures and their codecs |
| `fwkit pic-decode <resfile> <index> <png>` | Decode a picture to PNG |
| `fwkit pic-set <resfile> <index> <png>` | Replace a picture from PNG |
| `fwkit edit-res <image> <partition> <resfile> <key> <value> <out>` | Replace a string inside an AOTA image |
| `fwkit edit-pic <image> <partition> <resfile> <index> <png> <out>` | Replace a picture inside an AOTA image |

## STY scene bundles

| Command | Purpose |
|---|---|
| `fwkit sty-info <file>` | Header and scene table |
| `fwkit sty-nodes <file>` | Nodes of the bundle |
| `fwkit sty-tree <file> <scene>` | Node tree of one scene |
| `fwkit sty-add-scene <file> ...` | Add a scene with fresh node ids |
| `fwkit sty-del-scene <file> ...` | Remove a scene and compact records |
| `fwkit sty-set-pic <file> ...` | Repoint an image node at another picture |
| `fwkit edit-sty-pic <image> ...` | Repoint a node inside an AOTA image |

## AJS animations

| Command | Purpose |
|---|---|
| `fwkit ajs-info <file>` | Record layout, frame count, fps and tiles |
| `fwkit ajs-extract <file> <dir>` | Extract frames as JPEG |
| `fwkit ajs-repack <dir> <file>` | Rebuild from JPEG frames |
| `fwkit ajs-set-frame <file> <index> <jpeg> <out>` | Replace a single frame |

## WFDL watch faces

| Command | Purpose |
|---|---|
| `fwkit wf-info <file>` | Package header and structure |
| `fwkit wf-res <file>` | Resource list with indices |
| `fwkit wf-tree <file>` | TLV node tree |
| `fwkit wf-unpack <file> <dir>` | Header, tree, resources and raw LZ4 blocks |
| `fwkit wf-set-res <file> <index> <png> -o <out>` | Replace one resource from PNG |
| `fwkit wf-build <base> <out> --resources <dir>` | Rebuild, replacing `NNN_*.png` |

## app.bin

| Command | Purpose |
|---|---|
| `fwkit act-info <app>` | ACTH header fields |
| `fwkit app-scene-refs <app>` | Scene-id references in the app image |
| `fwkit app-patch-scene <app> ...` | Rewrite a scene-id literal and rebuild the header |
| `fwkit edit-app <image> <input> <out>` | Replace `app.bin` inside `TEMP` and rebuild the image |

## Conventions

- The toolkit preserves unknown fields, so a rebuild of an unmodified image is byte-identical.
- Formats are documented per device in the device section; the layouts differ between
  firmware generations.
- `tooling/tests/selftest.py` runs the round-trip checks:

```bash
uv run python tooling/tests/selftest.py firmware/JX402_V1.0.0.73.bin
```

The selftest requires a firmware image supplied locally; the published dumps under
`dumps/binaries/` work as well.
