import os
import struct
import sys
import zlib

P = sys.argv[1] if len(sys.argv) > 1 else r"D:\upitest\firmware\wp2_1724161837605-90.bin"
OUT = r"D:\upitest\firmware\extracted"
b = open(P, "rb").read()
os.makedirs(OUT, exist_ok=True)

entries = []
i = 0x200
for _ in range(8):
    name = b[i:i + 16].split(b"\0")[0]
    if not name or not all(32 <= c < 127 for c in name):
        break
    off, size, flags, crc = struct.unpack("<IIII", b[i + 16:i + 32])
    entries.append((name.decode(), off, size, flags, crc))
    i += 32

print(f"file size: {len(b):#x}")
print(f"TOC entries: {len(entries)}")
for name, off, size, flags, crc in entries:
    data = b[off:off + size]
    real = zlib.crc32(data) & 0xFFFFFFFF
    ok = "OK" if real == crc else f"crc? {real:#010x}"
    print(f"  {name:<12} off={off:#09x} size={size:#09x} flags={flags:#x} crc={crc:#010x} [{ok}]")
    if name != "ota.xml":
        with open(os.path.join(OUT, name), "wb") as f:
            f.write(data)
        print(f"      head: {data[:24].hex()}")

end = entries[-1][1] + entries[-1][2]
print(f"\ndata end: {end:#x}  trailing: {len(b) - end:#x} bytes")
tail = b[end:]
print("trailing head:", tail[:64].hex())
print("trailing entropy-ish:", len(set(tail[:4096])))

for name, off, size, flags, crc in entries:
    if name == "ota.xml":
        continue
    data = b[off:off + size]
    for wbits, tag in [(15, "zlib"), (-15, "raw-deflate")]:
        try:
            dec = zlib.decompressobj(wbits)
            out = dec.decompress(data)
            if len(out) > 1000:
                print(f"  {name}: {tag} decompresses -> {len(out):#x} bytes")
                break
        except Exception:
            pass
