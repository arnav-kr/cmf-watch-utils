---
title: Prompts & working agreements
description: Copy-paste prompts that keep an AI agent inside the project's safety and verification rules.
---

The blocks below are reusable prompts for a coding agent operating in this repository. Each
block is quoted material, reproduced as written; the surrounding notes state what each prompt
covers.

## System preamble

Scope, device-write approval and data handling:

> You are working in `cmf-watch-utils`, an unofficial CMF Watch Pro 2 research repo. Read
> `AGENTS.md` and `site/src/content/docs/ai/context.md` first. Never flash, upload, delete or
> change device settings without explicit approval. Never commit firmware, vendor assets,
> animation/watch-face media, MAC/serial/secret/authkey data, or personal logs. Prove format
> work with byte-identical round-trips. Prefer read-only commands. Explain risky commands
> before running them.

## Task: parse or edit a container

Container inspection and editing, with a round-trip proof ahead of any change:

> Inspect `<file>` with the appropriate `fwkit` command. Identify magic, header fields and
> record sizes. Extract the smallest entry, decode it, re-encode it and compare byte-for-byte.
> Only then make the requested edit. Report the exact command, byte sizes and any fields you
> could not explain. Do not modify firmware under `firmware/` in place.

## Task: add a CLI command

Addition of a CLI subcommand following the existing parser/cmd_* pattern:

> Add `<name>` to `tooling/src/fwkit/cli.py` following the existing parser/cmd_* pattern.
> Keep output stable and machine-readable where the command already is. Add a round-trip test
> to `tooling/tests/selftest.py` that does not require redistributing vendor files. Run the
> selftest against the local firmware image and report results.

## Task: BLE debugging

BLE investigation, read-only until a write is approved:

> Use only read-only BLE commands until I approve a write. Start with `info` and
> `ota-state --duration 5`. If pairing is stale, propose `pair` but do not run it without
> approval. For OTA, first run `tools/ota_client.py --stop-after-a040 -v`; verify INIT2 goes to
> the OTA characteristic and that `A040` arrives. Never send INIT2 on `CMD_W` first.

## Task: docs

Documentation edits under the Astro content tree:

> Edit the Astro content under `site/src/content/docs/`. Keep frontmatter `title` and
> `description`. Use relative links for internal pages and include the `/watch-utils/`
> base only for splash/absolute links. Do not include real device identifiers. Run
> `pnpm build` in `site/` and fix broken links.

## Review checklist

- [ ] No secrets, MAC, serial, authkeys or personal logs.
- [ ] No firmware, vendor resources or copyrighted media added.
- [ ] Format change has a round-trip proof.
- [ ] I/O change has a dry-run path.
- [ ] OTA order unchanged: `0x9052`, `0x9040` on OTA, `0x9042` pulls, `0x9041 A5` end.
- [ ] `pnpm build` and the relevant Python checks pass.
