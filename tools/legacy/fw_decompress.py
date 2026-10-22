import lzma
import os
import struct
import sys
import zlib

P = sys.argv[1] if len(sys.argv) > 1 else r"D:\upitest\firmware\wp2_1724161837605-90.bin"
OUT = r"D:\upitest\firmware\extracted"
b = open(P, "rb").read()
os.makedirs(OUT, exist_ok=True)

ORIG = {"TEMP.bin": 0x13f15c, "res.bin": 0x89da80, "fonts.bin": 0x912900,
        "res_e.bin": 0x380de00, "sdfs_k.bin": 0x7ff40}


def decomp_chunked2(data):
    out = bytearray()
    pos = 0
    nchunks = 0
    while pos + 16 <= len(data) and data[pos:pos + 4] == b"LZMA":
        dec = lzma.LZMADecompressor()
        chunk = data[pos + 16:]
        out += dec.decompress(chunk)
        if not dec.eof:
            break
        used = len(chunk) - len(dec.unused_data)
        pos += 16 + used
        nchunks += 1
    return bytes(out), nchunks


entries = []
i = 0x200
for _ in range(8):
    name = b[i:i + 16].split(b"\0")[0]
    if not name or not all(32 <= c < 127 for c in name):
        break
    off, size, flags, crc = struct.unpack("<IIII", b[i + 16:i + 32])
    entries.append((name.decode(), off, size, flags, crc))
    i += 32

for name, off, size, flags, crc in entries:
    if name == "ota.xml":
        continue
    data = b[off:off + size]
    hdr = data[:16]
    blk = struct.unpack("<I", data[12:16])[0]
    raw, nch = decomp_chunked2(data)
    exp = ORIG.get(name)
    print(f"=== {name} stored={size:#x} blk={blk:#x} chunks={nch} -> raw={len(raw):#x} "
          f"(orig={exp:#x} {'MATCH' if len(raw) == exp else 'DIFF'}) crc32={zlib.crc32(raw) & 0xffffffff:#010x}")
    print(f"    head: {raw[:48].hex()}")
    out = os.path.join(OUT, name.replace(".bin", ".raw"))
    with open(out, "wb") as f:
        f.write(raw)

end = entries[-1][1] + entries[-1][2]
tail = b[end:]
print(f"\ntrailing {len(tail):#x} bytes")
print("  head:", tail[:48].hex())
print("  last:", tail[-32:].hex())
