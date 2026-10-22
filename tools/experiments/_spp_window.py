import argparse
import asyncio
import time
from types import SimpleNamespace

from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W
import rfcomm


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


async def ble_auth(addr, timeout):
    w = Watch(address=addr)
    await w.connect(timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=timeout))
    print(f"[{ts()}] BLE authed key={'set' if w.key else 'none'}", flush=True)
    return w


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--channels", default="11")
    ap.add_argument("--layout", default="packed2")
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--window", type=float, default=45.0)
    ap.add_argument("--connect-timeout", type=float, default=1.2)
    ap.add_argument("--init2-hex", default="0100005a")
    ap.add_argument("--no-ble-init1", action="store_true")
    args = ap.parse_args()
    addr = args.address or load_state().get("address")
    chans = [int(c) for c in args.channels.split(",")]

    rfcomm.ws_init()
    w = await ble_auth(addr, args.timeout)
    key = w.key

    if not args.no_ble_init1:
        frame = build_frame(0xFFFF, 0x9052, b"\xa5", key=key)
        await w.client.write_gatt_char(CMD_W, frame, response=True)
        print(f"[{ts()}] >BLE INIT1 {frame.hex()}", flush=True)
    try:
        await w.disconnect()
    except Exception:
        pass

    t0 = time.time()
    print(f"[{ts()}] window start ({args.window}s) layout={args.layout} chans={chans}", flush=True)
    while time.time() - t0 < args.window:
        for ch in chans:
            dt = time.time() - t0
            s, err = rfcomm.connect_bth_timeout(addr, ch, args.layout, timeout=args.connect_timeout)
            if s is None:
                print(f"[{ts()}] +{dt:5.1f}s ch{ch:2d} fail {err}", flush=True)
                continue
            print(f"[{ts()}] +{dt:5.1f}s ch{ch:2d} *** CONNECTED", flush=True)
            s.settimeout(600)
            end = time.time() + 1.0
            got = bytearray()
            while time.time() < end:
                try:
                    d = s.recv(2048)
                except OSError as e:
                    print(f"[{ts()}]   recv-err {e}", flush=True)
                    d = None
                if d:
                    print(f"[{ts()}]   << {d.hex()} ({len(d)})", flush=True)
                    got += d
                else:
                    time.sleep(0.03)
            for enc in ("session", "none"):
                k = key if enc == "session" else None
                f2 = build_frame(0xFFFF, 0x9040, bytes.fromhex(args.init2_hex), key=k)
                n = s.send(f2)
                print(f"[{ts()}]   >> INIT2({enc}) {f2.hex()} ({n})", flush=True)
                end = time.time() + 4.0
                while time.time() < end:
                    try:
                        d = s.recv(2048)
                    except OSError as e:
                        print(f"[{ts()}]   recv-err {e}", flush=True)
                        d = None
                    if d:
                        print(f"[{ts()}]   << {d.hex()} ({len(d)})", flush=True)
                        got += d
                    else:
                        time.sleep(0.03)
            try:
                s.close()
            except Exception:
                pass
            print(f"[{ts()}]   closed (got {len(got)}B)", flush=True)
    print(f"[{ts()}] window done", flush=True)


asyncio.run(main())
