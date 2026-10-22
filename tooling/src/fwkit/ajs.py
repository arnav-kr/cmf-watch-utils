"""Reader/writer for ``.ajs`` JPEG animation files (``/NAND:E/<n>.ajs``).

The file is a tiny prelude followed by a chain of frame records.  The prelude
is 8 bytes and byte 7 is the frame rate (0 means the player's 25 fps default)::

    0x00  u32   0
    0x04  bytes 00 00 00 <fps>          (0.73 files: 0x19 = 25 fps)
    0x08  frame record 0
          ...

    record (relative to its u16 size field):
    +0x00 u16   size       bytes after this field
    +0x02 "JPEG"
    +0x06 u16   width
    +0x08 u16   height
    +0x0A u16   tile width
    +0x0C u16   tile height
    +0x0E u16   tile count N
    +0x10 N * (u16 tile offset, u16 tile size)
    +...        tile JPEG streams

Tile offsets are relative to the ``"JPEG"`` magic.  For a single 240x240 tile
the table is one entry ``(18, jpeg_len)`` and the JPEG starts at record +0x14.
The total record length is ``2 + size`` and ``size`` is
``0x0E + 4*N + sum(tile lengths)``.  The player walks frames by reading the
next record's ``size`` immediately after the current record.

The 0.73 widget decodes every tile with the hardware JPEG decoder and blits it
at ``(col * tile_width, row * tile_height)``, clipped to the canvas.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field
from pathlib import Path

MAGIC = b"JPEG"
SOI = b"\xff\xd8"
EOI = b"\xff\xd9"
PRELUDE = 8
SIZE_OFF = 0x08
MAGIC_OFF = 0x0A
DESC_LEN = 0x0E
JPEG_OFF = 0x14


def _ceil_div(a: int, b: int) -> int:
    return (a + b - 1) // b


@dataclass
class Tile:
    offset: int
    length: int

    @property
    def end(self) -> int:
        return self.offset + self.length


@dataclass
class Frame:
    index: int
    record_offset: int
    width: int
    height: int
    tile_width: int
    tile_height: int
    tiles: list[Tile] = field(default_factory=list)
    raw: bytes = b""
    size: int = 0

    @property
    def columns(self) -> int:
        return _ceil_div(self.width, self.tile_width)

    @property
    def rows(self) -> int:
        return _ceil_div(self.height, self.tile_height)

    @property
    def tile_count(self) -> int:
        return len(self.tiles)

    @property
    def offset(self) -> int:
        return self.tiles[0].offset

    @property
    def length(self) -> int:
        return self.tiles[0].length

    @property
    def tile_offset(self) -> int:
        return self.tiles[0].offset

    @property
    def tile_size(self) -> int:
        return self.tiles[0].length

    def tile_bytes(self, blob: bytes, i: int = 0) -> bytes:
        t = self.tiles[i]
        return blob[t.offset:t.end]

    def jpeg(self, blob: bytes) -> bytes:
        return self.tile_bytes(blob, 0)


@dataclass
class Ajs:
    raw: bytes
    frames: list[Frame]
    fps: int

    @property
    def version(self) -> int:
        return self.fps

    @property
    def width(self) -> int:
        return self.frames[0].width

    @property
    def height(self) -> int:
        return self.frames[0].height

    def build(self) -> bytes:
        return self.raw


def is_ajs(blob: bytes) -> bool:
    return (len(blob) >= PRELUDE + 20
            and blob[MAGIC_OFF:MAGIC_OFF + 4] == MAGIC)


def jpeg_size(data: bytes) -> tuple[int, int]:
    """Return ``(width, height)`` from a JPEG SOF marker."""
    if data[:2] != SOI:
        raise ValueError("not a JPEG stream")
    i = 2
    n = len(data)
    while i + 4 <= n:
        if data[i] != 0xFF:
            i += 1
            continue
        while i < n and data[i] == 0xFF:
            i += 1
        if i >= n:
            break
        marker = data[i]
        i += 1
        if marker in (0xD8, 0xD9) or 0xD0 <= marker <= 0xD7:
            continue
        if i + 2 > n:
            break
        seg = (data[i] << 8) | data[i + 1]
        if seg < 2 or i + seg > n:
            break
        if (0xC0 <= marker <= 0xC3 or 0xC5 <= marker <= 0xC7
                or 0xC9 <= marker <= 0xCB or 0xCD <= marker <= 0xCF):
            if seg < 7:
                break
            h = (data[i + 3] << 8) | data[i + 4]
            w = (data[i + 5] << 8) | data[i + 6]
            return w, h
        i += seg
    raise ValueError("no JPEG SOF marker")


def parse(blob: bytes) -> Ajs:
    if not is_ajs(blob):
        raise ValueError("not a .ajs animation")
    fps = blob[7] or 25
    frames: list[Frame] = []
    pos = SIZE_OFF
    total = len(blob)
    while True:
        if pos + 0x10 > total:
            raise ValueError("truncated record at %#x" % pos)
        size = struct.unpack_from("<H", blob, pos)[0]
        if blob[pos + 2:pos + 6] != MAGIC:
            raise ValueError("bad JPEG magic at %#x" % (pos + 2))
        w, h, tw, th, count = struct.unpack_from("<5H", blob, pos + 6)
        if not count:
            raise ValueError("zero tiles at %#x" % pos)
        desc_end = pos + 0x10 + count * 4
        end = pos + 2 + size
        if desc_end > end or end > total:
            raise ValueError("bad descriptor at %#x" % pos)
        tiles: list[Tile] = []
        for i in range(count):
            off, ln = struct.unpack_from("<HH", blob, pos + 0x10 + i * 4)
            start = pos + 2 + off
            stop = start + ln
            if start < desc_end or stop > end:
                raise ValueError("tile %d out of range at %#x" % (i, pos))
            if blob[start:start + 2] != SOI:
                raise ValueError("missing SOI at %#x" % start)
            if blob[stop - 2:stop] != EOI:
                raise ValueError("missing EOI at %#x" % stop)
            tiles.append(Tile(start, ln))
        if size != DESC_LEN + 4 * count + sum(t.length for t in tiles):
            raise ValueError("size mismatch at %#x" % pos)
        frames.append(Frame(
            len(frames), pos, w, h, tw, th, tiles,
            blob[pos + 2:desc_end], size))
        nxt = end
        if nxt == total:
            break
        if nxt > total - 0x10:
            raise ValueError("record at %#x runs past EOF" % pos)
        pos = nxt
    return Ajs(blob, frames, fps)


def _tile_list(tiles, count: int) -> list[bytes]:
    if isinstance(tiles, (bytes, bytearray)):
        tiles = [bytes(tiles)]
    else:
        tiles = [bytes(t) for t in tiles]
    if len(tiles) != count:
        raise ValueError("expected %d tiles, got %d" % (count, len(tiles)))
    for i, t in enumerate(tiles):
        if t[:2] != SOI or t[-2:] != EOI:
            raise ValueError("tile %d is not a JPEG stream" % i)
    return tiles


def make_record(width: int, height: int, tile_width: int, tile_height: int,
                tiles) -> bytes:
    """Build one frame record (starting with its u16 size field)."""
    if not (0 < width <= 0xFFFF and 0 < height <= 0xFFFF):
        raise ValueError("bad canvas %dx%d" % (width, height))
    if not (0 < tile_width <= 0xFFFF and 0 < tile_height <= 0xFFFF):
        raise ValueError("bad tile size %dx%d" % (tile_width, tile_height))
    columns = _ceil_div(width, tile_width)
    rows = _ceil_div(height, tile_height)
    count = columns * rows
    streams = _tile_list(tiles, count)
    table_size = 4 * count
    offset = DESC_LEN + table_size
    entries = bytearray()
    data = bytearray()
    for jpeg in streams:
        if offset > 0xFFFF or len(jpeg) > 0xFFFF:
            raise ValueError("tile offset/size exceeds u16")
        entries += struct.pack("<HH", offset, len(jpeg))
        data += jpeg
        offset += len(jpeg)
    size = DESC_LEN + table_size + len(data)
    if size > 0xFFFF:
        raise ValueError("frame record exceeds u16 size (%d)" % size)
    head = bytearray(struct.pack("<H", size))
    head += MAGIC
    head += struct.pack("<5H", width, height, tile_width, tile_height, count)
    return bytes(head) + bytes(entries) + bytes(data)


def build(frames, fps: int = 25, width: int | None = None,
          height: int | None = None, tile_width: int | None = None,
          tile_height: int | None = None) -> bytes:
    """Pack JPEG frame(s) into a .ajs file.

    Each item in ``frames`` may be a JPEG ``bytes`` (single tile) or a list of
    tile JPEGs for a multi-tile frame.  Dimensions are inferred from the first
    single-tile JPEG when omitted.
    """
    frames = list(frames)
    if not frames:
        raise ValueError("no frames")
    first = frames[0]
    if width is None or height is None:
        if isinstance(first, (bytes, bytearray)):
            jw, jh = jpeg_size(bytes(first))
            width = width or jw
            height = height or jh
        else:
            raise ValueError("width/height required for multi-tile frames")
    tile_width = tile_width or width
    tile_height = tile_height or height
    if not 0 <= fps <= 0xFF:
        raise ValueError("fps must fit in one byte")
    prelude = bytearray(PRELUDE)
    prelude[7] = fps
    out = bytearray(prelude)
    for i, item in enumerate(frames):
        try:
            out += make_record(width, height, tile_width, tile_height, item)
        except ValueError as exc:
            raise ValueError("frame %d: %s" % (i, exc)) from exc
    return bytes(out)


def set_frame(blob: bytes, index: int, tiles) -> bytes:
    """Replace frame ``index`` without touching any other record."""
    a = parse(blob)
    if not 0 <= index < len(a.frames):
        raise IndexError("frame %d out of range" % index)
    f = a.frames[index]
    rec = make_record(f.width, f.height, f.tile_width, f.tile_height, tiles)
    pos = f.record_offset
    old_len = 2 + f.size
    out = blob[:pos] + rec + blob[pos + old_len:]
    parse(out)
    return out


def extract(blob: bytes, outdir) -> list[Path]:
    """Write every tile as ``NNNN.jpg`` (or ``NNNN_TT.jpg``)."""
    a = parse(blob)
    out = Path(outdir)
    out.mkdir(parents=True, exist_ok=True)
    paths: list[Path] = []
    for f in a.frames:
        for i in range(f.tile_count):
            name = ("%04d.jpg" % f.index if f.tile_count == 1
                    else "%04d_%02d.jpg" % (f.index, i))
            p = out / name
            p.write_bytes(f.tile_bytes(blob, i))
            paths.append(p)
    return paths
