#!/usr/bin/env python3
"""Game screenshot capture: runs the game with --capture <preset> and collects the PNG.

Presets (handled by godot/scripts/capture.gd once Phase 7 exists):
  play     normal gameplay view (third person, camera following the robot)
  overview whole diorama in frame
"""
import argparse
import json
import subprocess
import sys
import time
from datetime import datetime
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GODOT = "/home/scott/tools/bin/godot"
SHOTS = ROOT / "pipeline" / "screenshots"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--scene", default="res://scenes/main.tscn")
    ap.add_argument("--preset", default="play")
    ap.add_argument("--out", default=None)
    ap.add_argument("--frames", type=int, default=240)
    args = ap.parse_args()

    SHOTS.mkdir(parents=True, exist_ok=True)
    stamp = datetime.now().strftime("%Y%m%d%H%M%S")
    out = Path(args.out) if args.out else SHOTS / ("%s-%s.png" % (args.preset, stamp))
    if not out.is_absolute():
        out = ROOT / out
    out.parent.mkdir(parents=True, exist_ok=True)

    cmd = [GODOT, "--path", str(ROOT / "godot"), "--scene", args.scene, "--quit-after", str(args.frames),
           "--", "--capture", args.preset, "--out", str(out)]
    p = subprocess.run(cmd, capture_output=True, text=True, timeout=600)
    blob = p.stdout + "\n" + p.stderr
    ok = out.exists() and out.stat().st_size > 10_000
    errors = [ln for ln in blob.splitlines() if "SCRIPT ERROR" in ln]
    record = {
        "preset": args.preset,
        "scene": args.scene,
        "out": str(out),
        "ok": ok,
        "size": out.stat().st_size if out.exists() else 0,
        "exit_code": p.returncode,
        "errors": errors[:20],
        "capture_lines": [ln for ln in blob.splitlines() if ln.startswith("CAPTURE")],
    }
    (SHOTS / "capture-log.jsonl").open("a", encoding="utf-8").write(json.dumps(record, ensure_ascii=False) + "\n")
    print(json.dumps(record, ensure_ascii=False))
    for ln in blob.splitlines()[-15:]:
        print("  | " + ln)
    return 0 if ok and not errors else 1


if __name__ == "__main__":
    sys.exit(main())
