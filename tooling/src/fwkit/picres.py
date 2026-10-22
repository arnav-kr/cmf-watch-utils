"""Picture resources inside a ``RES\\x19`` file (``bt_watch.res``, ``logo.res``).

Verified against all 2,192 entries of ``bt_watch.res`` (see docs/pictures.md).

Layout::

  0x00  "RES\\x19"
  0x04  u32 flags; low 16 bits == picture count
  0x08  u32 0x200
  0x0C  u32 subtype (0x00020000 for pictures)
  0x10  u32 pool_offset == 0x10 + count*16
  0x14  entries, 16 bytes:
          u16 len | u8 type | name[9] | u32 next_payload_start
        The last entry's trailing u32 is not stored (it would overlap
        the pool); its slot is the first 4 bytes of the payload.
  pool  payload 0 at pool_offset, payload i at the previous entry's
        trailing u32.  Payloads are padded to a 4-byte boundary; the
        padding is NOT included in ``len``.

Payload::

  +0x00 u16 w | +0x02 u16 h | body

  type 0x26 (38)  body = "JPEG" + 18-byte header + JFIF stream (w*h*3 out)
  type 0x05 (5)   body = raw LZ4 block, w*h*3 (RGB888)
  type 0x0E (14)  body = raw LZ4 block, w*h*2 (RGB565 LE)
"""

from __future__ import annotations

import io
import struct
from dataclasses import dataclass, field

from . import lz4

MAGIC = b"RES\x19"
HDR_SIZE = 0x14
ENTRY_SIZE = 16

TYPE_JPEG = 0x26
TYPE_LZ4_888 = 0x05
TYPE_LZ4_565 = 0x0E

BPP = {TYPE_LZ4_888: 3, TYPE_LZ4_565: 2}


def align4(n: int) -> int:
    return (n + 3) & ~3


@dataclass
class Pic:
    name: str
    type: int
    w: int
    h: int
    body: bytes
    pad: bytes = b""

    @property
    def bpp(self) -> int:
        return BPP.get(self.type, 3)

    @property
    def size(self) -> int:
        return 4 + len(self.body)


@dataclass
class PicRes:
    header: bytes
    pics: list[Pic] = field(default_factory=list)

    def __len__(self) -> int:
        return len(self.pics)

    def get(self, key) -> Pic:
        # int keys are 1-based picture indices (same numbering as PICn)
        if isinstance(key, int):
            if key < 1:
                raise KeyError(key)
            return self.pics[key - 1]
        for i, p in enumerate(self.pics, 1):
            if p.name == key:
                return p
        raise KeyError(key)

    def index(self, name: str) -> int:
        for i, p in enumerate(self.pics, 1):
            if p.name == name:
                return i
        raise KeyError(name)


def is_picres(blob: bytes) -> bool:
    return len(blob) >= HDR_SIZE and blob[:4] == MAGIC and \
        struct.unpack_from("<I", blob, 0x0C)[0] == 0x00020000


def parse(blob: bytes) -> PicRes:
    if not is_picres(blob):
        raise ValueError("not a picture RES file")
    count = struct.unpack_from("<I", blob, 4)[0] & 0xFFFF
    pool = struct.unpack_from("<I", blob, 0x10)[0]
    if pool != 0x10 + count * ENTRY_SIZE:
        raise ValueError("unexpected pool offset 0x%x" % pool)
    start = pool
    pics: list[Pic] = []
    for i in range(count):
        o = HDR_SIZE + i * ENTRY_SIZE
        ln, ty = struct.unpack_from("<HB", blob, o)
        name = blob[o + 3:o + 12].split(b"\0")[0].decode("ascii", "replace")
        if i < count - 1:
            nxt = struct.unpack_from("<I", blob, o + 12)[0]
        else:
            nxt = None
        end = nxt if nxt is not None else len(blob)
        pad = blob[start + ln:end]
        if align4(start + ln) != end:
            raise ValueError("entry %d not 4-byte aligned" % (i + 1))
        if start + ln > len(blob):
            raise ValueError("entry %d past EOF" % (i + 1))
        w, h = struct.unpack_from("<HH", blob, start)
        body = blob[start + 4:start + ln]
        pics.append(Pic(name, ty, w, h, body, pad))
        start = nxt if nxt is not None else len(blob)
    return PicRes(blob[:HDR_SIZE], pics)


