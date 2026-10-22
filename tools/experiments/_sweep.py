import argparse
import time

import rfcomm


def parse_range(text):
    out = []
    for part in text.split(","):
        part = part.strip()
        if "-" in part:
            a, b = part.split("-")
            out.extend(range(int(a), int(b) + 1))
        elif part:
            out.append(int(part))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--address", required=True)
    ap.add_argument("--channels", default="1-30")
    ap.add_argument("--layout", default="packed2")
    ap.add_argument("--timeout", type=float, default=2.0)
    args = ap.parse_args()
    rfcomm.ws_init()
    chans = parse_range(args.channels)
    open_ch = []
    for ch in chans:
        t0 = time.time()
        s, err = rfcomm.connect_bth_timeout(args.address, ch, args.layout, timeout=args.timeout)
        dt = time.time() - t0
        if s is not None:
            print(f"ch{ch:2d} OPEN      {dt:.2f}s", flush=True)
            open_ch.append(ch)
            s.close()
        else:
            print(f"ch{ch:2d} {err:22s} {dt:.2f}s", flush=True)
    print(f"\nOPEN CHANNELS: {open_ch}", flush=True)


if __name__ == "__main__":
    main()
