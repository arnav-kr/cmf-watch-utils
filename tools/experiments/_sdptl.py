import argparse
import concurrent.futures
import asyncio
import time
from types import SimpleNamespace

from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W
import sdp

SPP = "00001101-0000-1000-8000-00805F9B34FB"
OTA = "00006666-0000-1000-8000-00805F9B34FB"
DIAL = "0000CCAA-0000-1000-8000-00805F9B34FB"
UUIDS = [SPP, OTA, DIAL]


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


def probe(addr, uuid, timeout=5.0):
    with concurrent.futures.ThreadPoolExecutor(max_workers=1) as ex:
        fut = ex.submit(sdp.sdp_lookup, addr, uuid)
        try:
            res = fut.result(timeout=timeout)
            return "ok", [r["channel"] for r in res]
        except concurrent.futures.TimeoutError:
            return "TIMEOUT", []
        except OSError as e:
            return f"err:{e}", []


def scan_round(addr):
    out = {}
    for u in UUIDS:
        st, chs = probe(addr, u)
        out[u[:8]] = (st, chs)
    return out


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--duration", type=float, default=32.0)
    ap.add_argument("--interval", type=float, default=2.0)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--no-init1", action="store_true")
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    sdp.ws_init()
    print(f"[{ts()}] baseline SDP: {scan_round(addr)}", flush=True)

    t0 = time.time()
    if not args.no_init1:
        w = Watch(address=addr)
        await w.connect(args.timeout)
        await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
        key = w.key
        frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key)
        await w.client.write_gatt_char(CMD_W, frame, response=True)
        print(f"[{ts()}] >BLE INIT1 {frame.hex()}", flush=True)
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

    last = None
    while time.time() - t0 < args.duration:
        r = scan_round(addr)
        sig = tuple((k, v[0], tuple(v[1])) for k, v in r.items())
        if sig != last:
            print(f"[{ts()}] +{time.time()-t0:5.1f}s  {r}", flush=True)
            last = sig
        time.sleep(max(0.0, args.interval - 0.2))


asyncio.run(main())
