import bpy

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# crate: box body + corner slats + X braces on front/back
col = asset_collection("crate")
parts = []
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 0.40))
body = link_to(bpy.context.active_object, col)
body.name = "CrateBody"
body.scale = (0.78, 0.78, 0.78)
bpy.ops.object.transform_apply(scale=True)
body.data.materials.append(get_mat("M_crate", (0.55, 0.36, 0.18)))
parts.append(body)
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 0.815))
lid = link_to(bpy.context.active_object, col)
lid.name = "CrateLid"
lid.scale = (0.84, 0.84, 0.05)
bpy.ops.object.transform_apply(scale=True)
lid.data.materials.append(get_mat("M_crate_dark", (0.40, 0.25, 0.12)))
parts.append(lid)
# 4 vertical corner slats
for i, (sx, sy) in enumerate([(-1, -1), (-1, 1), (1, -1), (1, 1)]):
    bpy.ops.mesh.primitive_cube_add(size=1, location=(sx * 0.395, sy * 0.395, 0.40))
    s = link_to(bpy.context.active_object, col)
    s.name = f"CrateSlat_{i}"
    s.scale = (0.075, 0.075, 0.84)
    bpy.ops.object.transform_apply(scale=True)
    s.data.materials.append(get_mat("M_crate_dark", (0.40, 0.25, 0.12)))
    parts.append(s)
# X braces on front and back (+-Y faces)
for side in (-1, 1):
    for sgn in (-1, 1):
        bpy.ops.mesh.primitive_cube_add(size=1, location=(0, side * 0.41, 0.40))
        b = link_to(bpy.context.active_object, col)
        b.name = f"CrateX_{side}_{sgn}"
        b.scale = (0.06, 0.03, 1.02)
        b.rotation_euler = (0, sgn * 0.785, 0)
        bpy.ops.object.transform_apply(scale=True, rotation=True)
        b.data.materials.append(get_mat("M_crate_dark", (0.40, 0.25, 0.12)))
        parts.append(b)
fit_bbox(parts, [0.84, 0.84, 0.84], col)
print("crate built:", world_bbox(parts)[0])
