import argparse
import asyncio
import time
from types import SimpleNamespace

from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W
import rfcomm


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--channels", default="11,10,12,13")
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--init2-hex", default="0100005a")
    ap.add_argument("--enc", default="session", choices=["session", "none", "both"])
    ap.add_argument("--window", type=float, default=20.0)
    ap.add_argument("--settle", type=float, default=0.0)
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--no-init1", action="store_true")
    args = ap.parse_args()
    addr = args.address or load_state().get("address")
    chans = [int(c) for c in args.channels.split(",") if c.strip()]

    rfcomm.ws_init()

    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    key = w.key
    print(f"[{ts()}] BLE authed key={'set' if key else 'none'}", flush=True)

    t0 = time.time()
    if not args.no_init1:
        frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key)
        await w.client.write_gatt_char(CMD_W, frame, response=True)
        print(f"[{ts()}] >BLE INIT1 {frame.hex()}", flush=True)
        end = t0 + 3.0
        while time.time() < end:
            try:
                piece = await asyncio.wait_for(w.rx.get(), timeout=0.15)
            except asyncio.TimeoutError:
                piece = None
            if piece is not None:
                for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                    print(f"[{ts()}] <BLE cmd2={c2:#06x} {w._decode(c1,c2,body,enc).hex()} (+{time.time()-t0:.3f}s)", flush=True)
            if not w.client.is_connected:
                print(f"[{ts()}] *** BLE DROPPED +{time.time()-t0:.3f}s", flush=True)
                break
    try:
        await w.disconnect()
    except Exception:
        pass

    if args.settle:
        time.sleep(args.settle)

    print(f"[{ts()}] hunting SPP channels {chans} for {args.window:.0f}s (T+{time.time()-t0:.1f}s)", flush=True)
    got = False
    while time.time() - t0 < args.window and not got:
        for ch in chans:
            s, err = rfcomm.connect_nb_wait(addr, ch, "packed2", timeout=1.5)
            if s is None:
                continue
            got = True
            print(f"[{ts()}] SPP CONNECTED ch{ch} (+{time.time()-t0:.2f}s)", flush=True)
            s.settimeout(250)
            for enc_mode in (["session", "none"] if args.enc == "both" else [args.enc]):
                ekey = key if enc_mode == "session" else None
                fr2 = build_frame(0xFFFF, 0x9040, bytes.fromhex(args.init2_hex), key=ekey)
                s.sendall(fr2)
                print(f"[{ts()}] >>SPP INIT2({enc_mode}) {fr2.hex()}", flush=True)
            end = time.time() + 10.0
            while time.time() < end:
                try:
                    d = s.recv(4096)
                except OSError as e:
                    print(f"[{ts()}] <<SPP err {e}", flush=True)
                    break
                if d:
                    print(f"[{ts()}] <<SPP {d.hex()} ({len(d)}) (+{time.time()-t0:.2f}s)", flush=True)
                else:
                    time.sleep(0.05)
            s.close()
            break
        if not got:
            time.sleep(0.05)
    if not got:
        print(f"[{ts()}] no SPP connect within window", flush=True)


asyncio.run(main())
