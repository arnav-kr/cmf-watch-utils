import struct
import zlib

P = r"D:\upitest\firmware\wp2_1724161837605-90.bin"
b = open(P, "rb").read()
def c(x): return zlib.crc32(x) & 0xFFFFFFFF
target = struct.unpack("<I", b[0x18:0x1c])[0]
print("f18 target =", hex(target))

AGPS = b.find(b"AGPS")
print("AGPS magic at", hex(AGPS))
agps_size = struct.unpack("<I", b[AGPS+8:AGPS+12])[0]
print("AGPS size field =", hex(agps_size), " end->file:", hex(len(b)-AGPS))
# AGPS header dump
print("AGPS header:", b[AGPS:AGPS+0x40].hex())

ranges = {
    "[0x200:file]": (0x200, len(b)),
    "[0xc00:last_end]": (0xc00, 0x3448458),
    "[AGPS:file]": (AGPS, len(b)),
    "[AGPS+0x18:file]": (AGPS + 0x18, len(b)),
    "[0:0x200]": (0, 0x200),
    "[0x200:0xc00]": (0x200, 0xc00),
    "[0:0x3448458]": (0, 0x3448458),
}
for k, (s, e) in ranges.items():
    print(f"  {k:<20} crc32={c(b[s:e]):#010x} {'<== f18 MATCH' if c(b[s:e]) == target else ''}")

# last 4 bytes of file
print("last4:", b[-4:].hex(), hex(struct.unpack('<I', b[-4:])[0]))
# AGPS tail
print("AGPS last 8:", b[-8:].hex())
print("crc32 of AGPS payload size field:", )
print("agps version-ish words:", [hex(struct.unpack('<I', b[AGPS+o:AGPS+o+4])[0]) for o in range(0, 0x20, 4)])
