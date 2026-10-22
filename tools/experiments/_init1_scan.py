import argparse
import asyncio
import time
from types import SimpleNamespace

from bleak import BleakClient, BleakScanner
from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


def describe(dev, adv):
    name = (adv.local_name if adv else None) or (getattr(dev, "name", None) or "")
    su = list(adv.service_uuids) if adv and adv.service_uuids else []
    md = {}
    if adv and adv.manufacturer_data:
        md = {hex(k): v.hex() for k, v in adv.manufacturer_data.items()}
    return (getattr(dev, "address", "?"), name, su, md, getattr(adv, "rssi", None))


async def scan_baseline(seconds):
    print(f"---- BASELINE SCAN {seconds:.0f}s ----", flush=True)
    seen = {}

    def cb(dev, adv):
        a, n, su, md, r = describe(dev, adv)
        key = a
        if key not in seen:
            seen[key] = time.time()
            print(f"[{ts()}] ADV addr={a} name={n!r} rssi={r} svc={su} mfr={md}", flush=True)
        elif time.time() - seen[key] > 2.0:
            seen[key] = time.time()
            print(f"[{ts()}] ADV* addr={a} name={n!r} rssi={r}", flush=True)

    sc = BleakScanner(detection_callback=cb)
    await sc.start()
    await asyncio.sleep(seconds)
    await sc.stop()
    return seen


async def scan_after(seconds, baseline_addrs):
    print(f"---- POST-INIT1 SCAN {seconds:.0f}s ----", flush=True)
    first = {}
    order = []

    def cb(dev, adv):
        a = getattr(dev, "address", "?")
        if a not in first:
            first[a] = describe(dev, adv)
            order.append(a)
            _, n, su, md, r = first[a]
            new = "" if a in baseline_addrs else "  <== NEW/DIFFERENT"
            print(f"[{ts()}] ADV addr={a} name={n!r} rssi={r} svc={su} mfr={md}{new}", flush=True)

    sc = BleakScanner(detection_callback=cb)
    await sc.start()
    await asyncio.sleep(seconds)
    await sc.stop()
    return first


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--baseline", type=float, default=8.0)
    ap.add_argument("--post", type=float, default=45.0)
    ap.add_argument("--watch-monitor", type=float, default=25.0)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--probe-only", action="store_true", help="scan only, do not connect/send")
    args = ap.parse_args()

    addr = args.address or load_state().get("address")

    base = await scan_baseline(args.baseline) if not args.probe_only else {}
    base_addrs = set(base.keys())

    if args.probe_only:
        await scan_after(args.post, base_addrs)
        return

    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    print(f"[{ts()}] authed; sessionKey={w.key.hex()} connected={w.client.is_connected}", flush=True)

    payload = bytes.fromhex(args.init1_hex)
    frame = build_frame(0xFFFF, 0x9052, payload, key=w.key)
    print(f"[{ts()}] >INIT1 cmd2=0x9052 payload={payload.hex()} frame={frame.hex()}", flush=True)
    try:
        await w.client.write_gatt_char(CMD_W, frame, response=True)
    except Exception as e:
        print(f"[{ts()}] INIT1 write error: {e}", flush=True)

    a052 = None
    t_send = time.time()
    end = t_send + args.watch_monitor
    while time.time() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=0.5)
        except asyncio.TimeoutError:
            piece = None
        if piece is not None:
            for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                dec = w._decode(c1, c2, body, enc)
                print(f"[{ts()}] <frame cmd1={c1:#06x} cmd2={c2:#06x} enc={enc} len={len(dec)} {dec.hex()}", flush=True)
                if c2 == 0xA052:
                    a052 = dec
                    print(f"[{ts()}] *** A052 received after {time.time()-t_send:.3f}s: {dec.hex()}", flush=True)
        if not w.client.is_connected:
            print(f"[{ts()}] *** LINK DROPPED after {time.time()-t_send:.3f}s", flush=True)
            break

    print(f"[{ts()}] monitor done: a052={'yes' if a052 else 'no'} connected={w.client.is_connected}", flush=True)
    try:
        await w.disconnect()
    except Exception:
        pass

    await scan_after(args.post, base_addrs)

    print("---- RECONNECT ATTEMPT + SERVICE DUMP ----", flush=True)
    await asyncio.sleep(2.0)
    try:
        c = BleakClient(addr, timeout=30.0)
        await c.connect(timeout=30.0)
        print(f"[{ts()}] reconnect ok={c.is_connected}", flush=True)
        for s in c.services:
            print(f"  svc {s.uuid}", flush=True)
            for ch in s.characteristics:
                print(f"      chr {ch.uuid} {ch.properties}", flush=True)
        await c.disconnect()
    except Exception as e:
        print(f"[{ts()}] reconnect failed: {e}", flush=True)


asyncio.run(main())
