import argparse
import time
from cmf_watch import build_frame, load_state
import rfcomm


def ts():
    return time.strftime("%H:%M:%S", time.localtime()) + f".{int((time.time() % 1) * 1000):03d}"


ap = argparse.ArgumentParser()
ap.add_argument("--address", default=None)
ap.add_argument("--channel", type=int, default=11)
args = ap.parse_args()
addr = args.address or load_state().get("address")

rfcomm.ws_init()
s = rfcomm.connect_bth(addr, args.channel, "packed2")
s.settimeout(300)
print(f"[{ts()}] SPP ch{args.channel} connected; sending benign frames", flush=True)

tests = [
    ("devInfo 0x0004 none", build_frame(0xFFFF, 0x0004, b"", key=None)),
    ("resv 0xa061 none", build_frame(0xFFFF, 0xA061, b"", key=None)),
    ("init1 0x9052 none", build_frame(0xFFFF, 0x9052, b"\xa5", key=None)),
    ("raw f5 00 05 ffff 0001 0001 0001", bytes.fromhex("f50005ffff000100010001 0000".replace(" ", ""))),
]
for name, fr in tests:
    try:
        s.sendall(fr)
        print(f"[{ts()}] >> {name} {fr.hex()}", flush=True)
    except OSError as e:
        print(f"[{ts()}] >> {name} sendfail {e}", flush=True)
        break
    end = time.time() + 2.0
    while time.time() < end:
        try:
            d = s.recv(4096)
        except OSError as e:
            print(f"[{ts()}] << err {e}", flush=True)
            d = None
            break
        if d:
            print(f"[{ts()}] << {d.hex()} ({len(d)})", flush=True)
        else:
            time.sleep(0.03)
s.close()
