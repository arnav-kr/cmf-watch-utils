import hashlib
import lzma
import struct
import xml.etree.ElementTree as ET

from fwkit import aota, sdfs


def require(condition, message):
    if not condition:
        raise ValueError(message)


def container(data, model):
    require(data.startswith(b"AOTA"), "Invalid AOTA magic")
    header, entries = aota.parse(data)
    require(len(entries) <= (aota.HDR_SIZE - aota.TBL_OFF) // aota.ENTRY_SIZE,
            "Invalid AOTA entry count")
    require(aota.verify_header(header), "AOTA header CRC mismatch")
    start = struct.unpack_from("<H", header, 0x12)[0]
    end, checksum = struct.unpack_from("<II", header, 0x14)
    require(aota.HDR_SIZE <= start <= end <= len(data), "Invalid AOTA data bounds")
    require(aota.crc32(memoryview(data)[start:end]) == checksum, "AOTA data CRC mismatch")
    board = header[0x60:0x7c].split(b"\0", 1)[0].decode("ascii")
    require(board == model.board, f"AOTA board {board!r} does not match {model.board}")
    files = {}
    previous_end = start
    for entry in sorted(entries, key=lambda entry: entry.off):
        require(entry.name not in files and previous_end <= entry.off < entry.off + entry.size <= end
                and entry.raw[24:28] == bytes(4), f"Invalid or overlapping AOTA entry: {entry.name}")
        require(aota.verify_entry(data, entry), f"AOTA file CRC mismatch: {entry.name}")
        files[entry.name] = entry.stored(data)
        previous_end = entry.off + entry.size
    manifest = files.pop("ota.xml", None)
    require(manifest is not None and len(manifest) <= 65536, "Missing or oversized OTA manifest")
    xml_text = manifest.decode("utf-8-sig")
    require("<!" not in xml_text, "Unsupported XML declarations")
    xml = ET.fromstring(xml_text)
    require(xml.findtext("./firmware_version/board_name") == model.board, "OTA manifest board mismatch")
    nodes = xml.findall("./partitions/partition")
    declared = {node.findtext("file_name"): node for node in nodes}
    require(len(nodes) == len(declared) and set(declared) == set(files), "OTA manifest file mismatch")
    for name, node in declared.items():
        for key, expected in (("file_size", len(files[name])), ("orig_size", len(files[name])),
                              ("checksum", aota.crc32(files[name]))):
            require(int(node.findtext(key, ""), 0) == expected, f"Manifest {key} mismatch: {name}")
    return files, declared, end


def destination(nodes, name, partition, storage):
    node = nodes[name]
    require(int(node.findtext("file_id", ""), 0) == partition.file_id and
            int(node.findtext("storage_id", ""), 0) == storage, f"Invalid destination for {name}")


def decompress(data, max_output, allow_padding=False):
    pos = 0
    output = bytearray()
    while data[pos:pos + 4] == b"LZMA":
        require(pos + 16 <= len(data), "Truncated LZMA header")
        header, packed, raw = struct.unpack_from("<III", data, pos + 4)
        end = pos + header + packed
        require(header == 16 and 0 < packed <= 1572864 - header and
                0 < raw <= min(2097152, max_output - len(output)) and end <= len(data),
                "Invalid LZMA chunk bounds")
        decoder = lzma.LZMADecompressor(format=lzma.FORMAT_XZ, memlimit=64 * 1024 * 1024)
        block = decoder.decompress(data[pos + header:end], max_length=raw + 1)
        require(decoder.eof and not decoder.unused_data and len(block) == raw, "Invalid LZMA chunk")
        output.extend(block)
        pos = end
    sector = (pos + 511) & ~511
    require(pos and (pos == len(data) or (allow_padding and
            all(byte in (0, 255) for byte in data[pos:sector]) and
            data[sector:].count(255) == len(data[sector:]))), "Unexpected bytes after LZMA chunks")
    return bytes(output)


def inner_payload(data, model, allow_padding=False):
    if data.startswith(b"LZMA"):
        data = decompress(data, model.app.size + model.sdfs.size + 0x10000, allow_padding)
    files, nodes, end = container(data, model)
    require(end == len(data) and set(files) == {"app.bin", "sdfs.bin"}, "Unexpected recovery payloads")
    app, resources = files["app.bin"], files["sdfs.bin"]
    destination(nodes, "app.bin", model.app, 0)
    destination(nodes, "sdfs.bin", model.sdfs, 0)
    model.validate_app(app)
    size, checksum = struct.unpack_from("<II", app, 24)
    require(app.startswith(b"ACTHHTCA") and 0 < size <= len(app) - 512 and size % 4 == 0,
            "Invalid ACTH application")
    require(aota.sum32(app[:512]) == 0xffffffff and
            (aota.sum32(app[512:512 + size]) + checksum) & 0xffffffff == 0xffffffff,
            "Invalid ACTH checksum")
    require(0 < len(resources) <= model.sdfs.size and resources.startswith(b"sdfs.bin"),
            "Invalid SDFS header or size")
    entries = sdfs.parse(resources)
    previous_end = sdfs.HDR_SIZE + len(entries) * sdfs.ENTRY_SIZE
    names = set()
    for entry in sorted(entries, key=lambda entry: entry.off):
        require(entry.name and entry.name not in names and previous_end <= entry.off
                and entry.off + entry.size <= len(resources), "Invalid or overlapping SDFS entry")
        require(sdfs.verify(resources, entry), f"SDFS checksum mismatch: {entry.name}")
        previous_end = entry.off + entry.size
        names.add(entry.name)
    return app, resources


def verify_package(data, model):
    files, nodes, end = container(data, model)
    if set(files) == {"TEMP.bin"}:
        destination(nodes, "TEMP.bin", model.temp, 2)
        require(len(files["TEMP.bin"]) <= model.temp.size, "TEMP exceeds partition capacity")
        app, resources = inner_payload(files["TEMP.bin"], model)
        trailer = data[end:]
        require(not trailer or trailer == bytes(24) or
                (len(trailer) >= 24 and trailer.startswith(b"AGPS") and
                 struct.unpack_from("<Q", trailer, 8)[0] == len(trailer) - 24), "Invalid outer trailer")
        return app, resources, trailer
    app, resources = inner_payload(data, model)
    return app, resources, b""


def report(data, model):
    app, resources, trailer = verify_package(data, model)
    return {"model": model.name, "image_bytes": len(data), "sha256": hashlib.sha256(data).hexdigest(),
            "app_bytes": len(app), "app_sha256": hashlib.sha256(app).hexdigest(),
            "sdfs_bytes": len(resources), "sdfs_sha256": hashlib.sha256(resources).hexdigest(),
            "agps_trailer_bytes": len(trailer), "agps_trailer_sha256": hashlib.sha256(trailer).hexdigest(),
            "mode": "offline_package_validation"}
