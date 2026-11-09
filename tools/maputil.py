import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "src"))
from cmf_watch_utils import maputil as implementation

sys.modules[__name__] = implementation
