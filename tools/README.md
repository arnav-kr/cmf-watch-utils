# tools

BLE and shell tools for the CMF Watch Pro 2. They are run as scripts from the workspace root:

```bash
uv run python tools/cmf_watch.py --help
uv run python tools/ota_client.py --help
```

| Tool | Purpose |
|---|---|
| `cmf_watch.py` | CLI: `scan`, `pair`, `info`, `at`, `sync`, `workouts`, `specs`, `wf-dials`, `wf-flash`, `wf-delete`, `ota-init`, `ota-state`, `ota-flash`, `ota-push`, `ota-run` |
| `ota_client.py` | Full AOTA OTA client (INIT1/INIT2, pull requests, end ack) |
| `spp_ota.py` | SPP / RFCOMM OTA experiments |
| `rfcomm.py` | RFCOMM helper |
| `maputil.py` | address and offset helpers |

## Local state

`tools/.watch_state.json` holds the pairing secret and authkey. It is gitignored and must never
be published. If the watch is reset, or the factory-test menu clears app data, re-run
`cmf_watch.py pair`: a stale authkey still yields an `A052` reply but never reaches the OTA
data phase.

## Safety

Read-only commands (`scan`, `info`, `at`, `ota-state`) are safe. Commands that write to the
watch (`wf-flash`, `wf-delete`, `ota-*` writes) should only be run with explicit approval.
