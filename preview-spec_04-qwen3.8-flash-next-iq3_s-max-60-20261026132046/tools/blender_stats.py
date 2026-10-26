#!/usr/bin/env python3
"""Blender-side stats for one asset collection (via addon TCP).

Writes pipeline/checks/<name>_blender.json: world bbox of visual meshes,
tri counts, animation action names, col_* presence.
"""
import json
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
import blender_client

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

CODE_TMPL = r'''
import bpy, json, mathutils

name = {name!r}
out = {out!r}

def tri_count(o):
    t = 0
    for p in o.data.polygons:
        t += max(len(p.vertices) - 2, 0)
    return t

res = {{"asset": name, "found": False}}
col = bpy.data.collections.get("A_" + name)
if col:
    deps = bpy.context.evaluated_depsgraph_get()
    vmin = mathutils.Vector((1e9, 1e9, 1e9))
    vmax = mathutils.Vector((-1e9, -1e9, -1e9))
    tris = 0
    col_tris = 0
    ncol = 0
    for o in col.all_objects:
        if o.type != "MESH":
            continue
        ev = o.evaluated_get(deps)
        me = ev.to_mesh()
        if me is None:
            continue
        is_col = o.name.startswith("col_")
        for v in me.vertices:
            w = ev.matrix_world @ v.co
            for i in range(3):
                vmin[i] = min(vmin[i], w[i])
                vmax[i] = max(vmax[i], w[i])
        if is_col:
            col_tris += tri_count(ev)
            ncol += 1
        else:
            tris += tri_count(ev)
        ev.to_mesh_clear()
    actions = set()
    for o in col.all_objects:
        ad = o.animation_data
        if ad and ad.action:
            actions.add(ad.action.name)
    res = {{
        "asset": name, "found": True,
        "bbox_min": [round(x, 4) for x in vmin],
        "bbox_max": [round(x, 4) for x in vmax],
        "dims": [round(vmax[i] - vmin[i], 4) for i in range(3)],
        "tris": tris, "col_tris": col_tris, "col_nodes": ncol,
        "animations": sorted(actions),
        "blend_file": bpy.data.filepath,
    }}
with open(out, "w") as f:
    json.dump(res, f, indent=2)
'''


def main():
    if len(sys.argv) != 2:
        print("usage: blender_stats.py <asset>", file=sys.stderr)
        return 2
    name = sys.argv[1]
    out = os.path.join(ROOT, "pipeline", "checks", f"{name}_blender.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    stats = blender_client.run_code_capture(CODE_TMPL.format(name=name, out=out), out)
    print(json.dumps(stats))
    return 0 if stats.get("found") else 1


if __name__ == "__main__":
    sys.exit(main())
