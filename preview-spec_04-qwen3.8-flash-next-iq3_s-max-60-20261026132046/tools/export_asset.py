#!/usr/bin/env python3
"""Export one asset collection from Blender to godot/assets/<name>.glb and save the blend."""
import os
import sys

sys.path.insert(0, os.path.dirname(__file__))
import blender_client

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

CODE_TMPL = r'''
import bpy, json

name = {name!r}
glb = {glb!r}
blend = {blend!r}

col = bpy.data.collections.get("A_" + name)
if col is None:
    raise RuntimeError("collection A_" + name + " not found")

bpy.ops.object.select_all(action="DESELECT")
objs = list(col.all_objects)
if not objs:
    raise RuntimeError("A_" + name + " has no objects")
import mathutils
# recentre this asset at world origin for export (blend keeps grid offsets)
vmin = mathutils.Vector((1e9, 1e9, 1e9))
vmax = mathutils.Vector((-1e9, -1e9, -1e9))
for ob in objs:
    if ob.data is None:
        continue
    for corner in ob.bound_box:
        w = ob.matrix_world @ mathutils.Vector(corner)
        for i in range(3):
            vmin[i] = min(vmin[i], w[i])
            vmax[i] = max(vmax[i], w[i])
shift = (-(vmin.x + vmax.x) / 2, -(vmin.y + vmax.y) / 2, -vmin.z)
tops = [o for o in col.objects if o.parent is None or o.parent not in objs]
for o in tops:
    o.location = o.location + mathutils.Vector(shift)

# rename exported actions to the canonical names in the spec (blend may keep
# unique internal names to avoid clashes between assets)
import json as _json
want = []
try:
    want = _json.load(open({spec!r})).get("animations", [])
except Exception:
    pass
acts = []
for o in objs:
    ad = o.animation_data
    if ad and ad.action and ad.action not in acts:
        acts.append(ad.action)
renames = []
held = {{}}
for i, a in enumerate(acts):
    if i < len(want) and a.name != want[i]:
        ext = bpy.data.actions.get(want[i])
        if ext is not None and ext != a:
            held[ext] = ext.name
            ext.name = "TMP_" + ext.name
        renames.append((a, a.name))
        a.name = want[i]
for o in objs:
    try:
        o.hide_set(False)
    except RuntimeError:
        pass
    o.select_set(True)
bpy.context.view_layer.objects.active = objs[0]

bpy.ops.export_scene.gltf(
    filepath=glb,
    export_format="GLB",
    use_selection=True,
    export_apply=True,
    export_animations=True,
    export_skins=False,
    export_yup=True,
    export_cameras=False,
    export_lights=False,
    export_materials="EXPORT",
    export_extras=True,
)
for o in tops:
    o.location = o.location - mathutils.Vector(shift)
for a, oldname in renames:
    a.name = oldname
for ext, oldname in held.items():
    ext.name = oldname
bpy.ops.object.select_all(action="DESELECT")
bpy.ops.wm.save_as_mainfile(filepath=blend)
'''


def main():
    if len(sys.argv) != 2:
        print("usage: export_asset.py <asset>", file=sys.stderr)
        return 2
    name = sys.argv[1]
    glb = os.path.join(ROOT, "godot", "assets", f"{name}.glb")
    blend = os.path.join(ROOT, "blender", "coin_run.blend")
    blender_client.run_code(CODE_TMPL.format(name=name, glb=glb, blend=blend, spec=os.path.join(ROOT, "specs", f"{name}.json")))
    print(f"exported {glb}")
    return 0 if os.path.exists(glb) else 1


if __name__ == "__main__":
    sys.exit(main())
