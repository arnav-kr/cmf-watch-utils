---
title: Shell / AT channel
description: The debug shell, AT command family and pairing/secret retrieval.
---

The watch exposes a debug shell over BLE UUIDs `77d4ff01-...` (write) and `77d4ff02-...`
(notify). The payload is plain text, usually terminated by `\r\n`; replies end in `,OK` or an
error string.

## Identity and status

| Command | Example reply |
|---|---|
| `AT GETVERSION` | `GETVERSION:1.0.0.73,OK` |
| `AT GETPID` | `GETPID:<8 hex digits>,OK` |
| `AT GETSN` | `GETSN:<serial>,OK` |
| `AT GETNAME` | `GETNAME:CMF Watch Pro 2-XXXX,OK` |
| `AT GETBATLV` | `GETBATLV:3850mv,OK` |
| `AT GETNTCTEMP` | `GETNTCTEMP:29,OK` |
| `AT GETGSENSOR` | `GETGSENSOR:X=0.38 Y=0.01 Z=0.88,OK` |
| `AT GETSECRET` | `GETSECRET:<32 hex>,OK` |

`GETSECRET` returns the pairing secret used by the BLE auth derivation. It has the
sensitivity of a password. This repository does not include any real value.

## Factory / debug commands

The shell also exposes factory and debug entry points (`ENTERFATEMODE`, `SETKNOBDUMP`,
`GETFLASH`, motor/sensor setters, etc.). The full registry and app addresses are in
[BLE protocol](/devices/cmf-watch-pro-2/protocol/ble/) and
[app.bin internals](/devices/cmf-watch-pro-2/internals/appbin/).

Some factory-menu options are destructive to app state. In particular, a shutdown/reset option
cleared pairing data on the reference unit, which invalidated the stored authkey. Re-pairing is
required afterwards.

## Shell usage

```bash
uv run python tools/cmf_watch.py at "AT GETVERSION"
uv run python tools/cmf_watch.py info
```

The `info` command reads version, serial, name, PID, battery, temperature and the accelerometer.
Serial and MAC are personal data; raw output does not belong in issues or commits.

## Notes

- The shell is a normal authenticated channel, not a root shell.
- `AT GETFLASH` reports flash size, but there is no general file-read command.
- The UART shell exists on the physical debug pads (`nand logic read/write`, `devboot`, etc.);
  it is not reachable over BLE and requires hardware access.
