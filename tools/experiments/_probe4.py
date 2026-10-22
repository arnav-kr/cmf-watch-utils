import struct
import zlib

P = r"D:\upitest\firmware\wp2_1724161837605-90.bin"
b = open(P, "rb").read()
def c(x): return zlib.crc32(x) & 0xFFFFFFFF

f = {o: struct.unpack("<I", b[o:o+4])[0] for o in range(8, 0x20, 4)}
print("header fields:", {hex(k): hex(v) for k, v in f.items()})
print("f18 target =", hex(f[0x18]))

# TOC
toc, i = [], 0x200
while i + 32 <= len(b):
    name = b[i:i+16].split(b"\0")[0]
    if not name or not all(32 <= x < 127 for x in name):
        break
    toc.append((name.decode(),) + struct.unpack("<IIII", b[i+16:i+32]))
    i += 32
toc_end = i
ota_off, ota_size = [(o, s) for n, o, s, fl, cr in toc if n == "ota.xml"][0]
last_end = max(o + s for n, o, s, fl, cr in toc)

print(f"\nTOC end={toc_end:#x}  ota.xml {ota_off:#x}+{ota_size:#x}  last_end={last_end:#x}  f14={f[0x14]:#x}")
print(f"file={len(b):#x}  trailing after last_end={len(b)-last_end:#x}")

cands = {
    "ota.xml": b[ota_off:ota_off+ota_size],
    "toc": b[0x200:toc_end],
    "toc+ota": b[0x200:ota_off+ota_size],
    "toc+ota padded0x200": b[0x200:0xc00],
    "name+off+size concat": b"".join(b[i*32+0x200:i*32+0x210] + b[i*32+0x210:i*32+0x21c] for i in range(len(toc))),
    "all entry crcs": b"".join(struct.pack("<I", cr) for n, o, s, fl, cr in toc),
    "toc with crc zeroed": bytes(bytearray(b[0x200:toc_end])),
}
cands["toc with crc zeroed"] = bytes(bytes(bytearray(b[0x200:toc_end])[:e*32+28] + b"\0\0\0\0" + bytearray(b[0x200:toc_end])[e*32+32:]) for e in range(len(toc)))[0] if False else None
# fix: build zeroed-crc toc properly
tz = bytearray(b[0x200:toc_end])
for e in range(len(toc)):
    tz[e*32+28:e*32+32] = b"\0\0\0\0"
cands["toc with crc zeroed"] = bytes(tz)

for k, v in cands.items():
    if v is None:
        continue
    val = c(v)
    print(f"  {k:<24} crc32={val:#010x} {'<== f18 MATCH' if val == f[0x18] else ''}")

print("\n=== trailing region ===")
tail = b[last_end:]
print(f"len={len(tail):#x}")
print("head:", tail[:64].hex())
print("tail:", tail[-64:].hex())
# find first nonzero
for idx in range(len(tail)):
    if tail[idx] != 0:
        print(f"first nonzero at +{idx:#x} rel, abs {last_end+idx:#x}")
        break
nz = [idx for idx in range(len(tail)) if tail[idx] != 0]
if nz:
    print(f"nonzero bytes: {len(nz)} first={last_end+nz[0]:#x} last={last_end+nz[-1]:#x}")
    s0 = last_end + nz[0]
    print(f"nonzero block head @{s0:#x}:", b[s0:s0+64].hex())
    print("block ascii:", "".join(chr(x) if 32 <= x < 127 else "." for x in b[s0:s0+64]))
