---
title: Legal & attribution
description: Licensing, third-party notices and what is deliberately not redistributed.
---

## License

This repository is licensed under the **GNU Affero General Public License v3.0 (AGPL-3.0)**.
See [`LICENSE`](https://github.com/arnav-kr/cmf-watch-utils/blob/main/LICENSE).

Why AGPL: the BLE/frame behaviour and watch-face upload flow were informed by, and in part
ported from, the AGPL-3.0 licensed [`freethinkel/fmc`](https://github.com/freethinkel/fmc)
project. Keeping the combined work under AGPL-3.0 is the simplest way to respect that.

## Third-party attribution

- [`freethinkel/fmc`](https://github.com/freethinkel/fmc) — independent watch-face editor and
  protocol reference (AGPL-3.0). Used for cross-checking frame headers, AES/CRC rules, WFDL
  layout and the upload handshake.
- Nothing/CMF and the CMF Watch Pro 2 name are trademarks of their respective owners. This is
  an unofficial interoperability project and is not affiliated with or endorsed by Nothing.
- Bad Apple is a copyrighted work. This repository does **not** include Bad Apple frames or a
  prebuilt Bad Apple firmware image. The case study describes the process only.

## Not redistributed

To keep the repository small and to avoid redistributing third-party or proprietary material,
the following are intentionally excluded:

- Stock or modified firmware images (`*.bin`), including `JX402_V1.0.0.73*.bin`.
- Vendor resources, fonts, pictures, `.ajs` animations and watch-face packages.
- Captured personal data: MAC addresses, serial numbers, pairing secrets, authkeys, session
  keys, GPS tracks and notification contents.

Place your own legally obtained image under `firmware/` locally; it is gitignored.

## Safety and acceptable use

These tools are for research, interoperability and hardware you own or are authorised to test.
Do not use them to bypass ownership/account protections, to attack devices you do not own, or
to redistribute vendor firmware or copyrighted media. Firmware modification can permanently
damage a device. The authors provide no warranty; see the AGPL-3.0 for the full disclaimer.

Nothing here is legal advice. If you plan to distribute firmware, media or a commercial
product based on this work, get your own legal review.
