import lzma
import os
import struct
import sys
import zlib


def toc(b):
    out, i = [], 0x200
    while i + 32 <= len(b):
        name = b[i:i + 16].split(b"\0")[0]
        if not name or not all(32 <= c < 127 for c in name):
            break
        off, size, flags, crc = struct.unpack("<IIII", b[i + 16:i + 32])
        out.append((name.decode(), off, size, flags, crc))
        i += 32
        if len(out) > 64:
            break
    return out


def decomp_chunked(data):
    out, pos, n = bytearray(), 0, 0
    while pos + 16 <= len(data) and data[pos:pos + 4] == b"LZMA":
        dec = lzma.LZMADecompressor()
        chunk = data[pos + 16:]
        out += dec.decompress(chunk)
        if not dec.eof:
            break
        pos += 16 + (len(chunk) - len(dec.unused_data))
        n += 1
    return bytes(out), n


def show(path, outdir, depth=0):
    b = open(path, "rb").read()
    pad = "  " * depth
    fields = struct.unpack("<IIIIIII", b[4:0x20])
    vname = b[0x40:0x60].split(b"\0")[0].decode("latin1")
    board = b[0x60:0x80].split(b"\0")[0].decode("latin1")
    print(f"{pad}AOTA {path} size={len(b):#x} count={fields[2]} version_name='{vname}' board='{board}'")
    os.makedirs(outdir, exist_ok=True)
    for name, off, size, flags, crc in toc(b):
        data = b[off:off + size]
        real = zlib.crc32(data) & 0xFFFFFFFF
        tag = "raw" if data[:4] == b"LZMA" else data[:4].hex()
        print(f"{pad}  {name:<12} off={off:#09x} size={size:#09x} crc={'OK' if real == crc else 'x'} head={tag}")
        dst = os.path.join(outdir, name)
        if data[:4] == b"LZMA":
            raw, n = decomp_chunked(data)
            print(f"{pad}      LZMA chunks={n} -> {len(raw):#x}")
            with open(dst + ".raw", "wb") as f:
                f.write(raw)
            if raw[:4] == b"AOTA" and depth < 2:
                show(dst + ".raw", dst + "_", depth + 1)
        else:
            with open(dst, "wb") as f:
                f.write(data)


if __name__ == "__main__":
    show(sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else r"D:\upitest\firmware\tree")
