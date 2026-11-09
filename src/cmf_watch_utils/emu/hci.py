import asyncio
from contextlib import asynccontextmanager
import socket
import struct
import time


def read_exact(stream, size):
    data = stream.read(size)
    if len(data) != size:
        raise EOFError("H4 transport disconnected")
    return data


class Controller:
    def __init__(self, sock):
        self.sock = sock
        self.stream = sock.makefile("rb")

    def receive(self):
        kind = read_exact(self.stream, 1)[0]
        if kind == 4:
            event, length = read_exact(self.stream, 2)
            return event, read_exact(self.stream, length)
        if kind == 2:
            handle, length = struct.unpack("<HH", read_exact(self.stream, 4))
            return 0, struct.pack("<H", handle) + read_exact(self.stream, length)
        raise RuntimeError(f"Unexpected H4 packet type {kind:#x}")

    def command(self, opcode, data=b""):
        self.sock.sendall(struct.pack("<BHB", 1, opcode, len(data)) + data)
        while True:
            kind, payload = self.receive()
            if kind == 0x0e and int.from_bytes(payload[1:3], "little") == opcode:
                status = payload[3]
            elif kind == 0x0f and int.from_bytes(payload[2:4], "little") == opcode:
                status = payload[0]
            else:
                continue
            if status:
                raise RuntimeError(f"HCI {opcode:#06x} returned {status:#04x}")
            return

    def find(self, name):
        deadline = time.monotonic() + 15
        while time.monotonic() < deadline:
            kind, payload = self.receive()
            if kind != 0x3e or payload[0] != 2:
                continue
            offset = 2
            for _ in range(payload[1]):
                address_type = payload[offset + 1]
                address = payload[offset + 2:offset + 8]
                length = payload[offset + 8]
                data = payload[offset + 9:offset + 9 + length]
                offset += 10 + length
                for field in ad_fields(data):
                    if field[0] in (8, 9) and name in field[1:].lower():
                        return address_type, address
        raise TimeoutError(f"No matching advertisement for {name!r}")

    def connect(self, address_type, address):
        params = struct.pack("<HHBB6sBHHHHHH", 96, 48, 0, address_type, address,
                             0, 24, 40, 0, 200, 0, 0)
        self.command(0x200d, params)
        while True:
            kind, payload = self.receive()
            if kind != 0x3e or payload[0] not in (1, 0x0a):
                continue
            if payload[1]:
                raise RuntimeError(f"LE connection status {payload[1]:#04x}")
            return int.from_bytes(payload[2:4], "little")

    def send_att(self, handle, payload):
        l2cap = struct.pack("<HH", len(payload), 4) + payload
        self.sock.sendall(struct.pack("<BHH", 2, handle, len(l2cap)) + l2cap)

    def receive_att(self, handle):
        pending = bytearray()
        length = 0
        channel = None
        while True:
            kind, data = self.receive()
            if kind == 5:
                raise RuntimeError("Watch disconnected during GATT discovery")
            if kind != 0:
                continue
            flags = struct.unpack_from("<H", data)[0]
            if (flags & 0xfff) != handle:
                continue
            if flags & 0x3000 == 0x1000:
                pending.extend(data[2:])
            else:
                length, channel = struct.unpack_from("<HH", data, 2)
                pending = bytearray(data[6:])
            if channel == 4 and len(pending) >= length:
                return pending[:length]

def ad_fields(data):
    while data and 0 < data[0] < len(data):
        yield data[1:data[0] + 1]
        data = data[data[0] + 1:]


def watch_link(sock, port_name=b"cmf"):
    controller = Controller(sock)
    controller.command(0x0c03)
    controller.command(0x0c01, bytes.fromhex("ffffffffffff3f20"))
    controller.command(0x2001, bytes.fromhex("1f00000000000000"))
    controller.command(0x200b, struct.pack("<BHHBB", 1, 96, 48, 0, 0))
    controller.command(0x200c, b"\x01\x00")
    addr_type, address = controller.find(port_name)
    print("Matched a watch advertisement.")
    controller.command(0x200c, b"\x00\x00")
    handle = controller.connect(addr_type, address)
    print("LE link established.")
    return controller, handle


