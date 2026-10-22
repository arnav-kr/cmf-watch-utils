import json
import re

p = r'D:\upitest\nothingx\dart_strings.json'
txt = open(p, encoding='utf-8', errors='replace').read()
try:
    data = json.loads(txt)
except Exception:
    data = None


def walk(o):
    if isinstance(o, str):
        yield o
    elif isinstance(o, list):
        for x in o:
            yield from walk(x)
    elif isinstance(o, dict):
        for x in o.values():
            yield from walk(x)


strs = list(walk(data)) if data is not None else re.findall(r'"([^"]*)"', txt)
pat = re.compile(r'segment|seg_size|req_size|chunkLen|chunk_len|payloadLen|mtu|startFirmware|'
                 r'FirmwareUpgrade|otaStart|otaSend|writeFirmware|protocol5|otaData|otaChunk|'
                 r'offset|INIT2|init2|firmware_ota|fw_ota|ota_fw', re.I)
seen = []
for s in strs:
    if pat.search(s) and s not in seen:
        seen.append(s)
for s in seen[:120]:
    print(repr(s))
print('total', len(seen))
