# Successful AOTA flash run (redacted)

Shape of a successful full-firmware OTA over BLE. Device identifiers, MAC, serial, keys and
timestamps are removed. Line contents are paraphrased where they carried personal data.

Device: CMF Watch Pro 2 (JX402 / ATS3089C), firmware 1.0.0.73
Host: Windows 11, Python 3.12, Bleak
Transport: BLE, MTU 23, pipeline 4 frames in flight

## Phase 1 - session

```text
scan            -> CMF Watch Pro 2-XXXX  (address redacted)
pair            -> authkey established (redacted), secret read over shell (redacted)
info            -> version=1.0.0.73 pid=<redacted> battery=<redacted>mv
```

## Phase 2 - handshake

```text
INIT1  0x9052 (ota begin)                 -> A052 01        accepted
INIT2  0x9040 version=0100005a on ff01    -> A040 01        ~30 ms
```

Note: an earlier attempt sent INIT2 on CMD_W with a valid session and the watch rebooted,
dropping the link. Sending INIT2 on the OTA characteristic fixed it.

## Phase 3 - data phase (abridged)

```text
cmd2=0xa042 off=0x00001234 len=0x0480
  -> sent 5 frames (0x9042), 1120 bytes
cmd2=0xa042 off=0x323c2d8 len=0x0e60
  -> sent 32 frames (0x9042), 7168 bytes
... (watch requests ranges; offsets are not monotonic)
```

Served bytes for the whole run: 38,083,584 of 54,117,960 bytes in the staged image
(70.4% of the image; unchanged partitions were never requested).

## Phase 4 - commit

```text
watch status 0xa041 01
  -> sending 0x9041 END (payload a5)
waiting for post-flash status ...
  <= cmd2=0xa042 000000000000000064
watch rebooted
```

## Phase 5 - verify

```text
info -> version=1.0.0.73 battery=<redacted>mv
new animation played in the UI and was confirmed visually
```

## Second (partial) run

A follow-up transfer served only 1,632,768 bytes (3.0%) because the first run had already
staged the remaining data. It also ended with `0xA041 01` and the `0x9041 A5` end ack, which
is what committed the update.

## Takeaways

1. INIT2 (`0x9040`) must be written to the OTA characteristic `...ff01`, not to CMD_W first.
2. `A041` is the commit prompt: answer it with `0x9041` + payload `A5`.
3. A stale authkey still produces `A052`, so `A052` alone does not prove the session is usable.
4. The served percentage is relative to the whole image and can end far below 100%.
5. The end acknowledgement is the difference between "staged" and "committed".
