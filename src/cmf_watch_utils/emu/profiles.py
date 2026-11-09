from dataclasses import dataclass
from pathlib import Path
import hashlib
import struct


@dataclass(frozen=True)
class Partition:
    file_id: int
    offset: int
    size: int

    def read(self, nor):
        return nor[self.offset:self.offset + self.size]

    def stage(self, nor, payload):
        if not 0 < len(payload) <= self.size:
            raise ValueError(f"Payload does not fit partition {self.file_id}")
        nor[self.offset:self.offset + self.size] = payload.ljust(self.size, b"\xff")


@dataclass(frozen=True)
class Model:
    name: str
    title: str
    board: str
    machine: str
    env_prefix: str
    nor_bytes: int
    nand_bytes: int
    nand_id_bytes: int
    app: Partition
    sdfs: Partition
    temp: Partition
    vector_offset: int
    code_base: int
    ram_start: int
    ram_end: int

    def validate_app(self, app):
        if not self.vector_offset + 8 <= len(app) <= self.app.size:
            raise ValueError("Application size does not fit the selected model")
        sp, reset = struct.unpack_from("<II", app, self.vector_offset)
        if not (self.ram_start <= sp < self.ram_end and reset & 1
                and self.code_base + self.vector_offset <= reset < self.code_base + len(app)):
            raise ValueError("Application vectors do not match the selected model")


@dataclass(frozen=True)
class Profile:
    name: str
    model: Model
    stock_version: str
    stock_app_bytes: int
    stock_app_sha256: str
    nand_filename: str
    live_health: bool

    def stock_app(self, nor):
        app = self.model.app.read(nor)[:self.stock_app_bytes]
        if hashlib.sha256(app).hexdigest() != self.stock_app_sha256:
            raise ValueError(f"{self.name} requires its verified stock application in the fixture NOR")
        return app

    def fixtures(self, directory):
        nor, nand, nand_id = (directory / name for name in ("nor.bin", self.nand_filename, "nand-id.bin"))
        for path, size in ((nor, self.model.nor_bytes), (nand, self.model.nand_bytes),
                           (nand_id, self.model.nand_id_bytes)):
            if not path.is_file() or path.stat().st_size != size:
                raise ValueError(f"Expected {size} bytes at {path}")
        self.stock_app(nor.read_bytes())
        return nor, nand, nand_id


WP2 = Model("wp2", "CMF Watch Pro 2", "jx402_01_3089c", "ats3089c-wp2",
            "QEMU_WP2_", 0x400000, 2112 * 65536, 790,
            Partition(4, 0x24000, 0x34b000), Partition(5, 0x3d3000, 0x10000),
            Partition(254, 0, 0x380000), 0x200, 0x10000000, 0x2ff00000, 0x30000000)
_STOCK = ("1.0.0.73", 2465920, "30e7978c39d8fb79b825c7ebf69b4377d214e9cc6739a52f06dbb5ff7f13b490")
PROFILES = {
    "wp2-stock": Profile("wp2-stock", WP2, *_STOCK, "nand-defaults.bin", True),
    "wp2-onboarding": Profile("wp2-onboarding", WP2, *_STOCK, "nand-onboarding.bin", True),
}
