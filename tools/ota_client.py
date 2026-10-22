"""Standalone OTA flasher for CMF Watch Pro 2 (Nothing X protocol).

Flow (reverse-engineered from a successful app-driven OTA):

    reconnect auth  0x8049 / 0x804B (nonce) / 0x804D / 0x8004
    INIT1           0x9052  payload b"\\xa5"                 -> watcher replies 0xA052
    INIT2           0x9040  payload <version 4 bytes>        -> 0xA040
    data phase      watch sends 0xA042 with AES(session) body:
                        offset (u32 BE) | length (u32 BE) | 4B | 1B
                    we answer with ceil(length/224) frames of cmd 0x9042 on the
                    OTA characteristic 02f00000-0000-0000-0000-00000000ff01:
                        F5 | len(u16) | FFFF | count(u16) | index(u16) | 9042
                           | 224B plaintext firmware | crc32le(u32)
    end             watch sends 0xA042 offset=0 length=0 -> we send 0x9041

Session key = sha256(nonce + authkey)[:16] (established by the reconnect
handshake; the watch derives the same value).  The persisted
BluetoothSessionsKey in the app prefs is exactly this key, which is why
captured 0xA042 frames decrypt with it.

Usage:
    python ota_client.py --image ..\\firmware\\JX402_V1.0.0.73.bin
    python ota_client.py --image ... --dry-run
    python ota_client.py --image ... --key 61a86009ebfaeb84fef64e9041f758d6
"""

import argparse
import asyncio
import json
import os
import struct
import sys
import time

from bleak import BleakClient, BleakScanner

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cmf_watch as cw

DEFAULT_ADDR = None
REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_IMAGE = os.path.join(REPO_ROOT, "firmware", "JX402_V1.0.0.73.bin")
STATE = os.path.join(os.path.dirname(os.path.abspath(__file__)), ".watch_state.json")

OTA_SVC = "02f00000-0000-0000-0000-00000000ff01"
OTA_NOTIFY = "02f00000-0000-0000-0000-00000000ff02"
CHUNK = 224


def load_authkey():
    try:
        with open(STATE, "r", encoding="utf-8") as f:
            return bytes.fromhex(json.load(f)["authkey"])
    except Exception:
        return None


def ts():
    return time.strftime("%H:%M:%S") + f".{int(time.time() * 1000) % 1000:03d}"


