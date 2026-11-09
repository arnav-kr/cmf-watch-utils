#!/usr/bin/env bash
set -euo pipefail

REPO_URL="${CMF_WATCH_UTILS_REPO:-${REPO_URL:-https://github.com/arnav-kr/cmf-watch-utils.git}}"
TARGET="${1:-cmf-watch-utils}"
FIRMWARE="${2:-}"

if [[ "$FIRMWARE" != "" && "$FIRMWARE" != "--firmware" ]] || (( $# > 2 )); then
  echo "Usage: bash scripts/install.sh [directory] [--firmware]" >&2
  exit 1
fi

command -v uv >/dev/null || { echo "Install uv: https://docs.astral.sh/uv/getting-started/installation/" >&2; exit 1; }
command -v git >/dev/null || { echo "Install Git first." >&2; exit 1; }
if [[ "$FIRMWARE" == "--firmware" ]] && ! git lfs version >/dev/null 2>&1; then
  echo "Install Git LFS to use --firmware." >&2
  exit 1
fi

if [ ! -d "$TARGET/.git" ]; then
  GIT_LFS_SKIP_SMUDGE=1 git clone "$REPO_URL" "$TARGET"
fi

cd "$TARGET"
uv sync --locked

if [[ "$FIRMWARE" == "--firmware" ]]; then
  git lfs install --local
  git lfs pull --include="dumps/binaries/firmware/JX402_V1.0.0.73.bin" --exclude=""
fi

printf '\nWorkspace installed at %s\n' "$PWD"
echo 'Open that directory and run: uv run cmf-watch-utils emu profiles'
echo 'Setup guide: https://cmf.arnv.dev/getting-started/setup/'
