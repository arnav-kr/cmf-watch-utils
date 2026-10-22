"""fwkit command line interface."""

from __future__ import annotations

import argparse
import os
import struct
import sys

from . import ajs, aota, sdfs, res, act, sty, picres, png, wfdl
from .image import Image, verify as verify_image


def _key(s):
    """PIC keys: bare digits are 1-based picture indices, else names."""
    return int(s) if s.isdigit() else s


def cmd_info(args):
    img = Image.open(args.image)
    print("file      : %s (%d bytes)" % (args.image, len(img.data)))
    print("header crc: %s" % ("ok" if aota.verify_header(img.header) else "BAD"))
    print("version   : %r" % img.header[0x40:0x50].split(b"\0")[0].decode("ascii", "replace"))
    print("board     : %r" % img.header[0x60:0x70].split(b"\0")[0].decode("ascii", "replace"))
    print("agps off  : 0x%x" % aota.agps_offset(img.header))
    print("entries   : %d" % len(img.entries))
    print("%-14s %10s %10s %10s  %s" % ("name", "offset", "stored", "raw", "crc"))
    for e in img.entries:
        raw = "-"
        if e.name != "ota.xml":
            try:
                raw = str(len(aota.unpack_chunks(e.stored(img.data))))
            except Exception as ex:
                raw = "ERR:%s" % ex
        print("%-14s %10d %10d %10s  %s" % (e.name, e.off, e.size, raw,
                                           "ok" if aota.verify_entry(img.data, e) else "BAD"))


def cmd_unpack(args):
    img = Image.open(args.image)
    os.makedirs(args.outdir, exist_ok=True)
    with open(os.path.join(args.outdir, "hdr.bin"), "wb") as f:
        f.write(img.header)
    with open(os.path.join(args.outdir, "ota.xml"), "wb") as f:
        f.write(img.xml)
    agps = img.data[aota.agps_offset(img.header):]
    with open(os.path.join(args.outdir, "agps.bin"), "wb") as f:
        f.write(agps)
    pdir = os.path.join(args.outdir, "parts")
    rdir = os.path.join(args.outdir, "raw")
    os.makedirs(pdir, exist_ok=True)
    os.makedirs(rdir, exist_ok=True)
    for e in img.entries:
        if e.name == "ota.xml":
            continue
        blob = e.stored(img.data)
        with open(os.path.join(pdir, e.name + ".part"), "wb") as f:
            f.write(blob)
        if not args.no_raw:
            raw = aota.unpack_chunks(blob)
            with open(os.path.join(rdir, e.name + ".bin"), "wb") as f:
                f.write(raw)
    print("unpacked %d entries -> %s" % (len(img.entries), args.outdir))


def cmd_pack(args):
    with open(os.path.join(args.dir, "hdr.bin"), "rb") as f:
        hdr = bytearray(f.read())
    with open(os.path.join(args.dir, "ota.xml"), "rb") as f:
        xml = f.read()
    agps_path = os.path.join(args.dir, "agps.bin")
    agps = open(agps_path, "rb").read() if os.path.exists(agps_path) else b""
    cnt = struct.unpack_from("<I", hdr, 0x0C)[0]
    names = []
    for i in range(cnt):
        nm = hdr[0x200 + i * 32:0x200 + i * 32 + 16].split(b"\0")[0].decode()
        if nm != "ota.xml":
            names.append(nm)
    parts = []
    for nm in names:
        with open(os.path.join(args.dir, "parts", nm + ".part"), "rb") as f:
            parts.append((nm, f.read()))
    out = aota.build(hdr, xml, parts, agps)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d bytes)" % (args.out, len(out)))


def cmd_sdfs_list(args):
    data = open(args.file, "rb").read()
    ents = sdfs.parse(data)
    print("%-14s %10s %10s  %s" % ("name", "offset", "size", "hash"))
    for e in ents:
        print("%-14s %10d %10d  %s" % (e.name, e.off, e.size,
                                       "ok" if sdfs.verify(data, e) else "BAD"))


def cmd_sdfs_extract(args):
    data = open(args.file, "rb").read()
    os.makedirs(args.outdir, exist_ok=True)
    for e in sdfs.parse(data):
        with open(os.path.join(args.outdir, e.name), "wb") as f:
            f.write(e.data(data))
    print("extracted -> %s" % args.outdir)


