#!/usr/bin/env python3
"""Redact device identifiers and personal data from files before publishing.

Replaces the MAC address, serial number, processor id, pairing secret and authkeys
of the reference device with neutral placeholders. Text files only; binary files are
copied unchanged.
"""

from __future__ import annotations

import sys
from pathlib import Path

REPLACEMENTS = [
    # MAC, in the forms it appears in
    ("3C:B0:ED:46:2C:32", "AA:BB:CC:DD:EE:FF"),
    ("3c:b0:ed:46:2c:32", "AA:BB:CC:DD:EE:FF"),
    ("3C-B0-ED-46-2C-32", "AA-BB-CC-DD-EE-FF"),
    ("3cb0ed462c32", "aabbccddeeff"),
    ("3CB0ED462C32", "AABBCCDDEEFF"),
    ("CMF Watch Pro 2-2C32", "CMF Watch Pro 2-EEFF"),
    ("Pro 2-2c32", "Pro 2-eeff"),
    # serial number
    ("CI04112537002117", "SERIAL000000000"),
    # processor id
    ("34f72851", "00000000"),
    # pairing secret and authkeys
    ("5bd451a60ee448c085ea755b98161884", "SECRET00000000000000000000000000"),
    ("5bd451a6", "SECRET00"),
    ("e6a37014f147a98792bff69b82d269f5", "AUTHKEY0000000000000000000000000"),
    ("e6a37014", "AUTHKEY00"),
    ("a3284ab6894e28301ba094f892fcd9e3", "AUTHKEY1111111111111111111111111"),
    ("a3284ab6", "AUTHKEY11"),
]

TEXT_SUFFIXES = {
    ".py", ".sh", ".md", ".json", ".txt", ".tsv", ".csv", ".log", ".cmd", ".ps1",
    ".java", ".toml", ".yml", ".yaml", ".geojson", ".xml", ".html", ".cfg", ".ini",
}


def redact_text(text: str) -> str:
    for needle, replacement in REPLACEMENTS:
        text = text.replace(needle, replacement)
    return text


def decode(data: bytes) -> tuple[str, str]:
    if data.startswith(b"\xff\xfe") or data.startswith(b"\xfe\xff"):
        return data.decode("utf-16", errors="replace"), "utf-16"
    return data.decode("utf-8", errors="replace"), "utf-8"


def main(paths: list[str]) -> int:
    changed = 0
    scanned = 0
    targets: list[Path] = []
    for raw in paths:
        path = Path(raw)
        if path.is_dir():
            targets.extend(p for p in path.rglob("*") if p.is_file())
        elif path.exists():
            targets.append(path)
    for path in targets:
        if path.suffix.lower() not in TEXT_SUFFIXES:
            continue
        scanned += 1
        try:
            data = path.read_bytes()
        except OSError:
            continue
        text, encoding = decode(data)
        new = redact_text(text)
        if new != text:
            path.write_bytes(new.encode(encoding))
            changed += 1
    print(f"scanned {scanned} files, redacted {changed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
