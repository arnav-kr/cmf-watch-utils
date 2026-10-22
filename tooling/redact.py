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
    ("AA:BB:CC:DD:EE:FF", "AA:BB:CC:DD:EE:FF"),
    ("AA:BB:CC:DD:EE:FF", "AA:BB:CC:DD:EE:FF"),
    ("AA-BB-CC-DD-EE-FF", "AA-BB-CC-DD-EE-FF"),
    ("aabbccddeeff", "aabbccddeeff"),
    ("AABBCCDDEEFF", "AABBCCDDEEFF"),
    ("CMF Watch Pro 2-EEFF", "CMF Watch Pro 2-EEFF"),
    ("Pro 2-eeff", "Pro 2-eeff"),
    # serial number
    ("SERIAL000000000", "SERIAL000000000"),
    # processor id
    ("00000000", "00000000"),
    # pairing secret and authkeys
    ("SECRET00000000000000000000000000", "SECRET00000000000000000000000000"),
    ("SECRET00", "SECRET00"),
    ("AUTHKEY0000000000000000000000000", "AUTHKEY0000000000000000000000000"),
    ("AUTHKEY00", "AUTHKEY00"),
    ("AUTHKEY1111111111111111111111111", "AUTHKEY1111111111111111111111111"),
    ("AUTHKEY11", "AUTHKEY11"),
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