def cmd_sdfs_pack(args):
    files = []
    for name in sorted(os.listdir(args.dir)):
        p = os.path.join(args.dir, name)
        if os.path.isfile(p):
            files.append((name, open(p, "rb").read()))
    out = sdfs.pack(files)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d bytes, %d files)" % (args.out, len(out), len(files)))


def cmd_res_list(args):
    data = open(args.file, "rb").read()
    r = res.parse(data)
    print("%-10s %6s  %s" % ("name", "bytes", "text"))
    for e in r.entries:
        print("%-10s %6d  %s" % (e.name, len(e.text), str(e)[:80]))


def cmd_res_set(args):
    data = open(args.file, "rb").read()
    out = res.set_string(data, args.key, args.value)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d -> %d bytes)" % (args.out, len(data), len(out)))


def cmd_edit_sdfs(args):
    img = Image.open(args.image)
    data = open(args.input, "rb").read() if args.input != "-" else sys.stdin.buffer.read()
    img.edit_sdfs(args.partition, args.name, data)
    size = img.save(args.out)
    print("wrote %s (%d bytes)" % (args.out, size))


def cmd_edit_res(args):
    img = Image.open(args.image)
    img.edit_res_string(args.partition, args.resfile, args.key, args.value)
    size = img.save(args.out)
    print("wrote %s (%d bytes)" % (args.out, size))


def cmd_edit_app(args):
    img = Image.open(args.image)
    patch = bytes.fromhex(args.hex)
    img.edit_app(int(args.addr, 0), patch)
    size = img.save(args.out)
    print("wrote %s (%d bytes, patched %d byte(s) @ %s)" % (args.out, size, len(patch), args.addr))


def cmd_app_scene_refs(args):
    a = act.parse(open(args.file, "rb").read())
    value = int(args.scene_id, 0)
    offsets = a.find_u32(value, start=0x200)
    print("%#010x: %d literal(s)" % (value, len(offsets)))
    for off in offsets:
        print("  file=0x%06x addr=0x%08x" % (off, a.addr(off)))


def cmd_app_patch_scene(args):
    a = act.parse(open(args.file, "rb").read())
    old, new = int(args.old, 0), int(args.new, 0)
    offsets = ([int(x, 0) for x in args.offset] if args.offset
               else a.find_u32(old, start=0x200))
    try:
        out = a.patch_u32(old, new, offsets)
    except ValueError as e:
        raise SystemExit(str(e))
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d -> %d bytes, patched %d literal(s))"
          % (args.out, len(a.data), len(out), len(offsets)))


def cmd_act_info(args):
    a = act.parse(open(args.file, "rb").read())
    print("entry     : 0x%08x" % a.entry)
    print("load base : 0x%08x" % a.load_base)
    print("payload   : 0x%x" % a.payload_size)
    print("field_0c  : 0x%x" % a.field_0c)
    print("field_18  : 0x%x" % a.field_18)
    print("crc? 0x1c : 0x%08x" % a.field_1c)
    print("crc? 0x20 : 0x%08x" % a.field_20)
    print("hdr size  : 0x%x" % a.header_size)


def cmd_verify(args):
    print("ok" if verify_image(args.image) else "BAD")


def cmd_sty_info(args):
    s = sty.parse(open(args.file, "rb").read())
    nested = len(s.nodes) - len(s.top)
    print("file       : %s (%d bytes)" % (args.file, len(s.raw)))
    print("scenes     : %d" % len(s.scenes))
    print("records    : %d (%d top-level + %d nested, %d unique ids)"
          % (len(s.nodes), len(s.top), nested, len(s.by_id)))
    print("list base  : 0x%x" % s.list_base)
    counts = {}
    for n in s.nodes:
        counts[n.type] = counts.get(n.type, 0) + 1
    print("node types : %s" % ", ".join(
        "%s:%d" % (sty.NODE_NAMES.get(k, k), v) for k, v in sorted(counts.items())))
    if not args.scenes:
        return
    for sc in s.scenes:
        print("  " + str(sc))


def cmd_sty_nodes(args):
    s = sty.parse(open(args.file, "rb").read())
    for n in s.nodes:
        if args.type is not None and n.type != args.type:
            continue
        print(n)