def build(res: PicRes) -> bytes:
    n = len(res.pics)
    hdr = bytearray(res.header)
    struct.pack_into("<I", hdr, 4, (struct.unpack_from("<I", hdr, 4)[0] & 0xFFFF0000) | (n & 0xFFFF))
    struct.pack_into("<I", hdr, 0x0C, 0x00020000)
    pool = 0x10 + n * ENTRY_SIZE
    struct.pack_into("<I", hdr, 0x10, pool)

    table = bytearray()
    body = bytearray()
    cur = pool
    for i, p in enumerate(res.pics):
        ln = 4 + len(p.body)
        table += struct.pack("<HB", ln, p.type)
        table += p.name.encode("ascii")[:9].ljust(9, b"\0")
        if i < n - 1:
            table += struct.pack("<I", align4(cur + ln))
        body += struct.pack("<HH", p.w, p.h) + p.body
        pad = align4(cur + ln) - (cur + ln)
        body += b"\0" * pad
        cur += ln + pad
    out = bytearray(hdr) + table
    if len(out) != pool:
        raise ValueError("table size %d != pool %d" % (len(out), pool))
    out += body
    return bytes(out)


def _jpeg_tiles(body: bytes) -> tuple[list[bytes], int, int, int, int]:
    if body[:4] != b"JPEG":
        if body[:2] == b"\xff\xd8":
            return [body], 0, 0, 0, 0
        raise ValueError("bad JPEG body magic")
    w, h, tw, th, count = struct.unpack_from("<5H", body, 4)
    if count < 1 or 14 + count * 4 > len(body):
        raise ValueError("bad JPEG tile table")
    tiles = []
    for i in range(count):
        off, ln = struct.unpack_from("<HH", body, 14 + i * 4)
        start = off
        if start + ln > len(body) or body[start:start + 2] != b"\xff\xd8":
            raise ValueError("bad JPEG tile %d" % i)
        tiles.append(body[start:start + ln])
    return tiles, w, h, tw, th


def _pil():
    try:
        from PIL import Image
    except ImportError as exc:
        raise RuntimeError("JPEG decode/encode needs Pillow (uv add pillow)") from exc
    return Image


def decode(pic: Pic) -> bytes:
    """Return top-down RGB888 (``w*h*3``)."""
    if pic.type == TYPE_LZ4_888:
        return lz4.decompress(pic.body, pic.w * pic.h * 3)
    if pic.type == TYPE_LZ4_565:
        from .png import rgb565_to_rgb
        return rgb565_to_rgb(lz4.decompress(pic.body, pic.w * pic.h * 2))
    if pic.type == TYPE_JPEG:
        Image = _pil()
        tiles, w, h, tw, th = _jpeg_tiles(pic.body)
        if len(tiles) == 1 and not tw:
            im = Image.open(io.BytesIO(tiles[0])).convert("RGB")
            if im.size != (pic.w, pic.h):
                im = im.resize((pic.w, pic.h))
            return im.tobytes()
        canvas = Image.new("RGB", (pic.w, pic.h))
        cols = (pic.w + tw - 1) // tw
        for i, data in enumerate(tiles):
            im = Image.open(io.BytesIO(data)).convert("RGB")
            col, row = i % cols, i // cols
            canvas.paste(im, (col * tw, row * th))
        return canvas.tobytes()
    raise ValueError("cannot decode type 0x%02x (%s)" % (pic.type, pic.name))


def encode_jpeg_body(width: int, height: int, rgb: bytes,
                     quality: int = 85) -> bytes:
    Image = _pil()
    if len(rgb) != width * height * 3:
        raise ValueError("expected %d bytes of RGB888" % (width * height * 3))
    im = Image.frombytes("RGB", (width, height), rgb)
    buf = io.BytesIO()
    im.save(buf, "JPEG", quality=quality, subsampling=0)
    jpeg = buf.getvalue()
    body = bytearray(b"JPEG")
    body += struct.pack("<5H", width, height, width, height, 1)
    body += struct.pack("<HH", 18, len(jpeg))
    body += jpeg
    return bytes(body)


def encode_body(width: int, height: int, rgb: bytes, type: int) -> bytes:
    if type == TYPE_LZ4_888:
        if len(rgb) != width * height * 3:
            raise ValueError("expected %d bytes of RGB888" % (width * height * 3))
        return lz4.compress(rgb)
    if type == TYPE_LZ4_565:
        from .png import rgb_to_565
        if len(rgb) != width * height * 3:
            raise ValueError("expected %d bytes of RGB888" % (width * height * 3))
        return lz4.compress(rgb_to_565(rgb))
    if type == TYPE_JPEG:
        return encode_jpeg_body(width, height, rgb)
    raise ValueError("cannot encode type 0x%02x" % type)


def set_picture(blob: bytes, key, width: int, height: int, rgb: bytes) -> bytes:
    """Replace a picture's pixels (keeps type/name; updates w/h)."""
    res = parse(blob)
    p = res.get(key)
    p.w, p.h = width, height
    p.body = encode_body(width, height, rgb, p.type)
    return build(res)
