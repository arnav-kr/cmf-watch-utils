"""Tiny PNG reader/writer (8-bit RGB/RGBA, non-interlaced). No deps."""

from __future__ import annotations

import struct
import zlib

SIG = b"\x89PNG\r\n\x1a\n"


def _chunk(tag: bytes, data: bytes) -> bytes:
    return (struct.pack(">I", len(data)) + tag + data
            + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF))


def encode(width: int, height: int, rgb: bytes, alpha: bool = False) -> bytes:
    """``rgb`` is top-down; 3 bytes per pixel, or 4 with ``alpha=True``."""
    bpp = 4 if alpha else 3
    assert len(rgb) == width * height * bpp
    if alpha:
        raw = b"".join(b"\x00" + rgb[y * width * 4:(y + 1) * width * 4]
                       for y in range(height))
    else:
        raw = b"".join(b"\x00" + rgb[y * width * 3:(y + 1) * width * 3]
                       for y in range(height))
    return (SIG
            + _chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6 if alpha else 2, 0, 0, 0))
            + _chunk(b"IDAT", zlib.compress(raw, 6))
            + _chunk(b"IEND", b""))


def _paeth(a: int, b: int, c: int) -> int:
    p = a + b - c
    pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
    if pa <= pb and pa <= pc:
        return a
    return b if pb <= pc else c


def decode(blob: bytes) -> tuple[int, int, bytes]:
    """Return ``(w, h, rgb)`` for an 8-bit RGB/RGBA non-interlaced PNG."""
    return _decode(blob, alpha=False)


def decode_rgba(blob: bytes) -> tuple[int, int, bytes]:
    """Return ``(w, h, rgba)`` for an 8-bit RGB/RGBA non-interlaced PNG."""
    return _decode(blob, alpha=True)


def _decode(blob: bytes, alpha: bool) -> tuple[int, int, bytes]:
    """Decode to RGB or RGBA (alpha = 255 for RGB sources)."""
    if not blob.startswith(SIG):
        raise ValueError("not a PNG")
    o = 8
    idat = bytearray()
    w = h = depth = ctype = interlace = None
    while o < len(blob):
        ln, tag = struct.unpack_from(">I4s", blob, o)
        data = blob[o + 8:o + 8 + ln]
        o += 12 + ln
        if tag == b"IHDR":
            w, h, depth, ctype, _c, _f, interlace = struct.unpack(">IIBBBBB", data)
        elif tag == b"IDAT":
            idat += data
        elif tag == b"IEND":
            break
    if depth != 8 or ctype not in (2, 6) or interlace != 0:
        raise ValueError("unsupported PNG (depth=%s ctype=%s interlace=%s)"
                         % (depth, ctype, interlace))
    bpp = 3 if ctype == 2 else 4
    stride = w * bpp
    raw = zlib.decompress(bytes(idat))
    prev = bytearray(stride)
    out = bytearray(w * h * (4 if alpha else 3))
    p = 0
    for y in range(h):
        f = raw[p]
        p += 1
        line = bytearray(raw[p:p + stride])
        p += stride
        if f == 1:
            for i in range(bpp, stride):
                line[i] = (line[i] + line[i - bpp]) & 0xFF
        elif f == 2:
            for i in range(stride):
                line[i] = (line[i] + prev[i]) & 0xFF
        elif f == 3:
            for i in range(stride):
                a = line[i - bpp] if i >= bpp else 0
                line[i] = (line[i] + ((a + prev[i]) >> 1)) & 0xFF
        elif f == 4:
            for i in range(stride):
                a = line[i - bpp] if i >= bpp else 0
                c = prev[i - bpp] if i >= bpp else 0
                line[i] = (line[i] + _paeth(a, prev[i], c)) & 0xFF
        elif f != 0:
            raise ValueError("bad PNG filter %d" % f)
        prev = line
        if alpha:
            if bpp == 4:
                out[y * w * 4:(y + 1) * w * 4] = line
            else:
                for x in range(w):
                    o = (y * w + x) * 4
                    out[o:o + 3] = line[x * 3:x * 3 + 3]
                    out[o + 3] = 255
        elif bpp == 3:
            out[y * w * 3:(y + 1) * w * 3] = line
        else:
            for x in range(w):
                out[(y * w + x) * 3:(y * w + x) * 3 + 3] = line[x * 4:x * 4 + 3]
    return w, h, bytes(out)


def rgb_to_565(rgb: bytes) -> bytes:
    out = bytearray(len(rgb) // 3 * 2)
    for i in range(len(rgb) // 3):
        r, g, b = rgb[i * 3], rgb[i * 3 + 1], rgb[i * 3 + 2]
        v = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
        out[i * 2] = v & 0xFF
        out[i * 2 + 1] = v >> 8
    return bytes(out)


def rgb565_to_rgb(data: bytes) -> bytes:
    out = bytearray(len(data) // 2 * 3)
    for i in range(len(data) // 2):
        v = data[i * 2] | (data[i * 2 + 1] << 8)
        out[i * 3] = ((v >> 11) & 31) * 255 // 31
        out[i * 3 + 1] = ((v >> 5) & 63) * 255 // 63
        out[i * 3 + 2] = (v & 31) * 255 // 31
    return bytes(out)