def cmd_sty_tree(args):
    s = sty.parse(open(args.file, "rb").read())
    sc = s.scene(int(args.scene, 0))
    print(sc)

    def dump(n, depth):
        print("  " * depth + str(n))
        for c in n.children:
            dump(c, depth + 1)

    for n in s.scene_nodes(sc):
        dump(n, 1)


def cmd_ajs_info(args):
    a = ajs.parse(open(args.file, "rb").read())
    print("file      : %s (%d bytes)" % (args.file, len(a.raw)))
    print("fps       : %d (prelude %s)" % (a.fps, a.raw[:8].hex()))
    print("frames    : %d" % len(a.frames))
    print("canvas    : %dx%d" % (a.width, a.height))
    f = a.frames[0]
    print("tile grid : %dx%d (%d x %d tiles)"
          % (f.tile_width, f.tile_height, f.columns, f.rows))
    print("first     : offset=0x%x len=%d (+0x1A=%d)"
          % (f.offset, f.length, f.length))
    if args.frames:
        for fr in a.frames:
            print("  %4d  @0x%08x  %d B  tiles=%d"
                  % (fr.index, fr.offset, fr.length, fr.tile_count))


def cmd_ajs_extract(args):
    blob = open(args.file, "rb").read()
    paths = ajs.extract(blob, args.outdir)
    print("wrote %d frame(s) to %s" % (len(paths), args.outdir))


def cmd_ajs_repack(args):
    from pathlib import Path
    names = sorted(Path(args.dir).glob("*.jpg"))
    names += sorted(Path(args.dir).glob("*.jpeg"))
    if not names:
        raise SystemExit("no .jpg frames in %s" % args.dir)
    frames = [n.read_bytes() for n in names]
    out = ajs.build(frames, fps=args.fps, width=args.width, height=args.height,
                    tile_width=args.tile_width, tile_height=args.tile_height)
    with open(args.out, "wb") as f:
        f.write(out)
    a = ajs.parse(out)
    print("wrote %s (%d bytes, %d frames, %dx%d, %d fps)"
          % (args.out, len(out), len(a.frames), a.width, a.height, a.fps))


def cmd_ajs_set_frame(args):
    blob = open(args.file, "rb").read()
    out = ajs.set_frame(blob, args.index, open(args.image, "rb").read())
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d bytes)" % (args.out, len(out)))


def cmd_sty_add_scene(args):
    blob = open(args.file, "rb").read()
    src = int(args.src, 0)
    out = sty.add_scene(blob, src, int(args.new_id, 0), index=args.index,
                        force=args.force, fresh_ids=args.fresh_node_ids)
    with open(args.out, "wb") as f:
        f.write(out)
    s = sty.parse(out)
    sc = s.scenes[args.index if args.index is not None else len(s.scenes) - 1]
    print("wrote %s (%d bytes)" % (args.out, len(out)))
    print("scene %d id=0x%08X items=%d canvas=%dx%d"
          % (sc.index, sc.id, sc.count, sc.width, sc.height))


def cmd_sty_del_scene(args):
    blob = open(args.file, "rb").read()
    out = sty.remove_scene(blob, int(args.scene, 0),
                           compact_nodes=not args.keep_orphans)
    with open(args.out, "wb") as f:
        f.write(out)
    s = sty.parse(out)
    print("wrote %s (%d bytes, %d scenes, %d records)"
          % (args.out, len(out), len(s.scenes), len(s.nodes)))


def cmd_sty_set_pic(args):
    blob = open(args.file, "rb").read()
    out = sty.set_pic_index(blob, int(args.node_id, 0), int(args.pic_index),
                            from_pic=args.from_pic, all_matches=args.all)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d bytes)" % (args.out, len(out)))


def cmd_pic_list(args):
    r = picres.parse(open(args.file, "rb").read())
    print("%-9s %5s %5s %5s %8s" % ("name", "type", "w", "h", "bytes"))
    for p in r.pics:
        print("%-9s %#5x %5d %5d %8d" % (p.name, p.type, p.w, p.h, p.size))


