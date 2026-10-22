import struct
import zlib

P = r"D:\upitest\firmware\wp2_1724161837605-90.bin"
b = open(P, "rb").read()
target = struct.unpack("<I", b[0x18:0x1c])[0]

bounds = [0, 4, 8, 0x20, 0x40, 0x60, 0x80, 0x100, 0x1c0, 0x200, 0x2c0, 0x400, 0xb28, 0xc00,
          0x3448458, 0x3448600, len(b)]
found = []
for i, s in enumerate(bounds):
    for e in bounds[i+1:]:
        seg = b[s:e]
        v = zlib.crc32(seg) & 0xFFFFFFFF
        if v == target:
            found.append((hex(s), hex(e), "std"))
        if (v ^ 0xFFFFFFFF) == target:
            found.append((hex(s), hex(e), "std^x"))

# decompressed partitions
def decomp(data):
    import lzma
    out, pos = bytearray(), 0
    while pos + 16 <= len(data) and data[pos:pos+4] == b"LZMA":
        d = lzma.LZMADecompressor()
        c = data[pos+16:]
        out += d.decompress(c)
        pos += 16 + (len(c) - len(d.unused_data))
    return bytes(out)

toc, i = [], 0x200
while i + 32 <= len(b):
    name = b[i:i+16].split(b"\0")[0]
    if not name or not all(32 <= x < 127 for x in name):
        break
    toc.append((name.decode(),) + struct.unpack("<IIII", b[i+16:i+32]))
    i += 32

raws = {}
for name, off, size, flags, crc in toc:
    if name == "ota.xml":
        continue
    raws[name] = decomp(b[off:off+size])
for name, r in raws.items():
    for tag, data in ((name, r), (name+"^x", None)):
        if data is None:
            continue
        if (zlib.crc32(data) & 0xFFFFFFFF) == target:
            found.append((name, "raw", "std"))
allconcat = b"".join(raws[n] for n in raws)
if (zlib.crc32(allconcat) & 0xFFFFFFFF) == target:
    found.append(("concat raw", "", "std"))

print("f18 matches:", found if found else "NONE")
print("f18 =", hex(target))
