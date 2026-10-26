import bpy

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# ancient shrine: 3 steps + 4 columns + entablature + pediment (front -Y), one broken column
col = asset_collection("ancient_shrine")
parts = []
for i, (w, d, h) in enumerate([(4.6, 3.6, 0.28), (4.2, 3.2, 0.28), (3.8, 2.8, 0.28)]):
    bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 0.14 + i * 0.28))
    st = link_to(bpy.context.active_object, col)
    st.name = f"Step_{i}"
    st.scale = (w, d, h)
    bpy.ops.object.transform_apply(scale=True)
    st.data.materials.append(get_mat("M_shrine", (0.80, 0.76, 0.66), 0.9))
    parts.append(st)
base_z = 0.84
# cella back wall block
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0.7, base_z + 1.1))
cella = link_to(bpy.context.active_object, col)
cella.name = "Cella"
cella.scale = (2.2, 1.4, 2.2)
bpy.ops.object.transform_apply(scale=True)
cella.data.materials.append(get_mat("M_shrine", (0.80, 0.76, 0.66), 0.9))
parts.append(cella)
# doorway relief on cella front
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, -0.04, base_z + 0.8))
dr = link_to(bpy.context.active_object, col)
dr.name = "ShrineDoor"
dr.scale = (0.7, 0.08, 1.6)
bpy.ops.object.transform_apply(scale=True)
dr.data.materials.append(get_mat("M_door", (0.32, 0.22, 0.14)))
parts.append(dr)
# 4 front/side columns (one shorter = broken)
import math
colpos = [(-1.5, -1.1), (1.5, -1.1), (-1.5, 1.1), (1.5, 1.1)]
for i, (cx, cy) in enumerate(colpos):
    h = 1.5 if i == 3 else 2.6
    bpy.ops.mesh.primitive_cylinder_add(vertices=7, radius=0.22, depth=h, location=(cx, cy, base_z + h / 2))
    c = link_to(bpy.context.active_object, col)
    c.name = f"Column_{i}"
    c.data.materials.append(get_mat("M_shrine", (0.80, 0.76, 0.66), 0.9))
    parts.append(c)
    bpy.ops.mesh.primitive_cube_add(size=1, location=(cx, cy, base_z + h + 0.07))
    cap = link_to(bpy.context.active_object, col)
    cap.name = f"ColCap_{i}"
    cap.scale = (0.55, 0.55, 0.14)
    bpy.ops.object.transform_apply(scale=True)
    cap.data.materials.append(get_mat("M_shrine", (0.80, 0.76, 0.66), 0.9))
    parts.append(cap)
# entablature over full columns
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, base_z + 2.82))
ent = link_to(bpy.context.active_object, col)
ent.name = "Entablature"
ent.scale = (3.7, 2.7, 0.3)
bpy.ops.object.transform_apply(scale=True)
ent.data.materials.append(get_mat("M_shrine2", (0.72, 0.68, 0.58), 0.9))
parts.append(ent)
# pediment prism (ridge along X)
import bmesh
bm = bmesh.new()
z0 = base_z + 2.97
v = [bm.verts.new(p) for p in [
    (-1.85, -1.35, z0), (-1.85, 1.35, z0), (1.85, 1.35, z0), (1.85, -1.35, z0),
    (-1.85, 0.0, z0 + 1.1), (1.85, 0.0, z0 + 1.1),
]]
bm.faces.new([v[0], v[4], v[1]])
bm.faces.new([v[1], v[4], v[5], v[2]])
bm.faces.new([v[2], v[5], v[3]])
bm.faces.new([v[3], v[5], v[4], v[0]])
bm.faces.new([v[0], v[1], v[2], v[3]])
bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
me = bpy.data.meshes.new("Pediment")
bm.to_mesh(me)
bm.free()
ped = bpy.data.objects.new("Pediment", me)
link_to(ped, col)
ped.data.materials.append(get_mat("M_shrine2", (0.72, 0.68, 0.58), 0.9))
parts.append(ped)
fit_bbox(parts, [5.0, 4.0, 5.2], col)
bpy.context.view_layer.update()
print("shrine built:", world_bbox(list(col.all_objects))[0])
