---
title: AOTA OTA flow
description: The verified BLE firmware-update sequence, including INIT2 placement and the end acknowledgement.
---

This is the flow that actually committed a full AOTA image on hardware. It is the most
failure-sensitive part of the project, and the order of the steps matters.

## Characteristics

| Role | UUID |
|---|---|
| Command write | `0000fff2-0000-1000-8000-00805f9b34fb` (`CMD_W`) |
| Command notify | `0000fff1-...` (`CMD_N`) |
| OTA write | `02f00000-0000-0000-0000-00000000ff01` |
| OTA notify | `02f00000-0000-0000-0000-00000000ff02` |
| Shell write / notify | `77d4ff01-...` / `77d4ff02-...` |

The watch **pulls** firmware data: after `A040` it sends `A042` requests and the host answers
with `0x9042` frames on the OTA characteristic.

## Prerequisites

1. The watch is on the expected firmware and has enough battery.
2. Re-pairing has happened after any reset/factory-menu clear. A stale authkey can still
   produce `A052`, but `A040` will never arrive.
3. The AOTA image passes `fwkit verify`.
4. The relevant risks are documented on the
   [Limitations & risks](/devices/cmf-watch-pro-2/limitations/) page.

## Sequence

### 1. Reconnect and authenticate

```text
0x8049  A5 + device name          -> 0x0049
0x804B  A5                        -> 0x004C nonce
0x804D  A5                        -> 0x0004
0x8004  time + UTC offset         -> (optional)
```

Session key: `sha256(nonce || authkey)[:16]`, where
`authkey = sha256(random1 || random2 || secret)[:16]`. The secret is read over the shell
channel (`AT GETSECRET`). Frames after authentication are AES-CBC encrypted with the fixed IV
`5051525354555657606162636465665a`, PKCS#7 padding, and a CRC32-LE tail.

### 2. INIT1

```text
CMD_W: FFFF 9052 A5   ->  A052 01
```

`01` means accepted. Any other value indicates a wrong session/pairing state, and the
sequence ends there.

### 3. INIT2 — on the OTA characteristic

```text
OTA (ff01): FFFF 9040 <version:4 bytes>  ->  A040 01
```

**INIT2 on `CMD_W` reboots the watch.** With a valid session it can drop the link before the
data phase. Sent directly on `ff01` it returned `A040` in ~30 ms on the reference unit.

The version argument is a 4-byte little-endian-ish build value from the app; `0100005a` worked
for a `1.0.0.73`-based image. It is not the AOTA header's version string.

### 4. Data phase

The watch sends `A042` on `ff02`. Decrypted body:

```text
offset (u32 BE) | length (u32 BE) | progress/unknown (4 bytes) | 1 byte
```

For each request, the host answers with `ceil(length / 224)` frames of `0x9042` on `ff01`.
Each frame is:

```text
F5 | len(u16) | FFFF | count(u16) | index(u16) | 9042 | 224B plaintext firmware | crc32le(u32)
```

`index` starts at 1 and `count = ceil(length / 224)` **for that request**. The watch may
request non-sequential offsets; the host serves exactly what it asks for, and a single linear
pass is not implied.

### 5. Finish — the step that is easy to miss

The watch can send `A041 01` **before** the final zero-length request. `A041` is the commit
prompt, and the end acknowledgement is sent immediately:

```text
CMD_W: FFFF 9041 A5
```

Waiting only for `A042 offset=0 length=0` can miss it, because the notification may arrive
after `A041`; the safe client handles both and sends `0x9041 A5` on either. Skipping this
leaves the staged update uncommitted in testing.

## Verified run

The successful flash transferred the changed `res_e` range plus metadata, reported `A041 01`,
accepted `0x9041 A5`, then rebooted and played the new animation. A full candidate was ~54 MB;
the actual served range was ~38 MB because unchanged partitions were not requested.

Redacted shape of the end of a successful log:

```text
INIT1 0x9052 (OTA begin)
A052 ok
INIT2 0x9040 version=0100005a
A040 ok, entering data phase
...
served ... (70.4% of image) off=0x323c2d8 len=3680
watch status 0xa041 01; sending 0x9041 END
waiting for post-flash status ...
  <= cmd2=0xa042 000000000000000064
summary: frames=... bytes=... (70.4% of image)
```

The percentage is relative to the whole image, not the requested range; a successful run can
end well below 100%.

## Client usage

A probe that stops at `A040` without sending firmware data:

```bash
uv run python tools/ota_client.py --stop-after-a040 --version 0100005a -v
```

A full flash:

```bash
uv run python tools/ota_client.py \
  --image firmware/custom-image.bin \
  --version 0100005a
```

`-v` adds per-request logs. The client writes a summary even on failure. A disconnect during
the data phase is not automatically recoverable; another flash only starts once the watch is
back in the app and `info` works.

## Common failures

| Symptom | Likely cause |
|---|---|
| `A052` but never `A040` | stale authkey / pairing state, or INIT2 on the wrong characteristic |
| Link drops right after INIT2 | INIT2 was sent on `CMD_W` with a valid session |
| Transfer stops with `A041` and no commit | end acknowledgement `0x9041 A5` was not sent |
| `no frames for Ns` | watch rebooted, aborted, or the host lost the connection |
| Requests jump around | normal; the watch requests ranges, not a linear stream |
