# Watch Pro 2 emulator builds

These preview archives contain the ATS3089C QEMU machine or its
host-independent compatibility ROM, virtual NAND provisioner and staged OTA reader. Stock firmware,
resources, virtual flash images, and the Android lab stay outside the archives.
The checkout's firmware tools and local fixtures are still needed to run
firmware tests. These builds do not establish hardware fidelity.

Use the [Emulator CLI guide](https://cmf.arnv.dev/tooling/emulator/) for setup, profile
selection and test commands. From the repository root, `uv sync --locked` and
`uv run cmf-watch-utils emu profiles` list the two Watch Pro 2 baselines. Running
the emulator also requires a matching compatibility ROM and prepared local virtual
NOR, raw NAND and NAND ID files. Generate them with `emu prepare-fixtures` using
the exact stock package, pinned NAND ID and Arm-capable GDB. The interactive viewer uses Tcl/Tk `wish`, while
loopback OTA uses an isolated H4 controller. The recovery supervisor models
installation and rescue. It does not run the physical bootloader.

Build from the repository root with Python 3.12 or later.

```text
python emulator/build.py --component qemu --audio pa
python emulator/build.py --component qemu --audio dsound
python emulator/build.py --component qemu --audio coreaudio
python emulator/build.py --component rom
```

The commands use the native compiler and Ninja for the host. Use UCRT64 on
Windows. Install GLib, Pixman, libfdt, pkg-config and development files for the
selected audio backend. The ROM build uses Arm GNU GCC, binutils, and newlib.
The binary archives use the host's system libraries. On Windows, run with the
MSYS2 UCRT64 runtime packages on PATH.

The builder verifies the pinned QEMU source archive and downloads the required
source files from a pinned Actions SDK commit. Use `--qemu-archive` and
`--sdk-root` for local source inputs. `--work-dir`, `--output-dir`, `--jobs`,
`--arm-prefix` and repeatable `--configure-arg=...` are configurable. Select a
fresh work directory when changing QEMU configure flags. The default audio
configuration supports headless testing. Use `--wfc-dir` for an optional local
stock texture index. The published preview omits this index, so some stock
watch faces may render differently.

GitHub Actions builds Linux x64, Windows x64, macOS ARM64, and one shared ROM
and NAND-reader bundle. Each native build must execute a small synthetic ARM
program through the watch machine's NOR handoff. The program needs no stock
files. A manual run uploads artifacts for inspection. Pushing an `emulator-v*`
tag publishes a prerelease with checksums and corresponding source archives
after all build jobs pass.
