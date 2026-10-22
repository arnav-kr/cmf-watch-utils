import sys

from capstone import CS_ARCH_ARM, CS_MODE_THUMB, Cs

BASE = 0x10000000
P = r"D:\upitest\firmware\tree\app.bin"
b = open(P, "rb").read()

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
md.detail = False


def dis(start, end, base=BASE):
    print(f"--- {start:#x}..{end:#x} (addr {base+start:#x}) ---")
    for ins in md.disasm(b[start:end], base + start):
        raw = b[ins.address - base: ins.address - base + ins.size]
        print(f"{ins.address:08x}  {raw.hex():<8}  {ins.mnemonic:<8} {ins.op_str}")


if __name__ == "__main__":
    s = int(sys.argv[1], 0)
    e = int(sys.argv[2], 0)
    dis(s, e)
