import argparse
import asyncio
import time
from types import SimpleNamespace

from cmf_watch import Watch, ensure_session, load_state, CMD_W


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--timeout", type=float, default=12.0)
    ap.add_argument("--ver-hex", default="0100005a")
    ap.add_argument("--init1-hex", default="a5")
    ap.add_argument("--gap", type=float, default=0.0, help="delay before INIT2 (0=immediate)")
    ap.add_argument("--after-a052", action="store_true", help="send INIT2 only after A052")
    ap.add_argument("--listen", type=float, default=20.0)
    args = ap.parse_args()
    addr = args.address or load_state().get("address")

    w = Watch(address=addr)
    await w.connect(args.timeout)
    await ensure_session(w, SimpleNamespace(repair=False, address=addr, timeout=args.timeout))
    print(f"[{ts()}] authed connected={w.client.is_connected} sessionKey={w.key.hex()}", flush=True)

    init1 = bytes.fromhex(args.init1_hex)
    ver = bytes.fromhex(args.ver_hex)

    async def do_send(cmd2, payload, tag):
        f = __import__("cmf_watch").build_frame(0xFFFF, cmd2, payload, key=w.key)
        try:
            await w.client.write_gatt_char(CMD_W, f, response=True)
            print(f"[{ts()}] >{tag} cmd2={cmd2:#06x} payload={payload.hex()} frame={f.hex()}", flush=True)
        except Exception as e:
            print(f"[{ts()}] {tag} write err: {e}", flush=True)

    await do_send(0x9052, init1, "INIT1")

    got_a052 = False
    t0 = time.time()
    end = t0 + args.listen
    init2_sent = False
    while time.time() < end:
        try:
            piece = await asyncio.wait_for(w.rx.get(), timeout=0.3)
        except asyncio.TimeoutError:
            piece = None
        if piece is not None:
            for c1, c2, cc, ci, body, enc in w.parser.feed(piece):
                dec = w._decode(c1, c2, body, enc)
                print(f"[{ts()}] <frame cmd1={c1:#06x} cmd2={c2:#06x} enc={enc} len={len(dec)} {dec.hex()} (+{time.time()-t0:.3f}s)", flush=True)
                if c2 == 0xA052:
                    got_a052 = True
                if c2 == 0xA040:
                    print(f"[{ts()}] *** A040 (INIT2 ack!) — watch accepted INIT2", flush=True)
        if not init2_sent:
            if args.after_a052:
                if got_a052:
                    await do_send(0x9040, ver, "INIT2(after A052)")
                    init2_sent = True
            elif args.gap <= 0:
                await do_send(0x9040, ver, "INIT2(immediate)")
                init2_sent = True
            elif time.time() - t0 >= args.gap:
                await do_send(0x9040, ver, "INIT2(gap)")
                init2_sent = True
        if not w.client.is_connected:
            print(f"[{ts()}] *** LINK DROPPED at +{time.time()-t0:.3f}s", flush=True)
            break

    print(f"[{ts()}] done: a052={got_a052} connected={w.client.is_connected}", flush=True)
    try:
        await w.disconnect()
    except Exception:
        pass


asyncio.run(main())
