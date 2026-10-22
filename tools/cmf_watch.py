import argparse
import asyncio
import hashlib
import json
import os
import struct
import sys
import time
import zlib
from datetime import datetime

from maputil import write_map

from bleak import BleakClient, BleakScanner
from Crypto.Cipher import AES

CMD_W = "0000fff2-0000-1000-8000-00805f9b34fb"
CMD_N = "0000fff1-0000-1000-8000-00805f9b34fb"
SH_W = "77d4ff01-2fe2-2334-0d35-9ccd078f529c"
SH_N = "77d4ff02-2fe2-2334-0d35-9ccd078f529c"
DAT_W = "02f00000-0000-0000-0000-00000000ffe1"
DAT_N = "02f00000-0000-0000-0000-00000000ffe2"
CCCD = "00002902-0000-1000-8000-00805f9b34fb"

IV = bytes.fromhex("5051525354555657606162636465665A")
FRAME_MAGIC = 0xF5
DEFAULT_NAME_PREFIX = "CMF Watch Pro 2"
STATE_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), ".watch_state.json")


def load_state():
    if os.path.exists(STATE_PATH):
        try:
            return json.load(open(STATE_PATH))
        except Exception:
            return {}
    return {}


def save_state(d):
    try:
        json.dump(d, open(STATE_PATH, "w"), indent=2)
    except OSError:
        pass


def crc32_le(data):
    return struct.pack("<I", zlib.crc32(data) & 0xFFFFFFFF)


def local_utc_offset_ms():
    off = datetime.now().astimezone().utcoffset()
    return int(off.total_seconds() * 1000) if off else 0


def aes_enc(key, data):
    pad = 16 - (len(data) % 16)
    data = data + bytes([pad]) * pad
    return AES.new(key, AES.MODE_CBC, IV).encrypt(data)


def aes_dec(key, data):
    out = AES.new(key, AES.MODE_CBC, IV).decrypt(data)
    if out:
        pad = out[-1]
        if 1 <= pad <= 16:
            out = out[:-pad]
    return out


SESSION_NONCE_LEN = int(os.environ.get("SESSION_NONCE_LEN", "16"))
SESSION_KEY_LEN = int(os.environ.get("SESSION_KEY_LEN", "16"))


def derive_session(nonce, k1_bytes, nonce_len=None, key_len=None):
    nonce_len = SESSION_NONCE_LEN if nonce_len is None else nonce_len
    key_len = SESSION_KEY_LEN if key_len is None else key_len
    temp = bytes(nonce)[:nonce_len] + bytes(k1_bytes)
    return hashlib.sha256(temp).digest()[:key_len]


WF_MTU = 180
WF_CAPACITY = 6
WF_PLAINTEXT = {0x9064}


def dial_id(seed):
    """Watchface id derived from a name/seed (port of fmc's dialId/fnv1a)."""
    h = 0x811C9DC5
    units = seed.encode("utf-16-le", "surrogatepass")
    for i in range(0, len(units), 2):
        h ^= units[i] | (units[i + 1] << 8)
        h = (h * 0x01000193) & 0xFFFFFFFF
    return 10000 + h % (0x7FFFFFFF - 10000)


def build_frame(cmd1, cmd2, payload=b"", chunk_index=1, chunk_count=1, key=None, plaintext_no_crc=False):
    body = payload if plaintext_no_crc else payload + crc32_le(payload)
    if key is not None:
        body = aes_enc(key, body)
    header = bytes([FRAME_MAGIC]) + struct.pack(">HHHHH", len(body), cmd1, chunk_count, chunk_index, cmd2)
    return header + body


class FrameParser:
    def __init__(self):
        self.buf = bytearray()

    def feed(self, data):
        self.buf.extend(data)
        frames = []
        while len(self.buf) >= 11 and self.buf[0] == FRAME_MAGIC:
            chunk_len, cmd1, chunk_count, chunk_index, cmd2 = struct.unpack(">HHHHH", self.buf[1:11])
            encrypted = chunk_len > 0 and chunk_len % 16 == 0
            body_len = chunk_len
            total = 11 + body_len
            if len(self.buf) < total:
                if not encrypted and chunk_len >= 4 and len(self.buf) >= total - 4:
                    body_len = chunk_len - 4
                    total = 11 + body_len
                else:
                    break
            body = bytes(self.buf[11:total])
            del self.buf[:total]
            frames.append((cmd1, cmd2, chunk_count, chunk_index, body, encrypted))
        return frames


def to_int(text):
    return int(text, 16) if text.startswith("0x") else int(text)


