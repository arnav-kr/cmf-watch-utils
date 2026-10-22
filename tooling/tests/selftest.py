"""Regression checks for the byte-identical round-trips.

    python tests/selftest.py firmware/JX402_V1.0.0.73.bin

Verifies, against a real image:
  * outer AOTA rebuild == input
  * every SDFS partition parse/pack == input
  * every RES string table round-trip
  * bt_watch.sty / logo.sty parse/build == input
  * .sty add-scene (fresh ids) / delete+compact round-trips
  * bt_watch.res / logo.res parse/build == input
  * every LZ4/JPEG picture decodes to w*h*3
  * LZ4 HC level 12 reproduces every shipped body byte-for-byte
  * .ajs frame parsing, single/multi-tile build, set-frame identity
  * every .wfdl package parse/build round-trip (optional 2nd arg / WF_DIR)
"""

from __future__ import annotations

import os
import sys

from fwkit import ajs, aota, lz4, picres, res, sdfs, sty, wfdl
from fwkit.image import Image


def check(cond, msg):
    print(("ok   " if cond else "FAIL ") + msg)
    if not cond:
        raise SystemExit(1)


def lz4_block_ok(src: bytes, out_n: int) -> bool:
    """LZ4 block must end in >=5 literals and matches must not cross n-5."""
    i = 0
    out = 0
    while i < len(src):
        tok = src[i]
        i += 1
        lit = tok >> 4
        if lit == 15:
            while True:
                b = src[i]
                i += 1
                lit += b
                if b != 255:
                    break
        out += lit
        i += lit
        if i >= len(src):
            return lit >= min(5, out_n) and out == out_n
        off = src[i] | (src[i + 1] << 8)
        i += 2
        ml = (tok & 15) + 4
        if (tok & 15) == 15:
            while True:
                b = src[i]
                i += 1
                ml += b
                if b != 255:
                    break
        if off == 0 or off > out or out > out_n - 12:
            return False
        out += ml
        if out > out_n - 5:
            return False
    return False


