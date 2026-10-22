import struct
import zlib

P = r"D:\upitest\firmware\wp2_1724161837605-90.bin"
b = open(P, "rb").read()
target = struct.unpack("<I", b[4:8])[0]
print(f"target header crc = {target:#010x}")


def crc32_std(x):
    return zlib.crc32(x) & 0xFFFFFFFF


def crc32_custom(data, init=0xFFFFFFFF, xorout=0xFFFFFFFF, refin=True, refout=True):
    poly = 0xEDB88320 if refin else 0x04C11DB7
    crc = init
    for byte in data:
        if refin:
            crc ^= byte
            for _ in range(8):
                crc = (crc >> 1) ^ (poly & -(crc & 1))
        else:
            crc ^= byte << 24
            for _ in range(8):
                crc = ((crc << 1) ^ (poly & -(crc >> 31))) & 0xFFFFFFFF
    if refout != refin:
        crc = int(f"{crc:032b}"[::-1], 2)
    return (crc ^ xorout) & 0xFFFFFFFF


starts = [0, 4, 8, 0x20, 0x40, 0x60, 0x80, 0x100, 0x1c0, 0x200]
ends = [0x20, 0x40, 0x60, 0x80, 0xc0, 0x100, 0x180, 0x1c0, 0x200, 0x2c0, 0x400]
found = []
for s in starts:
    for e in ends:
        if e <= s:
            continue
        seg = b[s:e]
        for zero in (False, True):
            if zero and s <= 4 < e:
                seg2 = bytearray(seg); seg2[4 - s:8 - s] = b"\0\0\0\0"
            elif zero:
                continue
            else:
                seg2 = seg
            data = bytes(seg2)
            for name, val in (
                ("std", crc32_std(data)),
                ("std^x", crc32_std(data) ^ 0xFFFFFFFF),
                ("i0x", crc32_custom(data, 0xFFFFFFFF, 0)),
                ("i0", crc32_custom(data, 0, 0)),
                ("i0x0", crc32_custom(data, 0, 0xFFFFFFFF)),
            ):
                if val == target:
                    found.append((s, e, zero, name, val))
            ad = zlib.adler32(data) & 0xFFFFFFFF
            if ad == target:
                found.append((s, e, zero, "adler", ad))
for f in found:
    print("MATCH", f)
if not found:
    print("no match in sweep")
    # show a few close ones
    print("std hdr[0:0x200] zero =", hex(crc32_std(bytes(b[:4] + b"\0\0\0\0" + b[8:0x200]))))
