import argparse
import asyncio
import time
from types import SimpleNamespace

from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W
import rfcomm  # noqa


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


def dump(w, tag):
    print(f"--- services {tag} ---", flush=True)
    try:
        svcs = list(w.client.services)
    except Exception as e:
        print("  services err:", e, flush=True)
        return
    for s in svcs:
        print(f"svc {s.uuid}", flush=True)
        for ch in getattr(s, "characteristics", []):
            print(f"  chr {ch.uuid} {ch.properties}", flush=True)


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--duration", type=float, default=30.0)
    ap.add_argument("--timeout", type=float, default=10.0)
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    key = w.key
    dump(w, "pre-INIT1")
    fr = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key)
    await w.client.write_gatt_char(CMD_W, fr, response=True)
    print(f"[{ts()}] >BLE INIT1", flush=True)
    t0 = time.time()
    end = t0 + 3.0
    while time.time() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=0.15)
        except asyncio.TimeoutError:
            piece = None
        if piece is not None:
            for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                print(f"[{ts()}] <BLE cmd2={c2:#06x} (+{time.time()-t0:.2f}s)", flush=True)
        if not w.client.is_connected:
            print(f"[{ts()}] *** BLE DROPPED +{time.time()-t0:.2f}s", flush=True)
            break
    try:
        await w.disconnect()
    except Exception:
        pass

    print(f"[{ts()}] polling for post-INIT1 BLE ...", flush=True)
    while time.time() - t0 < args.duration:
        try:
            w2 = Watch(address=addr)
            await w2.connect(args.timeout)
        except Exception as e:
            print(f"[{ts()}] (+{time.time()-t0:.1f}s) connect fail: {type(e).__name__} {e}", flush=True)
            await asyncio.sleep(0.5)
            continue
        print(f"[{ts()}] CONNECTED (+{time.time()-t0:.2f}s)", flush=True)
        dump(w2, f"post-INIT1 +{time.time()-t0:.1f}s")
        try:
            await w2.disconnect()
        except Exception:
            pass
        await asyncio.sleep(0.5)
        break


asyncio.run(main())
