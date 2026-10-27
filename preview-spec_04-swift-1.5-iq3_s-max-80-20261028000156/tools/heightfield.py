"""Shared height field / layout math for Coin Run (PLAYBOOK v2.1 Phase 5-6).

Imported by:
  - tools/gen_layout.py        (author + validate godot/data/layout.json)
  - tools/blender_terrain.py   (runs inside Blender; builds the terrain mesh)
  - tools/check_layout.py      (bot-route / pad / placement validation)

Coordinate convention (AGENTS.md §2): all layout data is in GODOT space (x, z), y = up.
Blender side: blender_x = godot_x, blender_y = -godot_z, height = godot_y.
"""
import json
import math
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LAYOUT_PATH = ROOT / "godot" / "data" / "layout.json"

WALL_HALF = 20.9          # wall centre line; playable area is inside +/- 20.5
PLAY_HALF = 20.5
TERRAIN_HALF = 22.0       # diorama slab footprint (+/- 22 m)
ROAD_HALF_WIDTH = 1.9
PAD_BLEND = 2.2           # metres over which terrain eases into a flat pad
MAX_PAD_SLOPE = 0.06      # metres of height change per metre inside a pad core


def h(x, z):
    """Base undulation (metres), no pads/roads applied. Amplitude ~0.6 m."""
    n1 = math.sin(x * 0.30 + 1.3) * math.cos(z * 0.26 - 0.7)
    n2 = 0.5 * math.sin(x * 0.75 - 2.1) * math.sin(z * 0.62 + 0.4)
    n3 = 0.28 * math.sin(x * 1.31 + z * 0.97 + 2.0)
    v = 0.40 * n1 + 0.26 * n2 + 0.16 * n3
    return v


def seg_dist(px, pz, a, b):
    ax, az = a
    bx, bz = b
    dx, dz = bx - ax, bz - az
    len2 = dx * dx + dz * dz
    t = 0.0 if len2 == 0 else ((px - ax) * dx + (pz - az) * dz) / len2
    t = max(0.0, min(1.0, t))
    cx, cz = ax + t * dx, az + t * dz
    return math.hypot(px - cx, pz - cz)


def road_dist(x, z, roads):
    best = 1e9
    for poly in roads:
        for i in range(len(poly) - 1):
            best = min(best, seg_dist(x, z, poly[i], poly[i + 1]))
    return best


def pad_weight(x, z, pads):
    """1.0 inside a pad core, easing to 0 PAD_BLEND metres outside its edge."""
    w = 0.0
    for p in pads:
        hw, hd = p["size"][0] / 2.0, p["size"][1] / 2.0
        dx = max(0.0, abs(x - p["center"][0]) - hw)
        dz = max(0.0, abs(z - p["center"][1]) - hd)
        d = math.hypot(dx, dz)
        if d <= 0.0:
            w = max(w, 1.0)
        else:
            w = max(w, max(0.0, 1.0 - d / PAD_BLEND))
    return w


def terrain_height(x, z, layout):
    """Final surface height in Godot metres (y up)."""
    pads = layout["pads"]
    base = h(x, z)
    w = pad_weight(x, z, pads)
    pad_h = 0.0
    for p in pads:
        hw, hd = p["size"][0] / 2.0, p["size"][1] / 2.0
        if abs(x - p["center"][0]) <= hw and abs(z - p["center"][1]) <= hd:
            pad_h = p.get("height", 0.0)
            break
    surf = base * (1.0 - w) + pad_h * w
    rd = road_dist(x, z, layout["roads"])
    if rd < ROAD_HALF_WIDTH:
        # roads: ease toward the local pad height, keep a hard surface
        t = 1.0 - rd / ROAD_HALF_WIDTH
        surf = surf * (1.0 - 0.72 * t) + pad_h * (0.72 * t)
    return surf


def sample_grid(layout, half=TERRAIN_HALF, step=1.0):
    """Row-major height grid; row 0 is at z = -half, column 0 at x = -half."""
    n = int(round(2 * half / step)) + 1
    rows = []
    for j in range(n):
        z = -half + j * step
        rows.append([round(terrain_height(-half + i * step, z, layout), 4) for i in range(n)])
    return {"size": n, "step": step, "origin": [-half, -half], "axis_note": "row=z(-), col=x(+); height = godot y", "rows": rows}


def lookup_height(grid, x, z):
    n, step, ox, oz = grid["size"], grid["step"], grid["origin"][0], grid["origin"][1]
    i = (x - ox) / step
    j = (z - oz) / step
    i0, j0 = int(math.floor(i)), int(math.floor(j))
    i0 = max(0, min(n - 1, i0))
    j0 = max(0, min(n - 1, j0))
    i1, j1 = min(n - 1, i0 + 1), min(n - 1, j0 + 1)
    fi, fj = i - i0, j - j0
    a = grid["rows"][j0][i0] * (1 - fi) + grid["rows"][j0][i1] * fi
    b = grid["rows"][j1][i0] * (1 - fi) + grid["rows"][j1][i1] * fi
    return a * (1 - fj) + b * fj


def road_material_weight(x, z, layout):
    """0..1 mask used to pick grass/road/plaza material on the terrain surface."""
    rd = road_dist(x, z, layout["roads"])
    w = 1.0 - min(1.0, max(0.0, (rd - ROAD_HALF_WIDTH * 0.55) / (ROAD_HALF_WIDTH * 0.45)))
    for p in layout["pads"]:
        if p.get("kind") != "plaza":
            continue
        hw, hd = p["size"][0] / 2.0, p["size"][1] / 2.0
        d = max(max(0.0, abs(x - p["center"][0]) - hw), max(0.0, abs(z - p["center"][1]) - hd))
        if d <= 0.0:
            w = 1.0
    return w


def pad_rect_distance(x, z, p):
    """Distance from (x, z) to a pad rectangle (0 inside)."""
    hw, hd = p["size"][0] / 2.0, p["size"][1] / 2.0
    return math.hypot(max(0.0, abs(x - p["center"][0]) - hw), max(0.0, abs(z - p["center"][1]) - hd))


def surface_material(x, z, layout, rnd):
    """Per-cell surface material: plaza paving, dirt road with a soft shoulder, else grass shades.

    Shared by tools/blender_terrain.py (per-face materials on the mesh) and
    tools/gen_terrain_texture.py (the colour map Godot samples), so the mesh and the
    baked colours follow one rule.
    """
    plaza = min([pad_rect_distance(x, z, p) for p in layout["pads"] if p["kind"] == "plaza"] + [99.0])
    if plaza < 0.4:
        return "terrain_plaza"
    if plaza < 1.2:
        return "terrain_road"
    w = road_material_weight(x, z, layout)
    if w > 0.62:
        return "terrain_road"
    if w > 0.28:
        return "terrain_grass_dark"
    n = (0.5 + 0.42 * math.sin(x * 0.28 + 1.1) * math.cos(z * 0.24 - 0.6)
         + 0.26 * math.sin(x * 0.63 - 0.4) * math.sin(z * 0.57 + 1.3) + 0.06 * (rnd.random() - 0.5))
    if n < 0.34:
        return "terrain_grass_dark"
    if n < 0.66:
        return "terrain_grass_mid"
    return "terrain_grass_light"


def load_layout(path=LAYOUT_PATH):
    return json.loads(Path(path).read_text(encoding="utf-8"))
