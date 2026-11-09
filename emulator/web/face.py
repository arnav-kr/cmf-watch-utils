import hashlib
import struct

from fwkit import lz4, sdfs, wfdl

PAGE = 2048
RAW_PAGE = 2112
PAGES_PER_BLOCK = 64
FONT_BLOCK = 0x34
FONT_BLOCKS = 96
MAX_FACE = 3021359


def font_pages(nand):
    if len(nand) != RAW_PAGE * 65536:
        raise ValueError("Expected the prepared Watch Pro 2 NAND image.")
    blocks = {}
    for offset in range(0, len(nand), RAW_PAGE * PAGES_PER_BLOCK):
        tag = nand[offset + PAGE:offset + PAGE + 8]
        logical = int.from_bytes(tag[6:8], "little")
        if tag[:6] not in (b"\xff\xff\x00\xff\0\0", b"\xff\xff\x01\xff\0\0"):
            continue
        if FONT_BLOCK <= logical < FONT_BLOCK + FONT_BLOCKS:
            if logical in blocks:
                raise ValueError("Ambiguous font partition in virtual NAND.")
            blocks[logical] = offset
    for logical in range(FONT_BLOCK, FONT_BLOCK + FONT_BLOCKS):
        if logical not in blocks:
            break
        base = blocks[logical]
        for page in range(PAGES_PER_BLOCK):
            offset = base + page * RAW_PAGE
            if page and nand[offset + PAGE:offset + PAGE + 8] != struct.pack(
                "<4sHH", b"\xff\xff\x00\x55", page, logical
            ):
                return
            yield offset


def face_patch(nand, face):
    if not 0 < len(face) <= MAX_FACE:
        raise ValueError("Choose a Watch Pro 2 face up to 3,021,359 bytes.")
    parsed = wfdl.parse(face)
    textures = set()
    for resource in parsed.resources:
        if not (0 < resource.w <= 466 and 0 < resource.h <= 466):
            raise ValueError("Watch face contains an unsupported texture size.")
        if resource.cf == 5:
            pixels = lz4.decompress(resource.data, resource.raw_size)
            if len(pixels) != resource.raw_size:
                raise ValueError("Invalid packed watch-face texture.")
            textures.add(f"{resource.w}:{resource.h}:{hashlib.sha256(pixels).hexdigest()}")

    pages = list(font_pages(nand))
    original = b"".join(nand[offset:offset + PAGE] for offset in pages)
    if original[:8] != sdfs.MAGIC or len(original) < PAGE:
        raise ValueError("Missing stock font and watch-face partition.")
    count, size = struct.unpack_from("<II", original, 12)
    if not 0 < count <= 64 or size > len(original):
        raise ValueError("Invalid stock font partition.")
    original = original[:size]
    entries = sdfs.parse(original)
    if any(entry.off + entry.size > size or not sdfs.verify(original, entry) for entry in entries):
        raise ValueError("Stock font partition failed its resource checks.")
    files = sdfs.unpack(original)
    if "local273.wfc" not in files:
        raise ValueError("The default Activity Mood slot is missing.")
    default_face = files["local273.wfc"]
    files["local273.wfc"] = face
    packed = sdfs.pack(list(files.items()))
    if len(packed) > len(pages) * PAGE and "local277.wfc" in files:
        files["local277.wfc"] = default_face
        packed = sdfs.pack(list(files.items()))
    if len(packed) > len(pages) * PAGE:
        raise ValueError("This face exceeds the space in the prepared NAND fixture.")
    index = "".join(f"{texture}\n" for texture in sorted(textures)).encode("ascii")
    patch = bytearray(struct.pack("<I", len(index)) + index)
    for page, offset in enumerate(pages):
        start = page * PAGE
        if start >= len(packed):
            break
        data = packed[start:start + PAGE]
        if data != nand[offset:offset + len(data)]:
            patch.extend(struct.pack("<II", offset, len(data)))
            patch.extend(data)
    return patch
