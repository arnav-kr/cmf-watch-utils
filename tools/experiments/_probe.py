import lzma
import struct
import zlib

P = r"D:\upitest\firmware\wp2_1724161837605-90.bin"
b = open(P, "rb").read()
n = len(b)
print(f"file size = {n:#x} ({n})")

hcrc = struct.unpack("<I", b[4:8])[0]
print(f"header crc field @0x04 = {hcrc:#010x}")
print("fields:", [f"{struct.unpack('<I', b[o:o+4])[0]:#x}" for o in range(8, 0x20, 4)])
print("0x7c..0x84:", b[0x7c:0x84].hex())
print("0x20..0x40:", b[0x20:0x40].hex())

toc = []
i = 0x200
while i + 32 <= len(b):
    name = b[i:i + 16].split(b"\0")[0]
    if not name or not all(32 <= c < 127 for c in name):
        break
    off, size, flags, crc = struct.unpack("<IIII", b[i + 16:i + 32])
    toc.append((name.decode(), off, size, flags, crc))
    i += 32
    if len(toc) > 32:
        break
print(f"\nTOC ({len(toc)} entries, ends @{i:#x}):")
for name, off, size, flags, crc in toc:
    end = off + size
    real = zlib.crc32(b[off:end]) & 0xFFFFFFFF
    print(f"  {name:<12} off={off:#09x} size={size:#09x} end={end:#09x} "
          f"flags={flags:#x} crc={crc:#010x} {'OK' if real==crc else 'BAD('+hex(real)+')'}")

print("\n=== header-crc candidates ===")
def c(d): return zlib.crc32(d) & 0xFFFFFFFF
tmp = bytearray(b[:0x200]); tmp[4:8] = b"\0\0\0\0"
cands = {
    "hdr[0:0x200] zero-crc": bytes(tmp),
    "hdr[8:0x200]": b[8:0x200],
    "hdr[0:0x200]": b[0:0x200],
    "hdr[0x20:0x200]": b[0x20:0x200],
    "toc[0x200:ender]": b[0x200:i],
    "toc+ota[0x200:0xb28]": b[0x200:0xb28],
}
for k, v in cands.items():
    print(f"  {k:<24} crc={c(v):#010x} {'<== MATCH' if c(v)==hcrc else ''}")

# dump ota.xml (master)
for name, off, size, flags, crc in toc:
    if name == "ota.xml":
        print(f"\n=== master ota.xml ({size:#x} bytes) ===")
        print(b[off:off+size].decode("utf-8", "replace"))

# dump first stored partition chunk header
for name, off, size, flags, crc in toc:
    if name == "TEMP.bin":
        print(f"\n=== TEMP.bin stored head @{off:#x} ===")
        print(b[off:off+16].hex(), "| blk words:",
              [f"{struct.unpack('<I', b[off+o:off+o+4])[0]:#x}" for o in (4, 8, 12)])
        blob = b[off:off+size]
        dec = lzma.LZMADecompressor()
        out = dec.decompress(blob[16:])
        print(f"  1st chunk: consumed={len(blob)-16-len(dec.unused_data):#x} produced={len(out):#x} eof={dec.eof}")
        print(f"  xz magic? {blob[16:22].hex()}")
        # try reproduce exactly
        for preset in (6, 9):
            for fmt_name, fmt in (("XZ", lzma.FORMAT_XZ), ("ALONE", lzma.FORMAT_ALONE), ("RAW", lzma.FORMAT_RAW)):
                try:
                    if fmt == lzma.FORMAT_RAW:
                        comp = lzma.LZMACompressor(format=fmt, filters=[{"id": lzma.FILTER_LZMA2, "preset": preset}])
                    else:
                        comp = lzma.LZMACompressor(format=fmt, preset=preset)
                    got = comp.compress(out) + comp.flush()
                    eq = got == blob[16:16+len(got)]
                    print(f"  recompress {fmt_name} preset={preset}: len={len(got):#x} eq_prefix={eq}")
                except Exception as e:
                    print(f"  recompress {fmt_name} preset={preset}: ERR {e}")
        break
