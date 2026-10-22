import sys, re, math, zlib, os

def entropy(b):
    if not b:
        return 0.0
    c = [0] * 256
    for x in b:
        c[x] += 1
    n = len(b)
    return -sum((k / n) * math.log2(k / n) for k in c if k)

def hexdump(b, base=0, n=256):
    for off in range(0, min(len(b), n), 16):
        chunk = b[off:off + 16]
        hx = " ".join(f"{x:02x}" for x in chunk)
        asc = "".join(chr(x) if 32 <= x < 127 else "." for x in chunk)
        print(f"{base+off:08x}  {hx:<47}  {asc}")

def find_strings(b, minlen=6, limit=60, pattern=None):
    out = []
    cur = bytearray()
    start = 0
    for i, x in enumerate(b):
        if 32 <= x < 127:
            if not cur:
                start = i
            cur.append(x)
        else:
            if len(cur) >= minlen:
                s = cur.decode("latin1")
                if pattern is None or re.search(pattern, s):
                    out.append((start, s))
                    if len(out) >= limit:
                        return out
            cur = bytearray()
    return out

def main():
    p = sys.argv[1] if len(sys.argv) > 1 else r"D:\upitest\firmware\wp2_1724161837605-90.bin"
    b = open(p, "rb").read()
    print(f"file      : {p}")
    print(f"size      : {len(b)} (0x{len(b):x})")
    print(f"crc32     : {zlib.crc32(b) & 0xffffffff:08x}")
    print(f"entropy   : full={entropy(b[:2_000_000]):.3f} head1k={entropy(b[:1024]):.3f} tail1k={entropy(b[-1024:]):.3f}")
    print("--- head 256 bytes ---")
    hexdump(b, 0, 256)
    print("--- tail 128 bytes ---")
    hexdump(b[-128:], len(b) - 128, 128)
    print("--- version-like strings ---")
    for off, s in find_strings(b, 4, 40, r"\d+\.\d+\.\d+"):
        print(f"  {off:08x}  {s[:80]}")
    print("--- marker strings ---")
    for pat in [r"CMF", r"Watch", r"GETSECRET", r"GETVERSION", r"BEGIN CERTIFICATE",
                r"Actions", r"ATS\d", r"ota", r"OTA", r"boot", r"BOOT", r"sign", r"Sign", r"SIGN"]:
        hits = find_strings(b, 4, 8, pat)
        for off, s in hits:
            print(f"  [{pat}] {off:08x}  {s[:90]}")
    print("--- asn1/pem key markers ---")
    for marker, name in [(b"0\x82", "PUBKEY/seq candidate"), (b"BEGIN", "PEM"),
                         (b"MIIC", "PKCS8/rsa b64"), (b"MIIB", "rsa b64"), (b"MEkw", "rsa 512")]:
        idx = b.find(marker)
        print(f"  {name:<24} first at {idx:#x}")

if __name__ == "__main__":
    main()
