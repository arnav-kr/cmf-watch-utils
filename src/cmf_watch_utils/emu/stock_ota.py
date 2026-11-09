import asyncio
from types import SimpleNamespace

from .. import cmf_watch as cw
from ..ota_client import Ota, OTA_NOTIFY, OTA_SVC


class VirtualOta(Ota):
    allow_init2_fallback = False

    async def connect(self):
        self.client = self.args.client
        watch = cw.Watch(verbose=False)
        watch.client = self.client
        await watch.attach()
        await asyncio.sleep(0.6)
        await watch.handshake()
        self.key = watch.key
        await self.client.start_notify(cw.CMD_N, self.on_notify)
        await self.client.start_notify(OTA_NOTIFY, self.on_notify)

    async def handshake(self):
        self.log("Authenticated the virtual stock app")

    async def write_ota(self, frame):
        await super().write_ota(frame)
        await asyncio.sleep(self.args.pace)


async def transfer(args, client, data):
    version = bytes.fromhex(args.version)
    if len(version) != 4:
        raise ValueError("Stock OTA version must contain four bytes")
    config = SimpleNamespace(ota_write=OTA_SVC, client=client, pace=args.packet_delay,
                             dry_run=not args.flash, version=version.hex(), idle_timeout=args.timeout,
                             stop_after_a040=False, pipeline=1, verbose=False)
    ota = VirtualOta(config, image=data)
    try:
        await ota.run()
    except SystemExit as error:
        raise RuntimeError(str(error)) from error
    accepted = (ota.end_sent and bool(ota.served_bytes) and bool(ota.statuses) and
                all(status == (0xA041, b"\x01") for status in ota.statuses))
    return {"result": "watch_reported_image_valid" if accepted else
            "negotiated_only" if not args.flash and ota.a052.is_set() else "unconfirmed",
            "bytes_served": ota.served_bytes, "frames_served": ota.served_frames,
            "end_sent": ota.end_sent, "statuses": [{"command": command, "value": value.hex()}
                                                  for command, value in ota.statuses]}
