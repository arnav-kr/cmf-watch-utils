import uuid

PATH = r"D:\upitest\firmware\tree\app.bin"
BASE = 0x10000000
data = open(PATH, "rb").read()

uuids = {
    "DAT_W 02f0..ffe1": "02f00000-0000-0000-0000-00000000ffe1",
    "DAT_N 02f0..ffe2": "02f00000-0000-0000-0000-00000000ffe2",
    "CMD_W 0000fff2": "0000fff2-0000-1000-8000-00805f9b34fb",
    "CMD_N 0000fff1": "0000fff1-0000-1000-8000-00805f9b34fb",
    "SH_W 77d4ff01": "77d4ff01-2fe2-2334-0d35-9ccd078f529c",
    "SH_N 77d4ff02": "77d4ff02-2fe2-2334-0d35-9ccd078f529c",
}

def variants(u):
    b = uuid.UUID(u).bytes          # big-endian canonical
    return {"be": b, "le": b[::-1]}

for name, u in uuids.items():
    for tag, pat in variants(u).items():
        idx = 0
        hits = []
        while True:
            idx = data.find(pat, idx)
            if idx < 0:
                break
            hits.append(idx)
            idx += 1
        print(f"{name} [{tag}] {pat.hex()} : {len(hits)} hits")
        for h in hits[:10]:
            print(f"    off 0x{h:x} va 0x{BASE+h:08x}")
