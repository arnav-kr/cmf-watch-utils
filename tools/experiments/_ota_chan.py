import argparse, asyncio, time
from cmf_watch import Watch, ensure_session, build_frame, FrameParser

OTA_W = "02f00000-0000-0000-0000-00000000ff01"
OTA_N = "02f00000-0000-0000-0000-00000000ff02"


async def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", default=None)
    ap.add_argument("--timeout", type=float, default=30.0)
    ap.add_argument("--init1-hex", default="014a")
    ap.add_argument("--ver-hex", default="01000074")
    ap.add_argument("--init1", action="store_true")
    ap.add_argument("--init2", action="store_true")
    ap.add_argument("--both", action="store_true")
    ap.add_argument("--listen", type=float, default=4.0)
    args = ap.parse_args()
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    parser = FrameParser()
    seen = []

    def on_ota(_, d):
        b = bytes(d)
        print("  <ota raw>", b.hex(), flush=True)
        for c1, c2, cc, ci, body, enc in parser.feed(b):
            dec = w._decode(c1, c2, body, enc)
            seen.append((c1, c2, dec))

    try:
        await ensure_session(w, args)
        await w.client.start_notify(OTA_N, on_ota)
        print(f"subscribed OTA notify {OTA_N}", flush=True)
        if args.init1 or args.both:
            frame = build_frame(0xFFFF, 0x9052, bytes.fromhex(args.init1_hex), key=w.key)
            print(">OTA INIT1", frame.hex(), flush=True)
            await w.client.write_gatt_char(OTA_W, frame, response=True)
            await asyncio.sleep(args.listen)
            print("seen:", [(hex(c1), hex(c2), b.hex()) for c1, c2, b in seen], flush=True)
        if args.init2 or args.both:
            seen.clear()
            frame = build_frame(0xFFFF, 0x9040, bytes.fromhex(args.ver_hex), key=w.key)
            print(">OTA INIT2", frame.hex(), flush=True)
            try:
                await w.client.write_gatt_char(OTA_W, frame, response=True)
            except Exception as e:
                print("write err", e)
            end = time.time() + args.listen
            while time.time() < end and w.client.is_connected:
                await asyncio.sleep(0.3)
            print("seen:", [(hex(c1), hex(c2), b.hex()) for c1, c2, b in seen], flush=True)
            print("connected:", w.client.is_connected, flush=True)
    finally:
        await w.disconnect()


asyncio.run(main())
