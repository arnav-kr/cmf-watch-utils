#!/usr/bin/env python3
"""Redact device identifiers and personal data from files before publishing.

The identifiers themselves are never stored in the repository. They are read from a
local map that is git-ignored, for example ``internal/redactions.json``:

    {
      "mac": ["11:22:33:44:55:66"],
      "name_suffix": ["CMF Watch Pro 2-5566"],
      "serial": ["SERIALEXAMPLE0001"],
      "pid": ["89abcdef"],
      "secret": ["0123456789abcdef0123456789abcdef"],
      "authkey": ["fedcba9876543210fedcba9876543210"],
      "literals": {"some unique string": "PLACEHOLDER"}
    }

MAC addresses are expanded to colon, dash, compact and name-suffix forms. Text files are
decoded as UTF-16 when they carry a BOM and as UTF-8 otherwise; binary files are skipped.
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

PLACEHOLDER_MAC = "AA:BB:CC:DD:EE:FF"
PLACEHOLDER_SERIAL = "SERIAL000000000"
PLACEHOLDER_PID = "00000000"
PLACEHOLDER_SECRET = "SECRET00000000000000000000000000"
PLACEHOLDER_AUTHKEY = "AUTHKEY0000000000000000000000000"

DEFAULT_MAP = Path(__file__).resolve().parent.parent / "internal" / "redactions.json"

TEXT_SUFFIXES = {
    ".py", ".sh", ".md", ".json", ".txt", ".tsv", ".csv", ".log", ".cmd", ".ps1",
    ".java", ".toml", ".yml", ".yaml", ".geojson", ".xml", ".html", ".cfg", ".ini",
}


def mac_forms(mac: str) -> list[str]:
    digits = "".join(ch for ch in mac if ch in "0123456789abcdefABCDEF")
    if len(digits) != 12:
        return [mac]
    upper = digits.upper()
    lower = digits.lower()
    colon = ":".join(upper[i:i + 2] for i in range(0, 12, 2))
    dash = "-".join(upper[i:i + 2] for i in range(0, 12, 2))
    suffix = lower[-4:]
    return [
        colon,
        colon.lower(),
        dash,
        dash.lower(),
        lower,
        upper,
        f"-{suffix.upper()}",
        f"-{suffix}",
    ]


def load_replacements(path: Path) -> list[tuple[str, str]]:
    data = json.loads(path.read_text(encoding="utf-8"))
    replacements: list[tuple[str, str]] = []

    for mac in data.get("mac", []):
        for form in mac_forms(mac):
            replacements.append((form, PLACEHOLDER_MAC))
        digits = "".join(ch for ch in mac if ch in "0123456789abcdefABCDEF")
        if len(digits) == 12:
            replacements.append((digits[-4:].lower(), PLACEHOLDER_MAC[-4:].lower()))

    for value in data.get("name_suffix", []):
        suffix = value[-4:]
        replacements.append((value, value[: -len(suffix)] + "EEFF"))
        replacements.append((value.lower(), value.lower()[: -len(suffix)] + "eeff"))

    for value in data.get("serial", []):
        replacements.append((value, PLACEHOLDER_SERIAL))

    for value in data.get("pid", []):
        replacements.append((value, PLACEHOLDER_PID))

    for value in data.get("secret", []):
        replacements.append((value, PLACEHOLDER_SECRET))

    for value in data.get("authkey", []):
        replacements.append((value, PLACEHOLDER_AUTHKEY))
        replacements.append((value[:8], PLACEHOLDER_AUTHKEY[:8]))

    for needle, replacement in data.get("literals", {}).items():
        replacements.append((needle, replacement))

    replacements.sort(key=lambda pair: len(pair[0]), reverse=True)
    return replacements


def redact_text(text: str, replacements: list[tuple[str, str]]) -> str:
    for needle, replacement in replacements:
        text = text.replace(needle, replacement)
    return text


def decode(data: bytes) -> tuple[str, str]:
    if data.startswith(b"\xff\xfe") or data.startswith(b"\xfe\xff"):
        return data.decode("utf-16", errors="replace"), "utf-16"
    return data.decode("utf-8", errors="replace"), "utf-8"


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("paths", nargs="+", help="files or directories to redact in place")
    parser.add_argument(
        "--map",
        type=Path,
        default=DEFAULT_MAP,
        help=f"JSON file with the identifiers to replace (default: {DEFAULT_MAP})",
    )
    args = parser.parse_args(argv)

    if not args.map.exists():
        print(
            f"redaction map not found: {args.map}\n"
            "Create it locally; it is ignored by git and must never be committed.",
            file=sys.stderr,
        )
        return 2

    replacements = load_replacements(args.map)
    changed = 0
    scanned = 0
    targets: list[Path] = []
    for raw in args.paths:
        path = Path(raw)
        if path.is_dir():
            targets.extend(p for p in path.rglob("*") if p.is_file())
        elif path.exists():
            targets.append(path)
        else:
            print(f"skipping missing path: {path}", file=sys.stderr)

    for path in targets:
        if path.suffix.lower() not in TEXT_SUFFIXES:
            continue
        scanned += 1
        try:
            data = path.read_bytes()
        except OSError:
            continue
        text, encoding = decode(data)
        new = redact_text(text, replacements)
        if new != text:
            path.write_bytes(new.encode(encoding))
            changed += 1

    print(f"scanned {scanned} files, redacted {changed}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