def uuid(data):
    value = bytes(data[::-1]).hex()
    if len(value) == 4:
        return f"0000{value}-0000-1000-8000-00805f9b34fb"
    return f"{value[:8]}-{value[8:12]}-{value[12:16]}-{value[16:20]}-{value[20:]}"


class Gatt:
    def __init__(self, controller, handle):
        self.controller = controller
        self.handle = handle
        self.responses = asyncio.Queue()
        self.notifications = {}
        self.characteristics = {}
        self.is_connected = True
        self.reader = asyncio.create_task(self.receive())

    async def receive(self):
        try:
            while self.is_connected:
                data = await asyncio.to_thread(self.controller.receive_att, self.handle)
                if data[0] in (0x1b, 0x1d):
                    handle = struct.unpack_from("<H", data, 1)[0]
                    callback = self.notifications.get(handle)
                    if callback:
                        callback(handle, data[3:])
                    if data[0] == 0x1d:
                        self.controller.send_att(self.handle, b"\x1e")
                elif data[0] == 0x02:
                    self.controller.send_att(self.handle, b"\x03\xf7\x00")
                else:
                    self.responses.put_nowait(data)
        except (OSError, RuntimeError, EOFError) as exc:
            self.responses.put_nowait(exc)

    async def request(self, data, reply):
        self.controller.send_att(self.handle, data)
        result = await asyncio.wait_for(self.responses.get(), 12)
        if isinstance(result, Exception):
            raise result
        if result[0] == 1 and result[-1] == 0x0a and reply in (5, 9):
            return None
        if result[0] != reply:
            raise RuntimeError(f"ATT {data[0]:#04x} failed: {result.hex()}")
        return result

    async def discover(self):
        reply = await self.request(b"\x02\xf7\x00", 3)
        self.mtu_size = min(247, int.from_bytes(reply[1:3], "little"))
        start = 1
        declarations = []
        while start <= 0xffff:
            result = await self.request(struct.pack("<BHHH", 8, start, 0xffff, 0x2803), 9)
            if result is None:
                break
            size = result[1]
            for off in range(2, len(result), size):
                decl, _, value = struct.unpack_from("<HBH", result, off)
                declarations.append((decl, value, uuid(result[off + 5:off + size])))
                start = decl + 1
        for i, (_, value, name) in enumerate(declarations):
            end = declarations[i + 1][0] - 1 if i + 1 < len(declarations) else 0xffff
            self.characteristics[name] = value, end

    async def start_notify(self, name, callback):
        value, end = self.characteristics[name]
        start = value + 1
        while start <= end:
            result = await self.request(struct.pack("<BHH", 4, start, end), 5)
            if result is None:
                break
            size = 4 if result[1] == 1 else 18
            for off in range(2, len(result), size):
                handle = struct.unpack_from("<H", result, off)[0]
                if result[off + 2:off + size] == b"\x02\x29":
                    self.notifications[value] = callback
                    await self.request(struct.pack("<BH", 0x12, handle) + b"\x01\x00", 0x13)
                    return
                start = handle + 1
        raise RuntimeError(f"CCCD not found for {name}")

    async def write_gatt_char(self, name, value, response=True):
        handle, _ = self.characteristics[name]
        data = struct.pack("<BH", 0x12 if response else 0x52, handle) + value
        if response:
            await self.request(data, 0x13)
        else:
            self.controller.send_att(self.handle, data)

    async def disconnect(self):
        if not self.is_connected:
            return
        self.is_connected = False
        try:
            self.controller.sock.shutdown(socket.SHUT_RDWR)
        except OSError:
            pass
        await self.reader


@asynccontextmanager
async def virtual_gatt(port, name):
    with socket.create_connection(("127.0.0.1", port), timeout=15) as sock:
        controller, handle = await asyncio.to_thread(watch_link, sock, name.encode().lower())
        sock.settimeout(None)
        gatt = Gatt(controller, handle)
        try:
            await gatt.discover()
            yield gatt
        finally:
            await gatt.disconnect()
