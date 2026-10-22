import argparse, asyncio, time
from cmf_watch import Watch, _pull, ensure_session


def dump(w, tag):
    print(f"--- services {tag} ---")
    try:
        svcs = list(w.client.services)
    except Exception as e:
        print("  services err:", e)
        return
    for s in svcs:
        print(f"svc {s.uuid}")
        for ch in getattr(s, "characteristics", []):
            print(f"  chr {ch.uuid} {ch.properties}")


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--timeout", type=float, default=30.0)
    ap.add_argument("--init1", action="store_true")
    ap.add_argument("--init1-hex", default="014a")
    ap.add_argument("--init2", action="store_true")
    ap.add_argument("--ver-hex", default="01000074")
    ap.add_argument("--listen", type=float, default=0.0)
    args = ap.parse_args()
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        dump(w, "pre-INIT1")
        if args.init1:
            r = await w.send(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), expect=0xA052, timeout=8.0)
            for c1, c2, b in r:
                print(f"  INIT1 reply {c2:#06x} len={len(b)} {b.hex()}")
            dump(w, "post-INIT1 (cached)")
            try:
                await w.client.get_services()
            except Exception as e:
                print("  get_services err:", e)
            dump(w, "post-INIT1 (rediscover)")
        if args.init2:
            r = await w.send(0xFFFF, 0x9040, bytes.fromhex(args.ver_hex), expect=0xA040, timeout=8.0)
            for c1, c2, b in r:
                print(f"  INIT2 reply {c2:#06x} len={len(b)} {b.hex()}")
            dump(w, "post-INIT2")
        if args.listen > 0:
            end = time.time() + args.listen
            while time.time() < end and w.client.is_connected:
                for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                    dec = w._decode(c1, c2, body, enc)
                    print(f"<{src} {c1:#06x},{c2:#06x}> enc={enc} {dec.hex()}")
    finally:
        await w.disconnect()


asyncio.run(main())
