"""AOTA image unpack/repack for CMF Watch Pro 2 (Actions ATS3089C)."""
import struct
import zlib
import lzma

CHUNK = 0x8000


def sum32(d):
    n = len(d) // 4 * 4
    return sum(struct.unpack("<%dI" % (n // 4), d[:n])) & 0xFFFFFFFF


def align(n, a):
    return (n + a - 1) // a * a


def unpack_chunks(blob):
    out = bytearray()
    pos = 0
    while pos + 16 <= len(blob):
        if blob[pos:pos + 4] != b"LZMA":
            raise ValueError("bad chunk magic at %#x" % pos)
        hl, comp, unc = struct.unpack("<III", blob[pos + 4:pos + 16])
        data = lzma.decompress(blob[pos + 16:pos + 16 + comp], format=lzma.FORMAT_XZ)
        if len(data) != unc:
            raise ValueError("chunk size mismatch")
        out += data
        pos += 16 + comp
    return bytes(out)


def pack_chunks(data, chunk=CHUNK, preset=6):
    out = bytearray()
    for i in range(0, len(data), chunk):
        piece = data[i:i + chunk]
        comp = lzma.compress(piece, format=lzma.FORMAT_XZ, preset=preset)
        out += b"LZMA" + struct.pack("<III", 16, len(comp), len(piece)) + comp
    return bytes(out)


def unpack_sdfs(img):
    if img[:8] != b"sdfs.bin":
        raise ValueError("not sdfs")
    cnt = struct.unpack("<I", img[0x0C:0x10])[0]
    files = []
    for i in range(cnt):
        o = 0x20 + i * 32
        name = img[o:o + 12].split(b"\0")[0].decode()
        off, size = struct.unpack("<II", img[o + 12:o + 20])
        files.append((name, img[off:off + size]))
    return files


def pack_sdfs(files):
    cnt = len(files)
    table = bytearray()
    data_start = align(0x20 + cnt * 32, 0x40)
    body = bytearray()
    offs = []
    cur = data_start
    for name, data in files:
        cur = align(cur, 0x40)
        offs.append(cur)
        body += b"\0" * (cur - (data_start + len(body)))
        body += data
        cur += len(data)
    total = align(data_start + len(body), 0x40)
    body += b"\0" * (total - (data_start + len(body)))
    for (name, data), off in zip(files, offs):
        nm = name.encode()[:12].ljust(12, b"\0")
        table += nm + struct.pack("<IIIII", off, len(data), 0, 0, sum32(data))
    hdr = b"sdfs.bin" + struct.pack("<IIIIII", 0, cnt, total, 0,
                                     sum32(table), sum32(body))
    out = hdr + table + b"\0" * (data_start - 0x20 - cnt * 32) + body
    assert len(out) == total
    return bytes(out)


def parse(img):
    """Return (header_bytes, entries) where entries = [(name, off, size, ck)]."""
    cnt = struct.unpack("<I", img[0x0C:0x10])[0]
    ents = []
    for i in range(cnt):
        e = img[0x200 + i * 32:0x200 + (i + 1) * 32]
        name = e[:16].rstrip(b"\0").decode()
        off, size, z, ck = struct.unpack("<IIII", e[16:32])
        ents.append((name, off, size, ck))
    return img[:0x400], ents


def build(base, stored, xml, agps):
    """stored: ordered list of (name, blob) excluding ota.xml; xml: exact XML bytes."""
    hdr = bytearray(base[:0x400])
    ents = [("ota.xml", 0x400, len(xml), zlib.crc32(xml) & 0xFFFFFFFF)]
    body = bytearray()
    body += xml
    pos = 0x400 + len(xml)
    blobs = []
    for name, blob in stored:
        pos = align(pos, 0x200)
        ents.append((name, pos, len(blob), zlib.crc32(blob) & 0xFFFFFFFF))
        blobs.append((pos, blob))
        pos += len(blob)
    pos = align(pos, 0x200)
    agps_off = pos
    tbl = bytearray()
    for name, off, size, ck in ents:
        tbl += name.encode()[:16].ljust(16, b"\0") + struct.pack("<IIII", off, size, 0, ck)
    hdr[0x200:0x200 + len(tbl)] = tbl
    hdr[0x0C:0x10] = struct.pack("<I", len(ents))
    hdr[0x14:0x18] = struct.pack("<I", agps_off)
    struct.pack_into("<I", hdr, 4, zlib.crc32(bytes(hdr[8:0x400])) & 0xFFFFFFFF)
    out = bytearray(hdr)
    out = out[:0x400]
    out += xml
    for off, blob in blobs:
        out += b"\0" * (off - len(out))
        out += blob
    out += b"\0" * (agps_off - len(out))
    out += agps
    return bytes(out)


def xml_set(xml, fields):
    """fields: dict file_name -> (file_size, orig_size, checksum); None keeps value."""
    import re
    s = xml.decode()
    for name, vals in fields.items():
        m = re.search(r"<file_name>%s</file_name>.*?</partition>" % re.escape(name), s, re.S)
        if not m:
            raise KeyError(name)
        block = m.group(0)
        for tag, val in zip(("file_size", "orig_size", "checksum"), vals):
            if val is None:
                continue
            block = re.sub(r"<%s>0x[0-9a-fA-F]+</%s>" % (tag, tag),
                           "<%s>0x%x</%s>" % (tag, val, tag), block)
        s = s[:m.start()] + block + s[m.end():]
    return s.encode()
