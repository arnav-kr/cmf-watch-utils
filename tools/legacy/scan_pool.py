import sys

from capstone import CS_ARCH_ARM, CS_MODE_THUMB, Cs
from capstone.arm import ARM_OP_MEM
from capstone.arm import ARM_REG_PC

BASE = 0x10000000
P = r"D:\upitest\firmware\tree\app.bin"
b = open(P, "rb").read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
md.detail = True

lo = int(sys.argv[1], 0) if len(sys.argv) > 1 else 0x10154C00
hi = int(sys.argv[2], 0) if len(sys.argv) > 2 else 0x10156800


def cstr(a):
    fo = a - BASE
    if 0 <= fo < len(b) and b[fo] >= 32:
        e = fo
        while e < len(b) and b[e] != 0:
            e += 1
        if 0 < e - fo < 200:
            try:
                return b[fo:e].decode("ascii")
            except Exception:
                return None
    return None


hits = []
for ins in md.disasm(b, BASE):
    if not ins.operands:
        continue
    for op in ins.operands:
        if op.type == ARM_OP_MEM and op.mem.base == ARM_REG_PC:
            tgt = ((ins.address + 4) & ~3) + op.mem.disp
            if lo <= tgt <= hi:
                s = cstr(tgt)
                hits.append((ins.address, tgt, ins.mnemonic, ins.op_str, s))

for a, tgt, m, o, s in hits:
    print(f"{a:08x}  {m} {o}   -> {tgt:08x}  {('\"'+s+'\"') if s else ''}")
print(f"# hits={len(hits)}")
