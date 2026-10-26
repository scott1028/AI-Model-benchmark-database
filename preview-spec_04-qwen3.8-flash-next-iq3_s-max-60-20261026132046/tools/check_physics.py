#!/usr/bin/env python3
"""Compare PhysRunner output lines against specs/physics_tests.json.

Usage: check_physics.py <phys_log_path>
Writes pipeline/checks/physics_check.json and prints a summary.
Exit 0 when every test passes.
"""
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SPEC = ROOT / "specs" / "physics_tests.json"
OUT = ROOT / "pipeline" / "checks" / "physics_check.json"

LINE = re.compile(r"^PHYS (\S+) (\S+)=(-?\d+(?:\.\d+)?)\s*$")


def main() -> int:
    log_path = Path(sys.argv[1] if len(sys.argv) > 1 else ROOT / "pipeline" / "checks" / "phys_result.txt")
    measured = {}
    for line in log_path.read_text(encoding="utf-8", errors="replace").splitlines():
        m = LINE.match(line.strip())
        if m:
            measured[f"{m.group(1)}.{m.group(2)}"] = float(m.group(3))

    spec = json.loads(SPEC.read_text(encoding="utf-8"))
    results = []
    ok = True
    for t in spec["tests"]:
        key = f"{t['scenario']}.{t['metric']}"
        val = measured.get(key)
        if val is None:
            status = "MISSING"
        elif val < t["min"] or val > t["max"]:
            status = "FAIL"
        else:
            status = "PASS"
        if status != "PASS":
            ok = False
        results.append({**t, "measured": val, "status": status})

    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text(json.dumps({"pass": ok, "results": results}, indent=2) + "\n", encoding="utf-8")
    for r in results:
        print(f"{r['status']:7s} {r['scenario']}.{r['metric']} measured={r['measured']} "
              f"range=[{r['min']},{r['max']}]")
    print("PHYSICS_CHECK", "PASS" if ok else "FAIL", "->", OUT)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
