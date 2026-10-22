import asyncio
import hashlib
import sys
import time

sys.path.insert(0, r"D:\upitest\watools")
import cmf_watch as cw

ADDR = "AA:BB:CC:DD:EE:FF"


def xor(a, b):
    return bytes(x ^ y for x, y in zip(a, b))


async def probe(w, key, tag, tmo=1.8):
    w.key = key
    try:
        await w.client.write_gatt_char(
            cw.CMD_W, cw.build_frame(0xFFFF, 0x804D, b"\xa5", key=key), response=True)
    except Exception as e:
        print(f"  {tag}: write err {e}", flush=True)
        return None
    end = time.monotonic() + tmo
    frames = []
    while time.monotonic() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=end - time.monotonic())
        except (asyncio.TimeoutError, ValueError):
            break
        for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
            frames.append((c2, body, enc))
    print(f"  {tag:28s} key={key.hex()} -> {[(hex(c), b.hex(), e) for c, b, e in frames]}", flush=True)
    return frames


async def main():
    w = cw.Watch(address=ADDR, verbose=False)
    await w.connect(10.0)

    st = cw.load_state()
    ak = bytes.fromhex(st["authkey"])
    secret = bytes.fromhex(st["secret"])

    w.authkey = ak
    w.key = ak
    r = await w.send(0xFFFF, 0x8049, b"\xa5" + b"CMF Watch Pro 2", expect=0x0049)
    print("0x8049 ->", [(hex(c), b.hex()) for _, c, b in r], flush=True)
    nonce = None
    r = await w.send(0xFFFF, 0x804B, b"\xa5", expect=0x004C, timeout=4.0)
    print("0x804B ->", [(hex(c), b.hex()) for _, c, b in r], flush=True)
    for _, c, b in r:
        if c == 0x004C:
            nonce = b
    if not nonce:
        print("no nonce"); await w.disconnect(); return
    print("nonce len", len(nonce), nonce.hex(), flush=True)
    n16, n32 = nonce[:16], nonce
    stored = bytes.fromhex("f9ca45693c74239088bdb6d6324a99dc")

    cands = {
        "cur sha(n32+ak)": hashlib.sha256(n32 + ak).digest()[:16],
        "sha(n16+ak)": hashlib.sha256(n16 + ak).digest()[:16],
        "sha(n16+secret)": hashlib.sha256(n16 + secret).digest()[:16],
        "sha(n32+secret)": hashlib.sha256(n32 + secret).digest()[:16],
        "sha(ak+n16)": hashlib.sha256(ak + n16).digest()[:16],
        "sha(secret+n16)": hashlib.sha256(secret + n16).digest()[:16],
        "sha(n16)": hashlib.sha256(n16).digest()[:16],
        "authkey": ak,
        "secret16": secret[:16],
        "stored": stored,
        "ak^secret": xor(ak, secret[:16]),
        "ak^n16": xor(ak, n16),
    }
    for tag, k in cands.items():
        await probe(w, k, tag)

    await w.disconnect()


asyncio.run(main())
