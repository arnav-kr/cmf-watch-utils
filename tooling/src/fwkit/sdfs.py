"""SDFS filesystem image (Actions resource partition)."""

from __future__ import annotations

import struct
from dataclasses import dataclass

from .aota import align, sum32

MAGIC = b"sdfs.bin"
HDR_SIZE = 0x20
ENTRY_SIZE = 32
DATA_ALIGN = 0x40


@dataclass
class Entry:
    name: str
    off: int
    size: int
    hash: int

    def data(self, img: bytes) -> bytes:
        return img[self.off:self.off + self.size]


def parse(img: bytes) -> list[Entry]:
    if img[:8] != MAGIC:
        raise ValueError("not an sdfs image (magic %r)" % img[:8])
    cnt = struct.unpack_from("<I", img, 0x0C)[0]
    out = []
    for i in range(cnt):
        o = HDR_SIZE + i * ENTRY_SIZE
        name = img[o:o + 12].split(b"\0")[0].decode("ascii", "replace")
        off, size, _a, _b, h = struct.unpack_from("<IIIII", img, o + 12)
        out.append(Entry(name, off, size, h))
    return out


def unpack(img: bytes) -> dict[str, bytes]:
    return {e.name: e.data(img) for e in parse(img)}


def verify(img: bytes, e: Entry) -> bool:
    return sum32(img[e.off:e.off + e.size]) == e.hash


def pack(files: list[tuple[str, bytes]]) -> bytes:
    cnt = len(files)
    data_start = align(HDR_SIZE + cnt * ENTRY_SIZE, DATA_ALIGN)
    body = bytearray()
    offs = []
    for _name, data in files:
        cur = data_start + len(body)
        cur = align(cur, DATA_ALIGN)
        body += b"\0" * (cur - data_start - len(body))
        offs.append(cur)
        body += data
    total = align(data_start + len(body), DATA_ALIGN)
    body += b"\0" * (total - data_start - len(body))

    table = bytearray()
    for (name, data), off in zip(files, offs):
        table += name.encode("ascii")[:12].ljust(12, b"\0")
        table += struct.pack("<IIIII", off, len(data), 0, 0, sum32(data))

    hdr = MAGIC + struct.pack("<IIIIII", 0, cnt, total, 0, sum32(bytes(table)), sum32(bytes(body)))
    out = bytearray(hdr) + table
    out += b"\0" * (data_start - len(out))
    out += body
    assert len(out) == total, (len(out), total)
    return bytes(out)


def replace(img: bytes, name: str, data: bytes) -> bytes:
    """Replace (or add) one file and rebuild the image."""
    files = unpack(img)
    files[name] = data
    return pack(list(files.items()))


def pad_to(img: bytes, size: int) -> bytes:
    """Pad an SDFS image to ``size`` and refresh ``total_size``/data sum."""
    if len(img) > size:
        raise ValueError("image is %d bytes; cannot pad to %d" % (len(img), size))
    if len(img) == size:
        return img
    cnt = struct.unpack_from("<I", img, 0x0C)[0]
    data_start = align(HDR_SIZE + cnt * ENTRY_SIZE, DATA_ALIGN)
    out = bytearray(img) + b"\0" * (size - len(img))
    struct.pack_into("<I", out, 0x10, size)
    struct.pack_into("<I", out, 0x1C, sum32(bytes(out[data_start:size])))
    return bytes(out)
