from pathlib import Path

_SOURCE_ROOT = Path(__file__).resolve().parents[2]
REPO_ROOT = _SOURCE_ROOT if (_SOURCE_ROOT / "tools" / "cmf_watch.py").is_file() else Path.cwd()
TOOLS_DIR = REPO_ROOT / "tools"
