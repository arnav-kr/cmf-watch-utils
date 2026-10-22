import json, re

txt = open(r'D:\upitest\nothingx\dart_strings.json', encoding='utf-8', errors='replace').read()
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
uniq = []
seen = set()
for s in strs:
    if s not in seen:
        seen.add(s)
        uniq.append(s)

pats = re.compile(r'(ota|chunk|firmware|upgrade|dfu|bootload|ffe[0-9a-f]|02f0|efe7|watchface|agps|protocol|packet|partition|verify|crc|sha|\\boffset\\b|\\blength\\b|0x[0-9a-f]{3,4})', re.I)
out = [s for s in uniq if pats.search(s) and len(s) < 200]
print('=== candidate OTA strings:', len(out), 'of', len(uniq), '===')
for s in out[:400]:
    print(repr(s))
