#!/usr/bin/env python3
"""Physics test runner (PLAYBOOK Phase 8.2).

Runs res://scenes/tests/physics_test.tscn headless; the scene prints
  PHYS_TEST {"id": "...", "measured": x, "unit": "...", "note": "..."}
and every id must exist in specs/physics_thresholds.json with an op/threshold.
Writes pipeline/checks/physics.json.
"""
import json
import subprocess
import sys
from datetime import datetime
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GODOT = "/home/scott/tools/bin/godot"
SCENE = "res://scenes/tests/physics_test.tscn"
THRESHOLDS = ROOT / "specs" / "physics_thresholds.json"
TAG = "PHYS_TEST"


def judge(th, measured):
    op, a, b = th["op"], th["value"], th.get("value_max")
    if op == "min":
        return measured >= a
    if op == "max":
        return measured <= a
    if op == "range":
        return a <= measured <= b
    if op == "approx":
        return abs(measured - a) <= (b or 0)
    return False


def main():
    if not THRESHOLDS.exists():
        print("FAIL: thresholds missing at %s" % THRESHOLDS)
        return 1
    spec = json.loads(THRESHOLDS.read_text(encoding="utf-8"))
    cmd = [GODOT, "--headless", "--path", str(ROOT / "godot"), "--scene", SCENE, "--quit-after", "20000", "--", "--phys-test"]
    p = subprocess.run(cmd, capture_output=True, text=True, timeout=900)
    blob = p.stdout + "\n" + p.stderr
    rows = []
    for line in blob.splitlines():
        if line.startswith(TAG):
            try:
                rows.append(json.loads(line[len(TAG):].strip()))
            except json.JSONDecodeError:
                rows.append({"id": "<unparsed>", "measured": None, "raw": line})
    errors = [ln for ln in blob.splitlines() if "SCRIPT ERROR" in ln]

    measured = {}
    for r in rows:
        measured[r.get("id")] = r
    results = []
    for th in spec["thresholds"]:
        tid = th["id"]
        row = measured.get(tid)
        ok = False
        val = None
        if row is not None and isinstance(row.get("measured"), (int, float)):
            val = row["measured"]
            ok = judge(th, val)
        results.append({"id": tid, "ok": ok, "measured": val, "threshold": th,
                        "note": (row or {}).get("note", "no measurement reported")})
    missing = [k for k in measured if k not in {t["id"] for t in spec["thresholds"]}]
    passed = all(r["ok"] for r in results) and not errors and not missing
    out = {
        "test": "physics",
        "pass": passed,
        "ts": datetime.now().isoformat(timespec="seconds"),
        "results": results,
        "unexpected_ids": missing,
        "errors": errors[:20],
        "raw_rows": rows,
        "raw_tail": blob.splitlines()[-30:],
    }
    dst = ROOT / "pipeline" / "checks" / "physics.json"
    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_text(json.dumps(out, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    for r in results:
        t = r["threshold"]
        print("%s %-22s measured=%s  (%s %s%s)" % ("[v]" if r["ok"] else "[x]", r["id"], r["measured"], t["op"], t["value"],
                                                   ("..%s" % t["value_max"]) if t.get("value_max") is not None else ""))
    for e in errors[:10]:
        print("  ERR " + e)
    print("PHYSICS %s (pipeline/checks/physics.json)" % ("PASS" if passed else "FAIL"))
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
