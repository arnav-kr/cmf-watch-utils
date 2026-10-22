import argparse
import asyncio
import time
from types import SimpleNamespace

from cmf_watch import Watch, ensure_session, build_frame, load_state, CMD_W
import rfcomm


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


def spp_read(s, seconds, tag="<<", idle_stop=True):
    end = time.time() + seconds
    got = bytearray()
    s.settimeout(300)
    while time.time() < end:
        try:
            d = s.recv(4096)
        except OSError as e:
            print(f"[{ts()}] {tag} recv-error {e}", flush=True)
            break
        if d:
            print(f"[{ts()}] {tag} {d.hex()} ({len(d)})", flush=True)
            got += d
        else:
            time.sleep(0.03)
            if idle_stop and got and time.time() > end - seconds + 0.5:
                pass
    return bytes(got)


def spp_send(s, data, tag=">>"):
    n = s.send(data)
    print(f"[{ts()}] {tag} {data.hex()} ({n}/{len(data)})", flush=True)
    return n


async def ble_auth(addr, timeout):
    w = Watch(address=addr)
    await w.connect(timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=timeout))
    print(f"[{ts()}] BLE authed connected={w.client.is_connected} key={'set' if w.key else 'none'}", flush=True)
    return w


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--mode", default="listen",
                    choices=["listen", "spp-init1", "ble-init1", "ble-init1-spp", "full"])
    ap.add_argument("--spp-channel", type=int, default=11)
    ap.add_argument("--layout", default="packed2")
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--probe-seconds", type=float, default=25.0)
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--init2-hex", default="0100005a")
    ap.add_argument("--data-hex", default=None)
    ap.add_argument("--enc", default="session", choices=["session", "none"])
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    rfcomm.ws_init()
    key = None

    if args.mode != "listen":
        w = await ble_auth(addr, args.timeout)
        key = w.key
        if args.mode in ("ble-init1", "ble-init1-spp", "full"):
            frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=key)
            t0 = time.time()
            await w.client.write_gatt_char(CMD_W, frame, response=True)
            print(f"[{ts()}] >BLE INIT1 frame={frame.hex()}", flush=True)
            end = t0 + 3.0
            while time.time() < end:
                try:
                    piece = await asyncio.wait_for(w.rx.get(), timeout=0.2)
                except asyncio.TimeoutError:
                    piece = None
                if piece is not None:
                    for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                        dec = w._decode(c1, c2, body, enc)
                        print(f"[{ts()}] <BLE frame cmd2={c2:#06x} {dec.hex()} (+{time.time()-t0:.3f}s)", flush=True)
                if not w.client.is_connected:
                    print(f"[{ts()}] *** BLE LINK DROPPED +{time.time()-t0:.3f}s", flush=True)
                    break
        try:
            await w.disconnect()
        except Exception:
            pass
        if args.mode == "ble-init1":
            return

    ekey = key if args.enc == "session" else None

    if args.mode == "spp-init1":
        print(f"[{ts()}] opening SPP ch{args.spp_channel} ...", flush=True)
        s = rfcomm.connect_bth(addr, args.spp_channel, args.layout)
        print(f"[{ts()}] SPP CONNECTED", flush=True)
        frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=ekey)
        spp_send(s, frame, ">>SPP INIT1")
        spp_read(s, args.probe_seconds, "<<SPP")
        s.close()
        return

    if args.mode in ("ble-init1-spp", "full"):
        print(f"[{ts()}] probing SPP connect window ...", flush=True)
        t0 = time.time()
        s = None
        attempt = 0
        while time.time() - t0 < args.probe_seconds:
            attempt += 1
            dt = time.time() - t0
            try:
                s = rfcomm.connect_bth(addr, args.spp_channel, args.layout)
                print(f"[{ts()}] SPP CONNECTED +{dt:.1f}s attempt#{attempt}", flush=True)
                break
            except OSError as e:
                print(f"[{ts()}] +{dt:.1f}s attempt#{attempt} fail {e}", flush=True)
                time.sleep(0.5)
        if s is None:
            print(f"[{ts()}] SPP never connected (attempts={attempt})", flush=True)
            return
        spp_read(s, 1.5, "<<SPP-early")
        frame = build_frame(0xFFFF, 0x9040, bytes.fromhex(args.init2_hex), key=ekey)
        spp_send(s, frame, ">>SPP INIT2")
        resp = spp_read(s, 8.0, "<<SPP")
        if args.mode == "full" and args.data_hex:
            data = bytes.fromhex(args.data_hex)
            spp_send(s, data, ">>SPP DATA")
            spp_read(s, 15.0, "<<SPP")
        s.close()
        return

    if args.mode == "listen":
        print(f"[{ts()}] opening SPP ch{args.spp_channel} ...", flush=True)
        s = rfcomm.connect_bth(addr, args.spp_channel, args.layout)
        print(f"[{ts()}] SPP CONNECTED", flush=True)
        spp_read(s, args.probe_seconds, "<<SPP")
        s.close()
        return


asyncio.run(main())
