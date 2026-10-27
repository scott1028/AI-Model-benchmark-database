#!/usr/bin/env python3
"""Bot test runner: runs the game headless in bot mode and judges the printed progress.

Pass criteria (PLAYBOOK DoD):
  - bot collects 3 coins in order and reaches the endpoint
  - machine-readable progress + win message
  - no script errors in output
"""
import json
import re
import subprocess
import sys
from datetime import datetime
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GODOT = "/home/scott/tools/bin/godot"
SCENE = "res://scenes/main.tscn"
FRAMES = 60 * 240  # hard cap: 240 s of simulation at 60 fps
TAG = "COINRUN_BOT"


def main():
    cmd = [GODOT, "--headless", "--path", str(ROOT / "godot"), "--scene", SCENE,
           "--quit-after", str(FRAMES), "--", "--bot"]
    p = subprocess.run(cmd, capture_output=True, text=True, timeout=900)
    blob = p.stdout + "\n" + p.stderr
    events = []
    for line in blob.splitlines():
        if line.startswith(TAG):
            payload = line[len(TAG):].strip()
            try:
                events.append(json.loads(payload))
            except json.JSONDecodeError:
                events.append({"text": payload})
    errors = [ln for ln in blob.splitlines() if re.search(r"SCRIPT ERROR|Godot Engine exited with code|Failed to", ln)]

    coins = [e for e in events if e.get("event") == "coin"]
    win = next((e for e in events if e.get("event") == "win"), None)
    progress = [e for e in events if e.get("event") in ("progress", "coin", "reach", "start", "stuck")]
    checks = {
        "engine_errors": [],
        "bot_events": len(events),
        "coins_collected": len(coins),
        "coins_in_order": [c.get("index") for c in coins],
        "progress_lines": len(progress),
        "win": bool(win),
        "win_message": (win or {}).get("message", ""),
        "exit_code": p.returncode,
        "elapsed_s": (win or {}).get("elapsed"),
    }
    passed = (not errors and len(coins) == 3 and bool(win) and len(progress) >= 4
              and checks["coins_in_order"] == [c for c in range(3)] and p.returncode == 0)
    out = {
        "test": "bot",
        "pass": passed,
        "ts": datetime.now().isoformat(timespec="seconds"),
        "checks": checks,
        "errors": errors[:20],
        "events": events,
        "raw_tail": blob.splitlines()[-40:],
    }
    dst = ROOT / "pipeline" / "checks" / "bot.json"
    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_text(json.dumps(out, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    print("events=%d coins=%s win=%s errors=%d" % (len(events), checks["coins_in_order"], checks["win"], len(errors)))
    for e in events:
        print("  " + json.dumps(e, ensure_ascii=False))
    for e in errors[:10]:
        print("  ERR " + e)
    print("BOT %s (pipeline/checks/bot.json)" % ("PASS" if passed else "FAIL"))
    return 0 if passed else 1


if __name__ == "__main__":
    sys.exit(main())
