import lzma
import struct
import zlib

P = r"D:\upitest\firmware\wp2_1724161837605-90.bin"
b = open(P, "rb").read()


def toc(b):
    out, i = [], 0x200
    while i + 32 <= len(b):
        name = b[i:i + 16].split(b"\0")[0]
        if not name or not all(32 <= c < 127 for c in name):
            break
        out.append((name.decode(),) + struct.unpack("<IIII", b[i + 16:i + 32]))
        i += 32
    return out


def recompress(raw, block):
    out = bytearray()
    for p in range(0, len(raw), block):
        chunk = raw[p:p + block]
        comp = lzma.compress(chunk, format=lzma.FORMAT_XZ)  # default preset 6
        out += b"LZMA" + struct.pack("<III", 0x10, len(comp), len(chunk)) + comp
    return bytes(out)


def decomp(data):
    out, pos = bytearray(), 0
    while pos + 16 <= len(data) and data[pos:pos + 4] == b"LZMA":
        dec = lzma.LZMADecompressor()
        c = data[pos + 16:]
        out += dec.decompress(c)
        pos += 16 + (len(c) - len(dec.unused_data))
    return bytes(out)


print("=== per-partition reproduce check ===")
for name, off, size, flags, crc in toc(b):
    if name == "ota.xml":
        continue
    blob = b[off:off + size]
    blk = struct.unpack("<I", blob[12:16])[0]
    raw = decomp(blob)
    re = recompress(raw, blk)
    status = "IDENTICAL" if re == blob else f"DIFF(len {len(re):#x} vs {len(blob):#x})"
    print(f"  {name:<12} blk={blk:#09x} raw={len(raw):#09x} stored={len(blob):#09x} -> {status}")

print("\n=== header crc brute force (expected 0xc7ed77e8) ===")
target = struct.unpack("<I", b[4:8])[0]
def c(x): return zlib.crc32(x) & 0xFFFFFFFF
toczero = bytearray(b[0x200:0x2c0])
for e in range(6):
    toczero[e*32+28:e*32+32] = b"\0\0\0\0"
hdrzero = bytearray(b[:0x200]); hdrzero[4:8] = b"\0\0\0\0"
tz2 = bytearray(b[0x200:0x2c0+0x728])
for e in range(6):
    tz2[e*32+28:e*32+32] = b"\0\0\0\0"
tests = {
    "hdr[:0x200] zero": bytes(hdrzero),
    "toc zero": bytes(toczero),
    "toc+ota zero": bytes(tz2),
    "toc": b[0x200:0x2c0],
    "hdr[8:0x20]": b[8:0x20],
    "ver+board": b[0x40:0x80],
    "whole file zero hdr": None,
}
whole = bytearray(b); whole[4:8] = b"\0\0\0\0"
tests["whole file zero hdr"] = bytes(whole)
for k, v in tests.items():
    cr = c(v)
    print(f"  {k:<22} {cr:#010x} {'<== MATCH' if cr == target else ''}")
