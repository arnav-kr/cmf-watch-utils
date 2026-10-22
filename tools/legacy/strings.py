import re
import sys

path = sys.argv[1]
out = sys.argv[2] if len(sys.argv) > 2 else "strings.txt"
minlen = int(sys.argv[3]) if len(sys.argv) > 3 else 5
b = open(path, "rb").read()
pat = re.compile(rb"[\x20-\x7e]{%d,}" % minlen)
with open(out, "w", encoding="utf-8", errors="replace") as f:
    for m in pat.finditer(b):
        f.write(f"{m.start():08x}  {m.group().decode('latin1')}\n")
print(f"{out}: done ({len(b):#x} bytes scanned)")
