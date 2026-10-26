import bpy
import bmesh
import json
import math
import os

ROOT = "/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046"
exec(open(ROOT + "/blender/steps/lib.py").read())

layout = json.load(open(ROOT + "/godot/data/layout.json"))
W = layout["world"]
AMP = W["terrain_amp"]
ISLE = W["island_size"]

CELLS = 40
STEP = ISLE / CELLS  # 1.1


def base_h(x, y):
    return 0.7 * (
        0.35 * math.sin(0.28 * x + 0.9)
        + 0.28 * math.sin(0.23 * y + 2.1)
        + 0.18 * math.sin(0.19 * (x + y) + 0.5)
    )


def seg_dist(px, py, a, b):
    ax, ay = a
    bx, by = b
    dx, dy = bx - ax, by - ay
    L2 = dx * dx + dy * dy
    t = 0.0 if L2 == 0 else max(0.0, min(1.0, ((px - ax) * dx + (py - ay) * dy) / L2))
    cx, cy = ax + t * dx, ay + t * dy
    return math.hypot(px - cx, py - cy), base_h(cx, cy)


def smooth(t):
    t = max(0.0, min(1.0, t))
    return t * t * (3 - 2 * t)


# ---- height field (roads flattened, then pads flattened) ----
def terrain_h(x, y):
    h = base_h(x, y)
    rw = layout["road_width"]
    for road in layout["roads"]:
        d, roadlevel = seg_dist(x, y, road["a"], road["b"])
        if d < rw:
            w = smooth(1.0 - d / rw)
            h = h * (1 - w) + roadlevel * w
    for pad in layout["pads"]:
        px, py = pad["c"]
        dist = math.hypot(x - px, y - py)
        r = pad["r"]
        if dist < r:
            # level to pad-center height (roads first)
            w = smooth((r - dist) / 1.2)
            target = terrain_h_roads(px, py)
            h = h * (1 - w) + target * w
    return h


_roads_only = {}


def terrain_h_roads(x, y):
    h = base_h(x, y)
    rw = layout["road_width"]
    for road in layout["roads"]:
        d, roadlevel = seg_dist(x, y, road["a"], road["b"])
        if d < rw:
            w = smooth(1.0 - d / rw)
            h = h * (1 - w) + roadlevel * w
    return h


col = asset_collection("ground")
bm = bmesh.new()
grid = {}
half = ISLE / 2
for i in range(CELLS + 1):
    for j in range(CELLS + 1):
        x = -half + i * STEP
        y = -half + j * STEP
        grid[(i, j)] = bm.verts.new((x, y, terrain_h(x, y)))
for i in range(CELLS):
    for j in range(CELLS):
        bm.faces.new([grid[(i, j)], grid[(i + 1, j)], grid[(i + 1, j + 1)], grid[(i, j + 1)]])
me = bpy.data.meshes.new("GroundTop")
bm.to_mesh(me)
bm.free()
top = bpy.data.objects.new("GroundTop", me)
link_to(top, col)
top.data.materials.append(get_mat("M_dummy", (0.5, 0.5, 0.5)))  # slot placeholder

# heights lookup for game-side placement
hdata = {}
for x_i in range(int(ISLE / STEP) + 1):
    xs = -half + x_i * STEP
    hdata.setdefault("cells", []).append([round(terrain_h(xs, -half + j * STEP), 3) for j in range(CELLS + 1)])
hdata["step"] = STEP
hdata["min"] = -half
json.dump(hdata, open(ROOT + "/godot/data/heights.json", "w"))
bpy.ops.wm.save_as_mainfile(filepath=ROOT + "/blender/coin_run.blend")
print("ground top built:", len(grid), "verts; heights.json written")
