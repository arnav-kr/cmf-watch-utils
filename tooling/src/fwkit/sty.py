"""``.sty`` UI scene bundles (``bt_watch.sty``, ``logo.sty``).

Format verified against ``bt_watch.sty`` (5009 records, 181 scenes) and
``logo.sty``; ``.build()`` is byte-identical when nothing is edited
(see docs/style.md)::

  0x00  u32 scene_count
  0x04  u32 0
  0x08  scene table, 36 bytes each:
          +0x00 u32 type (0)
          +0x04 u32 canvas (u16 w | u16 h, e.g. 0x01F6019A = 410x502)
          +0x08 u32 0
          +0x0C u8 alpha (0xFF)
          +0x0D u8[3] 0
          +0x10 u32 group
          +0x14 u32 first item offset (relative to the node base)
          +0x18 u32 scene id (opaque 32-bit id)
          +0x1C u32 0x00010000
          +0x20 u32 0

The node base is ``0x08 + scene_count * 36``.  A scene's items are a
linked list (``scene+0x14``, ``scene+0x10`` entries) of **records**::

  +0x00 u32 type       1 = list/grid, 2 = text, 3 = container, 4 = image
  +0x04 u32 id
  +0x08 u32 next       offset of the next record, relative to the base
                       (0 = end of the chain / file)
  +0x0C ...            type-specific

A record's size is ``next - offset``; container records (types 1 and 3)
hold their children **inline** from ``+0x24``, and ``next`` jumps past the
whole subtree.  Children are chained with the same ``next`` field and the
last child points at the parent's ``next``.  Observed depth is 2.

  type 1 (list/grid, 6 records, size 0x24 + children)::

    +0x0C u16 x | u16 y
    +0x10 u16 w | u16 h
    +0x14 u16 gap | u16 columns
    +0x18 u32 child count
    +0x1C u32 first child offset (== self + 0x24)
    +0x20 u32 background colour (BGRA, e.g. 0xBC93BCB0)

  type 2 (text, 36 bytes)::

    +0x0C u32 text index into bt_watch.<lang>
    +0x10 u16 x | u16 y            (0,0 = auto)
    +0x14 u16 w | u16 h            (0,0 = auto)
    +0x18 u32 style/format (24 distinct values, not identified)
    +0x1C u32 colour 1 (0 or 0x00FFFFFF)
    +0x20 u32 colour 2 (0 or 0x00FFFFFF)

  type 3 (container/page, size 0x24 + children)::

    +0x0C u32 0
    +0x10 u16 canvas w | u16 canvas h
    +0x14 u32 0
    +0x18 u32 alpha (255)
    +0x1C u32 child count
    +0x20 u32 first child offset (== self + 0x24)

  type 4 (image, 40 bytes)::

    +0x0C u32 picture index (1-based into bt_watch.res)
    +0x10 u16 x | u16 y
    +0x14 u16 w | u16 h
    +0x18 u16 bytes/pixel | u16 format
    +0x1C u32 offset of the picture body inside bt_watch.res
    +0x20 u32 picture body length (0 for type-0x05 pictures; the
              runtime decoder seeks through the RES entry table)

``+0x1C``/``+0x20`` are a build-time mirror of the ``.res`` layout.  The
0.73 decoder (``FUN_101878e4``) does not read them - it seeks through the
RES pool - so they may go stale when ``.res`` is rebuilt.  Keep them in
sync anyway if you author files for other firmware.

Ids are **not** name hashes and there is no hash function in the app:
node ids often come in sequential runs (74 runs of 3+ in 0.73); scene
ids are opaque.  When authoring, pick unused 32-bit values and never
renumber ids that ``app.bin`` references (e.g. ``0x5D695BF2``).

Records are patched in place; sizes never change, so the bundle stays
byte-identical when nothing is edited.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field

SCENE_SIZE = 36
HDR_SIZE = 0x08

NODE_LIST = 1
NODE_TEXT = 2
NODE_CONTAINER = 3
NODE_IMAGE = 4

NODE_NAMES = {
    NODE_LIST: "list",
    NODE_TEXT: "text",
    NODE_CONTAINER: "container",
    NODE_IMAGE: "image",
}

CONTAINERS = (NODE_LIST, NODE_CONTAINER)

CHILD_COUNT_OFF = {NODE_LIST: 0x18, NODE_CONTAINER: 0x1C}


def canvas(w: int, h: int) -> int:
    return w | (h << 16)


def pack_xy(x: int, y: int) -> int:
    return (x & 0xFFFF) | (y << 16)


@dataclass
class Scene:
    raw: bytes
    index: int

    @property
    def type(self) -> int:
        return struct.unpack_from("<I", self.raw, 0)[0]

    @property
    def width(self) -> int:
        return struct.unpack_from("<H", self.raw, 4)[0]

    @property
    def height(self) -> int:
        return struct.unpack_from("<H", self.raw, 6)[0]

    @property
    def alpha(self) -> int:
        return self.raw[0x0C]

    @property
    def group(self) -> int:
        return struct.unpack_from("<I", self.raw, 0x10)[0]

    @property
    def count(self) -> int:
        """Number of top-level items."""
        return struct.unpack_from("<I", self.raw, 0x10)[0]

    @property
    def data_offset(self) -> int:
        return struct.unpack_from("<I", self.raw, 0x14)[0]

    @property
    def id(self) -> int:
        return struct.unpack_from("<I", self.raw, 0x18)[0]

    def __str__(self) -> str:
        return "scene %3d id=%08X %dx%d group=%d alpha=%d items=%d data=%s" % (
            self.index, self.id, self.width, self.height, self.group,
            self.alpha, self.count,
            hex(self.data_offset) if self.data_offset else "-")


@dataclass
class Node:
    off: int
    rel: int
    type: int
    id: int
    next: int
    raw: bytearray
    orig: bytes
    depth: int = 1
    parent: "Node | None" = None
    children: list["Node"] = field(default_factory=list)

    def u16(self, off: int) -> int:
        return struct.unpack_from("<H", self.raw, off)[0]

    def u32(self, off: int) -> int:
        return struct.unpack_from("<I", self.raw, off)[0]

    @property
    def name(self) -> str:
        return NODE_NAMES.get(self.type, "type%d" % self.type)

    @property
    def size(self) -> int:
        """Record size including any inline children."""
        return len(self.raw)

    @property
    def pic_index(self) -> int:
        if self.type != NODE_IMAGE:
            raise ValueError("not an image node")
        return self.u32(0x0C)

    @property
    def text_index(self) -> int:
        if self.type != NODE_TEXT:
            raise ValueError("not a text node")
        return self.u32(0x0C)

    @property
    def xy(self) -> tuple[int, int] | None:
        o = 0x0C if self.type == NODE_LIST else 0x10
        if self.type not in (NODE_LIST, NODE_TEXT, NODE_IMAGE):
            return None
        return self.u16(o), self.u16(o + 2)

    @property
    def wh(self) -> tuple[int, int]:
        o = 0x10 if self.type == NODE_LIST else 0x14
        return self.u16(o), self.u16(o + 2)

    @property
    def canvas(self) -> tuple[int, int] | None:
        if self.type != NODE_CONTAINER:
            return None
        return self.u16(0x10), self.u16(0x12)

    @property
    def res_offset(self) -> int:
        if self.type != NODE_IMAGE:
            raise ValueError("not an image node")
        return self.u32(0x1C)

    @property
    def res_length(self) -> int:
        if self.type != NODE_IMAGE:
            raise ValueError("not an image node")
        return self.u32(0x20)

    def geometry(self) -> tuple[int, int, int, int]:
        if self.type != NODE_IMAGE:
            raise ValueError("not an image node")
        return self.u16(0x14), self.u16(0x16), self.u16(0x18), self.u16(0x1A)

    def __str__(self) -> str:
        pad = "  " * (self.depth - 1)
        extra = ""
        if self.type == NODE_IMAGE:
            w, h, bpp, fmt = self.geometry()
            extra = "pic=%d %dx%d bpp=%d fmt=%d res=%#x+%#x" % (
                self.pic_index, w, h, bpp, fmt, self.res_offset, self.res_length)
        elif self.type == NODE_TEXT:
            extra = "text=%d" % self.text_index
            xy = self.xy
            if xy != (0, 0):
                extra += " xy=%d,%d" % xy
            wh = self.wh
            if wh != (0, 0):
                extra += " wh=%dx%d" % wh
        elif self.type == NODE_LIST:
            extra = "xy=%d,%d wh=%dx%d cell=%d/%d kids=%d" % (
                self.xy[0], self.xy[1], self.wh[0], self.wh[1],
                self.u16(0x14), self.u16(0x16), len(self.children))
        elif self.type == NODE_CONTAINER:
            w, h = self.canvas
            extra = "%dx%d alpha=%d kids=%d" % (
                w, h, self.u32(0x18), len(self.children))
        return "%snode @%06X %-9s id=%08X next=%#x %s" % (
            pad, self.off, self.name, self.id, self.next, extra)


@dataclass
class Sty:
    raw: bytes
    scenes: list[Scene]
    nodes: list[Node]
    top: list[Node]
    list_base: int
    by_id: dict[int, list[Node]] = field(default_factory=dict)

    def scene(self, key) -> Scene:
        if isinstance(key, int) and 0 <= key < len(self.scenes):
            return self.scenes[key]
        for s in self.scenes:
            if s.id == key:
                return s
        raise KeyError(key)

    def node(self, node_id: int) -> Node:
        return self.by_id[node_id][0]

    def scene_nodes(self, scene) -> list[Node]:
        """Top-level nodes of a scene (accepts a Scene or a key)."""
        if not isinstance(scene, Scene):
            scene = self.scene(scene)
        by_off = {n.off: n for n in self.nodes}
        out: list[Node] = []
        rel = scene.data_offset
        seen = set()
        for _ in range(scene.count):
            n = by_off.get(self.list_base + rel)
            if n is None or rel in seen:
                break
            seen.add(rel)
            out.append(n)
            rel = n.next
        return out

    def build(self) -> bytes:
        """Re-emit the original bundle with the (possibly patched) nodes.

        Only bytes that differ from the parsed original are written, so
        overlapping node records cannot be clobbered by untouched fields.
        """
        out = bytearray(self.raw)
        for n in self.nodes:
            stop = min(n.off + len(n.raw), len(out))
            for i in range(stop - n.off):
                if n.raw[i] != n.orig[i]:
                    out[n.off + i] = n.raw[i]
        return bytes(out)


def is_sty(blob: bytes) -> bool:
    if len(blob) < HDR_SIZE:
        return False
    count = struct.unpack_from("<I", blob, 0)[0]
    return 1 <= count < 4096 and (HDR_SIZE + count * SCENE_SIZE) < len(blob)


def _record_size(blob: bytes, base: int, rel: int) -> int:
    """Size of a record plus its inline children (independent of ``next``)."""
    ty, _, nxt = struct.unpack_from("<III", blob, base + rel)
    if ty in CONTAINERS:
        cnt_off = CHILD_COUNT_OFF[ty]
        cnt = struct.unpack_from("<I", blob, base + rel + cnt_off)[0]
        child = rel + 0x24
        total = 0x24
        for _ in range(cnt):
            size = _record_size(blob, base, child)
            total += size
            child += size
        return total
    if ty == NODE_TEXT:
        return 0x24
    if ty == NODE_IMAGE:
        return 0x28
    return (nxt - rel) if nxt else 0x24


def parse(blob: bytes, limit: int = 100000) -> Sty:
    if not is_sty(blob):
        raise ValueError("not a .sty bundle")
    count = struct.unpack_from("<I", blob, 0)[0]
    scenes = []
    for i in range(count):
        o = HDR_SIZE + i * SCENE_SIZE
        scenes.append(Scene(blob[o:o + SCENE_SIZE], i))
    base = HDR_SIZE + count * SCENE_SIZE
    nodes: list[Node] = []
    by_rel: dict[int, Node] = {}

    def read_node(rel: int, depth: int, parent: Node | None) -> Node:
        old = by_rel.get(rel)
        if old is not None:
            return old
        if rel < 0 or base + rel + 12 > len(blob):
            raise ValueError("node offset %#x out of range" % rel)
        ty, nid, nxt = struct.unpack_from("<III", blob, base + rel)
        size = _record_size(blob, base, rel)
        if size < 12 or base + rel + size > len(blob):
            raise ValueError("node at %#x has bad size %#x" % (rel, size))
        rec = bytearray(blob[base + rel:base + rel + size])
        n = Node(base + rel, rel, ty, nid, nxt, rec, bytes(rec),
                 depth=depth, parent=parent)
        by_rel[rel] = n
        nodes.append(n)
        if ty in CONTAINERS:
            cnt_off = CHILD_COUNT_OFF[ty]
            cnt = struct.unpack_from("<I", rec, cnt_off)[0]
            child = rel + 0x24
            for _ in range(cnt):
                c = read_node(child, depth + 1, n)
                n.children.append(c)
                child += len(c.raw)
        return n

    for sc in scenes:
        rel = sc.data_offset
        for _ in range(sc.count):
            n = read_node(rel, 1, None)
            rel = n.next
            if not rel:
                break

    top: list[Node] = []
    rel: int | None = 0
    while rel is not None and len(top) < limit:
        try:
            n = read_node(rel, 1, None)
        except ValueError:
            break
        top.append(n)
        rel = n.next or None

    nodes.sort(key=lambda n: n.off)
    by_id: dict[int, list[Node]] = {}
    for n in nodes:
        by_id.setdefault(n.id, []).append(n)
    return Sty(blob, scenes, nodes, top, base, by_id)


def _walk(node: Node):
    yield node
    for c in node.children:
        yield from _walk(c)


def _new_ids(nodes) -> dict[int, int]:
    """Map each node object to a fresh 32-bit id not used in the bundle."""
    used: set[int] = set()
    for n in nodes:
        used.add(n.id)
    nxt = (max(used) + 1) if used else 1
    ids: dict[int, int] = {}
    for n in nodes:
        key = id(n)
        if key in ids:
            continue
        while nxt in used:
            nxt += 1
        if nxt > 0xFFFFFFFF:
            raise ValueError("no free node ids")
        ids[key] = nxt
        used.add(nxt)
        nxt += 1
    return ids


def _emit(node: Node, rel: int, next_rel: int, start: int = 0,
          id_map: dict[int, int] | None = None) -> bytes:
    """Serialize one record (and children) with offsets relative to the base.

    ``rel``/``next_rel`` are clone-local; ``start`` is where the clone block
    begins relative to the file's node base.  All stored offsets must be
    absolute (base-relative), so ``start`` is added to each one.  ``id_map``
    optionally replaces node ids (keyed by object identity).
    """
    nxt = (start + next_rel) if next_rel else 0
    if node.type in CONTAINERS:
        out = bytearray(node.raw[:0x24])
        struct.pack_into("<I", out, 0x08, nxt)
        struct.pack_into("<I", out, 0x1C if node.type == NODE_LIST else 0x20,
                         start + rel + 0x24)
        child_rel = rel + 0x24
        last = len(node.children) - 1
        for i, c in enumerate(node.children):
            cnext = child_rel + len(c.raw) if i < last else next_rel
            out += _emit(c, child_rel, cnext, start, id_map)
            child_rel += len(c.raw)
    else:
        out = bytearray(node.raw)
        struct.pack_into("<I", out, 0x08, nxt)
    if id_map is not None:
        struct.pack_into("<I", out, 0x04, id_map[id(node)])
    if len(out) != len(node.raw):
        raise ValueError("record size changed at %#x" % node.rel)
    return bytes(out)


def add_scene(blob: bytes, src, new_id: int, index: int | None = None,
              force: bool = False, fresh_ids: bool = False) -> bytes:
    """Append a copy of scene ``src`` (id or index) with a new scene id.

    The node base moves by 36 bytes when the table grows, but every offset
    in a ``.sty`` is relative to that base, so the existing node block can
    be shifted verbatim.  The copied subtree is appended after it and its
    ``next``/child offsets are re-emitted relative to the new base.

    With ``fresh_ids=True`` every cloned node (including nested children)
    also gets a new id that does not occur anywhere in the bundle.
    """
    s = parse(blob)
    if not force and any(sc.id == new_id for sc in s.scenes):
        raise ValueError("scene id %08X already exists" % new_id)
    sc = s.scene(src)
    top = s.scene_nodes(sc)
    id_map = _new_ids([n for t in top for n in _walk(t)]) if fresh_ids else None
    old_block = len(blob) - s.list_base
    block = bytearray()
    rel = 0
    last = len(top) - 1
    for i, n in enumerate(top):
        nxt = rel + len(n.raw) if i < last else 0
        block += _emit(n, rel, nxt, old_block, id_map)
        rel += len(n.raw)
    rec = bytearray(sc.raw)
    struct.pack_into("<I", rec, 0x10, len(top))
    struct.pack_into("<I", rec, 0x14, old_block)
    struct.pack_into("<I", rec, 0x18, new_id)
    count = len(s.scenes)
    pos = count if index is None else index
    if not 0 <= pos <= count:
        raise ValueError("scene index %d out of range" % pos)
    table = bytearray(blob[HDR_SIZE:HDR_SIZE + count * SCENE_SIZE])
    table[pos * SCENE_SIZE:pos * SCENE_SIZE] = rec
    head = bytearray(blob[:HDR_SIZE])
    struct.pack_into("<I", head, 0, count + 1)
    return bytes(head) + bytes(table) + blob[s.list_base:] + bytes(block)


def _emit_scene(s: Sty, sc: Scene, start: int = 0) -> tuple[bytes, int]:
    top = s.scene_nodes(sc)
    out = bytearray()
    rel = 0
    last = len(top) - 1
    for i, n in enumerate(top):
        nxt = rel + len(n.raw) if i < last else 0
        out += _emit(n, rel, nxt, start)
        rel += len(n.raw)
    return bytes(out), len(top)


def _rebuild(s: Sty, scenes: list[Scene]) -> bytes:
    """Re-emit only ``scenes``, dropping unreachable node records.

    Shared records are copied once per scene rather than deduplicated.  The
    result is valid but may not reach the theoretical minimum size.
    """
    if not scenes:
        raise ValueError("a .sty bundle must keep at least one scene")
    table = bytearray()
    block = bytearray()
    for sc in scenes:
        data, count = _emit_scene(s, sc, len(block))
        rec = bytearray(sc.raw)
        struct.pack_into("<I", rec, 0x10, count)
        struct.pack_into("<I", rec, 0x14, len(block))
        table += rec
        block += data
    head = bytearray(HDR_SIZE)
    struct.pack_into("<I", head, 0, len(scenes))
    out = bytes(head) + bytes(table) + bytes(block)
    check = parse(out)
    if len(check.scenes) != len(scenes):
        raise ValueError("compaction lost scenes")
    return out


def compact(blob: bytes) -> bytes:
    """Drop node records that are not reachable from any scene."""
    s = parse(blob)
    return _rebuild(s, s.scenes)


def remove_scene(blob: bytes, target, compact_nodes: bool = True) -> bytes:
    """Remove scene ``target`` (index or id) and rebuild the bundle.

    With ``compact_nodes=False`` only the 36-byte table entry is removed and
    the node block is shifted down verbatim, leaving orphaned records behind.
    """
    s = parse(blob)
    sc = s.scene(target)
    keep = [x for x in s.scenes if x.index != sc.index]
    if not keep:
        raise ValueError("cannot remove the last scene")
    if compact_nodes:
        return _rebuild(s, keep)
    head = bytearray(blob[:HDR_SIZE])
    struct.pack_into("<I", head, 0, len(keep))
    table = b"".join(x.raw for x in keep)
    out = bytes(head) + table + blob[s.list_base:]
    check = parse(out)
    if len(check.scenes) != len(keep):
        raise ValueError("scene delete lost scenes")
    return out


def set_pic_index(blob: bytes, node_id: int, pic_index: int,
                  from_pic: int | None = None, all_matches: bool = False) -> bytes:
    """Point image node(s) with this id at a 1-based ``bt_watch.res`` index.

    The same id is reused for different variants of an icon, so pass
    ``from_pic`` to retarget only the nodes currently showing that picture,
    or ``all_matches=True`` to retarget every node with this id.  Nested
    nodes inside list/container records are included.
    """
    res = parse(blob)
    candidates = res.by_id[node_id]
    if from_pic is not None:
        candidates = [n for n in candidates
                      if n.type == NODE_IMAGE and n.pic_index == from_pic]
        if not candidates:
            raise ValueError("no image node %08X shows PIC%d" % (node_id, from_pic))
    elif not all_matches:
        candidates = candidates[:1]
    for n in candidates:
        if n.type != NODE_IMAGE:
            raise ValueError("node %08X is not an image" % node_id)
        struct.pack_into("<I", n.raw, 0x0C, pic_index)
    return res.build()
