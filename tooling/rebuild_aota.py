#!/usr/bin/env python3
"""Rebuild an AOTA image from an unpacked directory.

The directory layout is the one produced by the legacy unpacker:

    <dir>/meta.json      header fields and per-entry metadata
    <dir>/ota.xml        the XML manifest, byte for byte
    <dir>/parts/<name>   raw partition payloads
    <dir>/agps.bin       the AGPS trailer, appended uncompressed

The LZMA preset of every partition is recovered by matching the stored size and CRC
recorded in meta.json, so a rebuild is byte-identical to the original image when the
source directory came from that image.

    python tooling/rebuild_aota.py firmware/fwtest JX402_V1.0.0.57.bin
"""

from __future__ import annotations

import json
import struct
import sys
import zlib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent / "src"))

from fwkit.aota import pack_chunks  # noqa: E402

HDR_SIZE = 0x400
TBL_OFF = 0x200
ENTRY_SIZE = 32
PART_ALIGN = 0x200


def align(n: int, a: int) -> int:
    return (n + a - 1) // a * a


def build_header(meta: dict, entries: list[tuple[str, int, int, int]]) -> bytes:
    hdr = bytearray(HDR_SIZE)
    hdr[0x00:0x04] = b"AOTA"
    struct.pack_into("<I", hdr, 0x08, meta["header"]["f0"])
    struct.pack_into("<I", hdr, 0x0C, meta["header"]["count"])
    struct.pack_into("<I", hdr, 0x10, meta["header"]["f10"])
    agps_off = meta.get("agps_off", meta["header"].get("agps_off", 0))
    struct.pack_into("<I", hdr, 0x14, agps_off)
    struct.pack_into("<I", hdr, 0x18, meta["header"]["region_crc"])
    struct.pack_into("<I", hdr, 0x1C, meta["header"]["f1c"])
    hdr[0x40:0x50] = meta["header"]["version_name"].encode().ljust(16, b"\0")
    hdr[0x60:0x70] = meta["header"]["board_name"].encode().ljust(16, b"\0")
    hdr[0x7C:0x80] = bytes.fromhex(meta["header"]["w7c"])
    hdr[0x80:0x84] = bytes.fromhex(meta["header"]["w80"])
    table = bytearray()
    for name, off, size, crc in entries:
        table += name.encode()[:16].ljust(16, b"\0") + struct.pack("<IIII", off, size, 0, crc)
    hdr[TBL_OFF:TBL_OFF + len(table)] = table
    struct.pack_into("<I", hdr, 4, zlib.crc32(bytes(hdr[8:HDR_SIZE])) & 0xFFFFFFFF)
    return bytes(hdr)


def pack_partition(raw: bytes, block: int, stored_size: int, stored_crc: int) -> bytes:
    if block == 0:
        return raw
    for preset in range(10):
        blob = pack_chunks(raw, block, preset)
        if len(blob) == stored_size and (zlib.crc32(blob) & 0xFFFFFFFF) == stored_crc:
            return blob
    raise SystemExit(
        "no LZMA preset reproduces the stored size %d / crc %08x" % (stored_size, stored_crc)
    )


def rebuild(src: Path, out: Path) -> int:
    meta = json.loads((src / "meta.json").read_text(encoding="utf-8"))
    xml = (src / "ota.xml").read_bytes()
    agps = (src / "agps.bin").read_bytes()

    entries: list[tuple[str, int, int, int]] = []
    blobs: list[tuple[int, bytes]] = []
    pos = HDR_SIZE
    xml_crc = zlib.crc32(xml) & 0xFFFFFFFF
    entries.append(("ota.xml", pos, len(xml), xml_crc))
    blobs.append((pos, xml))
    pos += len(xml)

    for entry in meta["entries"]:
        raw = (src / "parts" / entry["name"]).read_bytes()
        if len(raw) != entry["raw"]:
            raise SystemExit("%s: raw size mismatch" % entry["name"])
        stored = pack_partition(raw, entry["block"], entry["stored"], entry["crc"])
        pos = align(pos, PART_ALIGN)
        entries.append((entry["name"], pos, len(stored), zlib.crc32(stored) & 0xFFFFFFFF))
        blobs.append((pos, stored))
        pos += len(stored)

    pos = align(pos, PART_ALIGN)
    agps_off = meta.get("agps_off", meta["header"].get("agps_off"))
    if agps_off is not None and pos != agps_off:
        raise SystemExit("agps offset mismatch: %d != %d" % (pos, agps_off))

    hdr = build_header(meta, entries)
    want_crc = meta["header"]["hdr_crc"]
    got_crc = struct.unpack_from("<I", hdr, 4)[0]
    if got_crc != want_crc:
        raise SystemExit("header crc mismatch: %08x != %08x" % (got_crc, want_crc))

    image = bytearray(hdr)
    for off, blob in blobs:
        image += b"\0" * (off - len(image))
        image += blob
    image += b"\0" * (pos - len(image))
    image += agps

    out.write_bytes(bytes(image))
    print("wrote %s (%d bytes)" % (out, len(image)))
    print("header crc %08x ok, %d entries" % (got_crc, len(entries)))
    return 0


def main() -> int:
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    return rebuild(Path(sys.argv[1]), Path(sys.argv[2]))


if __name__ == "__main__":
    raise SystemExit(main())
