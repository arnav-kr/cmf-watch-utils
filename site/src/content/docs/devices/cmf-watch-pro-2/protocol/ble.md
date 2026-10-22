---
title: BLE protocol
description: Frame format, authentication, AES/CRC, shell commands, file upload and the OTA state machine.
---

# Watch communication protocol (0.73)

The watch exposes two framed command channels that share the same
11-byte header:

- **BLE GATT** — used by Nothing X (`wewear` layer)
- **SPP / UART** — AT commands and the SPP firmware upload path

## 1. Frame format

The 11-byte header is **big-endian**. Payload fields are generally
**little-endian** (FMC reads them with `DataView(..., true)`); the OTA
A042/INIT payloads follow the same rule.

```
+0x00  u8   head_id      = 0xF5
+0x01  u16  length       payload length
+0x03  u16  c1           command key high 16 (0xFFFF for system commands)
+0x05  u16  chunk_count  number of chunks for this command
+0x07  u16  chunk_index  1-based index
+0x09  u16  c2           command key low 16
+0x0B  u8[] payload
```

The full command key is `(c1 << 16) | c2`; the firmware logs it as
`head_id, c1, c2, length`. This is why system commands appear as
`0xFFFF_9052` and ability commands as `<ability>_<cmd>`.

The reader (`FUN_100CBF84` for BLE, `FUN_100EC400` for SPP) reads the
11-byte header, byte-swaps the five u16 fields, and logs
`head_id, ability_id, cmd_id, length_id`. The payload is read on demand
with the requested `length`. For multi-chunk commands the receiver
reassembles all `chunk_count` frames in order.

Responses reuse the same buffer and set `c2 | 0xA000`
(0x9052 → 0xA052, and so on). `chunk_count`/`chunk_index` are zeroed.

Cross-checked against FMC's working Web-Bluetooth implementation
(`freethinkel/fmc`, `ble-protocol.ts`): identical 11-byte header,
big-endian, `chunk_index` 1-based, MTU 180.

## 2. Dispatch

`FUN_100D343C` (BLE) dispatches in two stages:

1. `c1 == 0xFFFF` → switch on **c2** (system commands).
2. otherwise `c1` is an **ability id** and the handler switch is on
   that value (app-level messages such as time/health/UI).

System cmds seen: `0x8004 0x8005 0x8006 0x8047 0x8049 0x804A 0x804B
0x804D 0x8051 0x8052 0x9052 0x9053 0x9055–0x9059 0x905C 0x905D
0x9066–0x9074 0x9076–0x907A 0x907F 0x9080 0x9083 0x908A 0x908B`.

Ability ids seen: `0x53 0x54 0x56 0x57 0x58 0x5A–0x62 0x65 0x95 0x99
0x9A 0x9B 0x9F`.

Notable handlers:

| cmd | behavior |
|---|---|
| `0x8004` | `FUN_100D2E78()` |
| `0x8005` | reads payload, 1-byte status reply, cmd 0x0000 in reply |
| `0x8006` | reads payload, 4-byte reply |
| `0x8047` | sets flags and copies payload (`FUN_100CC284`) |
| `0x804B` | encrypted payload (see §3) |
| `0x9052` | session/keepalive; reply status byte 0xC/2/10, may arm OTA |
| `0x9053` | resets UI (`FUN_101D0A9E(0)`), reply 0x5A |
| `0x9055–0x905D, 0x9066+` | per-command handlers, mostly notifications |

Unknown cmd → log `unkown cmd_id:0x%04x`; invalid ability → log
`invalid head_id: %04x`.

## 3. Payload checksum / encryption

`FUN_101B605A(&len_out, data, len, cmd)` is used to append the payload
tail:

