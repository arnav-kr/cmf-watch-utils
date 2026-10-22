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
    ap.add_argument("--start", type=float, default=18.0)
    ap.add_argument("--window", type=float, default=34.0)
    ap.add_argument("--timeout", type=float, default=12.0)
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    rfcomm.ws_init()
    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    key = w.key
    fr = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key)
    await w.client.write_gatt_char(CMD_W, fr, response=True)
    print(f"[{ts()}] >BLE INIT1 {fr.hex()}", flush=True)
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

    s = None
    while time.time() - t0 < args.window:
        if time.time() - t0 < args.start:
            time.sleep(0.1)
            continue
        s, err = rfcomm.connect_nb_wait(addr, args.channel, "packed2", timeout=1.0)
        if s is not None:
            print(f"[{ts()}] SPP ch{args.channel} CONNECTED (+{time.time()-t0:.2f}s)", flush=True)
            break
        time.sleep(0.05)
    if s is None:
        print(f"[{ts()}] never connected", flush=True)
        return
    s.settimeout(300)

    seq = [
        ("INIT1(session)", build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key), 2.0),
        ("INIT1(none)", build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=None), 2.0),
        ("INIT2(session)", build_frame(0xFFFF, 0x9040, bytes.fromhex(args.init2_hex), key=key), 3.0),
        ("INIT2(none)", build_frame(0xFFFF, 0x9040, bytes.fromhex(args.init2_hex), key=None), 3.0),
    ]
    for name, frx, wait in seq:
        try:
            s.sendall(frx)
            print(f"[{ts()}] >>SPP {name} {frx.hex()}", flush=True)
        except OSError as e:
            print(f"[{ts()}] >>SPP {name} sendfail {e}", flush=True)
            break
        end = time.time() + wait
        while time.time() < end:
            try:
                d = s.recv(4096)
            except OSError as e:
                print(f"[{ts()}] <<SPP err {e}", flush=True)
                d = None
                end = 0
                break
            if d:
                print(f"[{ts()}] <<SPP {d.hex()} ({len(d)}) (+{time.time()-t0:.2f}s)", flush=True)
            else:
                time.sleep(0.03)
    try:
        s.close()
    except Exception:
        pass


asyncio.run(main())
