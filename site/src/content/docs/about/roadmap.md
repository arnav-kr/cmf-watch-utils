---
title: Roadmap
description: How devices are added to the project and which models are planned.
---

## Structure

The project separates device-specific research from shared tooling:

```text
site/src/content/docs/
  index.mdx               project home and device matrix
  about/                  project-wide documentation (methodology, legal, this page)
  getting-started/        host setup and first steps
  tooling/                fwkit and BLE tool references
  devices/<model>/        protocol, formats, internals, guides for one device
  ai/                     agent-facing briefings
  dumps/                  manifests and downloadable images
```

Tooling in `tooling/` and `tools/` is device-agnostic. Everything that depends on a specific
firmware build — protocol constants, container layouts, addresses, verified results — lives
under `devices/<model>/`.

A device section is identified by a slug such as `cmf-watch-pro-2`, matching the model name
used in product documentation. The header carries a device switcher so that pages for
different models stay one click apart.

## Adding a device

The criteria for a new device section:

1. A firmware image is available and its AOTA container parses with `fwkit`.
2. At least one read-only BLE path works (pairing plus a status or shell command).
3. The device-specific facts are recorded as offsets, constants and UUIDs, not as impressions.

The section then contains the device overview, protocol details, format notes, internal
layout, verified results and the open questions that remain.

## Planned models

| Model | Status | Notes |
|---|---|---|
| CMF Watch Pro 3 | Not started | Expected to share the `AOTA` container family; every constant still has to be re-derived |
| CMF Watch Pro (1st generation) | Not started | Older baseband and UI stack; likely a different OTA path |
| CMF Watch (1st generation) | Not started | Same caveat |

Nothing in this table is verified. The CMF Watch Pro 2 section documents one device; the
constants, offsets and BLE behaviour described there apply to that device only, and the same
code paths are not assumed to exist elsewhere.

## Known scope boundaries

- The container formats are shared across devices from the same vendor generation, but the
  layouts are re-verified per image rather than assumed.
- The BLE command set is device-specific in detail; the shared tooling is a transport, not a
  protocol abstraction.
- No cross-device compatibility is claimed, and no device without a verified page is
  represented as supported.