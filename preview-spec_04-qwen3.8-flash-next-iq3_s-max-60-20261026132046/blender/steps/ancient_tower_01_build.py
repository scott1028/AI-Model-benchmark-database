import bpy
import math

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# ancient tower: tapered 3-stage cylinder + broken battlements + arch door + banner
col = asset_collection("ancient_tower")
parts = []
for i, (z0, z1, r) in enumerate([(0, 3.0, 1.7), (3.0, 5.6, 1.45), (5.6, 7.2, 1.25)]):
    me = bpy.data.meshes.new(f"TowerStage{i}")
    import bmesh
    bm = bmesh.new()
    r0, r1 = r, r * 0.88
    ring0 = [bm.verts.new((r0 * math.cos(k * math.tau / 10), r0 * math.sin(k * math.tau / 10), z0)) for k in range(10)]
    ring1 = [bm.verts.new((r1 * math.cos(k * math.tau / 10), r1 * math.sin(k * math.tau / 10), z1)) for k in range(10)]
    for k in range(10):
        j = (k + 1) % 10
        bm.faces.new([ring0[k], ring0[j], ring1[j], ring1[k]])
    bm.faces.new(list(reversed(ring0)))
    bm.faces.new(ring1)
    bm.normal_update()
    bm.to_mesh(me)
    bm.free()
    st = bpy.data.objects.new(f"TowerStage{i}", me)
    link_to(st, col)
    st.data.materials.append(get_mat("M_tower", (0.55, 0.56, 0.62), 0.9))
    parts.append(st)
# battlements on top ring (some broken)
import random
random.seed(5)
for k in range(10):
    if k in (2, 6, 7):  # broken gaps
        continue
    a = k * math.tau / 10
    bpy.ops.mesh.primitive_cube_add(size=1, location=(1.06 * math.cos(a), 1.06 * math.sin(a), 7.45))
    m = link_to(bpy.context.active_object, col)
    m.name = f"Battlement_{k}"
    m.scale = (0.34, 0.28, 0.5)
    m.rotation_euler = (0, 0, a)
    bpy.ops.object.transform_apply(scale=True, rotation=True)
    m.data.materials.append(get_mat("M_tower", (0.55, 0.56, 0.62), 0.9))
    parts.append(m)
# arch door (flat relief on -Y face)
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, -1.56, 1.05))
dr = link_to(bpy.context.active_object, col)
dr.name = "TowerDoor"
dr.scale = (0.7, 0.08, 2.0)
bpy.ops.object.transform_apply(scale=True)
dr.data.materials.append(get_mat("M_door", (0.3, 0.2, 0.12)))
parts.append(dr)
bpy.ops.mesh.primitive_cylinder_add(vertices=8, radius=0.35, depth=0.08, location=(0, -1.56, 2.05), rotation=(math.pi / 2, 0, 0))
arch = link_to(bpy.context.active_object, col)
arch.name = "TowerArch"
arch.data.materials.append(get_mat("M_door", (0.3, 0.2, 0.12)))
parts.append(arch)
# small banner pole+cloth on top
bpy.ops.mesh.primitive_cylinder_add(vertices=5, radius=0.04, depth=1.2, location=(0.9, 0.9, 7.9))
pole = link_to(bpy.context.active_object, col)
pole.name = "TowerPole"
pole.data.materials.append(get_mat("M_pole", (0.45, 0.30, 0.16)))
parts.append(pole)
bpy.ops.mesh.primitive_cube_add(size=1, location=(1.15, 0.9, 8.25))
ban = link_to(bpy.context.active_object, col)
ban.name = "TowerBanner"
ban.scale = (0.5, 0.03, 0.7)
bpy.ops.object.transform_apply(scale=True)
ban.data.materials.append(get_mat("M_banner", (0.30, 0.35, 0.62)))
parts.append(ban)
fit_bbox(parts, [4.0, 4.0, 8.2], col)
bpy.context.view_layer.update()
print("tower built:", world_bbox(list(col.all_objects))[0])
