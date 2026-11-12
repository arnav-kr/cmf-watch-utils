import hashlib
from pathlib import Path
import struct
from types import SimpleNamespace
import unittest
from unittest.mock import patch

from cmf_watch_utils import actions, cmf_watch
from cmf_watch_utils.emu import image, prepare, stock_ota, vm
from cmf_watch_utils.emu.profiles import PROFILES, WP2
from cmf_watch_utils.ota_client import OTA_SVC
from fwkit import aota


class ActionsPeer:
    mtu_size = 23

    def __init__(self, payload, serve, bad_request=False):
        self.payload = payload
        self.serve = serve
        self.bad_request = bad_request
        self.commands = []
        self.buffer = bytearray()
        self.chunks = []

    async def start_notify(self, uuid, callback):
        assert uuid == actions.NOTIFY
        self.notify = callback

    def reply(self, command, body):
        packet = actions.frame(command, body)
        self.notify(None, packet[:3])
        self.notify(None, packet[3:])

    async def write_gatt_char(self, uuid, data, response):
        assert uuid == actions.WRITE and response and 0 < len(data) <= 20
        self.buffer.extend(data)
        if len(self.buffer) < 5:
            return
        service, command, kind, size = struct.unpack_from("<BBBH", self.buffer)
        assert (service, kind) == (9, 128)
        if len(self.buffer) < size + 5:
            return
        assert len(self.buffer) == size + 5
        body = bytes(self.buffer[5:])
        self.buffer.clear()
        self.commands.append(command)
        if command == 1:
            assert actions.unpack_tlvs(body) == {9: b"\0"}
            self.reply(1, actions.tlv(0x7f, 100000, 4))
        elif command == 2:
            self.reply(2, actions.tlv(3, 35, 2) + actions.tlv(5, 0))
        elif command == 9:
            assert self.serve
            assert actions.unpack_tlvs(body) == {1: b"\x01"}
            length = len(self.payload) + self.bad_request
            self.reply(3, actions.tlv(1, 0, 4) + actions.tlv(2, length, 4) + actions.tlv(3, 0))
        elif command == 4:
            assert body[0] == len(self.chunks)
            self.chunks.append(body[1:])
            if len(b"".join(self.chunks)) == len(self.payload):
                self.reply(6, actions.tlv(1, 1))


class ActionsTests(unittest.IsolatedAsyncioTestCase):
    async def exchange(self, serve, bad_request=False):
        payload = bytes(range(100))
        peer = ActionsPeer(payload, serve, bad_request)
        opts = SimpleNamespace(timeout=1, packet_delay=0, flash=serve)
        result = await actions.exchange(peer, opts, payload, lambda *args, **kwargs: None)
        return result, peer

    async def test_probe_does_not_commit_or_send_data(self):
        result, peer = await self.exchange(False)
        self.assertEqual(result, "negotiated_only")
        self.assertEqual(peer.commands, [1, 2])

    async def test_transfer_fragments_and_serves_exact_requested_bytes(self):
        result, peer = await self.exchange(True)
        self.assertEqual(result, "watch_reported_image_valid")
        self.assertEqual(peer.commands, [1, 2, 9, 4, 4, 4])
        self.assertEqual(b"".join(peer.chunks), peer.payload)

    async def test_transfer_rejects_reads_past_image(self):
        with self.assertRaisesRegex(RuntimeError, "read request"):
            await self.exchange(True, bad_request=True)


class StockOtaTests(unittest.IsolatedAsyncioTestCase):
    async def test_init2_and_end_use_expected_characteristics(self):
        traffic = []

        class Peer:
            is_connected = True

            async def write_gatt_char(self, uuid, data, response):
                command = struct.unpack_from(">H", data, 9)[0]
                traffic.append((uuid, command))
                if command == 0x9041:
                    self.ota.rx.put_nowait((0xffff, 0xa041, 1, 1, b"\x01", False))

            async def disconnect(self):
                pass

        peer = Peer()
        payload = b"virtual firmware"

        async def connect(ota):
            peer.ota = ota
            ota.client = peer
            for command, value in (
                (0xa052, b"\x01"), (0xa040, b"\x01"),
                (0xa042, struct.pack(">II", 0, len(payload))), (0xa041, b"\x01"),
            ):
                ota.rx.put_nowait((0xffff, command, 1, 1, value, False))

        args = SimpleNamespace(flash=True, version="0100005a", packet_delay=0, timeout=1)
        with patch.object(stock_ota.VirtualOta, "connect", connect):
            result = await stock_ota.transfer(args, peer, payload)
        self.assertEqual(result["result"], "watch_reported_image_valid")
        self.assertTrue(result["end_sent"])
        self.assertEqual(traffic, [(cmf_watch.CMD_W, 0x9052), (OTA_SVC, 0x9040),
                                   (OTA_SVC, 0x9042), (cmf_watch.CMD_W, 0x9041)])


class ProfileTests(unittest.TestCase):
    def test_run_env_exports_motor_log_path(self):
        output = Path("/tmp/wp2-run")
        args = SimpleNamespace(runtime_dir=None, charger_connected=False)
        env = vm.run_env(args, WP2, output, output / "nor.bin", output / "nand.bin")
        self.assertEqual(env["QEMU_WP2_MOTOR"], str(output / "motor.log"))
        self.assertEqual(env["QEMU_WP2_FRAME"], str(output / "frame.ppm"))

    def test_reconstructed_partition_table_matches_the_wp2_layout(self):
        table = prepare.partition_table()
        self.assertEqual(hashlib.sha256(table).hexdigest(),
                         "f6ddbec107756774bcce91a0aab8c34f030f07a1e0edcbda81431b65fd8f0d5b")
        self.assertEqual(struct.unpack_from("<I", table, len(table) - 4)[0], aota.crc32(table[:-4]))

    def test_onboarding_selects_a_distinct_nand_fixture(self):
        normal, onboarding = PROFILES["wp2-stock"], PROFILES["wp2-onboarding"]
        self.assertIs(normal.model, onboarding.model)
        self.assertNotEqual(normal.nand_filename, onboarding.nand_filename)

    def test_lzma_checks_size_and_erased_tail(self):
        payload = b"emulator payload" * 3
        packed = aota.pack_chunks(payload)
        padding = bytes(-len(packed) % 512) + b"\xff" * 512
        self.assertEqual(image.decompress(packed + padding, len(payload), allow_padding=True), payload)
        with self.assertRaisesRegex(ValueError, "chunk bounds"):
            image.decompress(packed, len(payload) - 1)
        with self.assertRaisesRegex(ValueError, "Unexpected bytes"):
            image.decompress(packed + padding + b"\0", len(payload), allow_padding=True)

    def test_board_check_applies_even_with_valid_crc(self):
        hdr = bytearray(aota.HDR_SIZE)
        hdr[:4] = b"AOTA"
        hdr[0x60:0x65] = b"other"
        struct.pack_into("<H", hdr, 0x12, aota.HDR_SIZE)
        data = bytearray(aota.build(hdr, b"<unused/>", [], b""))
        struct.pack_into("<I", data, 0x18, aota.crc32(data[aota.HDR_SIZE:]))
        header = data[:aota.HDR_SIZE]
        aota.fix_header(header)
        data[:aota.HDR_SIZE] = header
        with self.assertRaisesRegex(ValueError, "does not match"):
            image.container(data, WP2)
        data[-1] ^= 1
        with self.assertRaisesRegex(ValueError, "data CRC"):
            image.container(data, WP2)


if __name__ == "__main__":
    unittest.main()
