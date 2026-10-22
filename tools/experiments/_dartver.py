import re, sys

for p in [r'D:\upitest\nothingx\lib\libapp.so', r'D:\upitest\nothingx\lib\libflutter.so']:
    data = open(p, 'rb').read()
    print('====', p, len(data))
    # Dart version strings
    for m in re.finditer(rb'Dart VM version[^\x00]{0,60}', data):
        print('  vmver:', m.group()[:80])
    for m in re.finditer(rb'(\d+\.\d+\.\d+(?:[-+][\w.]+)?)\s*\((stable|beta|dev|main|be\))[^\x00]{0,40}', data):
        print('  ver:', m.group()[:80])
    # features string
    for m in re.finditer(rb'product\s+no-code_comments[^\x00]{0,200}', data):
        print('  feats:', m.group()[:200])
        break
    # snapshot hash / engine
    for m in re.finditer(rb'[0-9a-f]{32}', data):
        s = m.group()
        if b'0' * 8 not in s:
            pass
    # look for 'stable' short strings
    idx = 0
    cnt = 0
    for m in re.finditer(rb'[\x20-\x7e]{6,40}stable[\x20-\x7e]{0,40}', data):
        print('  stable:', m.group()[:80]); cnt += 1
        if cnt > 5: break
