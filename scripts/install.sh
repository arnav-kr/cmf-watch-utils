#!/usr/bin/env bash
# Clone CMF Watch Utils, install the workspace and fetch the published firmware dumps.
set -euo pipefail

REPO_URL="${REPO_URL:-https://github.com/arnav-kr/cmf-watch-utils.git}"
TARGET="${1:-cmf-watch-utils}"

if ! command -v uv >/dev/null 2>&1; then
  echo "uv is required: https://docs.astral.sh/uv/getting-started/installation/" >&2
  exit 1
fi

if ! command -v git >/dev/null 2>&1; then
  echo "git is required" >&2
  exit 1
fi

if command -v git-lfs >/dev/null 2>&1 || git lfs version >/dev/null 2>&1; then
  git lfs install
fi

if [ ! -d "$TARGET/.git" ]; then
  git clone --recurse-submodules "$REPO_URL" "$TARGET"
fi

cd "$TARGET"
uv sync

if git lfs version >/dev/null 2>&1; then
  git lfs pull --include="dumps/binaries/**" --exclude=""
fi

cat <<'EOF'

Ready. Useful commands:

  uv run fwkit info   dumps/binaries/firmware/JX402_V1.0.0.73.bin
  uv run fwkit verify dumps/binaries/firmware/JX402_V1.0.73.bin
  uv run python tools/cmf_watch.py --help
  uv run python tooling/tests/selftest.py dumps/binaries/firmware/JX402_V1.0.0.73.bin

Documentation: https://cmf.arnv.dev/
EOF
