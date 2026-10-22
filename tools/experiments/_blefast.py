import asyncio
import sys
import time

sys.path.insert(0, r"D:\upitest\watools")
import cmf_watch as cw

ADDR = "AA:BB:CC:DD:EE:FF"
IMG = r"D:\upitest\firmware\wp2_1724161837605-90.bin"
VER = bytes.fromhex("0100005a")
INIT1 = b"\xa5"


async def main():
    w = cw.Watch(address=ADDR, verbose=False)
    await w.connect(10.0)

    st = cw.load_state()
    ak = bytes.fromhex(st["authkey"])
    await w.reconnect(ak)
    print(f"[session] key={w.key.hex()}", flush=True)

    img = open(IMG, "rb").read()
    print(f"[img] {len(img)} bytes", flush=True)

    t0 = 0.0
    fired = False
    served = 0
    served_bytes = 0
    a052_seen = asyncio.Event()
    done = asyncio.Event()

    def now_ms():
        return (time.monotonic() - t0) * 1000.0 if t0 else 0.0

    async def fire_init2(detected_ms):
        nonlocal fired
        if fired:
            return
        fired = True
        frame = cw.build_frame(0xFFFF, 0x9040, VER, key=w.key)
        print(f"[{now_ms():8.1f}ms] >> INIT2 build (ver={VER.hex()}) detected_a052_at={detected_ms:.1f}ms",
              flush=True)
        t = time.monotonic()
        try:
            await w.client.write_gatt_char(cw.CMD_W, frame, response=True)
            print(f"[{now_ms():8.1f}ms] >> INIT2 written CMD_W (response=True) took "
                  f"{(time.monotonic()-t)*1000:.1f}ms", flush=True)
        except Exception as e:
            print(f"[{now_ms():8.1f}ms] !! INIT2 CMD_W write failed: {e}", flush=True)
        try:
            t = time.monotonic()
            await w.client.write_gatt_char(cw.DAT_W, frame, response=False)
            print(f"[{now_ms():8.1f}ms] >> INIT2 written DAT_W (no-response) took "
                  f"{(time.monotonic()-t)*1000:.1f}ms", flush=True)
        except Exception as e:
            print(f"[{now_ms():8.1f}ms] !! INIT2 DAT_W write failed: {e}", flush=True)

    async def serve(off, ln):
        nonlocal served, served_bytes
        if ln <= 0 or off >= len(img):
            return
        end = min(off + ln, len(img))
        chunk = 200
        n = max(1, (end - off + chunk - 1) // chunk)
        o = off
        i = 0
        while o < end:
            piece = img[o:o + chunk]
            frame = cw.build_frame(0xFFFF, 0x9042, piece, chunk_index=i + 1,
                                   chunk_count=n, key=None)
            try:
                await w.client.write_gatt_char(cw.DAT_W, frame, response=False)
            except Exception as e:
                print(f"[{now_ms():8.1f}ms] !! data write fail: {e}", flush=True)
                return
            o += len(piece)
            i += 1
            served += 1
            served_bytes += len(piece)
            if served % 32 == 0:
                print(f"[{now_ms():8.1f}ms] .. served {served} frames {served_bytes}B "
                      f"({100.0*o/len(img):.1f}%)", flush=True)
        print(f"[{now_ms():8.1f}ms] .. served range off={off} len={ln} -> {served_bytes}B total",
              flush=True)

    def handle(ch):
        def _cb(_, data):
            b = bytes(data)
            for cmd1, cmd2, cc, ci, body, enc in w.parser.feed(b):
                dec = body
                if enc and w.key:
                    try:
                        d2 = cw.aes_dec(w.key, body)
                        if len(d2) >= 4 and cw.crc32_le(d2[:-4]) == d2[-4:]:
                            d2 = d2[:-4]
                        dec = d2
                    except Exception:
                        pass
                print(f"[{now_ms():8.1f}ms] <{ch} cmd1={cmd1:#06x} cmd2={cmd2:#06x} "
                      f"cc={cc} ci={ci} enc={enc} len={len(dec)} {dec.hex()}", flush=True)
                if cmd2 == 0xA052:
                    a052_seen.set()
                    asyncio.create_task(fire_init2(now_ms()))
                elif cmd2 in (0xA040, 0xA041, 0xA042, 0xA043, 0xA044, 0xA0A0):
                    done.set()
                    if cmd2 == 0xA042 and len(dec) >= 8:
                        off = int.from_bytes(dec[0:4], "big")
                        ln = int.from_bytes(dec[4:8], "big")
                        asyncio.create_task(serve(off, ln))
        return _cb

    # replace default handlers with raw ones
    for u in (cw.CMD_N, cw.DAT_N):
        try:
            await w.client.stop_notify(u)
        except Exception:
            pass
        try:
            await w.client.start_notify(u, handle("cmd" if u == cw.CMD_N else "dat"))
        except Exception as e:
            print(f"[warn] subscribe {u}: {e}", flush=True)

    frame = cw.build_frame(0xFFFF, 0x9052, INIT1, key=w.key)
    t0 = time.monotonic()
    print(f"[{now_ms():8.1f}ms] >> INIT1 0x9052 frame={frame.hex()}", flush=True)
    await w.client.write_gatt_char(cw.CMD_W, frame, response=True)
    print(f"[{now_ms():8.1f}ms] >> INIT1 written", flush=True)

    try:
        await asyncio.wait_for(a052_seen.wait(), timeout=6.0)
    except asyncio.TimeoutError:
        print(f"[{now_ms():8.1f}ms] !! no A052 within 6s", flush=True)

    # watch for a while after
    deadline = time.monotonic() + 20.0
    while time.monotonic() < deadline:
        await asyncio.sleep(0.2)
        if not w.client.is_connected:
            print(f"[{now_ms():8.1f}ms] !! BLE disconnected", flush=True)
            break

    print(f"[{now_ms():8.1f}ms] summary: fired={fired} served={served} "
          f"bytes={served_bytes} connected={w.client.is_connected}", flush=True)
    try:
        await w.disconnect()
    except Exception:
        pass


if __name__ == "__main__":
    asyncio.run(main())
