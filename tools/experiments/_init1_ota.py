import argparse
import asyncio
import time
from types import SimpleNamespace

from bleak import BleakClient
from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--probe-seconds", type=float, default=25.0)
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    print(f"[{ts()}] authed connected={w.client.is_connected}", flush=True)

    frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=w.key)
    t0 = time.time()
    await w.client.write_gatt_char(CMD_W, frame, response=True)
    print(f"[{ts()}] >INIT1 cmd2=0x9052 frame={frame.hex()}", flush=True)

    end = t0 + 3.0
    while time.time() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=0.2)
        except asyncio.TimeoutError:
            piece = None
        if piece is not None:
            for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                dec = w._decode(c1, c2, body, enc)
                print(f"[{ts()}] <frame cmd2={c2:#06x} {dec.hex()} (+{time.time()-t0:.3f}s)", flush=True)
        if not w.client.is_connected:
            print(f"[{ts()}] *** LINK DROPPED +{time.time()-t0:.3f}s", flush=True)
            break
    try:
        await w.disconnect()
    except Exception:
        pass

    print(f"[{ts()}] ---- probing connect during OTA window ----", flush=True)
    dead = time.time() + args.probe_seconds
    attempt = 0
    while time.time() < dead:
        attempt += 1
        dt = time.time() - t0
        try:
            c = BleakClient(addr, timeout=5.0)
            await c.connect(timeout=5.0)
            print(f"[{ts()}] +{dt:.1f}s attempt#{attempt} CONNECTED ok={c.is_connected}", flush=True)
            svcs = [str(s.uuid) for s in c.services]
            print(f"[{ts()}]   services({len(svcs)}): {svcs}", flush=True)
            for s in c.services:
                chs = [f"{ch.uuid}:{ch.properties}" for ch in s.characteristics]
                print(f"[{ts()}]     {s.uuid} -> {chs}", flush=True)
            try:
                await c.disconnect()
            except Exception:
                pass
            break
        except Exception as e:
            print(f"[{ts()}] +{dt:.1f}s attempt#{attempt} fail: {type(e).__name__} {e}", flush=True)
            await asyncio.sleep(1.0)
    print(f"[{ts()}] probe done (attempts={attempt})", flush=True)


asyncio.run(main())