- append `crc32(data)` as 4 **little-endian** bytes,
- pad to a 16-byte boundary, then AES-CBC encrypt (fixed IV
  `5051525354555657606162636465665a`, PKCS#7) with the session key.

The verifier is the mirror image (`FUN_101B608C` → `FUN_100CCC0C`).

**Plaintext commands** (CRC32 appended, no AES) are, per FMC (verified
live): `0xFFFF8047` (pair request), `0xFFFF0048` (pair reply),
`0xFFFF9064` (watch-face chunk data) and `0xFFFFA061` (auth failed —
6 bytes, not a block). Everything else is encrypted once a session key
exists. The 0.73 static analysis of `FUN_100CCAB4` only flagged
`0x804B`/`0x49` because those take the key-selection path; the plaintext
set above is the better rule for tooling.

Session key: `sha256(nonce || authkey)[:16]`, where `authkey` is
`sha256(random1 || random2 || secret)[:16]` and `secret` comes from the
shell channel (`AT GETSECRET` → `GETSECRET:<32 hex>,OK`). Matches the
AES key recovered from the phone-side OTA capture.

## 4. SPP firmware upload path

`FUN_100EC400` is the SPP dispatcher (same 11-byte header), opcodes:

| cmd | handler | role |
|---|---|---|
| `0x8052` | `FUN_100EC358` | watch-face upload init 1, reply `0x52` (status 1 = ok) |
| `0x905E` | `FUN_100EB344` | upload-flow handshake: zero 0x3C stack, `k_sem_give(ctx+0x50)`, **no reply** |
| `0x905F` | `FUN_100EB50C` | SPP chunk write: append at `state+0xC`, check `chunk_index`, per-chunk CRC |
| `0x9060` | `FUN_100EB218` | forward payload to a named file service (`FUN_100EAF48`), then reply `0x9060` status |
| `0x9061` | — | unknown command log |
| `0x9063` | `FUN_100EBAC4` | upload-flow handshake: `k_sem_give(ctx+0x50)`, **no reply** |
| `0x9064` | `FUN_100EC19C` | **watch-face chunk data** → `FUN_101DAFE0(ctx+0x50)` |
| `0x9065` | `FUN_100EB990` | **watch-face finish ack 2**, CRC verify; also writes `album.cfg` (see `watchface.md` §2.1) |
| `0x9075` | `FUN_100EBE24` | **watch-face upload init 2** → `FUN_101DAFE0(ctx+0x50)` |

`0x905F` enforces: `offset + len < 0xC01` per packet, sequential packet
index (`byte[state+0x10] + 1 == chunk_index`), and per-packet checksum.
Ghidra marks `0x9064`/`0x9075` as non-returning only because
`FUN_101DAFE0` is mis-annotated; FMC's working client proves both return
and are acknowledged.

### 4.1 Watch-face upload (verified end-to-end)

Channel: `02f00000-…-ffe1` (write, **without response**) / `…-ffe2`
(notify). Every step below — init1, init2, chunks, finish — goes over the
**data** channel, not `fff1/fff2`. The watch **pulls** the data:

1. phone → `0x8052` payload `[0xA5]`; watch → `0x0052`, `[1]` = ok.
2. phone → `0x9075` 13-byte LE payload:
   `type=3 (u8) | old_wf_id (u32) | new_wf_id (u32) | size (u32)`.
   Watch → `0xA075`, `[1]` = ok.
3. watch → `0xA064` chunk request: `offset (u32 **BE**) | len (u32 **BE**) |
   percent (u8)`. Phone → `0x9064` with that raw slice (plaintext,
   CRC32 tail only).
4. watch → `0xA065` finish ack; phone → `0x9065` `[0xA5]`. `[1]` =
   activated (or deleted), `[0x0A]` = stored but not activated (unknown
   `old_wf_id`, `new_wf_id == old_wf_id`, or full slots).
5. watch may send `0xA055` installed-dial list on `ffe2` right after auth
   (**encrypted**, PKCS7): `4-byte header | n × u32 LE id | CRC32 LE`.
   Sample `01 02 03 07 | 45 01 00 00 | d0 3f 51 71 | dc 9c 4d 3a |
   33 49 0f c2` = 3 ids (`325`, `1901150160`, `978164956`), header byte 2
   = count, byte 3 = capacity (7 on this unit, FMC hardcodes 6).
   Side-loaded dials are **not** listed (the installed `0x338dbb59` was
   absent) — `0xA055` alone cannot tell whether a slot is free.

Framing: encrypted payloads are split into `floor((180-11)/16)*16 - 5 =
155`-byte pieces, each `+CRC32` then AES-CBC (header stays plaintext);
plaintext (`0x9064`, `0x8047`, `0x0048`, `0xA061`) is split into 160-byte
pieces, each `+CRC32`. Header `count` = number of pieces, `index` starts
at 1. **Watch→phone plaintext frames omit the CRC but still declare
`plen` including it** (`0x0048` declares 52, sends 48): a parser that
trusts `plen` desyncs and never sees `0xA055`.

Slot rules (verified on hardware, 0.73):

- `old_wf_id = 0` **appends** a new dial. Re-verified after a delete: the
  freed slot is reused and the append returns `finish: 01`.
- `old_wf_id = <installed>` with `new_wf_id = 0` **deletes** that dial.
  This is an undocumented protocol primitive (the firmware state machine
  has a delete branch; FMC only exposes delete in the watch UI). The data
  phase still runs — the uploaded bytes are only a filler. Verified:
  `wf-delete --id 0x407d3a27` returned `finish: 01`, the dial vanished,
  and a following append succeeded.
- `new_wf_id == old_wf_id` is refused at finish (`0x0A`), so a
  "self-replace" is not possible; delete + append is the alternative.
- A replace with a **different** `new_wf_id` is unreliable: one replace
  (`0x338dbb59` → `0x417d3bba`) returned `finish: 01`, but afterwards
  neither id was addressable (`0x0A` on both). Replace is therefore
  unsuitable for slot management, and `0x0A` indicates that the id is not
  installed.
- `new_wf_id` is read **signed** — a negative id is rejected. FMC derives
  `id = 10000 + fnv1a(seed) % (0x7FFFFFFF - 10000)` to stay positive and
  above the stock ids (273–280). Capacity is 6 dials per FMC (a055 byte 3
  says 7); a full watch only refuses at the very end (finish `0x0A`).

`album.cfg` lives on `/NAND:C` and its entries are what `old_wf_id` is
matched against; the ids of side-loaded dials are only known to the host
(`a055` omits them), so the host keeps a local record of the dials it
appended.

Verified on hardware (0.73): 89,037-byte face, 29 chunk requests of
`0xC00`, `finish: 01`, appended with `old_wf_id=0`, id `0x338dbb59`.
An 86,884-byte 86 KB file with `old_wf_id=0x338dbb59`, `new_wf_id=
0x417d3bba` also returned `finish: 01` (see the replace caveat above).
Tooling: `watools/cmf_watch.py wf-dials`, `wf-flash --file X
[--old-id N] [--dry-run]` and `wf-delete --id N --file X`.

Cross-check note: the 0.73 static analysis had labelled `0x9064`/
`0x9075` as reset/bootloader — wrong; FMC's live client shows both are
the watch-face upload path. The SPP dispatcher labels do not match the
BLE path's opcodes (`0x905F` is the SPP-only chunk write).

## 5. AT command registry

A 71-entry table at `0x101F4DA4` (records 20 B, terminated by zeros):

```
+0x00  const char *name
+0x04  const char *name (duplicate)
+0x08  u32 0
+0x0C  handler (Thumb pointer, bit0 set)
+0x10  u32 0
```

Name strings live at `0x10229250`–`0x10229600`.

The group descriptor is at `0x101F4D84`:

```
0x101F4D70..0x101F4D84  six code pointers (0x100E7264, 0x100E755C,
                        0x100E73C0, 0x100E7A9C, 0x100E7CE0, 0x100E7F78)
0x101F4D88  "AT"
0x101F4D8C  "AT commands"
0x101F4D90  0x101F4D9C   (→ 0)
0x101F4D94/98/9C  0
0x101F4DA0  0x101F4DA4   (→ command table)
```

The descriptor itself has **no direct xref**; only `0x101F4D70` is
referenced (from `0x100E734C`/`0x100E7388`, a timer registration), so the
shell walks this region by address arithmetic at runtime. The six
pointers are read as the group's lifecycle callbacks (their exact role is
unknown; the last one is also a todo-expiry timer callback).

