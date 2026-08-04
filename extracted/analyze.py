import re, os

paths = [
    ('DLL', r'c:\Projects\00ClickI\extracted\jackz.click-indicators.dll'),
    ('SO64', r'c:\Projects\00ClickI\extracted\jackz.click-indicators.android64.so'),
    ('DYLIB', r'c:\Projects\00ClickI\extracted\jackz.click-indicators.dylib')
]

for name, path in paths:
    with open(path, 'rb') as f:
        data = f.read()
    urls = re.findall(b'https?://[^\s\x00\"\'<>]+', data)
    print(f'=== {name} URLs ===')
    for u in set(urls):
        print('  ', u.decode('ascii', errors='ignore'))

print("\n=== Log Messages & Format Strings ===")
with open(paths[0][1], 'rb') as f:
    data = f.read()

log_strings = re.findall(b'\[Click [^\]]+\][^\x00]+', data)
for ls in set(log_strings[:30]):
    print('  Log:', ls.decode('ascii', errors='ignore'))