class Watch:
    def __init__(self, name_prefix=DEFAULT_NAME_PREFIX, address=None, verbose=True):
        self.name_prefix = name_prefix
        self.address = address
        self.verbose = verbose
        self.client = None
        self.key = None
        self.parser = FrameParser()
        self.dparser = FrameParser()
        self.rx = asyncio.Queue()
        self.data = asyncio.Queue()
        self.shell = asyncio.Queue()
        self.authkey = None
        self.secret = None
        self.seen = []

    def log(self, *a):
        if self.verbose:
            print(*a, flush=True)

    async def find(self, timeout=10.0):
        self.log(f"scanning {timeout}s for '{self.name_prefix}' ...")
        devices = await BleakScanner.discover(timeout=timeout)
        for d in devices:
            nm = d.name or ""
            if self.address and d.address.upper() == self.address.upper():
                return d
            if not self.address and self.name_prefix.lower() in nm.lower():
                return d
        if self.verbose:
            for d in sorted(devices, key=lambda x: (x.name or "")):
                self.log(f"  {d.address}  {d.name}")
        return None

    async def connect(self, timeout=10.0):
        dev = await self.find(timeout)
        if dev is None:
            raise SystemExit("watch not found; run `scan` and pass --address")
        self.log(f"connecting {dev.address} {dev.name} ...")
        self.client = BleakClient(dev, timeout=30.0)
        await self.client.connect()
        self.log(f"connected={self.client.is_connected}")
        await self.attach()
        return dev

    async def attach(self):
        notify = getattr(self.client, "start_notify", None) or self.client.start_notification

        def on_cmd(_, d):
            b = bytes(d)
            if self.verbose:
                print(f"  <cmd raw> {b.hex()}", flush=True)
            self.rx.put_nowait(b)

        def on_sh(_, d):
            b = bytes(d)
            if self.verbose:
                print(f"  <shell raw> {b!r}", flush=True)
            self.shell.put_nowait(b)

        def on_data(_, d):
            b = bytes(d)
            if self.verbose:
                print(f"  <data raw> {b.hex()}", flush=True)
            self.data.put_nowait(b)

        await notify(CMD_N, on_cmd)
        await notify(SH_N, on_sh)
        try:
            await notify(DAT_N, on_data)
        except Exception as e:
            self.log(f"  (data channel {DAT_N} subscribe failed: {e})")

    async def write_data(self, cmd1, cmd2, payload=b"", chunk_index=1, chunk_count=1):
        frame = build_frame(cmd1, cmd2, payload, chunk_index=chunk_index, chunk_count=chunk_count, key=None)
        await self.client.write_gatt_char(DAT_W, frame, response=True)
        if self.verbose:
            print(f"  >data cmd1={cmd1:#06x} cmd2={cmd2:#06x} len={len(payload)}", flush=True)
        return frame

    async def disconnect(self):
        if self.client and self.client.is_connected:
            await self.client.disconnect()

    async def at(self, cmd, timeout=6.0, wait_ok=True):
        while not self.shell.empty():
            self.shell.get_nowait()
        data = (cmd + "\r\n").encode()
        await self.client.write_gatt_char(SH_W, data, response=True)
        chunks = []
        deadline = asyncio.get_event_loop().time() + timeout
        while True:
            remain = deadline - asyncio.get_event_loop().time()
            if remain <= 0:
                break
            try:
                piece = await asyncio.wait_for(self.shell.get(), timeout=remain)
            except asyncio.TimeoutError:
                break
            chunks.append(piece)
            joined = b"".join(chunks)
            if (not wait_ok) or b"OK" in joined or b"ERROR" in joined:
                break
        return b"".join(chunks)

    async def send(self, cmd1, cmd2, payload=b"", key_marker="auto", timeout=6.0, expect=None):
        key = self.key if key_marker == "auto" else (None if key_marker is None else self.key)
        frame = build_frame(cmd1, cmd2, payload, key=key)
        while not self.rx.empty():
            self.rx.get_nowait()
        await self.client.write_gatt_char(CMD_W, frame, response=True)
        deadline = asyncio.get_event_loop().time() + timeout
        raw = bytearray()
        while True:
            remain = deadline - asyncio.get_event_loop().time()
            if remain <= 0:
                return []
            try:
                piece = await asyncio.wait_for(self.rx.get(), timeout=remain)
            except asyncio.TimeoutError:
                return []
            raw.extend(piece)
            frames = self.parser.feed(bytes(raw))
            raw = bytearray(self.parser.buf)
            self.parser.buf = bytearray()
            out = []
            for cmd1_, cmd2_, cc, ci, body, enc in frames:
                out.append((cmd1_, cmd2_, self._decode(cmd1_, cmd2_, body, enc)))
            if out:
                self.seen.extend(out)
                if expect is None or any(c2 == expect for _, c2, _ in out):
                    return out

    def _decode(self, cmd1, cmd2, body, enc):
        if enc and self.key is not None and body:
            try:
                dec = aes_dec(self.key, body)
                if len(dec) >= 4 and crc32_le(dec[:-4]) == dec[-4:]:
                    body = dec[:-4]
                else:
                    body = dec
            except Exception:
                pass
        if self.verbose:
            print(f"  <frame> cmd1={cmd1:#06x} cmd2={cmd2:#06x} enc={enc} len={len(body)} {body.hex()}", flush=True)
        return body

    async def collect(self, seconds):
        out = []
        end = asyncio.get_event_loop().time() + seconds
        while True:
            remain = end - asyncio.get_event_loop().time()
            if remain <= 0:
                break
            try:
                piece = await asyncio.wait_for(self.rx.get(), timeout=remain)
            except asyncio.TimeoutError:
                break
            for cmd1, cmd2, cc, ci, body, enc in self.parser.feed(piece):
                out.append((cmd1, cmd2, self._decode(cmd1, cmd2, body, enc)))
        return out

    async def collect_all(self, seconds):
        out = []
        parser = FrameParser()
        end = asyncio.get_event_loop().time() + seconds
        while True:
            remain = end - asyncio.get_event_loop().time()
            if remain <= 0:
                break
            tasks = [asyncio.ensure_future(self.rx.get()),
                     asyncio.ensure_future(self.data.get())]
            done, not_done = await asyncio.wait(tasks, timeout=remain,
                                                return_when=asyncio.FIRST_COMPLETED)
            for t in not_done:
                t.cancel()
                try:
                    await t
                except BaseException:
                    pass
            for t in done:
                try:
                    piece = t.result()
                except BaseException:
                    continue
                for cmd1, cmd2, cc, ci, body, enc in parser.feed(piece):
                    out.append((cmd1, cmd2, self._decode(cmd1, cmd2, body, enc)))
        return out

    async def reconnect(self, authkey):
        self.authkey = authkey
        self.key = authkey
        await self.send(0xFFFF, 0x8049, b"\xa5" + b"CMF Watch Pro 2", expect=0x0049)
        nonce = None
        for _ in range(3):
            r = await self.send(0xFFFF, 0x804B, b"\xa5", expect=0x004C, timeout=4.0)
            for _, c2, b in r:
                if c2 == 0x004C:
                    nonce = b
            if nonce:
                break
        if not nonce:
            raise SystemExit("reconnect: no nonce")
        self.nonce = nonce
        self.key = derive_session(nonce, self.authkey)
        self.log(f"nonce={nonce.hex()} sessionKey={self.key.hex()}")
        await self.send(0xFFFF, 0x804D, b"\xa5", expect=0x0004, timeout=4.0)
        await self.send(0xFFFF, 0x8004, struct.pack(">ii", int(time.time()), local_utc_offset_ms()), expect=None, timeout=3.0)

    async def handshake(self):
        secret_hex = await self.at("AT GETSECRET")
        self.log("AT GETSECRET ->", secret_hex)
        if b"OK" not in secret_hex:
            raise SystemExit("could not read secret")
        text = secret_hex.decode("latin1")
        hexpart = text.split("GETSECRET:")[-1].split(",")[0].strip()
        secret = bytes.fromhex(hexpart)
        self.secret = secret
        rnd1 = os.urandom(16)
        signed1 = hashlib.sha256(rnd1 + secret).digest()
        self.log(f"secret={secret.hex()} rnd1={rnd1.hex()}")
        resp = None
        for attempt in range(1, 4):
            self.log(f"AUTH_PAIR_REQUEST attempt {attempt} ...")
            got = await self.send(0xFFFF, 0x8047, rnd1 + signed1, key_marker=None, expect=None, timeout=5.0)
            got = [f for f in got if f[1] == 0x0048]
            if got:
                resp = got[0]
                break
            await asyncio.sleep(1.0)
        if not resp:
            raise SystemExit("no AUTH_PAIR_REPLY")
        body = resp[2]
        rnd2, signed2 = body[:16], body[16:48]
        if signed2 != hashlib.sha256(rnd2 + secret).digest():
            raise SystemExit("watch signature mismatch")
        self.authkey = hashlib.sha256(rnd1 + rnd2 + secret).digest()[:16]
        self.key = self.authkey
        self.log(f"rnd2={rnd2.hex()} authkey={self.authkey.hex()}")

        mac = await self.send(0xFFFF, 0x8049, b"\xa5" + b"CMF Watch Pro 2", expect=0x0049)
        self.log("AUTH_WATCH_MAC:", [b.hex() for _, _, b in mac])

        nonce = None
        for _ in range(3):
            r = await self.send(0xFFFF, 0x804B, b"\xa5", expect=0x004C, timeout=4.0)
            for _, c2, b in r:
                if c2 == 0x004C:
                    nonce = b
            if nonce:
                break
        if not nonce:
            raise SystemExit("no AUTH_NONCE_REPLY")
        self.key = derive_session(nonce, self.authkey)
        self.log(f"nonce={nonce.hex()} sessionKey={self.key.hex()}")

        await self.send(0xFFFF, 0x804D, b"\xa5", expect=0x0004, timeout=4.0)
        await self.send(0xFFFF, 0x8004, struct.pack(">ii", int(time.time()), local_utc_offset_ms()), expect=None, timeout=3.0)
        return True

    @staticmethod
    def wf_validate(data):
        if len(data) < 64:
            raise ValueError("file too small for a watchface")
        if not (data[4] == 1 and data[5] == 0 and data[6] == 0 and data[7] in (0, 2)):
            raise ValueError("not a watchface file (bad header)")
        name = data[8:0x17].split(b"\x00", 1)[0].decode("utf-8", "replace")
        tname = data[-28:-13].split(b"\x00", 1)[0].decode("utf-8", "replace")
        if not name or name != tname:
            raise ValueError(f"name mismatch: header={name!r} trailer={tname!r}")
        return name

    def _cmd_frames(self, cmd1, cmd2, payload, plaintext=False):
        bodies = []
        if plaintext:
            step = WF_MTU - 20
            chunks = [payload[i:i + step] for i in range(0, len(payload), step)] or [b""]
            bodies = [c + crc32_le(c) for c in chunks]
        elif not payload:
            bodies = [b""]
        else:
            step = (WF_MTU - 11) // 16 * 16 - 5
            for i in range(0, len(payload), step):
                c = payload[i:i + step]
                bodies.append(aes_enc(self.key, c + crc32_le(c)))
        n = len(bodies)
        return [bytes([FRAME_MAGIC]) + struct.pack(">HHHHH", len(b), cmd1, n, i + 1, cmd2) + b
                for i, b in enumerate(bodies)]

    async def _write_cmd(self, cmd1, cmd2, payload, plaintext=False, data_channel=False, pace=0.005):
        frames = self._cmd_frames(cmd1, cmd2, payload, plaintext=plaintext)
        ch = DAT_W if data_channel else CMD_W
        for f in frames:
            if data_channel:
                await self.client.write_gatt_char(ch, f, response=False)
            else:
                await self.client.write_gatt_char(ch, f, response=True)
            if pace:
                await asyncio.sleep(pace)
        return len(frames)

    async def wf_upload(self, data, old_id=0, seed=None, timeout=30.0, progress=None,
                        new_id=None):
        name = self.wf_validate(data)
        if new_id is None:
            new_id = dial_id(seed or name)
        if not 0 <= new_id < 0x80000000:
            raise ValueError(f"dial id out of range: {new_id}")
        while not self.rx.empty():
            self.rx.get_nowait()
        while not self.data.empty():
            self.data.get_nowait()
        parser = FrameParser()
        pending = []

        def pump(chunk=b""):
            for cmd1_, cmd2_, cc, ci, body, enc in parser.feed(chunk):
                pending.append((cmd2_, self._decode(cmd1_, cmd2_, body, enc)))

        async def get_piece(remain):
            tasks = [asyncio.ensure_future(self.rx.get()),
                     asyncio.ensure_future(self.data.get())]
            done, not_done = await asyncio.wait(tasks, timeout=remain,
                                                return_when=asyncio.FIRST_COMPLETED)
            for t in not_done:
                t.cancel()
                try:
                    await t
                except BaseException:
                    pass
            out = []
            for t in done:
                try:
                    out.append(t.result())
                except BaseException:
                    pass
            return out

        async def wait_any(wants, tmo):
            loop = asyncio.get_event_loop()
            deadline = loop.time() + tmo
            while True:
                for i, (c2_, body) in enumerate(pending):
                    if c2_ in wants:
                        del pending[i]
                        return c2_, body
                remain = deadline - loop.time()
                if remain <= 0:
                    raise TimeoutError("watchface: timed out waiting for %s" %
                                       ", ".join(f"{x:#06x}" for x in wants))
                for piece in await get_piece(remain):
                    pump(piece)

        await self._write_cmd(0xFFFF, 0x8052, b"\xa5", data_channel=True, pace=0)
        r = await wait_any((0x0052,), timeout)
        if not r[1] or r[1][0] != 1:
            raise RuntimeError(f"wf init1 rejected: {r[1].hex() if r[1] else 'empty'}")

        init2 = bytes([3]) + struct.pack("<III", old_id & 0xFFFFFFFF, new_id, len(data))
        await self._write_cmd(0xFFFF, 0x9075, init2, data_channel=True, pace=0)
        r = await wait_any((0xA075,), timeout)
        if not r[1] or r[1][0] != 1:
            code = r[1][0] if r[1] else None
            hint = ""
            if code == 0x0A:
                hint = (" (no free slot / unknown old_wf_id)" if new_id
                        else " (watch refused the delete request)")
            raise RuntimeError(f"wf init2 rejected: {code!r}{hint}")

        sent = 0
        while True:
            c2_, body = await wait_any((0xA064, 0xA065), timeout)
            if c2_ == 0xA064:
                if len(body) < 9:
                    raise RuntimeError(f"short chunk request: {body.hex()}")
                off, ln = struct.unpack_from(">II", body, 0)
                pct = body[8]
                if ln == 0 or off + ln > len(data):
                    raise RuntimeError(f"bad chunk request off={off} len={ln} total={len(data)}")
                n = await self._write_cmd(0xFFFF, 0x9064, data[off:off + ln],
                                          plaintext=True, data_channel=True, pace=0.001)
                sent = max(sent, off + ln)
                self.log(f"  wf chunk off={off} len={ln} {pct}% frames={n}")
                if progress:
                    progress(off + ln, len(data), pct)
            else:
                await self._write_cmd(0xFFFF, 0x9065, b"\xa5", data_channel=True, pace=0)
                code = body[0] if body else None
                return code, new_id, name, sent