def cmd_pic_decode(args):
    r = picres.parse(open(args.file, "rb").read())
    p = r.get(_key(args.key))
    rgb = picres.decode(p)
    with open(args.out, "wb") as f:
        f.write(png.encode(p.w, p.h, rgb))
    print("wrote %s (%dx%d RGB888)" % (args.out, p.w, p.h))


def cmd_pic_set(args):
    blob = open(args.file, "rb").read()
    w, h, rgb = png.decode(open(args.input, "rb").read())
    out = picres.set_picture(blob, _key(args.key), w, h, rgb)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d -> %d bytes, %dx%d)" % (args.out, len(blob), len(out), w, h))


def cmd_edit_sty_pic(args):
    img = Image.open(args.image)
    img.edit_sty_pic(args.partition, args.styfile, int(args.node_id, 0),
                     int(args.pic_index), from_pic=args.from_pic, all_matches=args.all)
    size = img.save(args.out)
    print("wrote %s (%d bytes)" % (args.out, size))


def cmd_edit_pic(args):
    img = Image.open(args.image)
    w, h, rgb = png.decode(open(args.input, "rb").read())
    img.edit_picture(args.partition, args.resfile, _key(args.key), w, h, rgb)
    size = img.save(args.out)
    print("wrote %s (%d bytes, %dx%d)" % (args.out, size, w, h))


def cmd_wf_info(args):
    blob = open(args.file, "rb").read()
    w = wfdl.parse(blob)
    tree_len = struct.unpack_from("<H", blob, 0x25)[0]
    print("file      : %s (%d bytes)" % (args.file, len(blob)))
    print("name      : %r" % w.name)
    print("hdr crc   : %s" % ("ok" if struct.unpack_from("<I", blob, 0)[0]
                              == wfdl.raw_crc32(blob[4:0x27 + tree_len]) else "BAD"))
    print("res crc   : %s" % ("ok" if struct.unpack_from("<I", blob, 0x20)[0]
                              == wfdl.raw_crc32(blob[-36 - struct.unpack_from("<I", blob, 0x1C)[0]:-36])
                              else "BAD"))
    print("footer    : %s" % ("ok" if blob[:36] == blob[-36:] else "BAD"))
    print("tree      : %d nodes, %d bytes" % (len(list(w.nodes())), len(w.tree_sec)))
    print("resources : %d" % len(w.resources))
    counts = {}
    for r in w.resources:
        counts[r.cf] = counts.get(r.cf, 0) + 1
    print("cf        : %s" % ", ".join("%d(%s)x%d" % (k, wfdl.CF_NAMES.get(k, "?"), v)
                                       for k, v in sorted(counts.items())))
    if args.tree:
        _print_tree(w)


def _print_tree(w, depth=3):
    def rec(n, d):
        if d > depth:
            return
        print("%s%s len=%d @0x%x%s" % ("  " * d, n.name, len(n.value), n.offset,
                                       "" if not n.ref else " ref=%#x%s" % (n.ref.kind, n.ref.images)))
        for c in n.children or ():
            rec(c, d + 1)
    for n in w.tree:
        rec(n, 0)


def cmd_wf_res(args):
    w = wfdl.parse(open(args.file, "rb").read())
    print("%4s %-10s %5s %5s %9s %9s  %s" % ("idx", "format", "w", "h", "raw", "stored", "offset"))
    for i, r in enumerate(w.resources):
        raw = "-" if r.cf in (1, 28) else str(r.raw_size)
        print("%4d %-10s %5d %5d %9s %9d  0x%x" % (i, r.name, r.w, r.h, raw, len(r.data), r.offset))


def cmd_wf_tree(args):
    _print_tree(wfdl.parse(open(args.file, "rb").read()), args.depth)