Handler signature is roughly `(frame_ctx, has_data)`. Replies go through
`FUN_100E895C`, which selects transport:

- `*0x100E8988 == 0` → UART/print (`FUN_101BA194`)
- `*0x100E8988 == 1` → BLE send (`FUN_100CB9E0`)

| name | handler | name | handler |
|---|---|---|---|
| GETVERSION | `0x100EA50C` | SETMOTOR | `0x100EA4A4` |
| GETCAPACITY | `0x100EA458` | GETKEYSTATUS | `0x100EA408` |
| GETENCODERSTATUS | `0x100EAC1C` | GETENCODERINIT | `0x100EA3BC` |
| ENTERBTBQB | `0x100EA328` | GETGSENSOR | `0x100EA2C0` |
| SETGPSSWITCH | `0x100EA254` | GETGPSSNR | `0x100EA1D4` |
| GETCLOCKDRIFT | `0x100EA184` | SETNAME | `0x100EA080` |
| GETNAME | `0x100EA024` | SETBTMAC | `0x100E9FC4` |
| GETBTMAC | `0x100E9F48` | SETPID | `0x100E9EEC` |
| GETPID | `0x100E9E8C` | SETSECRET | `0x100E9E30` |
| GETSECRET | `0x100E9DD0` | SETGPIO | `0x100E9C7C` |
| GETFLASH | `0x100E9C30` | LOOPSWITCH | `0x100EABA0` |
| GETNTCTEMP | `0x100E9BE4` | SHUTDOWN | `0x100E9B98` |
| ENTERFCCMODE | `0x100E9B38` | SETWAKELOCK | `0x100E9AC0` |
| SETAPMODE | `0x100E9A34` | SETSN | `0x100E99D8` |
| GETSN | `0x100E9978` | SHIPMODE | `0x100E9924` |
| GETMCUCHIPID | `0x100E98C4` | REBOOT | `0x100E9878` |
| GETSCREEN | `0x100E9834` | SETLPHOSTLUX | `0x100E97D8` |
| SETLPALIGN | `0x100E9768` | GETLPALIGNSTATUS | `0x100E971C` |
| GETLPSLAVELUX | `0x100E96D0` | SETKNOBTARGET | `0x100E9674` |
| SETKNOBALIGN | `0x100E960C` | GETKNOBALIGN | `0x100E95BC` |
| SETKONBDISPLACEMENT | `0x100E9554` | GETKONBDISPLACEMENT | `0x100E9504` |
| SETKNOBREINIT | `0x100E94C0` | SETKNOBDUMP | `0x100E944C` |
| SETLIMITCHARPERCENT | `0x100E93D4` | GETLIMITCHARPERCENT | `0x100E9374` |
| ENTERFATE | `0x100E9338` | SETFHTESTSWITCH | `0x100E92D0` |
| GETFHTESTRESULT | `0x100E927C` | CHECKCWALGO | `0x100E91F8` |
| OPENCWALGO | `0x100E9184` | SETLCDSWITCH | `0x100E9104` |
| SETLCDPAGE | `0x100E90A0` | SETLTLKBKSWITCH | `0x100E9020` |
| GETLTLKBKDATA | `0x100E8FC0` | SETLTLKBESWITCH | `0x100E8F40` |
| GETLTLKBEDATA | `0x100E8EF0` | SETBIOSWITCH | `0x100E8E70` |
| GETBIODATA | `0x100E8E28` | SETKEYSWITCH | `0x100E8DA8` |
| GETKEYRESULT | `0x100E8D48` | SETFACTSWITCH | `0x100E8CE4` |
| GETBATLV | `0x100E8C9C` | SETLIGHTWITCH | `0x100E8C1C` |
| GETGPSVERSION | `0x100E8BD0` | SETHR | `0x100E8B70` |
| SETSPO2 | `0x100E8B0C` | SETHRV | `0x100E8AA8` |
| SETADT | `0x100E8A44` | SETHRTEST | `0x100E89E0` |
| SETBIOPERCENT | `0x100E8990` | | |

