import sys

from capstone import CS_ARCH_ARM, CS_MODE_THUMB, Cs
from capstone.arm import ARM_OP_MEM, ARM_OP_REG
from capstone.arm import ARM_REG_PC

BASE = 0x10000000
P = r"D:\upitest\firmware\tree\app.bin"
b = open(P, "rb").read()

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
md.detail = True


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


TARGETS = {
    0x10154DE8, 0x10154E30, 0x101562E8, 0x10156270,
    0x10155FBC, 0x10155FC4, 0x101562E4, 0x10153B60,
}


def dis(start, end, only_targets=False):
    for ins in md.disasm(b[start:end], BASE + start):
        note = ""
        tgt = None
        for op in ins.operands:
            if op.type == ARM_OP_MEM and op.mem.base == ARM_REG_PC:
                tgt = ((ins.address + 4) & ~3) + op.mem.disp
                s = cstr(tgt)
                note = f"  ; [pc] -> {tgt:08x}"
                if s:
                    note += f'  "{s}"'
        hit = tgt in TARGETS
        if (only_targets and hit) or (not only_targets and tgt is not None):
            raw = b[ins.address - BASE: ins.address - BASE + ins.size]
            mark = " ***" if hit else ""
            print(f"{ins.address:08x}  {raw.hex():<10} {ins.mnemonic:<8} {ins.op_str}{note}{mark}")


if __name__ == "__main__":
    s = int(sys.argv[1], 0)
    e = int(sys.argv[2], 0)
    only = len(sys.argv) > 3 and sys.argv[3] == "t"
    dis(s, e, only)