def cmd_wf_unpack(args):
    blob = open(args.file, "rb").read()
    w = wfdl.parse(blob)
    os.makedirs(args.outdir, exist_ok=True)
    with open(os.path.join(args.outdir, "header.bin"), "wb") as f:
        f.write(w.header)
    with open(os.path.join(args.outdir, "tree.txt"), "w", encoding="utf-8") as f:
        def rec(n, d):
            f.write("%s%s len=%d @0x%x%s\n" % ("  " * d, n.name, len(n.value), n.offset,
                                               "" if not n.ref else " ref=%#x%s" % (n.ref.kind, n.ref.images)))
            for c in n.children or ():
                rec(c, d + 1)
        for n in w.tree:
            rec(n, 0)
    rdir = os.path.join(args.outdir, "resources")
    os.makedirs(rdir, exist_ok=True)
    for i, r in enumerate(w.resources):
        base = "%03d_%s_%dx%d" % (i, r.name.replace("+", ""), r.w, r.h)
        with open(os.path.join(rdir, base + ".lz4"), "wb") as f:
            f.write(r.data)
        if not args.no_png and r.cf not in (1, 28):
            rw, rh, rgba = w.decode_rgba(i)
            with open(os.path.join(rdir, base + ".png"), "wb") as f:
                f.write(png.encode(rw, rh, rgba, alpha=True))
    print("unpacked %d resources -> %s" % (len(w.resources), args.outdir))


def _build_wf(w):
    try:
        return w.build()
    except ValueError as e:
        raise SystemExit(str(e))


def cmd_wf_set_res(args):
    blob = open(args.file, "rb").read()
    w = wfdl.parse(blob)
    w.set_png(int(args.index), open(args.input, "rb").read(), cf=args.cf)
    r = w.resources[int(args.index)]
    out = _build_wf(w)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d -> %d bytes, res %s %dx%d)" % (args.out, len(blob), len(out),
                                                       r.name, r.w, r.h))


def cmd_wf_build(args):
    base = args.base
    if os.path.isdir(base):
        base = os.path.join(base, "base.bin")
    blob = open(base, "rb").read()
    w = wfdl.parse(blob)
    if args.name is not None:
        nm = args.name.encode("utf-8")[:15]
        hdr = bytearray(w.header)
        hdr[8:0x17] = nm + b"\0" * (15 - len(nm))
        w.header = bytes(hdr)
    replaced = 0
    if args.resources and os.path.isdir(args.resources):
        import re
        for fn in sorted(os.listdir(args.resources)):
            m = re.match(r"^(\d+)", fn)
            if not m or not fn.lower().endswith(".png"):
                continue
            idx = int(m.group(1))
            w.set_png(idx, open(os.path.join(args.resources, fn), "rb").read())
            replaced += 1
    out = _build_wf(w)
    with open(args.out, "wb") as f:
        f.write(out)
    print("wrote %s (%d -> %d bytes, name %r, %d resource(s) replaced)"
          % (args.out, len(blob), len(out), w.name, replaced))


