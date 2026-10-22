import os
import struct
import sys
import zlib
from collections import Counter

SRC = r"D:\upitest\firmware\extracted"
OUT = r"D:\upitest\firmware\sdfs"


def parse(path, name):
    b = open(path, "rb").read()
    if b[:8] != b"sdfs.bin":
        print(f"{name}: not sdfs")
        return
    count, total = struct.unpack("<II", b[12:20])
    print(f"\n=== {name}: files={count} total={total:#x} (actual {len(b):#x})")
    root = os.path.join(OUT, name)
    os.makedirs(root, exist_ok=True)
    exts = Counter()
    bad = 0
    for i in range(count):
        e = 0x20 + i * 32
        fname = b[e:e + 12].split(b"\0")[0].decode("latin1")
        off, size, d1, d2, crc = struct.unpack("<IIIII", b[e + 12:e + 32])
        data = b[off:off + size]
        real = zlib.crc32(data) & 0xFFFFFFFF
        if real != crc:
            bad += 1
        ext = os.path.splitext(fname)[1].lower() or "(none)"
        exts[ext] += 1
        safe = fname.replace("/", "_").replace("\\", "_")
        with open(os.path.join(root, f"{i:04d}_{safe}"), "wb") as f:
            f.write(data)
    print("  extensions:", dict(exts.most_common()))
    if bad:
        print(f"  crc mismatches: {bad}/{count}")


for f in ("res.raw", "fonts.raw", "res_e.raw", "sdfs_k.raw"):
    p = os.path.join(SRC, f)
    if os.path.exists(p):
        parse(p, f.replace(".raw", ""))
