import argparse
import asyncio
import time
from types import SimpleNamespace

from bleak import BleakScanner
from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W

TARGET = None


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


async def main():
    global TARGET
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--window", type=float, default=60.0)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--no-init1", action="store_true")
    ap.add_argument("--all", action="store_true")
    args = ap.parse_args()
    addr = args.address or load_state().get("address")
    TARGET = addr.upper()

    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    key = w.key
    print(f"[{ts()}] authed key={'set' if key else 'none'}", flush=True)
    t0 = time.time()
    if not args.no_init1:
        frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key)
        await w.client.write_gatt_char(CMD_W, frame, response=True)
        print(f"[{ts()}] >BLE INIT1 {frame.hex()}", flush=True)
    try:
        await w.disconnect()
    except Exception:
        pass

    def cb(dev, adv):
        if not args.all and dev.address.upper() != TARGET:
            return
        dt = time.time() - t0
        uuids = []
        if adv.service_uuids:
            uuids = list(adv.service_uuids)
        name = adv.local_name or dev.name
        print(f"[{ts()}] +{dt:5.1f}s ADV addr={dev.address} name={name!r} rssi={adv.rssi} uuids={uuids}", flush=True)

    scanner = BleakScanner(detection_callback=cb)
    await scanner.start()
    print(f"[{ts()}] scanning {args.window}s (t0=INIT1)", flush=True)
    await asyncio.sleep(args.window)
    await scanner.stop()
    print(f"[{ts()}] done", flush=True)


asyncio.run(main())
