import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "src"))
from cmf_watch_utils import cmf_watch as implementation

if __name__ == "__main__":
    implementation.main()
else:
    sys.modules[__name__] = implementation