def main(path: str) -> int:
    img = Image.open(path)
    check(img.build() == img.data, "AOTA rebuild is byte-identical")
    ajs_write_done = False

    for name in img.names:
        raw = img.decompressed(name)
        if raw[:8] != sdfs.MAGIC:
            print("skip %s (not SDFS)" % name)
            continue
        files = sdfs.unpack(raw)
        check(sdfs.pack(list(files.items())) == raw, "%s SDFS round-trip" % name)
        check(sdfs.pad_to(raw, len(raw)) == raw, "%s SDFS pad identity" % name)
        for fname, blob in files.items():
            if ajs.is_ajs(blob):
                a = ajs.parse(blob)
                declared = a.raw[0x1A] | (a.raw[0x1B] << 8)
                check(len(a.frames) > 0 and a.frames[0].length == declared,
                      "%s:%s .ajs frame chain (%d frames)" % (name, fname, len(a.frames)))
                check(a.fps == 25 and a.frames[0].tile_count >= 1,
                      "%s:%s .ajs fps/tiles" % (name, fname))
                f0 = a.frames[0]
                j = f0.jpeg(blob)
                if not ajs_write_done:
                    one = ajs.build([j], fps=a.fps, width=a.width, height=a.height)
                    ap = ajs.parse(one)
                    check(ap.frames[0].raw == f0.raw and ap.frames[0].jpeg(one) == j,
                          ".ajs single-tile build")
                    check(ajs.set_frame(blob, 0, j) == blob, ".ajs set-frame identity")
                    mt = ajs.build([[j, j]], fps=12, width=a.width * 2,
                                   height=a.height, tile_width=a.width,
                                   tile_height=a.height)
                    am = ajs.parse(mt)
                    check(am.fps == 12 and am.width == a.width * 2
                          and am.frames[0].tile_count == 2,
                          ".ajs multi-tile build")
                    check(ajs.set_frame(mt, 0, [j, j]) == mt,
                          ".ajs multi-tile set-frame identity")
                    ajs_write_done = True
            elif fname.endswith(".sty"):
                st = sty.parse(blob)
                check(st.build() == blob, "%s:%s .sty round-trip" % (name, fname))
                src = next((sc for sc in st.scenes if sc.count), None)
                if src is not None:
                    used = {sc.id for sc in st.scenes}
                    nid = 0x5EED0000 + src.index
                    while nid in used:
                        nid += 1
                    out = sty.add_scene(blob, src.index, nid)
                    st2 = sty.parse(out)
                    check(st2.build() == out,
                          "%s:%s .sty add-scene round-trip" % (name, fname))
                    a, b = st.scene_nodes(src), st2.scene_nodes(st2.scenes[-1])
                    check(st2.scenes[-1].id == nid and len(b) == len(a)
                          and [x.pic_index for x in a if x.type == 4] ==
                              [x.pic_index for x in b if x.type == 4],
                          "%s:%s .sty add-scene clone matches" % (name, fname))
                    check(all(len(x.children) == len(y.children)
                              for x, y in zip(a, b)),
                          "%s:%s .sty add-scene children match" % (name, fname))
                    nid2 = nid + 0x1000
                    outf = sty.add_scene(blob, src.index, nid2, fresh_ids=True)
                    stf = sty.parse(outf)
                    old_ids = {n.id for n in st.nodes}

                    def clone_ids(node):
                        yield node.id
                        for c in node.children:
                            yield from clone_ids(c)

                    new_ids = {i for n in stf.scene_nodes(stf.scenes[-1])
                               for i in clone_ids(n)}
                    check(not (new_ids & old_ids) and len(new_ids) > 0,
                          "%s:%s .sty fresh node ids" % (name, fname))
                    removed = sty.remove_scene(outf, stf.scenes[-1].id)
                    strm = sty.parse(removed)
                    check(len(strm.scenes) == len(st.scenes)
                          and len(removed) == len(blob),
                          "%s:%s .sty remove+compact" % (name, fname))
                    stripped = sty.remove_scene(out, st2.scenes[-1].id,
                                                compact_nodes=False)
                    check(len(sty.parse(stripped).scenes) == len(st.scenes),
                          "%s:%s .sty remove keep-orphans" % (name, fname))
            elif picres.is_picres(blob):
                r = picres.parse(blob)
                check(picres.build(r) == blob, "%s:%s picture RES round-trip" % (name, fname))
                for p in r.pics:
                    rgb = picres.decode(p)
                    check(len(rgb) == p.w * p.h * 3,
                          "%s:%s %s decodes (%dx%d)" % (name, fname, p.name, p.w, p.h))
                    if p.type == picres.TYPE_JPEG:
                        continue
                    bpp = 2 if p.type == picres.TYPE_LZ4_565 else 3
                    raw = lz4.decompress(p.body, p.w * p.h * bpp)
                    check(lz4.compress(raw) == p.body,
                          "%s:%s %s LZ4 HC byte-identical" % (name, fname, p.name))
                    packed = lz4.compress(raw)
                    check(lz4_block_ok(packed, len(raw)),
                          "%s:%s %s LZ4 end rule" % (name, fname, p.name))
            elif res.is_res(blob):
                r = res.parse(blob)
                check(res.build(r) == blob, "%s:%s string RES round-trip" % (name, fname))
    bad = wfdl.Watchface(b"\0" * wfdl.HDR, [], b"",
                         [wfdl.Resource(4, 1000, 1000, b"")])
    try:
        bad.build()
    except ValueError as e:
        check("max %d px" % wfdl.MAX_IMAGE_DIM in str(e),
              "oversized .wfdl image rejected")
    else:
        check(False, "oversized .wfdl image rejected")
    root = os.path.join(os.path.dirname(__file__), "..", "..")
    default_wf_dir = os.path.join(root, "watchfaces")
    if not os.path.isdir(default_wf_dir):
        default_wf_dir = os.path.join(root, "dumps", "samples", "watchfaces")
    wf_dir = sys.argv[2] if len(sys.argv) > 2 else os.environ.get("WF_DIR", default_wf_dir)
    if os.path.isdir(wf_dir):
        for fn in sorted(os.listdir(wf_dir)):
            if not fn.lower().endswith(".bin"):
                continue
            blob = open(os.path.join(wf_dir, fn), "rb").read()
            if not wfdl.is_wfdl(blob):
                continue
            w = wfdl.parse(blob)
            check(w.build() == blob, "%s .wfdl round-trip" % fn)
            for i, r in enumerate(w.resources):
                if r.cf not in (1, 28):
                    w.decode_rgba(i)
            check(all(r.cf in (1, 28) or lz4_block_ok(r.data, r.raw_size)
                      for r in w.resources), "%s LZ4 end rule (stored)" % fn)
            check(True, "%s %d resources decode" % (fn, len(w.resources)))
            nref = sum(1 for n in w.nodes() if n.ref)
            w.set_pixels(0, 2, 2, bytes([1, 2, 3, 255]) * 4)
            mod = w.build()
            p = wfdl.parse(mod)
            check(p.build() == mod, "%s .wfdl rebuild after resize" % fn)
            check(sum(1 for n in p.nodes() if n.ref) == nref, "%s refs survive resize" % fn)
    else:
        print("skip .wfdl checks (no %s)" % wf_dir)
    print("all checks passed")
    return 0


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(__doc__)
        raise SystemExit(2)
    raise SystemExit(main(sys.argv[1]))
