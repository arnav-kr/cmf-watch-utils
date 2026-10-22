"""app.bin container ("ACTHHTCA") for the Actions ATS3089C application.

The whole file is mapped at ``LOAD_BASE`` (file offset 0 == 0x10000000):
  * 0x000..0x200  container header
  * 0x200..       Cortex-M vector table (SP, Reset, ...)
  * tail          small SDFS image (``sdfs.bin``) + build paths

Header fields (observed):
  0x00 magic "ACTHHTCA"
  0x08 u32 entry point (thumb)     0x14 u32 same entry point
  0x0C u32 unknown (0x62)
  0x18 u32 code size (payload - 0xC)
  0x1C u32 unknown checksum
  0x20 u32 unknown checksum
  0x24 u32 header size (0x200)
"""

from __future__ import annotations

import struct
from dataclasses import dataclass

MAGIC = b"ACTHHTCA"
LOAD_BASE = 0x10000000


@dataclass
class App:
    data: bytes

    @property
    def header(self) -> bytes:
        return self.data[:0x200]

    def u32(self, off: int) -> int:
        return struct.unpack_from("<I", self.data, off)[0]

    @property
    def entry(self) -> int:
        return self.u32(0x08)

    @property
    def field_0c(self) -> int:
        return self.u32(0x0C)

    @property
    def field_18(self) -> int:
        return self.u32(0x18)

    @property
    def field_1c(self) -> int:
        return self.u32(0x1C)

    @property
    def field_20(self) -> int:
        return self.u32(0x20)

    @property
    def header_size(self) -> int:
        return self.u32(0x24)

    @property
    def load_base(self) -> int:
        return LOAD_BASE

    @property
    def payload_size(self) -> int:
        return len(self.data) - self.header_size

    def addr(self, file_off: int) -> int:
        return LOAD_BASE + file_off

    def off(self, addr: int) -> int:
        return addr - LOAD_BASE

    def find_u32(self, value: int, start: int = 0,
                 end: int | None = None, align: int = 4) -> list[int]:
        """Return file offsets of 32-bit little-endian ``value`` literals."""
        end = len(self.data) if end is None else end
        out = []
        for off in range(start, end - 3, align):
            if struct.unpack_from("<I", self.data, off)[0] == value:
                out.append(off)
        return out

    def patch_u32(self, old: int, new: int,
                  offsets: list[int] | None = None) -> bytes:
        """Replace scene-id literals, optionally restricted to ``offsets``."""
        if offsets is None:
            offsets = self.find_u32(old)
        if not offsets:
            raise ValueError("no literal for %#010x" % old)
        out = bytearray(self.data)
        for off in offsets:
            if struct.unpack_from("<I", out, off)[0] != old:
                raise ValueError("offset %#x does not hold %#010x" % (off, old))
            struct.pack_into("<I", out, off, new)
        return bytes(out)


def parse(data: bytes) -> App:
    if data[:8] != MAGIC:
        raise ValueError("not an app.bin (magic %r)" % data[:8])
    return App(data)
