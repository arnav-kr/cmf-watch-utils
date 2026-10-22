import asyncio
import binascii
import hashlib
import struct
import sys
import time
import zlib

sys.path.insert(0, r"D:\upitest\watools")
import cmf_watch as cw
from Crypto.Cipher import AES

ADDR = "AA:BB:CC:DD:EE:FF"
IV = bytes.fromhex("5051525354555657606162636465665A")


def aes_dec_any(key, ct):
    out = AES.new(key, AES.MODE_CBC, IV).decrypt(ct)
    return out


def check(pt):
    if not pt:
        return None
    p = pt[-1]
    if 1 <= p <= 16:
        d = pt[:-p]
        if len(d) >= 4:
            want = struct.pack("<I", zlib.crc32(d[:-4]) & 0xFFFFFFFF)
            if want == d[-4:]:
                return ("crc+pkcs7", d[:-4])
        return ("pkcs7", d)
    return None


async def main():
    w = cw.Watch(address=ADDR, verbose=True)
    await w.connect(10.0)
    st = cw.load_state()
    ak = bytes.fromhex(st["authkey"])
    w.secret = bytes.fromhex(st["secret"])
    await w.reconnect(ak)
    nonce = getattr(w, "nonce", None)
    print("AUTHKEY=", ak.hex())
    print("SESSIONKEY=", w.key.hex() if w.key else None)

    # send INIT1 and capture A052
    frame = cw.build_frame(0xFFFF, 0x9052, b"\xa5", key=w.key)
    print("INIT1 frame=", frame.hex())
    await w.client.write_gatt_char(cw.CMD_W, frame, response=True)
    a052 = None
    deadline = time.monotonic() + 4.0
    while time.monotonic() < deadline and a052 is None:
        try:
            await asyncio.sleep(0.05)
        except Exception:
            pass
        while not w.rx.empty():
            piece = w.rx.get_nowait()
            for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                print(f"< cmd1={c1:#06x} cmd2={c2:#06x} enc={enc} {body.hex()}")
                if c2 == 0xA052:
                    a052 = body
    print("A052=", a052.hex() if a052 else None)
    if not a052:
        await w.disconnect()
        return

    cands = {}
    def add(name, k):
        if k:
            cands[name] = bytes(k)
    add("authkey", ak)
    add("secret", w.secret)
    if nonce:
        for nlen in (16, 32):
            base = nonce[:nlen]
            add(f"sha256(n{nlen}+ak)", hashlib.sha256(base + ak).digest())
            add(f"sha256(n{nlen}+ak)[:16]", hashlib.sha256(base + ak).digest()[:16])
            add(f"sha256(n{nlen}+secret)", hashlib.sha256(base + w.secret).digest())
            add(f"sha256(n{nlen}+secret)[:16]", hashlib.sha256(base + w.secret).digest()[:16])
            add(f"sha256(ak+n{nlen})", hashlib.sha256(ak + base).digest())
        add("sha256(nonce)[:16]", hashlib.sha256(nonce).digest()[:16])
        add("sha256(nonce)", hashlib.sha256(nonce).digest())
    add("sha256(ak)[:16]", hashlib.sha256(ak).digest()[:16])
    add("sha256(ak)", hashlib.sha256(ak).digest())
    add("md5(ak)", hashlib.md5(ak).digest())
    add("bindkey", bytes.fromhex("5051525354555657606162636465665a"))

    print("\n--- candidates ---")
    for name, k in cands.items():
        if len(k) not in (16, 24, 32):
            continue
        pt = aes_dec_any(k, a052)
        res = check(pt)
        flag = f"  <<< {res}" if res else ""
        print(f"{name:26s} len={len(k):2d} pt={pt.hex()}{flag}")

    try:
        await w.disconnect()
    except Exception:
        pass


asyncio.run(main())
