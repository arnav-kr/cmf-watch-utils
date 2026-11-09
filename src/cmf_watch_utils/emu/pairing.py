import asyncio
import secrets
import socket

from ..cmf_watch import Watch
from .hci import virtual_gatt
from .image import require
from .vm import Qmp


async def provision(port, name):
    async with virtual_gatt(port, name) as client:
        watch = Watch(verbose=False)
        watch.client = client
        await watch.attach()
        await asyncio.sleep(0.6)
        secret = secrets.token_hex(16)
        reply = await watch.at(f"AT SETSECRET {secret}", timeout=4)
        actual = await watch.at("AT GETSECRET", timeout=4)
        require(b"SETSECRET,OK" in reply and secret.encode() in actual,
                "Virtual stock firmware did not retain the synthetic secret")


def pair_fixture(args, profile):
    require(not args.output.exists(), f"Output already exists: {args.output}")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with socket.create_connection(("127.0.0.1", args.qmp_port), timeout=10) as conn:
        with conn.makefile("rwb", buffering=0) as stream:
            qmp = Qmp(stream)
            asyncio.run(provision(args.hci_port, args.name))
            qmp.request("stop")
            qmp.request("pmemsave", val=0x11000000, size=profile.model.nor_bytes, filename=str(args.output))
            qmp.request("quit")
    require(args.output.is_file() and args.output.stat().st_size == profile.model.nor_bytes,
            "QEMU did not export a complete virtual NOR")
    profile.stock_app(args.output.read_bytes())
    return {"profile": profile.name, "nor": str(args.output), "pairing": "synthetic"}
