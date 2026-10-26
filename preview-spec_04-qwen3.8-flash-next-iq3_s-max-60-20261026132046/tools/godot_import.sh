#!/usr/bin/env bash
# Headless Godot asset re-import.
set -eu
P=$(cd "$(dirname "$0")/.." && pwd)
GODOT=${GODOT_BIN:-godot}
"$GODOT" --headless --path "$P/godot" --import >/dev/null 2>&1 || "$GODOT" --headless --path "$P/godot" --editor --quit >/dev/null 2>&1
echo "import done: $P/godot"
