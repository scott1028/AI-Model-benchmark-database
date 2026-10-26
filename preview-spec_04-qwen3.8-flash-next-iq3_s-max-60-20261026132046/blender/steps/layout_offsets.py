import bpy
import mathutils

# grid layout for asset collections in the blend file (keeps the file readable).
# glb export recenters per-asset in tools/export_asset.py.
OFFSETS = {
    "rock": (0, 0),
    "robot": (6, 0),
    "coin": (12, 0),
    "flag": (18, 0),
    "crate": (0, 6),
    "barrel": (6, 6),
    "tree": (12, 6),
    "bush": (18, 6),
    "wall": (0, 12),
    "house": (6, 12),
    "ancient_tower": (12, 12),
    "ancient_shrine": (18, 12),
    "cloud": (0, 18),
    "ground": (36, 0),
}
BLEND = "/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/coin_run.blend"


def center_xy(col):
    vmin = mathutils.Vector((1e9, 1e9, 1e9))
    vmax = mathutils.Vector((-1e9, -1e9, -1e9))
    for ob in col.all_objects:
        if ob.data is None:
            continue
        for corner in ob.bound_box:
            w = ob.matrix_world @ mathutils.Vector(corner)
            vmin.x = min(vmin.x, w.x)
            vmin.y = min(vmin.y, w.y)
            vmax.x = max(vmax.x, w.x)
            vmax.y = max(vmax.y, w.y)
    return (vmin.x + vmax.x) / 2, (vmin.y + vmax.y) / 2


for name, (dx, dy) in OFFSETS.items():
    col = bpy.data.collections.get("A_" + name)
    if col is None:
        continue
    cx, cy = center_xy(col)
    if abs(cx - dx) < 1e-4 and abs(cy - dy) < 1e-4:
        continue
    for ob in [o for o in col.objects if o.parent is None]:
        ob.location.x += dx - cx
        ob.location.y += dy - cy
bpy.ops.wm.save_as_mainfile(filepath=BLEND)
print("grid offsets applied")
