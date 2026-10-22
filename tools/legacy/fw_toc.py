import struct
import sys

P = sys.argv[1] if len(sys.argv) > 1 else r"D:\upitest\firmware\wp2_1724161837605-90.bin"
b = open(P, "rb").read()


def hd(base, n):
    for off in range(base, min(len(b), base + n), 16):
        chunk = b[off:off + 16]
        hx = " ".join(f"{x:02x}" for x in chunk)
        asc = "".join(chr(x) if 32 <= x < 127 else "." for x in chunk)
        print(f"{off:08x}  {hx:<47}  {asc}")


print("=== header 0x00-0x120 ===")
hd(0, 0x120)
print("\n=== TOC 0x1f0-0x430 ===")
hd(0x1F0, 0x240)

print("\n=== header fields ===")
for off, fmt, name in [(0x04, "<I", "crc_hdr"), (0x08, "<I", "f0"), (0x0C, "<I", "count"),
                       (0x10, "<I", "f10"), (0x14, "<I", "f14"), (0x18, "<I", "f18"),
                       (0x1C, "<I", "f1c")]:
    print(f"  {name:<8} @{off:#06x} = {struct.unpack(fmt, b[off:off+4])[0]:#x} ({struct.unpack(fmt, b[off:off+4])[0]})")

print("\n=== TOC records (name + trailing dwords) ===")
for name in [b"TEMP.bin", b"res.bin", b"fonts.bin", b"res_e.bin", b"sdfs_k.bin"]:
    i = b.find(name, 0x200, 0x430)
    if i < 0:
        continue
    end = i + len(name)
    trail = b[end:end + 24]
    dw = struct.unpack("<6I", trail)
    print(f"  {name.decode():<10} @{i:#06x} trail={' '.join(f'{x:#010x}' for x in dw)}")
