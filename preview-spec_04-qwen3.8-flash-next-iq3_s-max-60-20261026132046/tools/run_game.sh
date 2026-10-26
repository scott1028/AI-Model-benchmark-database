#!/usr/bin/env bash
# Run the game with a mode flag and capture stdout for the parsers.
# Usage: run_game.sh <bot|phys|shots> [output-log-path]
set -eu
P=$(cd "$(dirname "$0")/.." && pwd)
GODOT=${GODOT_BIN:-godot}
MODE=${1:?mode}
OUT=${2:-/tmp/opencode/coinrun_$MODE.log}
TIMEOUT=${TIMEOUT:-120}
export RESULT_DIR="$P/pipeline/checks"
export SHOT_DIR="$P/pipeline/screenshots"
mkdir -p "$SHOT_DIR"
timeout "$TIMEOUT" "$GODOT" --path "$P/godot" -- --mode "$MODE" >"$OUT" 2>&1 || true
grep -E '^(BOT_|PHYS|SHOT_|GAME_ERROR|SCRIPT ERROR)' "$OUT" || true
