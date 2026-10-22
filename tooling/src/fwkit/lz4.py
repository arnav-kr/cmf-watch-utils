"""Raw LZ4 block codec (no frame header, no size prefix).

The watch stores UI pictures as bare LZ4 blocks; the firmware calls
``LZ4_decompress_safe(src, dst, src_len, dst_capacity)``.  The vendor uses
LZ4 HC level 12, and ``lz4.block`` reproduces its output byte-for-byte.
A pure-Python greedy encoder is kept as a fallback.
"""

from __future__ import annotations

MIN_MATCH = 4
LAST_LITERALS = 5
MF_LIMIT = 12
HASH_BITS = 16
MAX_DISTANCE = 0xFFFF


def _read_len(src: bytes, i: int) -> tuple[int, int]:
    n = 0
    while True:
        b = src[i]
        i += 1
        n += b
        if b != 255:
            return n, i


def decompress(src: bytes, capacity: int | None = None) -> bytes:
    out = bytearray()
    i, n = 0, len(src)
    while i < n:
        token = src[i]
        i += 1
        lit = token >> 4
        if lit == 15:
            extra, i = _read_len(src, i)
            lit += extra
        if i + lit > n:
            raise ValueError("LZ4: literal run past end")
        out += src[i:i + lit]
        i += lit
        if i >= n:
            break
        if i + 2 > n:
            raise ValueError("LZ4: truncated match offset")
        off = src[i] | (src[i + 1] << 8)
        i += 2
        if off == 0 or off > len(out):
            raise ValueError("LZ4: bad offset %d at %d" % (off, i))
        ml = (token & 0x0F) + MIN_MATCH
        if (token & 0x0F) == 15:
            extra, i = _read_len(src, i)
            ml += extra
        start = len(out) - off
        if ml <= off:
            out += out[start:start + ml]
        else:
            for k in range(ml):
                out.append(out[start + k])
        if capacity is not None and len(out) > capacity:
            raise ValueError("LZ4: output exceeds capacity")
    if capacity is not None and len(out) != capacity:
        raise ValueError("LZ4: output %d != expected %d" % (len(out), capacity))
    return bytes(out)


def _emit_len(out: bytearray, n: int) -> None:
    while n >= 255:
        out.append(255)
        n -= 255
    out.append(n)


def compress(src: bytes) -> bytes:
    """Encode a raw LZ4 block exactly like the vendor (HC level 12)."""
    try:
        import lz4.block as block
    except ImportError:
        return _compress_greedy(src)
    return block.compress(bytes(src), store_size=False,
                          mode="high_compression", compression=12)


def _compress_greedy(src: bytes) -> bytes:
    n = len(src)
    out = bytearray()
    table: dict[int, list[int]] = {}
    anchor = 0
    i = 0

    def match_len(a: int, b: int) -> int:
        k = 0
        while b + k < n - LAST_LITERALS and src[a + k] == src[b + k] and k < 0xFFFF:
            k += 1
        return k

    while i + MF_LIMIT < n:
        key = int.from_bytes(src[i:i + 4], "little")
        cands = table.get(key, ())
        best_len, best_off = 0, 0
        for c in reversed(cands[-4:]):
            off = i - c
            if off <= 0 or off > MAX_DISTANCE:
                continue
            ln = match_len(c, i)
            if ln > best_len:
                best_len, best_off = ln, off
                if ln >= 128:
                    break
        if best_len >= MIN_MATCH:
            lit = i - anchor
            ml = best_len
            token = (min(lit, 15) << 4) | min(ml - MIN_MATCH, 15)
            out.append(token)
            if lit >= 15:
                _emit_len(out, lit - 15)
            out += src[anchor:i]
            out.append(best_off & 0xFF)
            out.append(best_off >> 8)
            if ml - MIN_MATCH >= 15:
                _emit_len(out, ml - MIN_MATCH - 15)
            for p in range(i, i + best_len):
                table.setdefault(int.from_bytes(src[p:p + 4], "little"), []).append(p)
            i += best_len
            anchor = i
        else:
            table.setdefault(key, []).append(i)
            i += 1

    lit = n - anchor
    token = min(lit, 15) << 4
    out.append(token)
    if lit >= 15:
        _emit_len(out, lit - 15)
    out += src[anchor:]
    return bytes(out)
