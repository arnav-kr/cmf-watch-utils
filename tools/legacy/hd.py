import sys


def hd(path, base=0, n=512, start=0):
    b = open(path, "rb").read()
    for off in range(start, min(len(b), start + n), 16):
        chunk = b[off:off + 16]
        hx = " ".join(f"{x:02x}" for x in chunk)
        asc = "".join(chr(x) if 32 <= x < 127 else "." for x in chunk)
        print(f"{off:08x}  {hx:<47}  {asc}")


if __name__ == "__main__":
    path = sys.argv[1]
    start = int(sys.argv[2], 0) if len(sys.argv) > 2 else 0
    n = int(sys.argv[3], 0) if len(sys.argv) > 3 else 512
    hd(path, start=start, n=n)
