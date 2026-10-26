#!/usr/bin/env bash
# Phase 0 / Phase 2 environment check. Prints a JSON report; exit 1 if anything is missing.
set -u
GODOT=${GODOT_BIN:-$(command -v godot || true)}
GODOT_VER=$("$GODOT" --version 2>/dev/null || echo "")
NODE=$(command -v node || true)
UVX=$(command -v uvx || true)
GIT=$(command -v git || true)

BLENDER_OK=$(python3 - <<'EOF'
import json, socket
try:
    s = socket.create_connection(("127.0.0.1", 9876), timeout=5)
    s.sendall((json.dumps({"type": "get_addon_info"}) + "\n").encode())
    data = b""
    while True:
        try:
            r = json.loads(data.decode())
            caps = r.get("result", {}).get("capabilities", [])
            print("yes" if "execute_code" in caps else "partial")
            break
        except Exception:
            pass
        chunk = s.recv(65536)
        if not chunk:
            print("partial")
            break
        data += chunk
    s.close()
except Exception:
    print("no")
EOF
)

python3 - "$GODOT" "$GODOT_VER" "$NODE" "$UVX" "$GIT" "$BLENDER_OK" <<'EOF'
import sys
godot, gover, node, uvx, git, blender = sys.argv[1:7]
def ok_ver(v, minv):
    try:
        a = [int(x) for x in v.split(".")[:3]]
    except Exception:
        return False
    return a >= [int(x) for x in minv.split(".")[:3]]
report = {
    "godot_path": godot or "",
    "godot_version": gover,
    "godot_ok": bool(gover) and ok_ver(gover, "4.3"),
    "node": node or "",
    "uvx": uvx or "",
    "git": git or "",
    "blender_mcp": blender,  # yes / partial / no
}
print("ENV_JSON " + __import__("json").dumps(report, indent=2))
sys.exit(0 if (report["godot_ok"] and node and uvx and git and blender == "yes") else 1)
EOF
