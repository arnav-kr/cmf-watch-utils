"""CMF Watch Pro 2 watch-face package (``.wfdl`` / ``*.bin``).

Verified against three real packages (``wf1``/``wf2``/``wf3``) and
cross-checked with freethinkel/fmc's ``format/bin.ts`` (facts only).

Layout::

  0x00  36 B  header
  0x24   1 B  root tag 0x20
  0x25  u16   TLV tree length
  0x27  ...   TLV tree: [tag u8][len u16][value] (nested)
        ...   resources: [u32 hdr | u32 size | LZ4 block] ...
  end-36 36 B footer, byte-identical copy of the header

Header::

  0x00  crc32-raw(file[0x04 : 0x27+tree_len])
  0x04  01 00 00 00  (bit 1 of byte 7 may be set)
  0x08  15-byte NUL-padded name + 1 unknown byte at 0x17
  0x18  u32 body length = file_size - 36
  0x1C  u32 resources-section length
  0x20  crc32-raw(resources section)

``crc32-raw`` is the IEEE reflected polynomial with init 0 and **no**
final XOR.  Resource header: ``cf | w<<10 | h<<21`` (w/h 11 bits).

Image-node tails reference resources by **absolute file offset**:
``[kind u8][count u16][offset u32][...]``.  kind 0x01 stores one absolute
u32 per frame; kinds 0x61/0x71 store the first offset and then one u16
block size (compressed size + 8) per frame, walking consecutive blocks.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field

from . import lz4, png

HDR = 36

MAX_IMAGE_DIM = 480
MAX_PACKAGE_BYTES = 32 * 1024 * 1024

TAG_NAMES = {
    0x20: "root", 0x21: "main", 0x22: "aod", 0x86: "name", 0x28: "preview",
    0x01: "struct", 0x02: "bind", 0x05: "pivot", 0x08: "pv_struct",
    0x40: "fmt", 0x48: "frame", 0x30: "image", 0x60: "number",
    0x68: "group", 0x70: "hand", 0x5A: "arc", 0x5B: "arc_clipped",
    0x5F: "slot",
}

# fixed-layout leaves: their value must never be descended into
LEAF = {0x02, 0x40, 0x48, 0x5A, 0x5B, 0x5F}

HAND_KINDS = {0x0A: "hour", 0x0E: "minute", 0x72: "second", 0x12: "second"}

CF_BPP = {4: 2.0, 5: 3.0, 13: 0.5, 14: 1.0, 24: 4.0}
CF_NAMES = {1: "JPEG", 4: "RGB565", 5: "RGB565+A8", 13: "A4", 14: "A8",
            24: "BGRA8888", 28: "ETC2"}


def raw_crc32(data: bytes) -> int:
    c = 0
    for b in data:
        c ^= b
        for _ in range(8):
            c = (c >> 1) ^ (0xEDB88320 if c & 1 else 0)
    return c & 0xFFFFFFFF


def cf_bpp(cf: int) -> float:
    try:
        return CF_BPP[cf]
    except KeyError:
        raise ValueError("unsupported cf=%d (%s)" % (cf, CF_NAMES.get(cf, "?")))


def raw_size(cf: int, w: int, h: int) -> int:
    if cf in (1, 28):
        raise ValueError("cf=%d has no raw size" % cf)
    return (w * h + 1) // 2 if cf == 13 else int(w * h * cf_bpp(cf))


@dataclass
class Ref:
    tail: int               # offset of the ref tail inside the node value
    kind: int               # 0x01, 0x61 or 0x71
    images: list[int] = field(default_factory=list)


@dataclass
class Node:
    tag: int
    value: bytes
    offset: int = 0                 # absolute file offset of [tag][len]
    children: list["Node"] | None = None
    ref: Ref | None = None

    @property
    def name(self) -> str:
        return TAG_NAMES.get(self.tag, "0x%02x" % self.tag)

    def walk(self):
        yield self
        for c in self.children or ():
            yield from c.walk()

    def find(self, tag: int) -> list["Node"]:
        return [n for n in self.walk() if n.tag == tag]

    def __repr__(self):
        extra = ""
        if self.ref:
            extra = " ref=%#x%s" % (self.ref.kind, self.ref.images)
        return "<%s 0x%02x len=%d%s>" % (self.name, self.tag, len(self.value), extra)


@dataclass
class Resource:
    cf: int
    w: int
    h: int
    data: bytes             # LZ4 block (or JPEG payload for cf=1)
    offset: int = 0         # absolute file offset of the [hdr][size] header

    @property
    def name(self) -> str:
        return CF_NAMES.get(self.cf, "cf%d" % self.cf)

    @property
    def raw_size(self) -> int:
        return raw_size(self.cf, self.w, self.h)

    def decode_rgba(self) -> bytes:
        if self.cf in (1, 28):
            raise ValueError("cf=%d is not decodable here" % self.cf)
        raw = lz4.decompress(self.data, self.raw_size)
        return decode_pixels(raw, self.cf, self.w, self.h)


def decode_pixels(raw: bytes, cf: int, w: int, h: int) -> bytes:
    n = w * h
    out = bytearray(n * 4)
    if cf == 4:
        for i in range(n):
            v = raw[i * 2] | (raw[i * 2 + 1] << 8)
            out[i * 4] = ((v >> 11) & 31) * 255 // 31
            out[i * 4 + 1] = ((v >> 5) & 63) * 255 // 63
            out[i * 4 + 2] = (v & 31) * 255 // 31
            out[i * 4 + 3] = 255
    elif cf == 5:
        for i in range(n):
            v = raw[i * 3] | (raw[i * 3 + 1] << 8)
            out[i * 4] = ((v >> 11) & 31) * 255 // 31
            out[i * 4 + 1] = ((v >> 5) & 63) * 255 // 63
            out[i * 4 + 2] = (v & 31) * 255 // 31
            out[i * 4 + 3] = raw[i * 3 + 2]
    elif cf == 13:
        for i in range(n):
            nib = raw[i >> 1]
            a = (nib & 15) if i & 1 else (nib >> 4)
            out[i * 4:i * 4 + 3] = b"\xff\xff\xff"
            out[i * 4 + 3] = a * 17
    elif cf == 14:
        for i in range(n):
            out[i * 4:i * 4 + 3] = b"\xff\xff\xff"
            out[i * 4 + 3] = raw[i]
    elif cf == 24:
        for i in range(n):
            out[i * 4] = raw[i * 4 + 2]
            out[i * 4 + 1] = raw[i * 4 + 1]
            out[i * 4 + 2] = raw[i * 4]
            out[i * 4 + 3] = raw[i * 4 + 3]
    else:
        raise ValueError("unsupported cf=%d" % cf)
    return bytes(out)


def encode_pixels(rgba: bytes, cf: int, w: int, h: int) -> bytes:
    n = w * h
    if len(rgba) != n * 4:
        raise ValueError("expected %d bytes of RGBA" % (n * 4))
    if w > 2047 or h > 2047:
        raise ValueError("image %dx%d does not fit 11-bit fields" % (w, h))

    def r565(i):
        r, g, b = rgba[i * 4], rgba[i * 4 + 1], rgba[i * 4 + 2]
        return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)

    if cf == 4:
        out = bytearray(n * 2)
        for i in range(n):
            v = r565(i)
            out[i * 2] = v & 0xFF
            out[i * 2 + 1] = v >> 8
    elif cf == 5:
        out = bytearray(n * 3)
        for i in range(n):
            v = r565(i)
            out[i * 3] = v & 0xFF
            out[i * 3 + 1] = v >> 8
            out[i * 3 + 2] = rgba[i * 4 + 3]
    elif cf == 13:
        out = bytearray((n + 1) // 2)
        for i in range(n):
            a = rgba[i * 4 + 3] >> 4
            if i & 1:
                out[i >> 1] |= a
            else:
                out[i >> 1] |= a << 4
    elif cf == 14:
        out = bytearray(rgba[i * 4 + 3] for i in range(n))
    elif cf == 24:
        out = bytearray(n * 4)
        for i in range(n):
            out[i * 4] = rgba[i * 4 + 2]
            out[i * 4 + 1] = rgba[i * 4 + 1]
            out[i * 4 + 2] = rgba[i * 4]
            out[i * 4 + 3] = rgba[i * 4 + 3]
    else:
        raise ValueError("cf=%d cannot be encoded" % cf)
    return bytes(out)


def _u16(d, o):
    return d[o] | (d[o + 1] << 8)


def _u32(d, o):
    return d[o] | (d[o + 1] << 8) | (d[o + 2] << 16) | (d[o + 3] << 24)


def _parse_nodes(data, base, depth=0, lenient=False):
    nodes = []
    off = 0
    while off < len(data):
        if off + 3 > len(data):
            if lenient:
                return None
            raise ValueError("truncated TLV at 0x%x" % (base + off))
        tag = data[off]
        ln = _u16(data, off + 1)
        if off + 3 + ln > len(data):
            if lenient:
                return None
            raise ValueError("TLV 0x%02x overruns at 0x%x" % (tag, base + off))
        val = bytes(data[off + 3:off + 3 + ln])
        n = Node(tag, val, base + off)
        if ln >= 3 and depth < 12:
            n.children = _parse_nodes(val, base + off + 3, depth + 1, True)
        nodes.append(n)
        off += 3 + ln
    return nodes


def _parse_ref(v, off, res_by_off, resources):
    if off + 3 > len(v):
        return None
    kind = v[off]
    count = _u16(v, off + 1)
    p = off + 3
    if p + 4 > len(v):
        return None
    ref = _u32(v, p)
    p += 4
    if ref not in res_by_off:
        return None
    idx = res_by_off[ref]
    if kind == 0x01:
        if count == 0 or p + 4 * (count - 1) != len(v):
            return None
        images = [idx]
        for k in range(1, count):
            o = _u32(v, p + 4 * (k - 1))
            if o not in res_by_off:
                return None
            images.append(res_by_off[o])
        return Ref(off, kind, images)
    if kind in (0x61, 0x71):
        if count == 0 or p + 2 * count != len(v):
            return None
        images = []
        cur = ref
        for k in range(count):
            if cur not in res_by_off:
                return None
            i = res_by_off[cur]
            images.append(i)
            if k < count - 1:
                blk = len(resources[i].data) + 8
                if blk != _u16(v, p + 2 * k):
                    return None
                cur += blk
        return Ref(off, kind, images)
    return None


@dataclass
class Watchface:
    header: bytes
    tree: list[Node]
    tree_sec: bytes
    resources: list[Resource]
    raw: bytes = b""

    @property
    def name_raw(self) -> bytes:
        return self.header[8:0x17]

    @property
    def name(self) -> str:
        return self.name_raw.split(b"\0")[0].decode("utf-8", "replace")

    def save(self, path: str) -> int:
        blob = self.build()
        with open(path, "wb") as f:
            f.write(blob)
        return len(blob)

    def validate(self, max_dim: int = MAX_IMAGE_DIM,
                 max_bytes: int = MAX_PACKAGE_BYTES) -> None:
        for i, r in enumerate(self.resources):
            if r.w > max_dim or r.h > max_dim:
                raise ValueError(
                    "resource %d (%s) is %dx%d; max %d px per side"
                    % (i, r.name, r.w, r.h, max_dim))
        size = len(self.build(validate=False))
        if size > max_bytes:
            raise ValueError("package is %d bytes; max %d" % (size, max_bytes))

    def build(self, validate: bool = True) -> bytes:
        if validate:
            self.validate()
        tree_len = len(self.tree_sec)
        offs = []
        o = HDR + tree_len
        for r in self.resources:
            offs.append(o)
            o += 8 + len(r.data)
        if offs == [r.offset for r in self.resources]:
            tree = self.tree_sec
        else:
            body = b"".join(_render_node(n, offs, self.resources) for n in self.tree)
            tree = bytes([0x20]) + struct.pack("<H", len(body)) + body
        res = b"".join(_resource_bytes(r) for r in self.resources)
        hdr = bytearray(self.header)
        struct.pack_into("<I", hdr, 0x18, len(tree) + len(res) + HDR)
        struct.pack_into("<I", hdr, 0x1C, len(res))
        struct.pack_into("<I", hdr, 0x20, raw_crc32(res))
        struct.pack_into("<I", hdr, 0x00, raw_crc32(bytes(hdr[4:HDR]) + tree))
        return bytes(hdr) + tree + res + bytes(hdr)

    def resource(self, index: int) -> Resource:
        return self.resources[index]

    def decode_rgba(self, index: int) -> tuple[int, int, bytes]:
        r = self.resources[index]
        return r.w, r.h, r.decode_rgba()

    def set_pixels(self, index: int, w: int, h: int, rgba: bytes,
                   cf: int | None = None) -> None:
        r = self.resources[index]
        cf = r.cf if cf is None else cf
        raw = encode_pixels(rgba, cf, w, h)
        r.cf, r.w, r.h = cf, w, h
        r.data = lz4.compress(raw)

    def set_png(self, index: int, blob: bytes, cf: int | None = None) -> None:
        w, h, rgba = png.decode_rgba(blob)
        self.set_pixels(index, w, h, rgba, cf=cf)

    def nodes(self):
        for n in self.tree:
            yield from n.walk()


def _resource_bytes(r: Resource) -> bytes:
    hdr = (r.cf & 0x1F) | ((r.w & 0x7FF) << 10) | ((r.h & 0x7FF) << 21)
    return struct.pack("<II", hdr, len(r.data)) + r.data


def _render_node(n: Node, offs: list[int], resources: list[Resource]) -> bytes:
    val = _patched_value(n, offs, resources)
    if n.children and n.tag not in LEAF and n.ref is None:
        plain = b"".join(bytes([c.tag]) + struct.pack("<H", len(c.value)) + c.value
                         for c in n.children)
        if plain == n.value:
            val = b"".join(_render_node(c, offs, resources) for c in n.children)
    return bytes([n.tag]) + struct.pack("<H", len(val)) + bytes(val)


def _patched_value(n: Node, offs: list[int], resources: list[Resource]) -> bytes:
    val = bytearray(n.value)
    if n.ref:
        p = n.ref.tail
        images = n.ref.images
        struct.pack_into("<I", val, p + 3, offs[images[0]])
        if n.ref.kind == 0x01:
            for k, i in enumerate(images[1:], 1):
                struct.pack_into("<I", val, p + 7 + 4 * (k - 1), offs[i])
        else:
            for k, i in enumerate(images):
                struct.pack_into("<H", val, p + 7 + 2 * k, len(resources[i].data) + 8)
    return bytes(val)


def parse(blob: bytes) -> Watchface:
    if len(blob) < 2 * HDR + 3:
        raise ValueError("file too small: %d bytes" % len(blob))
    if not (blob[4] == 1 and blob[5] == 0 and blob[6] == 0 and blob[7] in (0, 2)):
        raise ValueError("no watch-face magic in header")
    if blob[:HDR] != blob[-HDR:]:
        raise ValueError("footer differs from header")
    body = blob[HDR:-HDR]
    if body[0] != 0x20:
        raise ValueError("root tag 0x%02x, expected 0x20" % body[0])
    tree_len = _u16(body, 1)
    if 3 + tree_len > len(body):
        raise ValueError("tree length exceeds body")
    tree_sec = bytes(body[:3 + tree_len])
    tree = _parse_nodes(body[3:3 + tree_len], HDR + 3)

    resources = []
    res_by_off = {}
    off = 3 + tree_len
    while off < len(body):
        if off + 8 > len(body):
            raise ValueError("truncated resource at body+0x%x" % off)
        hdr = _u32(body, off)
        size = _u32(body, off + 4)
        if off + 8 + size > len(body):
            raise ValueError("resource at body+0x%x exceeds body" % off)
        res_by_off[HDR + off] = len(resources)
        resources.append(Resource(hdr & 0x1F, (hdr >> 10) & 0x7FF,
                                  (hdr >> 21) & 0x7FF,
                                  bytes(body[off + 8:off + 8 + size]),
                                  HDR + off))
        off += 8 + size
    if off != len(body):
        raise ValueError("resource section does not end at body end")

    for n in tree:
        for x in n.walk():
            if x.tag == 0x01 and len(x.value) >= 18:
                x.ref = _parse_ref(x.value, 18, res_by_off, resources)
            elif x.tag == 0x08 and len(x.value) >= 12:
                x.ref = _parse_ref(x.value, 5, res_by_off, resources)
    return Watchface(bytes(blob[:HDR]), tree, tree_sec, resources, blob)


def is_wfdl(blob: bytes) -> bool:
    try:
        parse(blob)
        return True
    except Exception:
        return False


def dial_id(seed: str) -> int:
    """Dial id the uploader derives from a name (FMC ``dialId``).

    The firmware reads ``new_wf_id`` as a signed 32-bit value, so the id must
    stay below 0x80000000; ids below 10000 are reserved for factory dials.
    Re-uploading a face with the same seed replaces itself instead of taking
    another slot.
    """
    h = 0x811C9DC5
    units = seed.encode("utf-16-le", "surrogatepass")
    for i in range(0, len(units), 2):
        h ^= units[i] | (units[i + 1] << 8)
        h = (h * 0x01000193) & 0xFFFFFFFF
    return 10000 + h % (0x7FFFFFFF - 10000)
