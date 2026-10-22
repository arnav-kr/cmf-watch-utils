import struct, sys

PATH = r"D:\upitest\firmware\tree\app.bin"
BASE = 0x10000000
vals = [0x9040,0x9041,0x9042,0x9063,0x9064,0x9065,0x905e,0x905f,0x9060,0x8052,0xA040,0xA041,0xA042,0xA052,0xA064]
data = open(PATH,"rb").read()
print("size", len(data))

for v in vals:
    for enc, tag in ((v & 0xFFFF, "le16"), ((v>>8)|((v&0xff)<<8), "be16")):
        pat = struct.pack("<H", v)          # little-endian 2 bytes
        patbe = struct.pack(">H", v)
        for name, p in (("le", pat), ("be", patbe)):
            idx = 0
            cnt = 0
            while True:
                idx = data.find(p, idx)
                if idx < 0:
                    break
                cnt += 1
                if cnt <= 6:
                    va = BASE + idx
                    print(f"0x{v:04x} {name} @off 0x{idx:x} va 0x{va:08x}")
                idx += 1
            if cnt:
                print(f"0x{v:04x} {name}: {cnt} hits")
