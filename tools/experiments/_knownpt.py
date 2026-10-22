import hashlib
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

IV = bytes.fromhex("5051525354555657606162636465665A")

# app's captured INIT2 ciphertext body (cmd2 0x9040) -> plaintext should be 0100005a + crc32_le
CT = bytes.fromhex("dcc33985b240914612e3bc6c29a38711")
PT_HEAD = bytes.fromhex("0100005a")

authkey = bytes.fromhex("4ef8edd042374775acb25f503e0e2be4")
secret = bytes.fromhex("SECRET00000000000000000000000000")
bindkey = bytes.fromhex("5051525354555657606162636465665a")
stored = bytes.fromhex("f9ca45693c74239088bdb6d6324a99dc")

cands = {
    "authkey": authkey,
    "secret": secret,
    "bindkey": bindkey,
    "stored_session": stored,
    "sha256(authkey)": hashlib.sha256(authkey).digest(),
    "sha256(secret)": hashlib.sha256(secret).digest(),
    "md5(authkey)": hashlib.md5(authkey).digest(),
    "md5(secret)": hashlib.md5(secret).digest(),
}

def crc32_le(b):
    return (hashlib.new("crc32", b) if False else None)

import zlib
def crc_le(b):
    return (zlib.crc32(b) & 0xffffffff).to_bytes(4, "little")

print("expected plaintext:", (PT_HEAD + crc_le(PT_HEAD)).hex())
print("expected sizes:", len(PT_HEAD + crc_le(PT_HEAD)), "->", "9 bytes (+PKCS7 -> 16)")
print()

for name, k in cands.items():
    for mode in (16, 24, 32):
        if len(k) != mode:
            continue
        try:
            pt = AES.new(k, AES.MODE_CBC, IV).decrypt(CT)
        except Exception as e:
            print(f"{name:20s} keylen={len(k):2d} ERROR {e}")
            continue
        head_ok = pt.startswith(PT_HEAD)
        crc_ok = len(pt) >= 9 and pt[4:8] == crc_le(pt[:4])
        pad_ok = False
        try:
            unpad(pt, 16); pad_ok = True
        except Exception:
            pass
        tag = ""
        if head_ok: tag += " HEAD"
        if crc_ok: tag += " CRC"
        if pad_ok: tag += " PAD"
        print(f"{name:20s} keylen={len(k):2d} -> {pt.hex()}{'  <<<' + tag if tag else ''}")
        if head_ok or crc_ok:
            print("   *** PLAUSIBLE ***")

# also try the raw ciphertext if it were AES-ECB
print()
for name, k in cands.items():
    if len(k) not in (16, 24, 32):
        continue
    pt = AES.new(k, AES.MODE_ECB).decrypt(CT)
    if pt.startswith(PT_HEAD):
        print(f"ECB {name} -> {pt.hex()}  ***")

# and the "0xA5" INIT1 payload / guess: maybe field_37 is header+payload, try zlib-less