def main(argv=None):
    p = argparse.ArgumentParser(prog="fwkit", description="CMF Watch Pro 2 firmware toolkit")
    sub = p.add_subparsers(dest="cmd", required=True)

    s = sub.add_parser("info", help="show AOTA image info")
    s.add_argument("image")
    s.set_defaults(func=cmd_info)

    s = sub.add_parser("verify", help="verify all CRCs of an AOTA image")
    s.add_argument("image")
    s.set_defaults(func=cmd_verify)

    s = sub.add_parser("unpack", help="extract header/xml/agps/partitions")
    s.add_argument("image")
    s.add_argument("outdir")
    s.add_argument("--no-raw", action="store_true", help="do not decompress partitions")
    s.set_defaults(func=cmd_unpack)

    s = sub.add_parser("pack", help="rebuild an image from an unpacked directory")
    s.add_argument("dir")
    s.add_argument("out")
    s.set_defaults(func=cmd_pack)

    s = sub.add_parser("sdfs-list", help="list an SDFS image")
    s.add_argument("file")
    s.set_defaults(func=cmd_sdfs_list)

    s = sub.add_parser("sdfs-extract", help="extract an SDFS image")
    s.add_argument("file")
    s.add_argument("outdir")
    s.set_defaults(func=cmd_sdfs_extract)

    s = sub.add_parser("sdfs-pack", help="build an SDFS image from a directory")
    s.add_argument("dir")
    s.add_argument("out")
    s.set_defaults(func=cmd_sdfs_pack)

    s = sub.add_parser("res-list", help="list strings in a RES table")
    s.add_argument("file")
    s.set_defaults(func=cmd_res_list)

    s = sub.add_parser("res-set", help="set one string in a RES table")
    s.add_argument("file")
    s.add_argument("key")
    s.add_argument("value")
    s.add_argument("-o", "--out", required=True)
    s.set_defaults(func=cmd_res_set)

    s = sub.add_parser("edit-sdfs", help="replace a file inside an AOTA partition")
    s.add_argument("image")
    s.add_argument("partition")
    s.add_argument("name")
    s.add_argument("input", help="new file contents ('-' for stdin)")
    s.add_argument("out")
    s.set_defaults(func=cmd_edit_sdfs)

    s = sub.add_parser("edit-res", help="set a RES string inside an AOTA partition")
    s.add_argument("image")
    s.add_argument("partition")
    s.add_argument("resfile")
    s.add_argument("key")
    s.add_argument("value")
    s.add_argument("out")
    s.set_defaults(func=cmd_edit_res)

    s = sub.add_parser("edit-app", help="patch app.bin inside TEMP and rebuild")
    s.add_argument("image")
    s.add_argument("addr")
    s.add_argument("hex")
    s.add_argument("out")
    s.set_defaults(func=cmd_edit_app)

    s = sub.add_parser("act-info", help="show app.bin (ACTH) header")
    s.add_argument("file")
    s.set_defaults(func=cmd_act_info)

    s = sub.add_parser("app-scene-refs",
                       help="find scene-id literals in app.bin")
    s.add_argument("file")
    s.add_argument("scene_id")
    s.set_defaults(func=cmd_app_scene_refs)

    s = sub.add_parser("app-patch-scene",
                       help="retarget scene-id literal(s) in app.bin")
    s.add_argument("file")
    s.add_argument("old")
    s.add_argument("new")
    s.add_argument("-o", "--out", required=True)
    s.add_argument("--offset", action="append",
                   help="patch only this file offset (repeatable)")
    s.set_defaults(func=cmd_app_patch_scene)

    s = sub.add_parser("sty-info", help="show a .sty scene bundle")
    s.add_argument("file")
    s.add_argument("--scenes", action="store_true", help="list every scene")
    s.set_defaults(func=cmd_sty_info)

    s = sub.add_parser("sty-nodes", help="list nodes of a .sty bundle")
    s.add_argument("file")
    s.add_argument("--type", type=int, help="only this node type")
    s.set_defaults(func=cmd_sty_nodes)

    s = sub.add_parser("sty-tree", help="print the node tree of one scene")
    s.add_argument("file")
    s.add_argument("scene", help="scene index or 32-bit id (0x...)")
    s.set_defaults(func=cmd_sty_tree)

    s = sub.add_parser("ajs-info", help="show a .ajs JPEG animation")
    s.add_argument("file")
    s.add_argument("--frames", action="store_true", help="list every frame")
    s.set_defaults(func=cmd_ajs_info)

    s = sub.add_parser("ajs-extract", help="extract every .ajs frame to JPEG")
    s.add_argument("file")
    s.add_argument("outdir")
    s.set_defaults(func=cmd_ajs_extract)

    s = sub.add_parser("ajs-repack", help="build a .ajs from a directory of JPEGs")
    s.add_argument("dir")
    s.add_argument("-o", "--out", required=True)
    s.add_argument("--fps", type=int, default=25)
    s.add_argument("--width", type=int)
    s.add_argument("--height", type=int)
    s.add_argument("--tile-width", type=int)
    s.add_argument("--tile-height", type=int)
    s.set_defaults(func=cmd_ajs_repack)

    s = sub.add_parser("ajs-set-frame", help="replace one frame in a .ajs")
    s.add_argument("file")
    s.add_argument("index", type=int)
    s.add_argument("image")
    s.add_argument("-o", "--out", required=True)
    s.set_defaults(func=cmd_ajs_set_frame)

    s = sub.add_parser("sty-add-scene",
                       help="append a copy of a scene (rebases its offsets)")
    s.add_argument("file")
    s.add_argument("src", help="source scene index or 32-bit id (0x...)")
    s.add_argument("new_id", help="id for the new scene (0x...)")
    s.add_argument("-o", "--out", required=True)
    s.add_argument("--index", type=int, help="insert at this table position (default: append)")
    s.add_argument("--force", action="store_true", help="allow a duplicate scene id")
    s.add_argument("--fresh-node-ids", action="store_true",
                   help="give every cloned node a new unused id")
    s.set_defaults(func=cmd_sty_add_scene)

    s = sub.add_parser("sty-del-scene",
                       help="remove a scene and compact its node records")
    s.add_argument("file")
    s.add_argument("scene", help="scene index or 32-bit id (0x...)")
    s.add_argument("-o", "--out", required=True)
    s.add_argument("--keep-orphans", action="store_true",
                   help="only drop the table entry, keep unreachable records")
    s.set_defaults(func=cmd_sty_del_scene)

    s = sub.add_parser("sty-set-pic", help="point an image node at another PIC index")
    s.add_argument("file")
    s.add_argument("node_id")
    s.add_argument("pic_index")
    s.add_argument("--from-pic", type=int, help="only nodes currently showing this PIC")
    s.add_argument("--all", action="store_true", help="retarget every node with this id")
    s.add_argument("-o", "--out", required=True)
    s.set_defaults(func=cmd_sty_set_pic)

    s = sub.add_parser("pic-list", help="list pictures in a picture RES file")
    s.add_argument("file")
    s.set_defaults(func=cmd_pic_list)

    s = sub.add_parser("pic-decode", help="decode a picture to PNG")
    s.add_argument("file")
    s.add_argument("key", help="PIC name or 1-based index")
    s.add_argument("out")
    s.set_defaults(func=cmd_pic_decode)

    s = sub.add_parser("pic-set", help="replace a picture from a PNG")
    s.add_argument("file")
    s.add_argument("key")
    s.add_argument("input")
    s.add_argument("-o", "--out", required=True)
    s.set_defaults(func=cmd_pic_set)

    s = sub.add_parser("edit-sty-pic", help="repoint a .sty node inside an AOTA image")
    s.add_argument("image")
    s.add_argument("partition")
    s.add_argument("styfile")
    s.add_argument("node_id")
    s.add_argument("pic_index")
    s.add_argument("out")
    s.add_argument("--from-pic", type=int, help="only nodes currently showing this PIC")
    s.add_argument("--all", action="store_true", help="retarget every node with this id")
    s.set_defaults(func=cmd_edit_sty_pic)

    s = sub.add_parser("edit-pic", help="replace a picture inside an AOTA image")
    s.add_argument("image")
    s.add_argument("partition")
    s.add_argument("resfile")
    s.add_argument("key")
    s.add_argument("input")
    s.add_argument("out")
    s.set_defaults(func=cmd_edit_pic)

    s = sub.add_parser("wf-info", help="show a .wfdl watch-face package")
    s.add_argument("file")
    s.add_argument("--tree", action="store_true", help="also print the node tree")
    s.set_defaults(func=cmd_wf_info)

    s = sub.add_parser("wf-res", help="list resources of a .wfdl package")
    s.add_argument("file")
    s.set_defaults(func=cmd_wf_res)

    s = sub.add_parser("wf-tree", help="print the TLV node tree of a .wfdl package")
    s.add_argument("file")
    s.add_argument("--depth", type=int, default=3)
    s.set_defaults(func=cmd_wf_tree)

    s = sub.add_parser("wf-unpack", help="dump header/tree/resources of a .wfdl package")
    s.add_argument("file")
    s.add_argument("outdir")
    s.add_argument("--no-png", action="store_true", help="only dump the raw LZ4 blocks")
    s.set_defaults(func=cmd_wf_unpack)

    s = sub.add_parser("wf-set-res", help="replace one .wfdl resource from a PNG")
    s.add_argument("file")
    s.add_argument("index", help="0-based resource index (see wf-res)")
    s.add_argument("input")
    s.add_argument("-o", "--out", required=True)
    s.add_argument("--cf", type=int, help="force a format instead of keeping the old one")
    s.set_defaults(func=cmd_wf_set_res)

    s = sub.add_parser("wf-build", help="rebuild a .wfdl, replacing numbered PNGs")
    s.add_argument("base", help=".wfdl file or a directory holding base.bin")
    s.add_argument("out")
    s.add_argument("--resources", help="directory of NNN_*.png replacements")
    s.add_argument("--name", help="set the dial name (max 15 bytes)")
    s.set_defaults(func=cmd_wf_build)

    args = p.parse_args(argv)
    args.func(args)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

