import base64
import os
import re
import sys

ROOTS = [
    r"C:\Users\arnav\AppData\Local\Google\Chrome\User Data",
    r"C:\Users\arnav\AppData\Local\Microsoft\Edge\User Data",
    r"C:\Users\arnav\AppData\Local\BraveSoftware\Brave-Browser\User Data",
    r"C:\Users\arnav\AppData\Local\Chromium\User Data",
]

HEX32 = re.compile(rb"[0-9a-fA-F]{32}")


def scan_file(path):
    try:
        data = open(path, "rb").read()
    except OSError:
        return []
    hits = []
    for m in re.finditer(rb"fmc_authkey", data):
        tail = data[m.end():m.end() + 200]
        found = None
        for hm in HEX32.finditer(tail):
            found = hm.group().decode()
            break
        hits.append((path, found, tail[:120]))
    for m in re.finditer(rb"fmc_authkey", data):
        pass
    return hits


def main():
    total = 0
    for root in ROOTS:
        if not os.path.isdir(root):
            continue
        for dirpath, _dirs, files in os.walk(root):
            if "leveldb" not in dirpath.lower() and "Local Storage" not in dirpath:
                continue
            for f in files:
                if not (f.endswith(".ldb") or f.endswith(".log")):
                    continue
                p = os.path.join(dirpath, f)
                for path, hexkey, tail in scan_file(p):
                    total += 1
                    print(f"FOUND in {path}")
                    print(f"  authkey = {hexkey}")
                    print(f"  raw tail = {tail!r}")
    if not total:
        print("no fmc_authkey in any browser Local Storage leveldb")


if __name__ == "__main__":
    main()
