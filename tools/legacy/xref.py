import struct
import sys

BASE = 0x10000000
P = r"D:\upitest\firmware\tree\app.bin"
b = open(P, "rb").read()

TARGETS = [
    ("system_app_launch", 0x1FD5D8),
    ("already_safemode", 0x1FD5C1),
    ("ota_state", 0x1FE1EC),
    ("ota_app_prosess", 0x1FD047),
    ("ota_type_not_factory", 0x1FE1CF),
    ("OTA_UPG_TYPE", 0x1FE236),
    ("upgrade_retry_verify", 0x240883),
    ("file_verify_failed", 0x24073F),
    ("file_verify_pass", 0x240756),
    ("sdfs_verify_fail", 0x228AF9),
    ("sdfs_fsystem_verify", 0x228BBD),
    ("ota_force_reboot", 0x1FE361),
    ("ota_app_start", 0x1FE2EA),
]

for name, off in TARGETS:
    addr = BASE + off
    needle = struct.pack("<I", addr)
    hits = []
    i = b.find(needle)
    while i >= 0 and len(hits) < 12:
        hits.append(i)
        i = b.find(needle, i + 1)
    print(f"{name:<22} str@{off:#08x} addr={addr:#010x} literal_hits={[hex(h) for h in hits]}")
