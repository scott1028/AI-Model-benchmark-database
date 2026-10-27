"""Build the Coin Run diorama slab (asset id: terrain) inside Blender.

Run from Blender MCP:
    exec(open(ROOT + "/tools/blender_terrain.py").read(), globals())

What it does (PLAYBOOK Phase 5, appendix C):
  - samples the shared height field on a 1 m grid over the +/-22 m slab
  - top surface: quad grid, per-face material = 3 grass shades / road & plaza paving
  - side skirt: 3 earth bands (topsoil, clay, rock) so the slab reads like a model on a table
  - flat bottom, origin = bottom centre (AGENTS.md §2)
  - writes godot/data/heightmap.json (WorldBuilder seats props on it and builds the collision)
  - exports godot/assets/terrain/terrain.glb through tools/blender_export.py

Material slots follow MATERIAL_ORDER; the same order is appended to the mesh.
"""
import json
import math
import random
import runpy
import sys

import bpy
import bmesh

ROOT = globals().get("ROOT", "/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-swift-1.5-iq3_s-max-20261028000156")
sys.path.insert(0, ROOT + "/tools")
import heightfield as hf  # noqa: E402

SPEC = json.loads((hf.ROOT / "specs" / "terrain.json").read_text(encoding="utf-8"))
BUILD = SPEC["build"]
HALF, STEP, SKIRT = BUILD["half_m"], BUILD["step_m"], BUILD["skirt_m"]
BANDS = BUILD["bands"]

MATERIAL_ORDER = ("terrain_grass_light", "terrain_grass_mid", "terrain_grass_dark", "terrain_road",
                  "terrain_plaza", "terrain_soil_top", "terrain_soil_mid", "terrain_rock")
BAND_MATERIALS = ("terrain_soil_top", "terrain_soil_mid", "terrain_rock")
IDX = {name: i for i, name in enumerate(MATERIAL_ORDER)}
MATS = {}


def surface_material(x, z, layout, rnd):
    """Per-cell material — shared rule lives in heightfield.surface_material (single source)."""
    return hf.surface_material(x, z, layout, rnd)


def border_ring(n):
    ring = [(i, 0) for i in range(n - 1)]
    ring += [(n - 1, j) for j in range(n - 1)]
    ring += [(i, n - 1) for i in range(n - 1, 0, -1)]
    ring += [(0, j) for j in range(n - 1, 0, -1)]
    return ring


def inset(i, j, n, amount):
    """Move a border grid point inward, along its dominant axis (square slab)."""
    x = -HALF + i * STEP
    z = -HALF + j * STEP
    if abs(x) >= abs(z):
        return x - math.copysign(amount, x), z
    return x, z - math.copysign(amount, z)


def build_mesh(layout, grid):
    n = grid["size"]
    rnd = random.Random(11)
    h_at = lambda i, j: hf.terrain_height(-HALF + i * STEP, -HALF + j * STEP, layout)

    me = bpy.data.meshes.new("terrain_slab")
    bm = bmesh.new()

    top = [[bm.verts.new((-HALF + i * STEP, -(-HALF + j * STEP), h_at(i, j))) for i in range(n)] for j in range(n)]

    ring = border_ring(n)
    prev = [(top[j][i], (i, j)) for (i, j) in ring]
    for (inset_m, depth), mat_name in zip(BANDS, BAND_MATERIALS):
        cur = []
        for (i, j) in ring:
            x, z = inset(i, j, n, inset_m)
            cur.append((bm.verts.new((x, -z, h_at(i, j) - depth)), (i, j)))
        k_len = len(prev)
        for k in range(k_len):
            a = prev[k][0]
            b = prev[(k + 1) % k_len][0]
            c = cur[(k + 1) % k_len][0]
            d = cur[k][0]
            f = bm.faces.new((a, b, c, d))
            f.material_index = IDX[mat_name]
        prev = cur

    bottom = bm.faces.new([v for v, _ in prev])
    bottom.material_index = IDX["terrain_rock"]

    for j in range(n - 1):
        for i in range(n - 1):
            f = bm.faces.new((top[j][i], top[j][i + 1], top[j + 1][i + 1], top[j + 1][i]))
            f.material_index = IDX[surface_material(-HALF + (i + 0.5) * STEP, -HALF + (j + 0.5) * STEP, layout, rnd)]

    # every face points away from the slab centre (grid winding alone is not reliable here)
    mid_z = -SKIRT * 0.5
    from mathutils import Vector
    center = Vector((0.0, 0.0, mid_z))
    for f in bm.faces:
        f.normal_update()
        if (f.calc_center_median() - center).normalized().dot(f.normal) < 0:
            f.normal_flip()

    bm.normal_update()
    bm.to_mesh(me)
    bm.free()
    return me


def main():
    layout = hf.load_layout()
    grid = hf.sample_grid(layout, half=HALF, step=STEP)

    new_scene("terrain")
    colors = BUILD["colors"]
    hexes = {
        "terrain_grass_light": colors["grass_light"], "terrain_grass_mid": colors["grass_mid"],
        "terrain_grass_dark": colors["grass_dark"], "terrain_road": colors["road"],
        "terrain_plaza": colors["plaza"], "terrain_soil_top": colors["soil_top"],
        "terrain_soil_mid": colors["soil_mid"], "terrain_rock": colors["rock_side"],
    }
    for name in MATERIAL_ORDER:
        MATS[name] = mat(name, hexes[name], rough=0.95)

    me = build_mesh(layout, grid)
    for name in MATERIAL_ORDER:
        me.materials.append(MATS[name])

    obj = bpy.data.objects.new("terrain_slab", me)
    bpy.context.scene.collection.objects.link(obj)
    obj.parent = CTX["root"]
    CTX["parts"].append(obj)

    shift = set_origin_bottom()
    rep = report(tuple(m["name"] for m in SPEC["materials"]))
    rep["node_y"] = round(shift["dz"], 4)
    zs = [v for row in grid["rows"] for v in row]
    rep["height_range_m"] = [round(min(zs), 3), round(max(zs), 3)]
    rep["grid"] = {"size": grid["size"], "step_m": grid["step"], "half_m": HALF}

    grid["node_y"] = round(shift["dz"], 4)
    grid["note"] = "rows[j][i] at x = origin[0] + i*step, z_godot = origin[1] + j*step; value = godot y"
    out = hf.ROOT / "godot" / "data" / "heightmap.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(grid) + "\n", encoding="utf-8")
    rep["heightmap"] = str(out.relative_to(hf.ROOT))

    runpy.run_path(ROOT + "/tools/blender_export.py", init_globals={"ROOT": ROOT, "ASSET_ID": "terrain"})
    print("TERRAIN " + json.dumps(rep, ensure_ascii=False))


main()
