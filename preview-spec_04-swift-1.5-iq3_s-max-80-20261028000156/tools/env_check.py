#!/usr/bin/env python3
"""Environment check for the Coin Run pipeline (PLAYBOOK v2.1 Phase 1.5 / 2.1).

Usage: tools/env_check.py [--json]
Exit 0 when all required checks pass, 1 otherwise.
"""
import json
import os
import shutil
import socket
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ENV_FILE = ROOT / "pipeline" / "env.json"

BLENDER_MCP_PORT = 9876
REQUIRED = ["git", "node", "npx", "uvx", "uv"]


def run(cmd, timeout=25):
    try:
        p = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        return (p.stdout + p.stderr).strip().splitlines()
    except Exception as exc:  # missing binary, timeout, ...
        return ["<error: %s>" % exc]


def first_line(lines):
    return lines[0] if lines else ""


def find_binary(name, extra_paths):
    found = shutil.which(name)
    for p in extra_paths:
        pp = Path(p).expanduser()
        if pp.is_file() and os.access(pp, os.X_OK):
            found = str(pp)
    return found


def blender_binary():
    env = os.environ.get("COINRUN_BLENDER")
    if env:
        return env
    for cand in ("/home/scott/tools/blender/blender", "blender"):
        p = shutil.which(cand) or (cand if Path(cand).is_file() else None)
        if p and os.access(p, os.X_OK):
            return p
    return None


def godot_binary():
    env = os.environ.get("GODOT_PATH")
    if env and Path(env).is_file():
        return env
    return shutil.which("godot") or shutil.which("godot4")


def port_listeners(port):
    out = run(["ss", "-ltnp"])
    return [line for line in out if (":%d " % port) in line or (":%d\t" % port) in line]


def collect():
    checks = []

    def add(name, ok, value, note=""):
        checks.append({"name": name, "ok": bool(ok), "value": str(value), "note": note})

    g = godot_binary()
    ver = first_line(run([g, "--version"])) if g else ""
    ok_g = bool(g and ver.count(".") >= 2 and int(ver.split(".")[0]) >= 4 and int(ver.split(".")[1]) >= 3)
    add("godot>=4.3", ok_g, g, ver)

    b = blender_binary()
    bver = first_line(run([b, "--version"], 60)) if b else ""
    add("blender", bool(b), b, bver)

    for tool in REQUIRED:
        p = shutil.which(tool)
        if tool == "node":
            v = first_line(run([p, "--version"])) if p else ""
            major = int(v.lstrip("v").split(".")[0]) if v and v.lstrip("v").split(".")[0].isdigit() else 0
            add("node>=18", major >= 18, p, v)
        else:
            v = first_line(run([p, "--version"])) if p else ""
            add(tool, bool(p), p, v)

    listeners = port_listeners(BLENDER_MCP_PORT)
    owner = "blender" if any("blender" in ln.lower() for ln in listeners) else ""
    sock_ok = False
    if listeners:
        try:
            with socket.create_connection(("127.0.0.1", BLENDER_MCP_PORT), timeout=3):
                sock_ok = True
        except Exception as exc:
            sock_ok = False
            listeners.append("connect failed: %s" % exc)
    add(
        "blender-mcp-port:%d" % BLENDER_MCP_PORT,
        sock_ok and bool(owner),
        (owner or "no owner") + " | " + (listeners[0] if listeners else "not listening"),
        "Blender 端 MCP server 需已 Start MCP Server，且埠由 blender 持有（Blender Lab MCP 會佔用同一埠）",
    )

    add("godot-mcp(GODOT_PATH resolvable)", ok_g, g, "godot-mcp 用 GODOT_PATH 啟動 headless Godot")
    add("display(for screenshots)", os.environ.get("DISPLAY") or os.environ.get("WAYLAND_DISPLAY"), os.environ.get("XDG_SESSION_TYPE", ""),
        "無顯示器時截圖改用 --write-movie")

    passed = all(c["ok"] for c in checks)
    return {"pass": passed, "checks": checks}


def main():
    result = collect()
    ENV_FILE.parent.mkdir(parents=True, exist_ok=True)
    ENV_FILE.write_text(json.dumps(result, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    if "--json" in sys.argv:
        print(json.dumps(result, ensure_ascii=False))
    else:
        for c in result["checks"]:
            print("%s %-32s %s  %s" % ("[v]" if c["ok"] else "[x]", c["name"], c["value"], c["note"]))
        print("PASS" if result["pass"] else "FAIL")
    return 0 if result["pass"] else 1


if __name__ == "__main__":
    sys.exit(main())
