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
    ap.add_argument("--channel", type=int, default=11)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--init2-hex", default="0100005a")
    ap.add_argument("--enc", default="session", choices=["session", "none", "both"])
    ap.add_argument("--send-delay", type=float, default=0.0)
    ap.add_argument("--read", type=float, default=12.0)
    ap.add_argument("--timeout", type=float, default=12.0)
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    rfcomm.ws_init()
    t0 = time.time()
    print(f"[{ts()}] opening SPP ch{args.channel} ...", flush=True)
    s = rfcomm.connect_bth(addr, args.channel, "packed2")
    print(f"[{ts()}] SPP CONNECTED (+{time.time()-t0:.2f}s)", flush=True)
    s.settimeout(300)

    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    key = w.key
    print(f"[{ts()}] BLE authed key={'set' if key else 'none'} (+{time.time()-t0:.2f}s)", flush=True)

    frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key)
    await w.client.write_gatt_char(CMD_W, frame, response=True)
    print(f"[{ts()}] >BLE INIT1 {frame.hex()}", flush=True)

    sent = False
    end = time.time() + 6.0
    while time.time() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=0.1)
        except asyncio.TimeoutError:
            piece = None
        if piece is not None:
            for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                print(f"[{ts()}] <BLE cmd2={c2:#06x} (+{time.time()-t0:.3f}s)", flush=True)
                if c2 in (0xA052, 0x9052) and not sent:
                    if args.send_delay:
                        time.sleep(args.send_delay)
                    for m in (["session", "none"] if args.enc == "both" else [args.enc]):
                        ekey = key if m == "session" else None
                        fr2 = build_frame(0xFFFF, 0x9040, bytes.fromhex(args.init2_hex), key=ekey)
                        try:
                            s.sendall(fr2)
                            print(f"[{ts()}] >>SPP INIT2({m}) {fr2.hex()}", flush=True)
                            sent = True
                        except OSError as e:
                            print(f"[{ts()}] >>SPP INIT2({m}) failed {e}", flush=True)
        # opportunistically read SPP
        try:
            d = s.recv(4096)
            if d:
                print(f"[{ts()}] <<SPP {d.hex()} ({len(d)}) (+{time.time()-t0:.2f}s)", flush=True)
        except OSError as e:
            print(f"[{ts()}] <<SPP err {e} (+{time.time()-t0:.2f}s)", flush=True)
            break
        if not w.client.is_connected:
            print(f"[{ts()}] *** BLE DROPPED +{time.time()-t0:.3f}s", flush=True)
            break

    print(f"[{ts()}] draining SPP {args.read:.0f}s ...", flush=True)
    end = time.time() + args.read
    while time.time() < end:
        try:
            d = s.recv(4096)
        except OSError as e:
            print(f"[{ts()}] <<SPP err {e}", flush=True)
            break
        if d:
            print(f"[{ts()}] <<SPP {d.hex()} ({len(d)})", flush=True)
        else:
            time.sleep(0.05)
    try:
        s.close()
    except Exception:
        pass


asyncio.run(main())
