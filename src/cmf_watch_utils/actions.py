import asyncio
import struct


SERVICE = "e49a25f8-f69a-11e8-8eb2-f2801f1b9fd1"
WRITE = "e49a25e0-f69a-11e8-8eb2-f2801f1b9fd1"
NOTIFY = "e49a28e1-f69a-11e8-8eb2-f2801f1b9fd1"


def tlv(kind, value, width=1):
    return struct.pack("<BH", kind, width) + value.to_bytes(width, "little")


def unpack_tlvs(body):
    result = {}
    while body:
        if len(body) < 3:
            raise ValueError("Truncated TLV header")
        kind, size = struct.unpack_from("<BH", body)
        if size > len(body) - 3 or kind in result:
            raise ValueError("Truncated or duplicate TLV")
        result[kind] = body[3:3 + size]
        body = body[3 + size:]
    return result


def number(fields, kind, width):
    value = fields.get(kind)
    if value is None or len(value) != width:
        raise ValueError(f"Missing/invalid TLV {kind:#x}, expected {width} bytes")
    return int.from_bytes(value, "little")


def frame(cmd, body=b""):
    return struct.pack("<BBBH", 9, cmd, 0x80, len(body)) + body


class Receiver:
    def __init__(self, log):
        self.buffer = bytearray()
        self.queue = asyncio.Queue()
        self.log = log

    def notify(self, sender, data):
        self.log("rx", hex=bytes(data).hex())
        self.buffer.extend(data)
        try:
            while len(self.buffer) >= 5:
                service, cmd, kind, size = struct.unpack_from("<BBBH", self.buffer)
                if service != 9 or kind != 0x80 or size > 0x3fff:
                    raise ValueError("Notification does not match Actions OTA framing")
                if len(self.buffer) < 5 + size:
                    return
                body = bytes(self.buffer[5:5 + size])
                del self.buffer[:5 + size]
                self.queue.put_nowait((cmd, body))
        except ValueError as exc:
            self.buffer.clear()
            self.queue.put_nowait(exc)

    async def receive(self, timeout, expected=None):
        item = await asyncio.wait_for(self.queue.get(), timeout)
        if isinstance(item, Exception):
            raise item
        cmd, body = item
        if expected is not None and cmd != expected:
            raise RuntimeError(f"Expected command {expected:#x}, got {cmd:#x}")
        return cmd, unpack_tlvs(body)


async def transfer(client, rx, args, image, log):
    limit = min(512, max(20, client.mtu_size - 3))

    async def send(cmd, body=b""):
        packet = frame(cmd, body)
        log("tx", command=cmd, length=len(packet))
        for start in range(0, len(packet), limit):
            await asyncio.wait_for(client.write_gatt_char(
                WRITE, packet[start:start + limit], response=True), args.timeout)
            if getattr(args, "packet_delay", 0):
                await asyncio.sleep(args.packet_delay)

    await send(1, tlv(9, 0))
    _, fields = await rx.receive(args.timeout, 1)
    status = number(fields, 0x7f, 4)
    if status != 100000:
        raise RuntimeError(f"Watch rejected upgrade request: {status}")
    await send(2)
    _, fields = await rx.receive(args.timeout, 2)
    unit = number(fields, 3, 2)
    if not 1 <= unit <= 4096 or number(fields, 5, 1) != 0:
        raise RuntimeError("Unsupported unit size or acknowledgement mode")
    log("negotiated", unit=unit, parameters={str(k): v.hex() for k, v in fields.items()})
    print(f"Actions OTA negotiation accepted; unit={unit}", flush=True)
    if not args.flash:
        print("Probe complete. No firmware bytes sent; negotiation result not committed.")
        return "negotiated_only"
    await send(9, tlv(1, 1))
    served = 0
    while True:
        cmd, fields = await rx.receive(args.timeout)
        if cmd == 3:
            offset = number(fields, 1, 4)
            length = number(fields, 2, 4)
            mask = number(fields, 3, 1)
            if mask or length == 0 or length > 16 * 1024 * 1024 or offset + length > len(image):
                raise RuntimeError(f"Invalid/unsupported read request: {offset=}, {length=}, {mask=}")
            log("read_request", offset=offset, length=length)
            for seq, pos in enumerate(range(offset, offset + length, unit)):
                chunk = image[pos:min(pos + unit, offset + length)]
                await send(4, bytes([seq & 255]) + chunk)
            served += length
            print(f"Served offset=0x{offset:x} length={length}; total served={served}", flush=True)
        elif cmd == 6:
            valid = number(fields, 1, 1)
            if valid != 1 or not served:
                raise RuntimeError("Watch reported image validation failure")
            log("image_valid", bytes_served=served)
            print("Peer reported image validation success.")
            return "watch_reported_image_valid"
        else:
            raise RuntimeError(f"Unexpected command {cmd:#x}; stopping without guessed replies")


async def exchange(client, args, image, log):
    receiver = Receiver(log)
    await asyncio.wait_for(client.start_notify(NOTIFY, receiver.notify), args.timeout)
    return await transfer(client, receiver, args, image, log)


async def bluetooth(args, image, log):
    from bleak import BleakClient, BleakScanner
    from bleak.exc import BleakError

    try:
        device = await BleakScanner.find_device_by_address(args.address, timeout=args.scan_timeout)
        if device is None:
            raise RuntimeError("The requested BLE address was not discovered")
        async with BleakClient(device, services=[SERVICE], timeout=args.connect_timeout,
                               winrt={"use_cached_services": False}) as client:
            characteristic = client.services.get_characteristic(WRITE)
            if characteristic is None or "write" not in characteristic.properties:
                raise RuntimeError("Actions OTA requires an acknowledged-write characteristic")
            return await exchange(client, args, image, log)
    except BleakError as error:
        raise RuntimeError(f"Actions BLE transport failed: {error}") from error
