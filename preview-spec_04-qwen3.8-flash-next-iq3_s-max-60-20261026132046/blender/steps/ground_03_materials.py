import bpy
import json
import math

ROOT = "/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046"
exec(open(ROOT + "/blender/steps/lib.py").read())

layout = json.load(open(ROOT + "/godot/data/layout.json"))
W = layout["world"]
ISLE = W["island_size"]
CELLS = 40
STEP = ISLE / CELLS

top = bpy.data.objects["GroundTop"]
me = top.data
mats = [
    ("M_grass_dark", (0.20, 0.42, 0.17)),
    ("M_grass_light", (0.34, 0.55, 0.24)),
    ("M_road", (0.60, 0.45, 0.27)),
    ("M_dirt", (0.50, 0.35, 0.20)),
    ("M_soil", (0.40, 0.28, 0.18)),
    ("M_soil_dark", (0.30, 0.20, 0.13)),
    ("M_bottom", (0.22, 0.15, 0.10)),
    ("M_cliff", (0.50, 0.45, 0.42)),
]
me.materials.clear()
for nm, rgba in mats:
    me.materials.append(get_mat(nm, rgba, 0.9))
idx = {nm: i for i, (nm, _) in enumerate(mats)}

rw = layout["road_width"]
# NOTE: mesh local coords were built in layout coordinates (ground_01), so
# poly.center (local) already matches layout.json; do NOT subtract the
# collection grid offset (that moves object location only, not mesh data).


def seg_d(px, py, a, b):
    ax, ay = a
    bx, by = b
    dx, dy = bx - ax, by - ay
    L2 = dx * dx + dy * dy
    t = 0.0 if L2 == 0 else max(0.0, min(1.0, ((px - ax) * dx + (py - ay) * dy) / L2))
    return math.hypot(px - (ax + t * dx), py - (ay + t * dy))


n_top = CELLS * CELLS
n_ring = 4 * CELLS
for fi, poly in enumerate(me.polygons):
    c = poly.center
    lx = c.x
    ly = c.y
    if fi < n_top:
        # top: road / dirt yard / two grass tones
        on_road = min(seg_d(lx, ly, r["a"], r["b"]) for r in layout["roads"]) < rw * 0.72
        if on_road:
            poly.material_index = idx["M_road"]
            continue
        dirt = False
        for pad in layout["pads"]:
            if pad["id"] == "house" and math.hypot(lx - pad["c"][0], ly - pad["c"][1]) < pad["r"] * 0.8:
                dirt = True
        if dirt:
            poly.material_index = idx["M_dirt"]
            continue
        tone = (
            0.5
            + 0.32 * math.sin(0.19 * lx + 1.1) * math.sin(0.23 * ly + 0.6)
            + 0.22 * math.sin(0.47 * lx + 2.3) * math.sin(0.42 * ly + 1.7)
        )
        poly.material_index = idx["M_grass_light"] if tone > 0.58 else idx["M_grass_dark"]
    elif fi < n_top + 3 * n_ring:
        rel = fi - n_top
        band = rel % 3  # faces appended as (lip, cliff, soil) per rim segment
        kk = rel // 3
        if band == 0:
            poly.material_index = idx["M_grass_dark"]
        elif band == 1:
            poly.material_index = idx["M_cliff"]
        else:
            poly.material_index = idx["M_soil"] if (kk // 2) % 2 == 0 else idx["M_soil_dark"]
    else:
        poly.material_index = idx["M_bottom"]
for p in me.polygons:
    p.use_smooth = False
me.update()
bpy.context.view_layer.update()
dims, vmin, vmax = world_bbox([top])
print("ground mats done; dims:", dims, "z:", vmin[2], vmax[2])
