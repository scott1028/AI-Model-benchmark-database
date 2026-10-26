import bpy
import math
import random

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# bush: 4 blobs on SwayRoot + Sway action
col = asset_collection("bush")
blobs = []
random.seed(7)
specs = [(0, 0, 0.55, 0.55), (0.45, 0.1, 0.45, 0.42), (-0.4, -0.15, 0.42, 0.40), (0.05, -0.45, 0.38, 0.36)]
for i, (bx, by, bz, r) in enumerate(specs):
    bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=1, radius=r, location=(bx, by, bz))
    blob = link_to(bpy.context.active_object, col)
    blob.name = f"Bush_{i}"
    for v in blob.data.vertices:
        v.co += v.co.normalized() * random.uniform(-0.07, 0.07)
        if v.co.z < -r * 0.7:
            v.co.z = -r * 0.7
    for pl in blob.data.polygons:
        pl.use_smooth = False
    blob.data.materials.append(get_mat("M_bush_mid" if i % 2 else "M_bush_dark", (0.20, 0.48, 0.22) if i % 2 else (0.14, 0.38, 0.18)))
    blobs.append(blob)
sway = bpy.data.objects.new("SwayRoot", None)
link_to(sway, col)
sway.location = (0, 0, 0.3)
bpy.context.view_layer.update()
for b in blobs:
    b.parent = sway
    b.matrix_parent_inverse = sway.matrix_world.inverted()
purge_actions("Sway_bush")
act = bpy.data.actions.new("Sway_bush")
act.use_fake_user = True
sway.animation_data_create()
sway.animation_data.action = act
seq = [(1, 0), (12, 7), (24, 0), (36, -7), (48, 0)]
for f, a in seq:
    sway.rotation_euler = (0, math.radians(a), 0)
    sway.keyframe_insert("rotation_euler", index=1, frame=f)
for layer in act.layers:
    for strip in layer.strips:
        for cb in strip.channelbags:
            for fcv in cb.fcurves:
                for kp in fcv.keyframe_points:
                    kp.interpolation = "LINEAR"
fit_bbox([sway], [1.3, 1.3, 1.0], col, measure=list(col.all_objects))
bpy.context.view_layer.update()
print("bush built:", world_bbox(list(col.all_objects))[0])
