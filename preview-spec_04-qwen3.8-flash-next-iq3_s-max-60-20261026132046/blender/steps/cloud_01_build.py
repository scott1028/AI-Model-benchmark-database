import bpy

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# cloud: 4 squashed flat-shaded ico blobs joined
col = asset_collection("cloud")
parts = []
specs = [(0, 0, 0.75, 1.05), (1.25, 0.15, 0.62, 0.8), (-1.15, -0.1, 0.6, 0.78), (0.35, -0.55, 0.55, 0.7)]
import random
random.seed(9)
for i, (cx, cy, cz, r) in enumerate(specs):
    bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=1, radius=r, location=(cx, cy, cz))
    b = link_to(bpy.context.active_object, col)
    b.name = f"Cloud_{i}"
    b.scale = (1.15, 1.0, 0.72)
    for v in b.data.vertices:
        v.co += v.co.normalized() * random.uniform(-0.08, 0.08)
        if v.co.z < -r * 0.55:
            v.co.z = -r * 0.55
    bpy.ops.object.transform_apply(scale=True)
    for pl in b.data.polygons:
        pl.use_smooth = False
    b.data.materials.append(get_mat("M_cloud", (0.95, 0.96, 0.98), 0.95))
    parts.append(b)
bpy.ops.object.select_all(action="DESELECT")
for b in parts:
    b.select_set(True)
bpy.context.view_layer.objects.active = parts[0]
bpy.ops.object.join()
cloud = bpy.context.active_object
cloud.name = "Cloud"
link_to(cloud, col)
fit_bbox([cloud], [4.0, 2.6, 1.5], col)
bpy.context.view_layer.update()
print("cloud built:", world_bbox(list(col.all_objects))[0])
