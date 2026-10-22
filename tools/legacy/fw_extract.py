import sys

def hexdump(b, base, n=256):
    for off in range(0, min(len(b), n), 16):
        chunk = b[off:off + 16]
        hx = " ".join(f"{x:02x}" for x in chunk)
        asc = "".join(chr(x) if 32 <= x < 127 else "." for x in chunk)
        print(f"{base+off:08x}  {hx:<47}  {asc}")

def main():
    p = sys.argv[1]
    b = open(p, "rb").read()
    print("=== ota.xml manifest region (0x200 .. 0xb40) ===")
    seg = b[0x200:0xb40]
    txt = seg.decode("latin1")
    import re
    txt = re.sub(r"[^\x09\x0a\x0d\x20-\x7e]", lambda m: "." if m.group() != "\n" else "\n", txt)
    print(txt)
    print("=== 0x3a00 .. 0x3b80 (possible ASN.1/key) ===")
    hexdump(b[0x3a00:0x3b80], 0x3a00, 0x180)

if __name__ == "__main__":
    main()
