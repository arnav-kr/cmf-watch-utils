---
title: Ghidra workflow
description: Import app.bin, set the right base address and use the bundled analysis scripts.
---

## Import

`app.bin` is a raw Thumb-2 image mapped at **`0x10000000`**. The import uses the
`ARM:LE:32:Cortex` processor with an image base of `0x10000000`. The ACTH header starts at file
offset 0, so file offset `X` maps to address `0x10000000 + X`.

## Bundled scripts

`tooling/ghidra_scripts/` contains helpers for repeatable exports:

| Script | Purpose |
|---|---|
| `DumpApp.java` | Summary, entry points and header fields |
| `CountFns.java` | Function count and address ranges |
| `FuncAt.java` / `ListFnsInRange.java` | Function boundaries |
| `Decompile.java` / `DecompileAt.java` / `DecompRange.java` | Export decompilation |
| `Callers.java` / `CallersOf.java` / `RefsTo.java` / `Xrefs.java` | Call and data xrefs |
| `FindImm.java` / `FindOps.java` / `SearchDecomp.java` | Immediate/opcode/text search |
| `FindStrings.java` / `FindStrXrefs.java` | String tables and references |
| `FindOta.java` / `OtaFns.java` / `OtaGraph.java` / `OtaHandlers.java` / `OtaReq.java` | OTA state-machine mapping |
| `NameMap.java` / `ExportAnnotated.java` | Export names and annotated listings |

## Headless example

```bash
analyzeHeadless <projdir> wp2 \
  -import app.bin \
  -processor ARM:LE:32:Cortex \
  -loader BinaryLoader \
  -loader-baseAddr 0x10000000 \
  -scriptPath tooling/ghidra_scripts \
  -postScript DumpApp.java \
  -analysisTimeoutPerFile 900
```

A single function is decompiled with:

```bash
analyzeHeadless <projdir> wp2 -process app.bin -noanalysis \
  -scriptPath tooling/ghidra_scripts \
  -postScript DecompileAt.java 0x101F4D70
```

Exact argument order depends on the script; the Java file header lists its parameters.

## Conventions

- Addresses in this documentation are virtual addresses (`0x10...`), not file offsets.
- `0x101F4D70` is the BLE/AT group dispatcher; `0x10054CF4` is `system_app_launch`.
- The ACTH fields are referenced in [app.bin internals](/watch-utils/devices/cmf-watch-pro-2/internals/appbin/).
- Decompiler output is a hypothesis, confirmed by a live reply or a byte-level round-trip.

## Reproducibility

Ghidra versions differ in analysis quality, so the version, the script arguments and the
SHA-256 of `app.bin` accompany a published finding. The firmware and the Ghidra project are
not committed; both are large and the firmware is not redistributable here.