class Ota:
    def __init__(self, args):
        self.args = args
        self.img = open(args.image, "rb").read()
        self.key = None
        self.authkey = None
        self.client = None
        self.rx = asyncio.Queue()
        self.parser = cw.FrameParser()
        self.served_frames = 0
        self.served_bytes = 0
        self.last_off = -1
        self.ota_write = args.ota_write or OTA_SVC
        self.started = False
        self.done = asyncio.Event()
        self.a052 = asyncio.Event()
        self.a040 = asyncio.Event()

    def log(self, msg):
        print(f"[{ts()}] {msg}", flush=True)

    async def find(self):
        self.log("scanning ...")
        devs = await BleakScanner.discover(timeout=10.0)
        for d in devs:
            if self.args.address and (d.address or "").upper() == self.args.address.upper():
                return d
        for d in devs:
            if "watch" in (d.name or "").lower() or "cmf" in (d.name or "").lower():
                return d
        raise SystemExit("watch not found (pass --address)")

    async def connect(self):
        dev = await self.find()
        self.log(f"connecting {dev.address} {dev.name} ...")
        self.client = BleakClient(dev, timeout=30.0)
        await self.client.connect()
        self.log(f"connected={self.client.is_connected} mtu={getattr(self.client,'mtu_size',None)}")
        await self.subscribe_all()

    async def subscribe_all(self):
        notify = []
        for svc in self.client.services:
            for ch in svc.characteristics:
                if "notify" in ch.properties or "indicate" in ch.properties:
                    notify.append(ch.uuid)
        self.log(f"notify chars: {len(notify)}")
        for u in notify:
            try:
                await self.client.start_notify(u, self.on_notify)
            except Exception as e:
                self.log(f"  (notify {u} failed: {e})")

    def on_notify(self, sender, data):
        b = bytes(data)
        for cmd1, cmd2, cc, ci, body, enc in self.parser.feed(b):
            self.rx.put_nowait((cmd1, cmd2, cc, ci, body, enc))

    def decrypt(self, body, enc):
        if not enc or not self.key or not body:
            return body
        try:
            d = cw.aes_dec(self.key, body)
            if len(d) >= 4 and cw.crc32_le(d[:-4]) == d[-4:]:
                return d[:-4]
            return d
        except Exception:
            return body

    async def wait_cmd(self, cmd2, timeout):
        end = time.monotonic() + timeout
        while True:
            remain = end - time.monotonic()
            if remain <= 0:
                return None
            try:
                cmd1, c2, cc, ci, body, enc = await asyncio.wait_for(self.rx.get(), timeout=remain)
            except asyncio.TimeoutError:
                return None
            dec = self.decrypt(body, enc)
            if self.args.verbose:
                self.log(f"  < cmd2={c2:#06x} cc={cc} ci={ci} enc={enc} {dec.hex()}")
            if c2 == cmd2:
                return dec

    async def handshake(self):
        self.authkey = bytes.fromhex(self.args.authkey) if self.args.authkey else load_authkey()
        if not self.authkey:
            raise SystemExit("no authkey (pass --authkey or create .watch_state.json)")
        self.log(f"authkey={self.authkey.hex()}")
        self.key = self.authkey
        await self.write_cmd(cw.build_frame(0xFFFF, 0x8049, b"\xa5CMF Watch Pro 2", key=self.authkey))
        await self.wait_cmd(0x0049, 5.0)
        nonce = None
        for _ in range(3):
            await self.write_cmd(cw.build_frame(0xFFFF, 0x804B, b"\xa5", key=self.authkey))
            nonce = await self.wait_cmd(0x004C, 4.0)
            if nonce:
                break
        if not nonce:
            raise SystemExit("no nonce (0x804C)")
        self.log(f"nonce={nonce.hex()}")
        self.key = cw.derive_session(nonce, self.authkey)
        self.log(f"sessionKey={self.key.hex()}")
        await self.write_cmd(cw.build_frame(0xFFFF, 0x804D, b"\xa5", key=self.key))
        await self.wait_cmd(0x0004, 4.0)
        await self.write_cmd(cw.build_frame(0xFFFF, 0x8004,
                                            struct.pack(">ii", int(time.time()), cw.local_utc_offset_ms()),
                                            key=self.key))
        if self.args.key:
            self.key = bytes.fromhex(self.args.key)
            self.log(f"sessionKey overridden -> {self.key.hex()}")

    async def write_cmd(self, frame, resp=True):
        await self.client.write_gatt_char(cw.CMD_W, frame, response=resp)

    async def write_ota(self, frame):
        try:
            await self.client.write_gatt_char(self.ota_write, frame, response=False)
        except Exception as e:
            if self.args.verbose:
                self.log(f"  (ota write failed on {self.ota_write}: {e}; retry response=True)")
            await self.client.write_gatt_char(self.ota_write, frame, response=True)

    async def send_data(self, off, ln):
        end = min(off + ln, len(self.img))
        if off >= len(self.img):
            self.log(f"!! requested offset {off} beyond image ({len(self.img)})")
            return
        if self.args.verbose:
            self.log(f"  req off={off:#x} len={ln}")
        if self.last_off >= 0 and off != self.last_off and self.args.verbose:
            self.log(f"  (non-sequential request: expected {self.last_off:#x}, got {off:#x})")
        n = max(1, (end - off + CHUNK - 1) // CHUNK)
        pos = off
        idx = 0
        pending = []
        while pos < end:
            piece = self.img[pos:min(pos + CHUNK, end)]
            pending.append(cw.build_frame(0xFFFF, 0x9042, piece, chunk_index=idx + 1, chunk_count=n))
            pos += len(piece)
            idx += 1
        step = max(1, self.args.pipeline)
        for i in range(0, len(pending), step):
            batch = pending[i:i + step]
            t0 = time.monotonic()
            if len(batch) == 1:
                await self.write_ota(batch[0])
            else:
                await asyncio.gather(*(self.write_ota(f) for f in batch))
            dt = (time.monotonic() - t0) * 1000
            if self.args.verbose and dt > 100:
                self.log(f"  (batch {len(batch)} took {dt:.0f}ms)")
        self.served_frames += len(pending)
        self.served_bytes += (end - off)
        self.last_off = end
        if self.served_frames % 1800 < n:
            pct = 100.0 * self.served_bytes / len(self.img)
            self.log(f"  served {self.served_frames} frames {self.served_bytes}B "
                     f"({pct:.1f}%) off={off:#x} len={ln}")

    async def run(self):
        await self.connect()
        try:
            await self.handshake()

            ver = bytes.fromhex(self.args.version)
            self.log(f"INIT1 0x9052 (OTA begin)")
            await self.write_cmd(cw.build_frame(0xFFFF, 0x9052, b"\xa5", key=self.key))
            if not await self.wait_cmd(0xA052, 6.0):
                self.log("!! no A052, retrying INIT1 on OTA char")
                await self.write_ota(cw.build_frame(0xFFFF, 0x9052, b"\xa5", key=self.key))
                if not await self.wait_cmd(0xA052, 6.0):
                    raise SystemExit("no A052 (0x9052 rejected)")
            self.log("A052 ok")
            if self.args.dry_run:
                self.log("dry-run: stopping after A052")
                return

            self.log(f"INIT2 0x9040 version={ver.hex()}")
            await self.write_ota(cw.build_frame(0xFFFF, 0x9040, ver, key=self.key))
            got = await self.wait_cmd(0xA040, 5.0)
            if got is None and self.client.is_connected:
                self.log("!! no A040 on OTA char, retrying INIT2 on CMD char")
                await self.write_cmd(cw.build_frame(0xFFFF, 0x9040, ver, key=self.key))
                got = await self.wait_cmd(0xA040, 5.0)
            if got is None:
                self.log(f"link connected={self.client.is_connected}")
                raise SystemExit("no A040 (0x9040 rejected) - watch may have reset the link")
            self.log("A040 ok, entering data phase")
            if self.args.stop_after_a040:
                self.log("stopping after A040 (no data sent)")
                return

            while True:
                try:
                    cmd1, c2, cc, ci, body, enc = await asyncio.wait_for(self.rx.get(), timeout=self.args.idle_timeout)
                except asyncio.TimeoutError:
                    self.log(f"no frames for {self.args.idle_timeout}s, aborting")
                    break
                dec = self.decrypt(body, enc)
                if c2 == 0xA042:
                    if len(dec) < 8:
                        continue
                    off = int.from_bytes(dec[0:4], "big")
                    ln = int.from_bytes(dec[4:8], "big")
                    if off == 0 and ln == 0:
                        self.log(f"END request; served {self.served_bytes} bytes, sending 0x9041")
                        await self.write_cmd(cw.build_frame(0xFFFF, 0x9041, b"\xa5", key=self.key))
                        self.done.set()
                        break
                    await self.send_data(off, ln)
                elif c2 == 0xA041:
                    self.log(f"watch status 0x{c2:04x} {dec.hex()}; sending 0x9041 END")
                    await self.write_cmd(cw.build_frame(0xFFFF, 0x9041, b"\xa5", key=self.key))
                    self.done.set()
                    break
                elif c2 in (0xA043, 0xA044):
                    self.log(f"watch status 0x{c2:04x} {dec.hex()}")
                    self.done.set()
                    break

            if self.done.is_set():
                self.log("waiting for post-flash status ...")
                deadline = time.monotonic() + 20
                while time.monotonic() < deadline:
                    try:
                        cmd1, c2, cc, ci, body, enc = await asyncio.wait_for(self.rx.get(), timeout=2.0)
                    except asyncio.TimeoutError:
                        continue
                    self.log(f"  <= cmd2=0x{c2:04x} {self.decrypt(body, enc).hex()}")
                    if c2 in (0xA041, 0xA043, 0xA044):
                        break
        finally:
            self.log(f"summary: frames={self.served_frames} bytes={self.served_bytes} "
                     f"({100.0*self.served_bytes/len(self.img):.1f}% of image)")
            try:
                await self.client.disconnect()
            except Exception:
                pass


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--image", default=DEFAULT_IMAGE)
    ap.add_argument("--address", default=DEFAULT_ADDR)
    ap.add_argument("--authkey", default=None, help="16-byte hex; default from .watch_state.json")
    ap.add_argument("--key", default=None, help="override session key (hex), e.g. prefs BluetoothSessionsKey")
    ap.add_argument("--version", default="0100005a", help="4-byte version hex sent in 0x9040")
    ap.add_argument("--ota-write", default=OTA_SVC)
    ap.add_argument("--idle-timeout", type=float, default=120.0)
    ap.add_argument("--pace", type=float, default=0.0, help="sleep between write batches")
    ap.add_argument("--pipeline", type=int, default=4, help="frames written concurrently per batch")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--stop-after-a040", action="store_true")
    ap.add_argument("-v", "--verbose", action="store_true")
    args = ap.parse_args()
    if not os.path.exists(args.image):
        raise SystemExit(f"image not found: {args.image}")
    try:
        asyncio.run(Ota(args).run())
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
