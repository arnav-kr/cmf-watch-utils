import struct
d = open(r'D:\upitest\firmware\tree\app.bin', 'rb').read()


def w(va):
    return struct.unpack_from('<I', d, va - 0x10000000)[0]


def s(va):
    o = va - 0x10000000
    e = d.find(b'\x00', o)
    return d[o:e].decode('latin1', 'replace')


lits = {
    'DAT_10155468': 0x10155468, 'DAT_10155470': 0x10155470,
    'DAT_10155474': 0x10155474, 'DAT_10155478': 0x10155478,
    'DAT_10155488': 0x10155488, 'DAT_1015548c': 0x1015548c,
    'DAT_10155490': 0x10155490, 'DAT_10155498': 0x10155498,
    'DAT_1015547c': 0x1015547c, 'DAT_10155480': 0x10155480,
    'DAT_10155484': 0x10155484, 'DAT_10155494': 0x10155494,
}
for n, va in lits.items():
    v = w(va)
    extra = ''
    if 0x10200000 <= v < 0x10300000:
        extra = ' = "' + s(v) + '"'
    print(n, hex(v), extra)
