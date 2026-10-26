#!/usr/bin/env python3
"""Minimal TCP client for the Blender MCP addon (port 9876).

Lets automation run Blender-side stats/export without an agent round trip.
execute_code output is not returned by the addon protocol, so scripts write
result JSON to a file and callers read it back.
"""
import json
import socket
import sys

HOST, PORT = "127.0.0.1", 9876


def send(cmd, timeout=180):
    s = socket.create_connection((HOST, PORT), timeout=timeout)
    try:
        s.sendall((json.dumps(cmd) + "\n").encode())
        data = b""
        while True:
            try:
                return json.loads(data.decode())
            except (UnicodeDecodeError, json.JSONDecodeError):
                pass
            chunk = s.recv(1 << 16)
            if not chunk:
                break
            data += chunk
        return json.loads(data.decode())
    finally:
        s.close()


def run_code(code, timeout=180):
    r = send({"type": "execute_code", "params": {"code": code}}, timeout)
    if r.get("status") != "success":
        raise RuntimeError(f"blender execute_code failed: {r}")
    return r


def run_code_capture(code, out_path, timeout=180):
    """Run code that writes JSON to out_path (abs path), return parsed JSON."""
    run_code(code, timeout)
    with open(out_path) as f:
        return json.load(f)


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "ping":
        print(json.dumps(send({"type": "get_addon_info"}), indent=2)[:400])
