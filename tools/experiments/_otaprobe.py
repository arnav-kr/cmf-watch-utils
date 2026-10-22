import asyncio
import types

from cmf_watch import Watch, ensure_session

ADDR = "AA:BB:CC:DD:EE:FF"


async def main():
    args = types.SimpleNamespace(address=ADDR, timeout=10.0, repair=False)
    w = Watch(address=ADDR)
    await w.connect(10.0)
    try:
        await ensure_session(w, args)
        for p, label in [(b"", "empty"), (b"\xa5", "a5"), (b"\x00", "00"),
                         (b"\x01", "01"), (b"\x0b\x00\x00\x39", "ver 11.0.0.57")]:
            r = await w.send(0xFFFF, 0x9052, p, expect=0xA052, timeout=6.0)
            for c1, c2, b in r:
                print(f"INIT1 payload={label:<14} -> {c2:#06x} len={len(b)} {b.hex()}")
            if not r:
                print(f"INIT1 payload={label:<14} -> (no A052)")
    finally:
        await w.disconnect()


asyncio.run(main())
