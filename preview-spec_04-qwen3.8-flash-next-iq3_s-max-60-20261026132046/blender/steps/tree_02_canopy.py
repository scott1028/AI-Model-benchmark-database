import bpy

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# tree step 2: canopy blobs parented to SwayRoot empty
col = bpy.data.collections["A_tree"]
blobs = []
for i, (bx, by, bz, r) in enumerate([(0, 0, 2.9, 1.15), (0.75, 0.35, 2.45, 0.85), (-0.7, -0.4, 2.35, 0.8)]):
    bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=1, radius=r, location=(bx, by, bz))
    blob = link_to(bpy.context.active_object, col)
    blob.name = f"Canopy_{i}"
    import random
    random.seed(30 + i)
    for v in blob.data.vertices:
        v.co += v.co.normalized() * random.uniform(-0.10, 0.10)
        if v.co.z < -r * 0.75:
            v.co.z = -r * 0.75
    for pl in blob.data.polygons:
        pl.use_smooth = False
    blob.data.materials.append(get_mat("M_leaf_dark" if i == 0 else "M_leaf_mid", (0.16, 0.42, 0.20) if i == 0 else (0.24, 0.55, 0.24)))
    blobs.append(blob)
sway = bpy.data.objects.new("SwayRoot", None)
link_to(sway, col)
sway.location = (0, 0, 1.9)
bpy.context.view_layer.update()
for b in blobs:
    b.parent = sway
    b.matrix_parent_inverse = sway.matrix_world.inverted()
print("canopy built, SwayRoot ready")
