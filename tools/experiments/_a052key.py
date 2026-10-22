import asyncio
import hashlib
import json
import struct
import sys
import time
import zlib

sys.path.insert(0, r"D:\upitest\watools")
import cmf_watch as cw
from Crypto.Cipher import AES

ADDR = "AA:BB:CC:DD:EE:FF"
IV = bytes.fromhex("5051525354555657606162636465665A")


def crc_ok(p):
    return len(p) >= 4 and struct.pack("<I", zlib.crc32(p[:-4]) & 0xFFFFFFFF) == p[-4:]


async def main():
    w = cw.Watch(address=ADDR, verbose=False)
    await w.connect(10.0)

    cap = {}
    orig = w.log

    def log(*a):
        s = " ".join(str(x) for x in a)
        if "nonce=" in s:
            cap["nonce_line"] = s
        orig(*a)

    w.log = log

    st = cw.load_state()
    ak = bytes.fromhex(st["authkey"])
    secret = bytes.fromhex(st.get("secret") or "")
    await w.reconnect(ak)
    print("nonce_line:", cap.get("nonce_line"))
    print("sessionKey:", w.key.hex())
    print("authkey:", ak.hex(), "secret:", secret.hex())

    loop = asyncio.get_event_loop()
    fut = loop.create_future()
    seen = []

    def cb(_, data):
        for c1, c2, cc, ci, body, enc in w.parser.feed(bytes(data)):
            seen.append((c2, body))
            if c2 == 0xA052 and not fut.done():
                fut.set_result(body)
                return

    try:
        await w.client.stop_notify(cw.CMD_N)
    except Exception:
        pass
    await w.client.start_notify(cw.CMD_N, cb)

    frame = cw.build_frame(0xFFFF, 0x9052, b"\xa5", key=w.key)
    await w.client.write_gatt_char(cw.CMD_W, frame, response=True)
    try:
        body = await asyncio.wait_for(fut, timeout=5.0)
    except asyncio.TimeoutError:
        print("no A052")
        await w.disconnect()
        return

    nonce = None
    nl = cap.get("nonce_line", "")
    if "nonce=" in nl:
        nonce = bytes.fromhex(nl.split("nonce=")[1].split()[0])
    print("a052 body:", body.hex(), "nonce:", nonce.hex() if nonce else None)

    cands = {}
    cands["session"] = w.key
    if nonce:
        cands["sha(nonce+ak)"] = hashlib.sha256(nonce + ak).digest()[:16]
        cands["sha(ak+nonce)"] = hashlib.sha256(ak + nonce).digest()[:16]
        cands["sha(nonce+ak)full"] = hashlib.sha256(nonce + ak).digest()
        cands["sha(ak+nonce)full"] = hashlib.sha256(ak + nonce).digest()
        cands["md5(nonce+ak)"] = hashlib.md5(nonce + ak).digest()
        cands["md5(ak+nonce)"] = hashlib.md5(ak + nonce).digest()
        cands["nonce16"] = nonce[:16]
    cands["authkey"] = ak
    if secret:
        cands["secret16"] = secret[:16]
        cands["sha(secret)"] = hashlib.sha256(secret).digest()[:16]

    ivs = {"static": IV, "zero": bytes(16), "nonce": (nonce or bytes(16))[:16]}

    for kn, k in cands.items():
        for ivn, iv in ivs.items():
            for mode in ("cbc", "ecb"):
                try:
                    c = AES.new(k, AES.MODE_CBC, iv) if mode == "cbc" else AES.new(k, AES.MODE_ECB)
                    d = c.decrypt(body)
                except Exception:
                    continue
                for cut in range(0, 9):
                    p = d[:len(d) - cut] if cut else d
                    if crc_ok(p):
                        print(f"*** MATCH key={kn} iv={ivn} mode={mode} cut={cut} "
                              f"payload={p[:-4].hex()}")
                        await w.disconnect()
                        return
    print("no candidate decrypted A052 to a valid CRC")
    await w.disconnect()


asyncio.run(main())
