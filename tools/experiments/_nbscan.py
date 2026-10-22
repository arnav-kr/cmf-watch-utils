import argparse
import asyncio
import time
from types import SimpleNamespace

from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W
import rfcomm


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


async def ble_init1(addr, timeout, init1_hex, watch_seconds=3.0):
    w = Watch(address=addr)
    await w.connect(timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=timeout))
    print(f"[{ts()}] BLE authed connected={w.client.is_connected} key={'set' if w.key else 'none'}", flush=True)
    key = w.key
    frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(init1_hex), key=key)
    t0 = time.time()
    await w.client.write_gatt_char(CMD_W, frame, response=True)
    print(f"[{ts()}] >BLE INIT1 frame={frame.hex()}", flush=True)
    end = t0 + watch_seconds
    while time.time() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=0.2)
        except asyncio.TimeoutError:
            piece = None
        if piece is not None:
            for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                dec = w._decode(c1, c2, body, enc)
                print(f"[{ts()}] <BLE cmd2={c2:#06x} {dec.hex()} (+{time.time()-t0:.3f}s)", flush=True)
        if not w.client.is_connected:
            print(f"[{ts()}] *** BLE LINK DROPPED +{time.time()-t0:.3f}s", flush=True)
            break
    try:
        await w.disconnect()
    except Exception:
        pass


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--channels", default="1-30")
    ap.add_argument("--duration", type=float, default=28.0)
    ap.add_argument("--interval", type=float, default=0.5)
    ap.add_argument("--connect-timeout", type=float, default=None)
    ap.add_argument("--layout", default="packed2")
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--no-init1", action="store_true")
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    chans = []
    for part in args.channels.split(","):
        part = part.strip()
        if "-" in part:
            a, b = part.split("-")
            chans.extend(range(int(a), int(b) + 1))
        elif part:
            chans.append(int(part))

    rfcomm.ws_init()

    if not args.no_init1:
        await ble_init1(addr, args.timeout, args.init1_hex)
        print(f"[{ts()}] BLE closed; starting non-blocking RFCOMM sweep", flush=True)
    else:
        print(f"[{ts()}] baseline sweep (no INIT1)", flush=True)

    t0 = time.time()
    rfcomm.sweep_loop(addr, chans, duration=args.duration, interval=args.interval, layout=args.layout,
                      connect_timeout=args.connect_timeout)
    print(f"[{ts()}] sweep done after {time.time()-t0:.1f}s", flush=True)


asyncio.run(main())
