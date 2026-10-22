"""RES string table (``RES\\x19``, Actions resource strings).

Layout (verified byte-identical on ``bt_watch.zht``/``bt_watch.eng`` and
``bt_watch.res``):
  0x00  magic  "RES\\x19"
  0x04  u32    flags/subtype; low 16 bits == entry_count
  0x08  u32    unknown (0x200)
  0x0C  u32    unknown (0)
  0x10  u32    pool offset == 0x10 + entry_count * 16
  0x14  entries, 16 bytes each
         u16 len | u8 type | name[9] | u32 end_offset
         type 3 = UTF-8 string, type 0x26 = JPEG picture
         string == blob[end_offset-len : end_offset]  (includes trailing NUL)
  pool  concatenated payloads, in entry order

**Overlap quirk:** ``pool == 0x10 + count*16`` while entries start at
``0x14``, so the *last* entry's ``end_offset`` field sits inside the first
4 bytes of the pool.  The vendor therefore omits it; the last entry's end
is implicitly EOF.  ``build()`` reproduces this exactly.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass

MAGIC = b"RES\x19"
HDR_SIZE = 0x14
ENTRY_SIZE = 16
POOL_ALIGN = 0x10


def align(n: int, a: int) -> int:
    return (n + a - 1) // a * a


@dataclass
class Entry:
    name: str
    text: bytes
    implicit_end: bool = False

    def __str__(self) -> str:
        return self.text.rstrip(b"\0").decode("utf-8", "replace")


@dataclass
class ResFile:
    header: bytes
    entries: list[Entry]

    def get(self, key) -> Entry:
        if isinstance(key, int):
            return self.entries[key]
        for e in self.entries:
            if e.name == key:
                return e
        raise KeyError(key)

    def set(self, key, value: str | bytes) -> None:
        e = self.get(key)
        e.text = value.encode("utf-8") if isinstance(value, str) else value


def is_res(blob: bytes) -> bool:
    return len(blob) >= HDR_SIZE and blob[:4] == MAGIC


def is_string_table(blob: bytes) -> bool:
    if not is_res(blob):
        return False
    pool = struct.unpack_from("<I", blob, 0x10)[0]
    if not (HDR_SIZE + ENTRY_SIZE <= pool <= len(blob)):
        return False
    return blob[0x16] == 3 and blob[0x14 + 3:0x14 + 7] == b"STR"


def parse(blob: bytes) -> ResFile:
    if not is_res(blob):
        raise ValueError("not a RES file")
    pool = struct.unpack_from("<I", blob, 0x10)[0]
    count16 = struct.unpack_from("<I", blob, 4)[0] & 0xFFFF
    entries: list[Entry] = []
    o = HDR_SIZE
    while o + 12 <= pool and len(entries) < count16:
        ln, ty = struct.unpack_from("<HB", blob, o)
        name = blob[o + 3:o + 12].split(b"\0")[0].decode("ascii", "replace")
        if ty != 3 or not name.startswith("STR"):
            break
        if o + ENTRY_SIZE <= pool:
            end = struct.unpack_from("<I", blob, o + 12)[0]
            implicit = False
        else:
            end = len(blob)
            implicit = True
        if end < ln or end > len(blob):
            raise ValueError("bad string offset at entry %d" % len(entries))
        text = blob[end - ln:end]
        if not text.endswith(b"\0"):
            raise ValueError("string %r not NUL terminated" % name)
        entries.append(Entry(name, text, implicit))
        o += ENTRY_SIZE
    if count16 and len(entries) != count16:
        raise ValueError("entry count mismatch: header %d, parsed %d" % (count16, len(entries)))
    return ResFile(blob[:HDR_SIZE], entries)


def build(res: ResFile) -> bytes:
    n = len(res.entries)
    hdr = bytearray(res.header)
    assert len(hdr) == HDR_SIZE
    count16 = struct.unpack_from("<I", hdr, 4)[0] & 0xFFFF
    # Vendor convention: pool_off == 0x10 + count16 * ENTRY_SIZE.
    # String tables store count+1 (one sentinel record), so the pool starts
    # 12 bytes after the last real entry; picture tables store count and the
    # pool overlaps the last entry's 4-byte end field.
    if count16 in (n, n + 1):
        pool_off = 0x10 + count16 * ENTRY_SIZE
    else:
        pool_off = align(HDR_SIZE + n * ENTRY_SIZE, POOL_ALIGN)
    if count16 == n + 1:
        struct.pack_into("<I", hdr, 4, (struct.unpack_from("<I", hdr, 4)[0] & 0xFFFF0000) | (n + 1))
    struct.pack_into("<I", hdr, 0x10, pool_off)

    table = bytearray()
    pool = bytearray()
    for idx, e in enumerate(res.entries):
        text = e.text if e.text.endswith(b"\0") else e.text + b"\0"
        table += struct.pack("<HB", len(text), 3)
        table += e.name.encode("ascii")[:9].ljust(9, b"\0")
        implicit = n > 0 and idx == n - 1 and (e.implicit_end or count16 == n)
        if not implicit:
            table += struct.pack("<I", pool_off + len(pool) + len(text))
        pool += text

    out = bytearray(hdr) + table
    if len(out) > pool_off:
        raise ValueError("entry table overruns pool offset")
    out += b"\0" * (pool_off - len(out))
    out += pool
    return bytes(out)


def set_string(blob: bytes, key, value: str | bytes) -> bytes:
    res = parse(blob)
    res.set(key, value)
    return build(res)
