import asyncio
import hashlib
import sys
import time

sys.path.insert(0, r"D:\upitest\watools")
import cmf_watch as cw

ADDR = "AA:BB:CC:DD:EE:FF"


def try_dec(key, body):
    try:
        d = cw.aes_dec(key, body)
    except Exception:
        return None
    if len(d) >= 4 and cw.crc32_le(d[:-4]) == d[-4:]:
        return ("CRC", d[:-4])
    return ("raw", d)


async def roundtrip(w, cmd, key, tmo=2.0, payload=b"\xa5"):
    await w.client.write_gatt_char(cw.CMD_W, cw.build_frame(0xFFFF, cmd, payload, key=key), response=True)
    end = time.monotonic() + tmo
    out = []
    while time.monotonic() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=max(0.05, end - time.monotonic()))
        except (asyncio.TimeoutError, ValueError):
            break
        for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
            out.append((c2, body, enc))
    return out


async def main():
    w = cw.Watch(address=ADDR, verbose=False)
    await w.connect(10.0)
    st = cw.load_state()
    ak = bytes.fromhex(st["authkey"])
    secret = bytes.fromhex(st["secret"])
    w.key = ak
    w.authkey = ak

    r = await roundtrip(w, 0x8049, ak, payload=b"\xa5" + b"CMF Watch Pro 2")
    print("0x8049/ak ->", [(hex(c), b.hex()) for c, b, e in r], flush=True)

    r = await roundtrip(w, 0x804B, ak)
    print("0x804B/ak ->", [(hex(c), b.hex(), e) for c, b, e in r], flush=True)
    nonce = None
    for c2, body, enc in r:
        if c2 == 0x004C:
            print("  decrypt-with-ak:", try_dec(ak, body), flush=True)
            res = try_dec(ak, body)
            if res and res[0] == "CRC":
                nonce = res[1]
    if nonce is None:
        for c2, body, enc in r:
            if c2 == 0x004C:
                nonce = bytes.fromhex(body.hex())
    print("nonce:", nonce.hex() if nonce else None, "len", len(nonce) if nonce else None, flush=True)
    if not nonce:
        await w.disconnect(); return

    cands = {
        "sha(n16+ak)": hashlib.sha256(nonce[:16] + ak).digest()[:16],
        "sha(n32+ak)": hashlib.sha256(nonce + ak).digest()[:16],
        "sha(n16+secret)": hashlib.sha256(nonce[:16] + secret).digest()[:16],
        "sha(n32+secret)": hashlib.sha256(nonce + secret).digest()[:16],
        "sha(ak+n16)": hashlib.sha256(ak + nonce[:16]).digest()[:16],
    }
    for tag, k in cands.items():
        r = await roundtrip(w, 0x804B, k)
        info = []
        for c2, body, enc in r:
            info.append((hex(c2), "withcand=" + str(try_dec(k, body)), "withak=" + str(try_dec(ak, body))))
        print(f"{tag} {k.hex()} -> {info}", flush=True)

    await w.disconnect()


asyncio.run(main())
