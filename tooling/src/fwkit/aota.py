"""AOTA container (Actions ATS3089C OTA) parse / build.

Layout
------
0x000..0x400   header
  0x00  magic "AOTA" (implementation-defined)
  0x04  u32  crc32(header[0x08:0x400])
  0x0C  u32  number of table entries
  0x14  u32  AGPS trailer offset (0x200 aligned)
  0x18  u32  unknown (preserved)
  0x40  char[16] version string
  0x60  char[16] board string
  0x200 table: count * 32 byte entries
        name[16] | u32 off | u32 size | u32 0 | u32 crc32(stored bytes)
0x400..       ota.xml (entry 0, size from table)
partitions    0x200 aligned, stored as a chunk stream
trailer       AGPS partition at header[0x14]
"""

from __future__ import annotations

import lzma
import re
import struct
import zlib
from dataclasses import dataclass

HDR_SIZE = 0x400
TBL_OFF = 0x200
ENTRY_SIZE = 32
PART_ALIGN = 0x200

# resource partitions use 32 KiB chunks, TEMP uses 2 MiB
CHUNK_RES = 0x8000
CHUNK_TEMP = 0x200000


def crc32(data: bytes) -> int:
    return zlib.crc32(data) & 0xFFFFFFFF


def align(n: int, a: int) -> int:
    return (n + a - 1) // a * a


def sum32(d: bytes) -> int:
    n = len(d) // 4 * 4
    return sum(struct.unpack("<%dI" % (n // 4), d[:n])) & 0xFFFFFFFF


@dataclass
class Entry:
    name: str
    off: int
    size: int
    crc: int
    raw: bytes = b""

    def stored(self, img: bytes) -> bytes:
        return img[self.off:self.off + self.size]


def parse(img: bytes) -> tuple[bytearray, list[Entry]]:
    if len(img) < HDR_SIZE:
        raise ValueError("file too small")
    cnt = struct.unpack_from("<I", img, 0x0C)[0]
    if cnt == 0 or cnt > 64:
        raise ValueError("implausible entry count %d" % cnt)
    entries: list[Entry] = []
    for i in range(cnt):
        o = TBL_OFF + i * ENTRY_SIZE
        e = img[o:o + ENTRY_SIZE]
        if len(e) < ENTRY_SIZE:
            raise ValueError("table truncated")
        name = e[:16].split(b"\0")[0].decode("ascii", "replace")
        off, size, _z, ck = struct.unpack_from("<IIII", e, 16)
        entries.append(Entry(name, off, size, ck, e))
    return bytearray(img[:HDR_SIZE]), entries


def verify_header(hdr: bytes) -> bool:
    return crc32(hdr[8:HDR_SIZE]) == struct.unpack_from("<I", hdr, 4)[0]


def fix_header(hdr: bytearray) -> None:
    struct.pack_into("<I", hdr, 4, crc32(bytes(hdr[8:HDR_SIZE])))


def verify_entry(img: bytes, ent: Entry) -> bool:
    return crc32(ent.stored(img)) == ent.crc


def agps_offset(hdr: bytes) -> int:
    return struct.unpack_from("<I", hdr, 0x14)[0]


def build(hdr: bytearray, xml: bytes, parts: list[tuple[str, bytes]], agps: bytes) -> bytes:
    """Rebuild an image.

    ``parts`` holds the stored (still compressed/chunked) partition blobs in
    order, excluding ``ota.xml``.  All offsets, sizes and checksums are
    recomputed; the caller-supplied header fields (0x18, version, board) are
    preserved.
    """
    hdr = bytearray(hdr)
    entries: list[tuple[str, int, int, int]] = [("ota.xml", HDR_SIZE, len(xml), crc32(xml))]
    out = bytearray(hdr[:HDR_SIZE]) + xml
    for name, blob in parts:
        off = align(len(out), PART_ALIGN)
        out += b"\0" * (off - len(out))
        entries.append((name, off, len(blob), crc32(blob)))
        out += blob
    off = align(len(out), PART_ALIGN)
    out += b"\0" * (off - len(out))
    hdr[0x14:0x18] = struct.pack("<I", off)
    out += agps

    tbl = bytearray()
    for name, eoff, size, ck in entries:
        tbl += name.encode("ascii")[:16].ljust(16, b"\0") + struct.pack("<IIII", eoff, size, 0, ck)
    hdr[TBL_OFF:TBL_OFF + len(tbl)] = tbl
    struct.pack_into("<I", hdr, 0x0C, len(entries))
    fix_header(hdr)
    out[:HDR_SIZE] = hdr
    return bytes(out)


def unpack_chunks(blob: bytes) -> bytes:
    out = bytearray()
    pos = 0
    while pos + 16 <= len(blob):
        if blob[pos:pos + 4] != b"LZMA":
            raise ValueError("bad chunk magic at %#x" % pos)
        hl, comp, unc = struct.unpack_from("<III", blob, pos + 4)
        data = lzma.decompress(blob[pos + 16:pos + 16 + comp], format=lzma.FORMAT_XZ)
        if len(data) != unc:
            raise ValueError("chunk %#x size mismatch: %d != %d" % (pos, len(data), unc))
        out += data
        pos += 16 + comp
    return bytes(out)


def pack_chunks(data: bytes, chunk: int = CHUNK_RES, preset: int = 6) -> bytes:
    out = bytearray()
    for i in range(0, len(data), chunk):
        piece = data[i:i + chunk]
        comp = lzma.compress(piece, format=lzma.FORMAT_XZ, preset=preset)
        out += b"LZMA" + struct.pack("<III", 16, len(comp), len(piece)) + comp
    return bytes(out)


# --------------------------------------------------------------------------
# ota.xml helpers
# --------------------------------------------------------------------------

def xml_get(xml: bytes, name: str) -> dict:
    s = xml.decode()
    m = re.search(r"<file_name>%s</file_name>(.*?)</partition>" % re.escape(name), s, re.S)
    if not m:
        raise KeyError(name)
    d = {}
    for tag in ("file_size", "orig_size", "checksum"):
        mm = re.search(r"<%s>0x([0-9a-fA-F]+)</%s>" % (tag, tag), m.group(1))
        if mm:
            d[tag] = int(mm.group(1), 16)
    return d


def xml_names(xml: bytes) -> list[str]:
    return re.findall(r"<file_name>([^<]+)</file_name>", xml.decode())


def xml_set(xml: bytes, fields: dict[str, dict]) -> bytes:
    """fields: name -> {"file_size": int|None, "orig_size":..., "checksum":...}."""
    s = xml.decode()
    for name, vals in fields.items():
        m = re.search(r"<file_name>%s</file_name>.*?</partition>" % re.escape(name), s, re.S)
        if not m:
            raise KeyError(name)
        block = m.group(0)
        for tag in ("file_size", "orig_size", "checksum"):
            val = vals.get(tag)
            if val is None:
                continue
            block = re.sub(r"<%s>0x[0-9a-fA-F]+</%s>" % (tag, tag),
                           "<%s>0x%x</%s>" % (tag, val, tag), block)
        s = s[:m.start()] + block + s[m.end():]
    return s.encode()
