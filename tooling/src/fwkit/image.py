"""High level AOTA image editing (decompress -> edit -> repack)."""

from __future__ import annotations

import struct

from . import aota, sdfs

TEMP_CHUNK = 0x200000


class Image:
    def __init__(self, data: bytes):
        self.data = data
        self.header, self.entries = aota.parse(data)
        self.xml = self.entries[0].stored(data)
        self._parts: dict[str, bytes] = {}
        self._dirty: set[str] = set()

    @classmethod
    def open(cls, path: str) -> "Image":
        with open(path, "rb") as f:
            return cls(f.read())

    # -- access ---------------------------------------------------------
    @property
    def names(self) -> list[str]:
        return [e.name for e in self.entries if e.name != "ota.xml"]

    def stored(self, name: str) -> bytes:
        if name in self._parts:
            return self._parts[name]
        for e in self.entries:
            if e.name == name:
                return e.stored(self.data)
        raise KeyError(name)

    def decompressed(self, name: str) -> bytes:
        return aota.unpack_chunks(self.stored(name))

    def part_info(self, name: str) -> dict:
        return aota.xml_get(self.xml, name)

    # -- mutation -------------------------------------------------------
    def set_stored(self, name: str, blob: bytes) -> None:
        self._parts[name] = blob
        self._dirty.add(name)

    def set_decompressed(self, name: str, data: bytes) -> None:
        chunk = TEMP_CHUNK if name == "TEMP.bin" else aota.CHUNK_RES
        self.set_stored(name, aota.pack_chunks(data, chunk))

    def edit_sdfs(self, partition: str, filename: str, data: bytes) -> None:
        img = self.decompressed(partition)
        self.set_decompressed(partition, sdfs.replace(img, filename, data))

    def edit_res_string(self, partition: str, resfile: str, key, value: str) -> None:
        from . import res
        img = self.decompressed(partition)
        blob = sdfs.unpack(img)[resfile]
        self.edit_sdfs(partition, resfile, res.set_string(blob, key, value))

    def edit_sty_pic(self, partition: str, styfile: str, node_id: int, pic_index: int,
                     from_pic: int | None = None, all_matches: bool = False) -> None:
        from . import sdfs, sty
        img = self.decompressed(partition)
        blob = sdfs.unpack(img)[styfile]
        self.edit_sdfs(partition, styfile,
                       sty.set_pic_index(blob, node_id, pic_index,
                                         from_pic=from_pic, all_matches=all_matches))

    def edit_picture(self, partition: str, resfile: str, key, w: int, h: int, rgb: bytes) -> None:
        from . import picres, sdfs
        img = self.decompressed(partition)
        blob = sdfs.unpack(img)[resfile]
        self.edit_sdfs(partition, resfile, picres.set_picture(blob, key, w, h, rgb))

    def app_image(self) -> bytes:
        """The raw app.bin extracted from the nested AOTA inside TEMP.bin."""
        raw = self.decompressed("TEMP.bin")
        hdr, ents = aota.parse(raw)
        for e in ents:
            if e.name == "app.bin":
                return e.stored(raw)
        raise KeyError("app.bin not found in TEMP.bin")

    def edit_app(self, addr: int, patch: bytes) -> None:
        """Patch app.bin at virtual ``addr`` (base 0x10000000) and rebuild TEMP."""
        raw = self.decompressed("TEMP.bin")
        hdr, ents = aota.parse(raw)
        xml = ents[0].stored(raw)
        parts = [(e.name, e.stored(raw)) for e in ents if e.name != "ota.xml"]
        pmap = dict(parts)
        app = bytearray(pmap["app.bin"])
        off = addr - 0x10000000
        if off < 0 or off + len(patch) > len(app):
            raise ValueError("patch outside app.bin: %#x" % off)
        app[off:off + len(patch)] = patch
        app = bytes(app)
        pmap["app.bin"] = app
        xml = aota.xml_set(xml, {"app.bin": {
            "file_size": len(app),
            "orig_size": len(app),
            "checksum": aota.crc32(app),
        }})
        agps = raw[aota.agps_offset(hdr):]
        new_temp = aota.build(hdr, xml, [(n, pmap[n]) for n, _ in parts], agps)
        self.set_decompressed("TEMP.bin", new_temp)

    def build(self) -> bytes:
        parts = [(e.name, self.stored(e.name)) for e in self.entries if e.name != "ota.xml"]
        agps = self.data[aota.agps_offset(self.header):]
        xml = self.xml
        for name in self.names:
            blob = dict(parts)[name]
            if name == "TEMP.bin":
                xml = aota.xml_set(xml, {name: {
                    "file_size": len(blob),
                    "orig_size": len(blob),
                    "checksum": aota.crc32(blob),
                }})
            else:
                raw = aota.unpack_chunks(blob)
                xml = aota.xml_set(xml, {name: {
                    "file_size": len(blob),
                    "orig_size": len(raw),
                    "checksum": aota.crc32(raw),
                }})
        self.xml = xml
        return aota.build(self.header, xml, parts, agps)

    def save(self, path: str) -> int:
        out = self.build()
        with open(path, "wb") as f:
            f.write(out)
        return len(out)


def verify(path: str) -> bool:
    data = open(path, "rb").read()
    hdr, ents = aota.parse(data)
    if not aota.verify_header(hdr):
        return False
    for e in ents:
        if not aota.verify_entry(data, e):
            return False
    return True
