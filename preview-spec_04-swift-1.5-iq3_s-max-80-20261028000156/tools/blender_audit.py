"""Blender-side asset audit. Runs INSIDE headless Blender.

Usage:
  blender --background blender/<id>.blend --python tools/blender_audit.py -- --id <id> --json-out <path>
"""
import json
import sys
from pathlib import Path

argv = sys.argv[sys.argv.index("--") + 1:]
opt = {"id": None, "json_out": None}
i = 0
while i < len(argv):
    if argv[i] == "--id":
        opt["id"] = argv[i + 1]
        i += 2
    elif argv[i] == "--json-out":
        opt["json_out"] = argv[i + 1]
        i += 2
    else:
        i += 1

import bpy  # noqa: E402

COL_SUFFIX = "_collision"
ASSET = opt["id"]


def tri_count(obj):
    me = obj.data
    if not hasattr(me, "calc_loop_triangles"):
        return 0
    me.calc_loop_triangles()
    return len(me.loop_triangles)


def world_verts(objs, depsgraph):
    """World-space mesh vertices; bound_box would inflate rotated parts (AABB of AABB)."""
    xs, ys, zs = [], [], []
    for o in objs:
        if o.type != "MESH":
            continue
        me = o.evaluated_get(depsgraph).to_mesh()
        if me is None:
            continue
        for v in me.vertices:
            w = o.matrix_world @ v.co
            xs.append(w.x)
            ys.append(w.y)
            zs.append(w.z)
        o.evaluated_get(depsgraph).to_mesh_clear()
    return xs, ys, zs


def bbox(objs, depsgraph):
    xs, ys, zs = world_verts(objs, depsgraph)
    if not xs:
        return None
    return {
        "min": [min(xs), min(ys), min(zs)],
        "max": [max(xs), max(ys), max(zs)],
        "size": [max(xs) - min(xs), max(ys) - min(ys), max(zs) - min(zs)],
    }


def linear_to_srgb_hex(v):
    def enc(c):
        c = max(0.0, min(1.0, c))
        s = 1.055 * (c ** (1.0 / 2.4)) - 0.055 if c > 0.0031308 else 12.92 * c
        return int(round(max(0.0, min(1.0, s)) * 255))
    return "#%02X%02X%02X" % tuple(enc(c) for c in v[0:3])


def material_report(objs):
    seen = {}
    for o in objs:
        for slot in o.material_slots:
            mat = slot.material
            if mat is None:
                continue
            color = None
            if mat.node_tree:
                node = next((n for n in mat.node_tree.nodes if n.type == "BSDF_PRINCIPLED"), None)
                if node and "Base Color" in node.inputs:
                    v = node.inputs["Base Color"].default_value
                    color = linear_to_srgb_hex(v)
            seen[mat.name] = color
    return [{"name": k, "color": v} for k, v in sorted(seen.items())]


def main():
    depsgraph = bpy.context.evaluated_depsgraph_get()
    all_meshes = [o for o in bpy.context.scene.collection.all_objects if o.type == "MESH"]
    visual = [o for o in all_meshes if not o.name.endswith(COL_SUFFIX)]
    cols = [o for o in all_meshes if o.name.endswith(COL_SUFFIX)]
    top_level = sorted({o.name for o in bpy.context.scene.objects if o.parent is None})

    vb = bbox(visual, depsgraph)
    parts_detail = []
    for o in visual:
        pb = bbox([o], depsgraph)
        parts_detail.append({
            "name": o.name,
            "dims": ({"x": round(pb["size"][0], 4), "y": round(pb["size"][1], 4), "z": round(pb["size"][2], 4)} if pb else None),
            "min": [round(v, 4) for v in pb["min"]] if pb else None,
            "max": [round(v, 4) for v in pb["max"]] if pb else None,
        })
    report = {
        "id": ASSET,
        "blender_version": bpy.app.version_string,
        "blend_file": bpy.data.filepath,
        "top_level_objects": top_level,
        "visual_objects": sorted(o.name for o in visual),
        "parts_detail": parts_detail,
        "dims_blender": ({"x": vb["size"][0], "y": vb["size"][1], "z": vb["size"][2]} if vb else None),
        "bbox_min": vb["min"] if vb else None,
        "bbox_max": vb["max"] if vb else None,
        "tris_visual": sum(tri_count(o) for o in visual),
        "tris_col": sum(tri_count(o) for o in cols),
        "origin": {
            "min_z": (vb["min"][2] if vb else None),
            "xy_center_offset": ([round((vb["min"][0] + vb["max"][0]) / 2.0, 4), round((vb["min"][1] + vb["max"][1]) / 2.0, 4)] if vb else None),
        },
        "materials": material_report(visual),
        "collision": {
            "present": bool(cols),
            "names": sorted(o.name for o in cols),
            "flag_ok": all(o.get("crcollider", 0) == 1 for o in cols) if cols else False,
            "hide_render": all(o.hide_render for o in cols) if cols else False,
        },
        "animations": [
            {
                "name": a.name,
                "frame_start": int(a.frame_range[0]),
                "frame_end": int(a.frame_range[1]),
                "fps": bpy.context.scene.render.fps,
            }
            for a in bpy.data.actions
        ],
        "scene_fps": bpy.context.scene.render.fps,
    }
    text = json.dumps(report, indent=2, ensure_ascii=False)
    if opt["json_out"]:
        Path(opt["json_out"]).parent.mkdir(parents=True, exist_ok=True)
        Path(opt["json_out"]).write_text(text + "\n", encoding="utf-8")
    print("BLENDER_AUDIT " + json.dumps(report, ensure_ascii=False))
    return 0


sys.exit(main())