`AT+GETVERSION` returns a **compile-time** version string
(`ux2sys_api_7(0x11, buf)`), not data from the OTA XML.

## 6. OTA over BLE (AOTA path)

The phone uploads a full `JX402_*.bin` through a second, encrypted BLE
channel:

| role | UUID |
|---|---|
| OTA data write | `02f00000-0000-0000-0000-00000000ff01` |
| OTA data notify | `02f00000-0000-0000-0000-00000000ff02` |
| control write | `0000fff2-…` (`CMD_W`) |
| control notifies | `CMD_N` and `DAT_N` characteristics |

Control commands (`CMD_W` → `CMD_N`):

- `INIT1` — session nonce (`0x804C`).
- `INIT2` — starts the upgrade. It is sent on `ff01`, not on `CMD_W`.
  With a valid session, `CMD_W` can reboot the watch and drop the link
  before the data phase; on `ff01` the `A040` reply arrives in ~30 ms.
- `A041` — commit prompt. The end acknowledgement is `0x9041` with
  payload `A5`; without it the staged update is not committed.
- `A042` — write data. Plaintext is 13 bytes:
  `offset(u32 BE) | length(u32 BE) | 4 bytes | 1 byte`, then AES-CBC
  (fixed IV, PKCS#7) with `key = sha256(nonce || authkey)[:16]`.
  Watch replies on `ff02`; the host then sends the next chunk.
- `A040`/`A041` — progress/status.

The watch drives the transfer: it requests byte ranges from the host
and writes them to flash. In practice it only asks for the ranges it
needs, so a successful run can serve well under the whole image
(70-75% was typical). CRC checks are per-partition only
(see `aota.md`).

On-device side:

| address | what |
|---|---|
| `0x101566F0` | `ota_backend_bt_init` |
| `0x10157278` | state setter (old → new, callback at `+0xC`) |
| `0x101581E8` | `ota_rx` thread |
| `0x10158390` | upgrade state machine ("handle upgrade") |
| `0x10159264` | progress init: `+0x228` total, `+0x22C` offset, `+0x230` start, `+0x238` percent, `+0x239` active |
| `0x1015A164` | `ota_cfg` default init (0x233 B, magic `A1 A2 A3 A4`) |
| `0x1015A66C` | `OTA_INFO` read (4-byte magic `A5 A5 A5 A5`) |

`OTA_UPG_FLAG` / `OTA_UPG_TYPE` (`0x10058508`, `0x100585BC`) persist the
"upgrade in progress" state so the watch re-enters the OTA UI after a
reset.

### State machine (`FUN_10158390`, setter `FUN_10157278`)

`FUN_10157278(ctx, new)` stores `new` at `ctx[0]` and calls the
callback at `ctx[3]` with `(new, old)`. Values:

| state | meaning | set at |
|---|---|---|
| 0 | idle / reset | start, and after a terminal result |
| 1 | transferring (image accepted) | after the header/manifest check |
| 2 | committed (reboot pending) | after `OTA_INFO` + manifest update + `FUN_101592d4` |
| 3 | error / finished | any failure path, then `0` |

Paths:
1. Validate `ota.ota` magic (`FUN_1015a6fc` reads it) and the image
   header (`FUN_10159fe8`). On mismatch → state 3 → 0.
2. Compare version/serial (`FUN_101d099c`, `FUN_101d09a0`, `FUN_101bf37a`
   on `+0x50`), check the `+0x2E6`/`+0x2E7` type bytes → state 3 on
   mismatch, else **state 1** and start the RX thread.
3. After all files are written: per-file size/CRC checks
   (`FUN_1015a600`, `FUN_1015a630`), `FUN_1015a66c` writes `OTA_INFO`,
   `FUN_1015a238` updates the manifest, `FUN_1015a1f0` closes it →
   **state 2** and `*(0x6d + DAT_10158e94) = 1` (reboot flag).
4. Reboot applies the update; failure paths call `FUN_101574a4`
   (cleanup) and return to **state 0**.

`ota.ota` is a 0x233-byte (563) record, zeroed and stamped
`A4 A3 A2 A1` at `+0x00`; the separate 4-byte `OTA_INFO` is `0` or
`0xA5 A5 A5 A5`.

### 6.1 Read surfaces that do not exist

All host-reachable paths were checked for a way to read `/NAND:C` files;
none of them can:

| surface | result |
|---|---|
| AT registry | no file commands at all — `GETFLASH` returns the flash size (`FUN_1011FC30`), `SETKNOBDUMP` posts a UI message via `ux2sys_api(0x5E)`, `ENTERFATEMODE` calls `FUN_100CC278` |
| SPP `wewear` | `A042` is write-only; `wewear_ota_cmd_d2h_image_data_req` (`0x10156150`) means *the watch asks the phone* for the next image chunk; `FUN_10156948` is a flash write |
| `logsrv` BLE service (`102030E1`, `_logsrv_init`) | log streaming only: TLV frames with `svc_id`/`cmd`, `stream_create`, and the strings `logsrv: log/tx/rx/stream …`. No path or filename is ever passed, so it cannot be turned into a file read |
| FMC reference client | has no file-read opcode either (`ble-protocol.ts` only has `wfInit2 0x9075`, `wfChunkWrite 0x9064`, `wfFinishAck2 0x9065`) |

Consequence: `album.cfg` and `.wfdl` files are only obtainable from a
NAND dump / hardware read, or by flashing a patched app that exports
them — zero-copy host access does not exist.

## 7. Pitfalls

- The head byte is always `0xF5`; frames with other values are dropped.
- **Byte order**: fields are big-endian on the wire but host-order in
  the dispatcher (the reader swaps in place first). Frames crafted from
  a script are written big-endian.
- Response cmd = request cmd `| 0xA000`; the request value is not
  echoed.
- `length` counts payload only (header is always 0x0B).
- For `0x804B` and `cmd & 0x7FFF == 0x49`, the payload is AES-CBC with
  an extra 4-byte CRC and padding; a plain payload will be rejected.
- The SPP file path (`0x905F`) requires strictly sequential `chunk_index`
  indices, starting from the state's current index.
- `0x905E/0x9063/0x9064/0x9075` do **not** "return" in the decompiler only
  because they end in `k_sem_give` (mis-annotated as non-returning); they
  are upload-flow handshakes, not reset/bootloader. `0x905E`/`0x9063` send
  no reply at all.
- There is **no device→host file read** in the SPP dispatcher: `wewear_d2h_*`
  only covers OTA image data. `album.cfg` (watch-side `/NAND:C`) therefore
  cannot be pulled over SPP — it has to be read from a NAND dump.
- BLE `INIT2` (`0x9040`) must be written to the OTA characteristic
  `ff01`. Sending it on `CMD_W` with a valid session reboots the watch
  and drops the link. See [AOTA OTA flow](/devices/cmf-watch-pro-2/protocol/ota/).
- The end of the data phase needs `0x9041` with payload `A5` when the
  watch reports `A041`; skipping it leaves the update staged, not
  committed.
- Not every `0x905x` cmd has a handler in 0.73; unknown cmds log and are
  ignored.
