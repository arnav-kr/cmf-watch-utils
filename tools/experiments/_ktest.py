import asyncio
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

CANDS = """
19dd0486dc121b8cb31d294cd4f86ce4
699d7ea24066bce03175e87aa57ab7dc
7d73d21f1bd82c9e5268b6dcf9fde2cb
3071c8717539de5d5353f4c8cd59a032
d903f09703b8f8a5c8aa7863ff9f3088892794a512599a37cb23e9dd327c8006
2776352bc70e1b1bfabc66da7b6abcba
a072ce9deec12178be837ebd53ae9408
577dab1cd69e41a0bf2fc59407238ac7
99c6f234b3cb18000983ec753dd5f9998abf60e2ab4b70d20a4a66daebc7c884
b9c117b1914d1104a21aee62003b2bc2
6AD69601008900015B0936C1CFAC216C
85d56b048933437e893493173de2eb23
7cb11e567dda407b8ae172469e48476e
21265E7CE3696FF67A2CF572EC79C5D663E0A9FDDBC27E900A9198B0546AD121
7bbcb87944ce5b6146463be5db22a464
eb9c28ed1abc4b5a23bfd2b162935190
dd0bff439fbd73a4187d4e9f02e91374
e334b450c243a87f99bed0ccfe65ec27
4751d01e4e3fd15bc0ac0d0517c1a5eb
8AD81BCA316C5D037901E53464B29D1F
55099bcbafcefc1d3b95c55e7f282fe0
5d6c45d5bdce14d5eb795506597cb54b
fb458413ce5972f3700bbe6d6701fe80
e31de20566b887d2773b744738562ffe
6076de75ee63aac74c43a58a7023fbc7
835dce8729e878ac21ce6dd3bc5d5446
0de1c452916ab6bb8a1b2dbf44b07a0f
450c0c495bd2eff84a41ec0ee5ba6f93
45cd31bef4d60767cc6c74699c4e7791
9aa23ed477b87bb80867808a2f1a9bd2
806054db98414524b66e2a1de19f01dd
83fae7ee5e7a0508a6e253a33c6cf366
0a79d412061b8664e7682069cc32feed
005fd0f56a111d6da3904ffe550562cc
6863db61aadd16fc236a749b0bdacc1f
3118476ae584e3ca6ad34111683cef81
2dbdb722d43eb11fd8ede65610691842
1722aadb21e1758801b67ef49dc29168
8f016084db97fc8a94b0b4156d68cabf
5b02f39a1fa3298b01a4473e7b06a161
d7bb741fdf0f561828ecc039580c3682
4e22c061c44251bd94c107e304b25a05
66a25093695df097415378ec01523660
14e811c416f47ec4b797893af7b37f27
0457413b8f43799dd379c1a36d17f8d3
65af44317d057892a2fff7d4d675911c
""".split()


def check(pt):
    if not pt:
        return None
    p = pt[-1]
    if 1 <= p <= 16:
        d = pt[:-p]
        if len(d) >= 4:
            want = struct.pack("<I", zlib.crc32(d[:-4]) & 0xFFFFFFFF)
            if want == d[-4:]:
                return ("CRC+PKCS7", d[:-4])
        return ("PKCS7", d)
    return None


def try_key(name, key, a052, hits):
    if len(key) not in (16, 24, 32):
        return
    try:
        pt = AES.new(key, AES.MODE_CBC, IV).decrypt(a052)
    except Exception:
        return
    res = check(pt)
    if res:
        hits.append((name, key.hex(), res[0], res[1].hex()))
        print(f"  *** {name} key={key.hex()} {res[0]} pt={res[1].hex()}")


async def main():
    w = cw.Watch(address=ADDR, verbose=True)
    await w.connect(10.0)
    st = cw.load_state()
    ak = bytes.fromhex(st["authkey"])
    w.secret = bytes.fromhex(st["secret"])
    await w.reconnect(ak)
    nonce = getattr(w, "nonce", None)
    print("nonce=", nonce.hex() if nonce else None, "sessionKey=", w.key.hex() if w.key else None)

    frame = cw.build_frame(0xFFFF, 0x9052, b"\xa5", key=w.key)
    await w.client.write_gatt_char(cw.CMD_W, frame, response=True)
    a052 = None
    deadline = time.monotonic() + 4.0
    while time.monotonic() < deadline and a052 is None:
        await asyncio.sleep(0.05)
        while not w.rx.empty():
            for c1, c2, cc, ci, body, enc in w.parser.feed(w.rx.get_nowait()):
                print(f"< {c1:#06x},{c2:#06x} enc={enc} {body.hex()}")
                if c2 == 0xA052:
                    a052 = body
    print("A052=", a052.hex() if a052 else None)
    if not a052:
        return

    hits = []
    print("\n--- testing memory-scan candidates as K1 ---")
    for c in CANDS:
        try:
            k1b = bytes.fromhex(c)
        except ValueError:
            continue
        if nonce:
            for nlen in (16, 32):
                base = nonce[:nlen]
                for klen in (16, 32):
                    s = hashlib.sha256(base + k1b).digest()[:klen]
                    try_key(f"sha256(n{nlen}+K1[{c[:8]}])[:{klen}]", s, a052, hits)
        try_key(f"K1raw[{c[:8]}]", k1b, a052, hits)
        try_key(f"sha256(K1[{c[:8]}])[:16]", hashlib.sha256(k1b).digest()[:16], a052, hits)
        try_key(f"sha256(K1[{c[:8]}])", hashlib.sha256(k1b).digest(), a052, hits)

    print("\n--- extras ---")
    for name, key in (("authkey", ak), ("secret", w.secret),
                      ("sha256(nonce[:16]+secret)[:16]", hashlib.sha256(nonce[:16] + w.secret).digest()[:16] if nonce else b""),
                      ("sha256(nonce[:16]+ak)[:16]", hashlib.sha256(nonce[:16] + ak).digest()[:16] if nonce else b"")):
        try_key(name, key, a052, hits)

    print(f"\nHITS={len(hits)}")
    try:
        await w.disconnect()
    except Exception:
        pass


asyncio.run(main())