async def cmd_scan(args):
    devs = await BleakScanner.discover(timeout=args.timeout)
    for d in sorted(devs, key=lambda x: (x.name or "")):
        print(f"{d.address}  rssi={getattr(d,'rssi',None)}  {d.name}")


async def cmd_at(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        for c in args.cmd:
            r = await w.at(c)
            print(f"{c} -> {r!r}")
    finally:
        await w.disconnect()


async def cmd_info(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        for c in ["AT GETVERSION", "AT GETSN", "AT GETNAME", "AT GETPID",
                  "AT GETBATLV", "AT GETNTCTEMP", "AT GETGSENSOR"]:
            r = await w.at(c)
            print(f"{c} -> {r.decode('latin1', 'replace').strip()!r}")
    finally:
        await w.disconnect()


async def cmd_pair(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await w.handshake()
        r = await w.send(0xFFFF, 0x8006, expect=0x0006)
        for _, _, b in r:
            print("FIRMWARE_VERSION_RET:", b.hex(), repr(b.decode("latin1", "replace")))
        r = await w.send(0x005C, 0x0001, expect=None, timeout=4.0)
        for c1, c2, b in r:
            print(f"BATTERY[{c1:#06x},{c2:#06x}]:", b.hex(), repr(b.decode("latin1", "replace")))
        print("authkey =", w.authkey.hex())
        print("sessionKey =", w.key.hex())
        save_state({"address": args.address or "", "secret": (w.secret or b"").hex(), "authkey": w.authkey.hex()})
        print("saved state ->", STATE_PATH)
    finally:
        await w.disconnect()


async def ensure_session(w, args):
    st = load_state()
    ak = st.get("authkey")
    if ak and not getattr(args, "repair", False):
        try:
            print("reconnecting with stored authkey ...")
            await w.reconnect(bytes.fromhex(ak))
            return
        except SystemExit as e:
            print("reconnect failed:", e, "-> full pair")
    await w.handshake()
    save_state({"address": w.address or "", "secret": w.secret.hex(), "authkey": w.authkey.hex()})


def parse_activity(frames):
    recs = []
    for c1, c2, b in frames:
        if c1 == 0x0056 and c2 == 0x0001:
            for off in range(0, len(b) - 31, 32):
                ts, steps, dist, cal = struct.unpack("<IIII", b[off:off + 16])
                recs.append((ts, steps, dist, cal))
    return recs


def parse_samples(frames):
    out = {"hr": [], "resting_hr": [], "spo2": [], "stress": []}
    for c1, c2, b in frames:
        if c2 != 0x0001:
            continue
        if c1 == 0x0053:
            for off in range(0, len(b) - 7, 8):
                ts, v = struct.unpack("<ii", b[off:off + 8])
                out["hr"].append((ts, v))
        elif c1 == 0x00da and len(b) >= 5:
            out["resting_hr"].append((struct.unpack("<i", b[:4])[0], b[4]))
        elif c1 == 0x0055:
            for off in range(0, len(b) - 7, 8):
                ts, v = struct.unpack("<ii", b[off:off + 8])
                out["spo2"].append((ts, v))
        elif c1 == 0x009d:
            for off in range(0, len(b) - 7, 8):
                ts, v = struct.unpack("<ii", b[off:off + 8])
                out["stress"].append((ts, v))
    return out


def parse_gps(frames):
    pts = []
    for c1, c2, b in frames:
        if c1 == 0xFFFF and c2 == 0xA05A and len(b) >= 12:
            ts, lon, lat = struct.unpack("<iii", b[:12])
            pts.append((ts, lon, lat))
    return pts


async def cmd_sync(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        got = await w.send(0xFFFF, 0x8005, expect=0x0005, timeout=6.0)
        print("FETCH_ACK_1:", [(f"{c2:#06x}", b.hex()) for _, c2, b in got])
        await w.send(0xFFFF, 0x9057, expect=0xA057, timeout=6.0)
        frames = await w.collect(args.duration)
        hist = {}
        for c1, c2, _ in frames:
            hist[(c1, c2)] = hist.get((c1, c2), 0) + 1
        print("frame histogram:")
        for (c1, c2), n in sorted(hist.items()):
            print(f"  {c1:#06x},{c2:#06x}: {n}")
        acts = parse_activity(frames)
        print(f"activity records: {len(acts)}")
        for ts, steps, dist, cal in acts[:args.limit]:
            print(f"  {time.strftime('%Y-%m-%d %H:%M', time.localtime(ts))}  steps={steps} dist_m={dist} cal={cal}")
        samples = parse_samples(frames)
        for k, v in samples.items():
            print(f"{k}: {len(v)} samples  {v[-3:]}")
        with open("activity.json", "w") as f:
            json.dump([{"ts": a[0], "steps": a[1], "dist_m": a[2], "cal": a[3]} for a in acts], f)
        with open("sync_frames.json", "w") as f:
            json.dump([{"cmd1": f"{c1:#06x}", "cmd2": f"{c2:#06x}", "hex": b.hex()} for c1, c2, b in frames], f, indent=1)
        print("wrote activity.json, sync_frames.json")
    finally:
        await w.disconnect()


async def cmd_workouts(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        await w.send(0xFFFF, 0x8005, expect=0x0005, timeout=6.0)
        await w.send(0xFFFF, 0x9057, expect=0xA057, timeout=6.0)
        frames = await w.collect(args.duration)
        hist = {}
        for c1, c2, _ in frames:
            hist[(c1, c2)] = hist.get((c1, c2), 0) + 1
        print("frame histogram:")
        for (c1, c2), n in sorted(hist.items()):
            print(f"  {c1:#06x},{c2:#06x}: {n}")
        gps = parse_gps(frames)
        print(f"WORKOUT_GPS points: {len(gps)}")
        for ts, lon, lat in gps[:args.limit]:
            print(f"  {time.strftime('%Y-%m-%d %H:%M', time.localtime(ts))}  lat={lat / 1e7:.6f} lon={lon / 1e7:.6f}")
        with open("gps.json", "w") as f:
            json.dump([{"ts": g[0], "lat": g[2] / 1e7, "lon": g[1] / 1e7} for g in gps], f)
        if gps:
            write_map([(g[2] / 1e7, g[1] / 1e7) for g in gps], "gps_map.html", "CMF Watch Pro 2 - live GPS")
        with open("workout_frames.json", "w") as f:
            json.dump([{"cmd1": f"{c1:#06x}", "cmd2": f"{c2:#06x}", "hex": b.hex()} for c1, c2, b in frames], f, indent=1)
        print("wrote gps.json, workout_frames.json")
    finally:
        await w.disconnect()


async def cmd_specs(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        for c2 in (0x907F, 0x8005):
            await w.send(0xFFFF, c2, expect=None, timeout=3.0)
        frames = await w.collect(args.duration)
        for c1, c2, b in frames:
            print(f"  {c1:#06x},{c2:#06x} len={len(b)} {b.hex()}")
    finally:
        await w.disconnect()


async def cmd_ota_state(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        print("read-only OTA state probe (NO upgrade/INIT commands are sent)")
        r = await w.send(0xFFFF, 0x8006, expect=0x0006, timeout=5.0)
        for c1, c2, b in r:
            print(f"  FIRMWARE_VERSION {c2:#06x}: {b.hex()} {b.decode('latin1', 'replace')!r}")
        for at in ("AT GETVERSION", "AT GETPID", "AT GETBATLV"):
            rr = await w.at(at)
            print(f"  {at} -> {rr.decode('latin1', 'replace').strip()!r}")
        print(f"passively listening {args.duration:.0f}s for watch-initiated OTA-state frames ...")
        frames = await w.collect(args.duration)
        hist = {}
        for c1, c2, _ in frames:
            hist[(c1, c2)] = hist.get((c1, c2), 0) + 1
        for (c1, c2), n in sorted(hist.items()):
            print(f"  {c1:#06x},{c2:#06x}: {n}")
        if not frames:
            print("  (no OTA-state notifications -> watch is not mid-OTA)")
    finally:
        await w.disconnect()


async def cmd_ota_init(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        payload = bytes.fromhex(args.payload_hex)
        print(f"OTA INIT dry-run: cmd1=0xFFFF cmd2=0x9052 payload={payload.hex()} "
              f"(handshake only, no data chunks sent)")
        r = await w.send(0xFFFF, 0x9052, payload, expect=0xA052, timeout=args.duration)
        if not r:
            print("no 0xA052 reply; collecting any late frames ...")
            r = await w.collect(3.0)
        for c1, c2, b in r:
            print(f"  {c1:#06x},{c2:#06x} len={len(b)} {b.hex()} "
                  f"{b.decode('latin1', 'replace')!r}")
        print("done - disconnecting (no OTA data was transferred)")
    finally:
        await w.disconnect()


async def _pull(w, timeout):
    rxt = asyncio.ensure_future(w.rx.get())
    dat = asyncio.ensure_future(w.data.get())
    done, pending = await asyncio.wait({rxt, dat}, timeout=timeout, return_when=asyncio.FIRST_COMPLETED)
    for t in pending:
        t.cancel()
    frames = []
    if rxt in done and not rxt.cancelled():
        try:
            for c1, c2, cc, ci, body, enc in w.parser.feed(rxt.result()):
                frames.append(("cmd", c1, c2, cc, ci, body, enc))
        except Exception:
            pass
    if dat in done and not dat.cancelled():
        try:
            for c1, c2, cc, ci, body, enc in w.dparser.feed(dat.result()):
                frames.append(("data", c1, c2, cc, ci, body, enc))
        except Exception:
            pass
    return frames


def _u32be(b, o):
    return struct.unpack(">I", b[o:o + 4])[0]


def _u16be(b, o):
    return struct.unpack(">H", b[o:o + 2])[0]


async def _write_ota_chunk(w, payload, chunk_index=1, chunk_count=1, channel=None, plaintext=True):
    frame = build_frame(0xFFFF, 0x9042, payload, chunk_index=chunk_index,
                        chunk_count=chunk_count, key=None if plaintext else w.key)
    char = channel or CMD_W
    await w.client.write_gatt_char(char, frame, response=True)
    return frame


async def cmd_ota_run(args):
    img = open(args.image, "rb").read() if args.image else b""
    init1 = bytes.fromhex(args.init1_hex)
    ver = bytes.fromhex(args.ver_hex)
    chunk = max(20, min(args.chunk or 200, 227))
    channel = DAT_W if args.resp_data else CMD_W
    ch_name = "DATA(ffe1)" if args.resp_data else "CMD(fff2)"
    print(f"OTA RUN (app-faithful): INIT1 0x9052 payload={init1.hex()} -> A052 | "
          f"INIT2 0x9040 ver={ver.hex()} -> A040 | serve={not args.dry_run} "
          f"chunk={chunk} resp={ch_name} image={len(img)}B")
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    served = 0
    served_bytes = 0
    try:
        await ensure_session(w, args)
        ak = load_state().get("authkey")

        print("== INIT1 (OTA_Begin, 0x9052) ==")
        r = await w.send(0xFFFF, 0x9052, init1, expect=0xA052, timeout=8.0)
        a052 = None
        for c1, c2, b in r:
            print(f"  INIT1 reply {c2:#06x} len={len(b)} {b.hex()}")
            if c2 == 0xA052:
                a052 = b
        if a052 is None:
            print("  no A052 -> aborting")
            return
        if len(a052) >= 4:
            print(f"  A052 decode: first_u8={a052[0]} be16@0={_u16be(a052,0)} be32@0={_u32be(a052,0)}")
        elif len(a052) >= 2:
            print(f"  A052 decode: first_u8={a052[0]} be16@0={_u16be(a052,0)}")
        elif len(a052) == 1:
            print(f"  A052 decode: first_u8={a052[0]}")

        a040 = None
        if args.no_init2:
            print("== INIT2 skipped (--no-init2) ==")
        else:
            print("== INIT2 (OtaStart, 0x9040) ==")
            r = await w.send(0xFFFF, 0x9040, ver, expect=0xA040, timeout=8.0)
            for c1, c2, b in r:
                print(f"  INIT2 reply {c2:#06x} len={len(b)} {b.hex()}")
                if c2 == 0xA040:
                    a040 = b
            if a040 is None:
                print("  note: NO A040 (watch may have gone straight to reboot/OTA mode)")
            else:
                print(f"  A040 decode: first_u8={a040[0]} be32@0={_u32be(a040,0)}")

        print("== serve loop ==")
        t0 = time.time()
        end = t0 + args.max_seconds
        last_rx = t0
        reconnected = False
        while time.time() < end:
            frames = await _pull(w, 1.0)
            for src, c1, c2, cc, ci, body, enc in frames:
                dec = w._decode(c1, c2, body, enc)
                last_rx = time.time()
                print(f"  <{src} {c1:#06x},{c2:#06x}> cc={cc} ci={ci} enc={enc} "
                      f"len={len(dec)} {dec.hex()}")
                if c2 == 0xA042 and len(dec) >= 8:
                    off = _u32be(dec, 0)
                    ln = _u32be(dec, 4)
                    prog = _u32be(dec, 8) if len(dec) >= 12 else None
                    print(f"    A042 DATA REQUEST off={off} len={ln} progress={prog} "
                          f"tail={dec[12:].hex()}")
                    if not args.dry_run and img:
                        end_off = min(off + ln, len(img))
                        n_chunks = (end_off - off + chunk - 1) // chunk
                        o = off
                        i = 0
                        while o < end_off and w.client.is_connected:
                            piece = img[o:o + chunk]
                            await _write_ota_chunk(w, piece, chunk_index=i + 1,
                                                   chunk_count=n_chunks, channel=channel)
                            o += len(piece)
                            i += 1
                            served += 1
                            served_bytes += len(piece)
                            if served % 64 == 0:
                                pct = 100.0 * o / max(1, len(img))
                                print(f"    ... {served} frames {served_bytes}B "
                                      f"({pct:.1f}%) link={w.client.is_connected}")
                elif c2 == 0xA041:
                    print("    WATCH FINISH (A041)")
                    if not args.dry_run:
                        rr = await w.send(0xFFFF, 0x9041, b"\xa5", expect=None, timeout=8.0)
                        for _, c2b, bb in rr:
                            print(f"    FINISH reply {c2b:#06x} {bb.hex()}")
                    end = time.time()
            if not w.client or not w.client.is_connected:
                print(f"  link DOWN at t+{time.time()-t0:.1f}s")
                if args.reconnect and not reconnected:
                    if await _reconnect(w, ak, args.scan_window):
                        print(f"  reconnected t+{time.time()-t0:.1f}s; awaiting A042 ...")
                        reconnected = True
                        end = max(end, time.time() + args.observe)
                        continue
                break
            if served == 0 and time.time() - last_rx > args.idle:
                print(f"  idle {args.idle}s, no A042 -> stop")
                break
        print(f"summary: served={served} frames bytes={served_bytes} "
              f"link={bool(w.client and w.client.is_connected)}")
        try:
            rr = await w.at("AT GETVERSION", timeout=6.0)
            print("post AT GETVERSION ->", rr.decode("latin1", "replace").strip())
        except Exception as e:
            print("version check failed:", e)
    finally:
        await w.disconnect()


async def cmd_ota_flash(args):
    ver = bytes.fromhex(args.ver_hex)
    img = open(args.image, "rb").read()
    print(f"image={args.image} size={len(img)} ({len(img):#x}) "
          f"sha256={hashlib.sha256(img).hexdigest()}")
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        print(f"OTA FLASH: INIT1 0x9052(A5) | INIT2 0x9040 ver={ver.hex()} "
              f"dry_run={args.dry_run} idle={args.idle}s")

        r = await w.send(0xFFFF, 0x9052, b"\xa5", expect=0xA052, timeout=6.0)
        a052 = None
        for c1, c2, b in r:
            print(f"  INIT1 reply {c2:#06x} len={len(b)} {b.hex()}")
            if c2 == 0xA052:
                a052 = b
        if a052 is None:
            print("  no A052 -> aborting")
            return

        a040 = None
        if args.no_init2:
            print("  (skipping INIT2 per --no-init2)")
        else:
            r = await w.send(0xFFFF, 0x9040, ver, expect=0xA040, timeout=6.0)
            for c1, c2, b in r:
                print(f"  INIT2 reply {c2:#06x} len={len(b)} {b.hex()}")
                if c2 == 0xA040:
                    a040 = b
            if a040 is None:
                print("  WARNING: no A040 reply to INIT2")

        if args.push:
            try:
                mtu = w.client.mtu_size
            except Exception:
                mtu = 0
            chunk = args.chunk or (mtu - 20 if mtu > 40 else 200)
            chunk = max(20, chunk)
            total = min(args.push_bytes, len(img)) if args.push_bytes else len(img)
            print(f"PUSH mode: mtu={mtu} chunk={chunk} total={total} ({total:#x})")
            off = 0
            n = 0
            while off < total:
                if not (w.client and w.client.is_connected):
                    print(f"  link lost at {off}/{total}"); break
                slice_ = img[off:off + chunk]
                try:
                    await w.write_data(0xFFFF, 0x9042, slice_)
                except Exception as e:
                    print(f"  write failed at {off}: {e!r}"); break
                off += len(slice_)
                n += 1
                for src, c1, c2, cc, ci, body, enc in await _pull(w, 0.0):
                    dec = w._decode(c1, c2, body, enc)
                    print(f"    <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
                if n % 50 == 0:
                    print(f"  pushed {off}/{total} ({100.0 * off / total:.1f}%) link={w.client.is_connected}")
            print(f"push done: {n} frames, {off} bytes; link={bool(w.client and w.client.is_connected)}")
            print(f"observing {args.observe:.0f}s after push ...")
            end = asyncio.get_event_loop().time() + args.observe
            while w.client and w.client.is_connected and asyncio.get_event_loop().time() < end:
                for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                    dec = w._decode(c1, c2, body, enc)
                    print(f"    <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
            try:
                rr = await w.at("AT GETVERSION", timeout=6.0)
                print("post AT GETVERSION ->", rr.decode("latin1", "replace").strip())
            except Exception as e:
                print("version check failed:", e)
            return

        start = asyncio.get_event_loop().time()
        last = start
        n_req = n_sent = sent_bytes = max_end = 0
        done = False
        while not done:
            now = asyncio.get_event_loop().time()
            if now - start > args.max_seconds:
                print("  max-seconds reached"); break
            if n_req and now - last > args.idle:
                print(f"  idle {args.idle}s after {n_req} requests -> stop"); break
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"  <{src} {c1:#06x},{c2:#06x}> cc={cc} ci={ci} enc={enc} "
                      f"len={len(dec)} {dec.hex()}")
                if c2 == 0xA042:
                    n_req += 1
                    last = asyncio.get_event_loop().time()
                    off = _u32be(dec, 0) if len(dec) >= 8 else None
                    ln = _u32be(dec, 4) if len(dec) >= 8 else None
                    print(f"    FW CHUNK REQUEST #{n_req} offset={off} length={ln} "
                          f"tail={dec[8:].hex()}")
                    if args.dry_run or off is None or ln is None:
                        continue
                    slice_ = img[off:off + ln] if off < len(img) else b""
                    await w.write_data(0xFFFF, 0x9042, slice_)
                    n_sent += 1
                    sent_bytes += len(slice_)
                    max_end = max(max_end, off + len(slice_))
                    if n_sent % 50 == 0:
                        print(f"    ... {n_sent} chunks, {sent_bytes} bytes, "
                              f"{100.0 * max_end / len(img):.1f}%")
                elif c2 == 0xA041:
                    print("    WATCH FINISH (A041) received")
                    if not args.dry_run:
                        rr = await w.send(0xFFFF, 0x9041, b"\xa5", expect=None, timeout=8.0)
                        for _, c2b, bb in rr:
                            print(f"    FINISH reply {c2b:#06x} {bb.hex()}")
                    done = True
        print(f"summary: requests={n_req} sent={n_sent} bytes={sent_bytes} "
              f"max_end={max_end}/{len(img)} ({100.0 * max_end / max(1, len(img)):.2f}%)")
        try:
            rr = await w.at("AT GETVERSION", timeout=6.0)
            print("post AT GETVERSION ->", rr.decode("latin1", "replace").strip())
        except Exception as e:
            print("version check failed:", e)
    finally:
        await w.disconnect()


async def cmd_ota_push(args):
    ver = bytes.fromhex(args.ver_hex)
    img = open(args.image, "rb").read()
    print(f"image size={len(img)} sha256={hashlib.sha256(img).hexdigest()[:16]}")
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        ak = load_state().get("authkey")
        r = await w.send(0xFFFF, 0x9052, b"\xa5", expect=0xA052, timeout=6.0)
        for c1, c2, b in r:
            print(f"  INIT1 {c2:#06x} {b.hex()}")
        t0 = time.time()
        print("sending INIT2 (fire-and-forget) ...")
        try:
            await w.send(0xFFFF, 0x9040, ver, expect=None, timeout=0.3)
        except Exception as e:
            print("  INIT2 err:", e)
        while w.client and w.client.is_connected and time.time() - t0 < 20:
            await asyncio.sleep(0.3)
        print(f"link after INIT2 t+{time.time()-t0:.1f}s connected={bool(w.client and w.client.is_connected)}")
        print("waiting for re-advertise ...")
        hit = None
        st = time.time()
        while time.time() - st < args.scan_window and hit is None:
            for d in await BleakScanner.discover(timeout=3.0):
                if w.address and d.address.upper() == w.address.upper():
                    hit = d
                    print(f"  re-advertised t+{time.time()-st:.0f}s {d.address} {d.name!r}")
                    break
        if not hit:
            print("  watch did not re-advertise; aborting"); return
        try:
            await w.disconnect()
        except Exception:
            pass
        w.client = BleakClient(hit, timeout=30.0)
        await w.client.connect()
        await w.attach()
        print(f"reconnected={w.client.is_connected} t+{time.time()-t0:.1f}s")
        if args.reauth and ak:
            try:
                await w.reconnect(bytes.fromhex(ak))
                print("re-auth OK")
            except BaseException as e:
                print("re-auth failed:", e)
        try:
            mtu = w.client.mtu_size
        except Exception:
            mtu = 0
        chunk = args.chunk or (mtu - 20 if mtu and mtu > 40 else 200)
        chunk = max(20, min(chunk, 400))
        total = min(args.push_bytes, len(img)) if args.push_bytes else len(img)
        print(f"PUSH: mtu={mtu} chunk={chunk} total={total} reauth={args.reauth}")
        off = 0
        n = 0
        while off < total and w.client and w.client.is_connected:
            piece = img[off:off + chunk]
            await w.write_data(0xFFFF, 0x9042, piece)
            off += len(piece)
            n += 1
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 0.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"    <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
            if n % 50 == 0:
                print(f"  pushed {off}/{total} ({100.0*off/total:.1f}%) link={w.client.is_connected}")
        print(f"push done {n} frames {off} bytes link={bool(w.client and w.client.is_connected)}")
        end = time.time() + args.observe
        while w.client and w.client.is_connected and time.time() < end:
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"    <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
        try:
            rr = await w.at("AT GETVERSION", timeout=6.0)
            print("post AT GETVERSION ->", rr.decode("latin1", "replace").strip())
        except Exception as e:
            print("version check failed:", e)
    finally:
        await w.disconnect()


async def _reconnect(w, ak, scan_window):
    hit = None
    st = time.time()
    while time.time() - st < scan_window and hit is None:
        for d in await BleakScanner.discover(timeout=2.0):
            if w.address and d.address.upper() == w.address.upper():
                hit = d
                break
    if not hit:
        return False
    try:
        await w.disconnect()
    except Exception:
        pass
    w.client = BleakClient(hit, timeout=30.0)
    await w.client.connect()
    await w.attach()
    if ak:
        try:
            await w.reconnect(bytes.fromhex(ak))
        except BaseException as e:
            print("  re-auth failed:", e)
    return True


async def cmd_ota_resume(args):
    ver = bytes.fromhex(args.ver_hex)
    img = open(args.image, "rb").read()
    print(f"OTA RESUME: INIT1(cmd) -> push {args.push_bytes}B on data -> reconnect -> serve A042")
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        ak = load_state().get("authkey")
        r = await w.send(0xFFFF, 0x9052, b"\xa5", expect=0xA052, timeout=6.0)
        for c1, c2, b in r:
            print(f"  INIT1 {c2:#06x} {b.hex()}")
        n = 0
        off = 0
        while off < min(args.push_bytes, len(img)) and w.client and w.client.is_connected:
            piece = img[off:off + 227]
            try:
                await w.write_data(0xFFFF, 0x9042, piece)
            except Exception as e:
                print("  write err:", e); break
            off += len(piece); n += 1
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 0.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"    <{src} {c1:#06x},{c2:#06x}> {dec.hex()}")
        print(f"pushed {n} frames ({off}B); link={bool(w.client and w.client.is_connected)}")
        if not (w.client and w.client.is_connected):
            print("reconnecting ...")
            if not await _reconnect(w, ak, args.scan_window):
                print("  watch not found; abort"); return
            print(f"  reconnected={w.client.is_connected}")
        print(f"observing/serving {args.observe:.0f}s ...")
        end = time.time() + args.observe
        served = 0
        while w.client and w.client.is_connected and time.time() < end:
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"  <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
                if c2 == 0xA042 and len(dec) >= 8:
                    o = _u32be(dec, 0); ln = _u32be(dec, 4)
                    print(f"    A042 offset={o} len={ln} tail={dec[8:].hex()}")
                    if not args.dry_run:
                        await w.write_data(0xFFFF, 0x9042, img[o:o + ln])
                        served += 1
                if c2 == 0xA041:
                    print("    FINISH A041")
                    if not args.dry_run:
                        await w.write_data(0xFFFF, 0x9041, b"\xa5")
        print(f"done; served {served}; link={bool(w.client and w.client.is_connected)}")
        try:
            rr = await w.at("AT GETVERSION", timeout=6.0)
            print("post AT GETVERSION ->", rr.decode("latin1", "replace").strip())
        except Exception as e:
            print("version check failed:", e)
    finally:
        await w.disconnect()


async def cmd_ota_dtry(args):
    ver = bytes.fromhex(args.ver_hex)
    img = open(args.image, "rb").read()
    print(f"OTA DTRY: INIT1/INIT2 on the DATA channel; image {len(img)} dry={args.dry_run}")
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        print("INIT1 (0x9052) -> DATA channel")
        await w.write_data(0xFFFF, 0x9052, b"\xa5")
        end = time.time() + 3
        while time.time() < end:
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"  <{src} {c1:#06x},{c2:#06x}> enc={enc} {dec.hex()}")
        print("INIT2 (0x9040) -> DATA channel")
        await w.write_data(0xFFFF, 0x9040, ver)
        end = time.time() + args.observe
        n = 0
        while w.client and w.client.is_connected and time.time() < end:
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"  <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
                if c2 == 0xA042 and len(dec) >= 8:
                    off = _u32be(dec, 0); ln = _u32be(dec, 4)
                    print(f"    A042 offset={off} len={ln} tail={dec[8:].hex()}")
                    if not args.dry_run:
                        await w.write_data(0xFFFF, 0x9042, img[off:off + ln])
                        n += 1
                        if n % 50 == 0:
                            print(f"    served {n} chunks, off={off}")
                if c2 == 0xA041:
                    print("    FINISH A041 received")
                    if not args.dry_run:
                        await w.write_data(0xFFFF, 0x9041, b"\xa5")
        print(f"done; served {n} chunks; link={bool(w.client and w.client.is_connected)}")
        try:
            rr = await w.at("AT GETVERSION", timeout=6.0)
            print("post AT GETVERSION ->", rr.decode("latin1", "replace").strip())
        except Exception as e:
            print("version check failed:", e)
    finally:
        await w.disconnect()


async def cmd_ota_arm(args):
    ver = bytes.fromhex(args.ver_hex)
    print(f"OTA ARM: INIT1 vs INIT2-then-reconnect-then-INIT1; dry={args.dry_run}")
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        ak = load_state().get("authkey")
        r = await w.send(0xFFFF, 0x9052, b"\xa5", expect=0xA052, timeout=6.0)
        for c1, c2, b in r:
            print(f"  INIT1a {c2:#06x} {b.hex()}")
        print("sending INIT2 (fire-and-forget) ...")
        try:
            await w.send(0xFFFF, 0x9040, ver, expect=None, timeout=0.3)
        except Exception as e:
            print("  INIT2 err:", e)
        t0 = time.time()
        while w.client and w.client.is_connected and time.time() - t0 < 20:
            await asyncio.sleep(0.3)
        print("waiting for re-advertise ...")
        hit = None
        st = time.time()
        while time.time() - st < args.scan_window and hit is None:
            for d in await BleakScanner.discover(timeout=3.0):
                if w.address and d.address.upper() == w.address.upper():
                    hit = d
                    print(f"  re-advertised t+{time.time()-st:.0f}s {d.address} {d.name!r}")
                    break
        if not hit:
            print("  no re-advertise; aborting"); return
        try:
            await w.disconnect()
        except Exception:
            pass
        w.client = BleakClient(hit, timeout=30.0)
        await w.client.connect()
        await w.attach()
        print(f"reconnected={w.client.is_connected} t+{time.time()-t0:.1f}s")
        if ak:
            try:
                await w.reconnect(bytes.fromhex(ak))
                print("re-auth OK")
            except BaseException as e:
                print("re-auth failed:", e)
        r = await w.send(0xFFFF, 0x9052, b"\xa5", expect=0xA052, timeout=6.0)
        for c1, c2, b in r:
            print(f"  INIT1b {c2:#06x} {b.hex()}")
        print(f"observing {args.observe:.0f}s for A042 requests ...")
        end = time.time() + args.observe
        while w.client and w.client.is_connected and time.time() < end:
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                dec = w._decode(c1, c2, body, enc)
                print(f"  <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
                if c2 == 0xA042 and len(dec) >= 8:
                    off = _u32be(dec, 0); ln = _u32be(dec, 4)
                    print(f"    A042 offset={off} len={ln} tail={dec[8:].hex()}")
        print("done")
    finally:
        await w.disconnect()


async def cmd_ota_recon(args):
    ver = bytes.fromhex(args.ver_hex)
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        r = await w.send(0xFFFF, 0x9052, b"\xa5", expect=0xA052, timeout=6.0)
        for c1, c2, b in r:
            print(f"  INIT1 reply {c2:#06x} {b.hex()}")
        t0 = time.time()
        print("sending INIT2 (recon; NO data will be written) ...")
        try:
            r = await w.send(0xFFFF, 0x9040, ver, expect=0xA040, timeout=5.0)
            for c1, c2, b in r:
                print(f"  INIT2 reply {c2:#06x} {b.hex()}")
        except Exception as e:
            print("  INIT2 err:", e)
        print(f"t+{time.time()-t0:.1f}s INIT2 sent; watching link for disconnect ...")
        while w.client and w.client.is_connected and time.time() - t0 < args.max_seconds:
            for src, c1, c2, cc, ci, body, enc in await _pull(w, 0.5):
                dec = w._decode(c1, c2, body, enc)
                print(f"  <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
        print(f"link at t+{time.time()-t0:.1f}s: connected={bool(w.client and w.client.is_connected)}")
        print(f"scanning up to {args.scan_window:.0f}s for watch re-advertise ...")
        st = time.time()
        while time.time() - st < args.scan_window:
            devs = await BleakScanner.discover(timeout=5.0)
            for d in devs:
                nm = d.name or ""
                if (w.address and d.address.upper() == w.address.upper()) or \
                   "cmf" in nm.lower() or "watch" in nm.lower():
                    print(f"  t+{time.time()-st:5.0f}s {d.address} rssi={getattr(d,'rssi',None)} {nm!r}")
            hit = None
            if w.address:
                hit = next((d for d in devs if d.address.upper() == w.address.upper()), None)
            if hit:
                print("watch re-advertised; reconnecting ...")
                try:
                    await w.disconnect()
                except Exception:
                    pass
                w.client = BleakClient(hit, timeout=30.0)
                await w.client.connect()
                await w.attach()
                print(f"reconnected={w.client.is_connected}")
                try:
                    for s in w.client.services:
                        print(f"  svc {s.uuid}")
                        for ch in getattr(s, "characteristics", []):
                            print(f"      chr {ch.uuid} {ch.properties}")
                except Exception as e:
                    print("  (service dump failed:", e, ")")
                ak = load_state().get("authkey")
                if ak:
                    try:
                        await w.reconnect(bytes.fromhex(ak))
                        print("re-auth OK; sessionKey=", w.key.hex())
                    except BaseException as e:
                        print("re-auth failed:", e)
                print(f"listening {args.observe:.0f}s ...")
                end = time.time() + args.observe
                while time.time() < end and w.client.is_connected:
                    for src, c1, c2, cc, ci, body, enc in await _pull(w, 1.0):
                        dec = w._decode(c1, c2, body, enc)
                        print(f"  <{src} {c1:#06x},{c2:#06x}> enc={enc} len={len(dec)} {dec.hex()}")
                if args.retrigger and w.client.is_connected:
                    print("re-sending INIT1/INIT2 on receiver ...")
                    try:
                        r = await w.send(0xFFFF, 0x9052, b"\xa5", expect=0xA052, timeout=5.0)
                        for c1, c2, b in r:
                            print(f"  INIT1 {c2:#06x} {b.hex()}")
                        r = await w.send(0xFFFF, 0x9040, ver, expect=0xA040, timeout=5.0)
                        for c1, c2, b in r:
                            print(f"  INIT2 {c2:#06x} {b.hex()}")
                    except BaseException as e:
                        print("  retrigger err:", e)
                break
    finally:
        await w.disconnect()


async def cmd_wf_dials(args):
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        frames = list(w.seen) + await w.collect_all(args.duration)
        payloads = [b for c1, c2, b in frames if c2 == 0xA055]
        if not payloads:
            print("no a055 (wfInstalled) received")
            return
        p = payloads[-1]
        print(f"a055 header={p[:4].hex()} payload={len(p)} bytes")
        ids = []
        for off in range(4, len(p) - 3, 4):
            v = struct.unpack_from("<I", p, off)[0]
            if v == 0xFFFFFFFF:
                print("  -- sentinel --")
            else:
                ids.append(v)
                print(f"  0x{v:08x}  {v}")
        print(f"listed dials: {len(ids)}  (a055 omits side-loaded faces)")
    finally:
        await w.disconnect()


async def cmd_wf_flash(args):
    data = open(args.file, "rb").read()
    name = Watch.wf_validate(data)
    new_id = dial_id(args.seed or name)
    init2 = bytes([3]) + struct.pack("<III", args.old_id & 0xFFFFFFFF, new_id, len(data))
    print(f"file={args.file} bytes={len(data)} name={name!r} dialId={new_id} (0x{new_id:08x})")
    print(f"init2={init2.hex()}  old_wf_id={args.old_id} "
          f"({'append' if args.old_id == 0 else 'replace'})")
    if args.dry_run:
        print("dry run: no BLE")
        return
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        code, new_id, name, sent = await w.wf_upload(
            data, old_id=args.old_id, seed=args.seed, timeout=args.ack_timeout)
        if code == 1:
            print(f"OK: installed {name!r} id={new_id} ({sent}/{len(data)} bytes)")
        elif code == 0x0A:
            raise SystemExit("watch stored the face but did not activate it (0x0A): "
                             "all 6 slots full or stale old_wf_id")
        else:
            raise SystemExit(f"watchface flash failed: {code!r}")
    finally:
        await w.disconnect()


async def cmd_wf_delete(args):
    data = open(args.file, "rb").read()
    name = Watch.wf_validate(data)
    print(f"file={args.file} bytes={len(data)} (payload is only a transfer filler)")
    print(f"delete dial id={args.id} (0x{args.id & 0xFFFFFFFF:08x})")
    if args.dry_run:
        print("dry run: no BLE")
        return
    w = Watch(address=args.address)
    await w.connect(args.timeout)
    try:
        await ensure_session(w, args)
        code, _, _, sent = await w.wf_upload(
            data, old_id=args.id, new_id=0, timeout=args.ack_timeout)
        if code == 1:
            print(f"OK: dial {args.id} (0x{args.id & 0xFFFFFFFF:08x}) deleted "
                  f"({sent}/{len(data)} filler bytes)")
        elif code == 0x0A:
            raise SystemExit(f"dial {args.id} (0x{args.id & 0xFFFFFFFF:08x}) is not "
                             "installed (finish 0x0A): nothing deleted")
        else:
            raise SystemExit(f"dial delete failed: {code!r}")
    finally:
        await w.disconnect()


def main():
    ap = argparse.ArgumentParser(prog="cmf_watch")
    ap.add_argument("--address")
    ap.add_argument("--timeout", type=float, default=10.0)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("scan")
    p.add_argument("--timeout", type=float, default=10.0)
    p.set_defaults(fn=cmd_scan)
    p = sub.add_parser("at")
    p.add_argument("cmd", nargs="+")
    p.set_defaults(fn=cmd_at)
    p = sub.add_parser("info")
    p.set_defaults(fn=cmd_info)
    p = sub.add_parser("pair")
    p.set_defaults(fn=cmd_pair)
    p = sub.add_parser("sync")
    p.add_argument("--duration", type=float, default=30.0)
    p.add_argument("--limit", type=int, default=40)
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_sync)
    p = sub.add_parser("workouts")
    p.add_argument("--duration", type=float, default=30.0)
    p.add_argument("--limit", type=int, default=40)
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_workouts)
    p = sub.add_parser("specs")
    p.add_argument("--duration", type=float, default=6.0)
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_specs)
    p = sub.add_parser("wf-dials")
    p.add_argument("--duration", type=float, default=5.0)
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_wf_dials)
    p = sub.add_parser("wf-flash")
    p.add_argument("--file", required=True)
    p.add_argument("--old-id", type=to_int, default=0,
                   help="installed dial id to overwrite; 0 = append to a free slot")
    p.add_argument("--seed", help="dial-id seed (defaults to the name inside the file)")
    p.add_argument("--ack-timeout", type=float, default=30.0)
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_wf_flash)
    p = sub.add_parser("wf-delete")
    p.add_argument("--id", type=to_int, required=True,
                   help="installed dial id to delete (new_wf_id is sent as 0)")
    p.add_argument("--file", required=True,
                   help="any valid .wfdl; only used as the upload filler")
    p.add_argument("--ack-timeout", type=float, default=30.0)
    p.add_argument("--dry-run", action="store_true")
    p.set_defaults(fn=cmd_wf_delete)
    p = sub.add_parser("ota-init")
    p.add_argument("--payload-hex", default="a5")
    p.add_argument("--duration", type=float, default=8.0)
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_init)
    p = sub.add_parser("ota-state")
    p.add_argument("--duration", type=float, default=8.0)
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_state)
    p = sub.add_parser("ota-flash")
    p.add_argument("--image", required=True)
    p.add_argument("--ver-hex", default="01000000")
    p.add_argument("--idle", type=float, default=10.0)
    p.add_argument("--max-seconds", type=float, default=1200.0)
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--push", action="store_true")
    p.add_argument("--no-init2", action="store_true")
    p.add_argument("--push-bytes", type=int, default=65536)
    p.add_argument("--chunk", type=int, default=0)
    p.add_argument("--observe", type=float, default=8.0)
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_flash)
    p = sub.add_parser("ota-recon")
    p.add_argument("--ver-hex", default="01000000")
    p.add_argument("--scan-window", type=float, default=90.0)
    p.add_argument("--observe", type=float, default=20.0)
    p.add_argument("--max-seconds", type=float, default=60.0)
    p.add_argument("--retrigger", action="store_true")
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_recon)
    p = sub.add_parser("ota-push")
    p.add_argument("--image", required=True)
    p.add_argument("--ver-hex", default="01000000")
    p.add_argument("--push-bytes", type=int, default=65536)
    p.add_argument("--chunk", type=int, default=0)
    p.add_argument("--scan-window", type=float, default=60.0)
    p.add_argument("--observe", type=float, default=8.0)
    p.add_argument("--reauth", action="store_true")
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_push)
    p = sub.add_parser("ota-arm")
    p.add_argument("--ver-hex", default="01000000")
    p.add_argument("--scan-window", type=float, default=60.0)
    p.add_argument("--observe", type=float, default=25.0)
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_arm)
    p = sub.add_parser("ota-dtry")
    p.add_argument("--image", required=True)
    p.add_argument("--ver-hex", default="01000000")
    p.add_argument("--observe", type=float, default=30.0)
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_dtry)
    p = sub.add_parser("ota-resume")
    p.add_argument("--image", required=True)
    p.add_argument("--ver-hex", default="01000000")
    p.add_argument("--push-bytes", type=int, default=681)
    p.add_argument("--scan-window", type=float, default=60.0)
    p.add_argument("--observe", type=float, default=40.0)
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_resume)
    p = sub.add_parser("ota-run")
    p.add_argument("--image")
    p.add_argument("--init1-hex", default="014a")
    p.add_argument("--ver-hex", default="01000000")
    p.add_argument("--idle", type=float, default=15.0)
    p.add_argument("--max-seconds", type=float, default=180.0)
    p.add_argument("--observe", type=float, default=30.0)
    p.add_argument("--scan-window", type=float, default=90.0)
    p.add_argument("--chunk", type=int, default=0)
    p.add_argument("--dry-run", action="store_true")
    p.add_argument("--no-init2", action="store_true")
    p.add_argument("--reconnect", action="store_true")
    p.add_argument("--resp-data", action="store_true",
                   help="send 0x9042 responses on the DATA channel (ffe1) instead of CMD (fff2)")
    p.add_argument("--repair", action="store_true")
    p.set_defaults(fn=cmd_ota_run)
    args = ap.parse_args()
    try:
        asyncio.run(args.fn(args))
    except KeyboardInterrupt:
        pass


if __name__ == "__main__":
    main()
