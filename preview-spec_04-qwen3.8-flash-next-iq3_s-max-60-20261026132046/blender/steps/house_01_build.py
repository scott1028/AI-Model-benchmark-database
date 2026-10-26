import bpy
import math

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# house: walls, gable roof, door, windows, chimney (front = -Y)
col = asset_collection("house")
parts = []
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 1.25))
walls = link_to(bpy.context.active_object, col)
walls.name = "HouseWalls"
walls.scale = (4.6, 3.6, 2.5)
bpy.ops.object.transform_apply(scale=True)
walls.data.materials.append(get_mat("M_house", (0.85, 0.78, 0.64)))
parts.append(walls)
# gable roof: two sloped slabs + gable end triangles (manifold via bmesh recalc)
import bmesh
bm = bmesh.new()
v = [bm.verts.new(p) for p in [
    (-2.5, -2.1, 2.4), (-2.5, 2.1, 2.4), (2.5, 2.1, 2.4), (2.5, -2.1, 2.4),
    (-2.5, 0.0, 3.6), (2.5, 0.0, 3.6),
]]
bm.faces.new([v[0], v[4], v[1]])
bm.faces.new([v[1], v[4], v[5], v[2]])
bm.faces.new([v[2], v[5], v[3]])
bm.faces.new([v[3], v[5], v[4], v[0]])
bm.faces.new([v[0], v[1], v[2], v[3]])
bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
me = bpy.data.meshes.new("Roof")
bm.to_mesh(me)
bm.free()
roof = bpy.data.objects.new("Roof", me)
link_to(roof, col)
roof.data.materials.append(get_mat("M_roof", (0.62, 0.28, 0.22)))
parts.append(roof)
# door on -Y
bpy.ops.mesh.primitive_cube_add(size=1, location=(-1.0, -1.83, 0.85))
door = link_to(bpy.context.active_object, col)
door.name = "Door"
door.scale = (0.75, 0.08, 1.7)
bpy.ops.object.transform_apply(scale=True)
door.data.materials.append(get_mat("M_door", (0.42, 0.26, 0.14)))
parts.append(door)
# windows
for i, wx in enumerate([0.9, 2.0]):
    bpy.ops.mesh.primitive_cube_add(size=1, location=(wx, -1.83, 1.5))
    w = link_to(bpy.context.active_object, col)
    w.name = f"Window_{i}"
    w.scale = (0.6, 0.06, 0.6)
    bpy.ops.object.transform_apply(scale=True)
    w.data.materials.append(get_mat("M_window", (0.35, 0.55, 0.75)))
    parts.append(w)
bpy.ops.mesh.primitive_cube_add(size=1, location=(2.6, 0.6, 3.0))
win2 = link_to(bpy.context.active_object, col)
win2.name = "Window_Side"
win2.scale = (0.06, 0.6, 0.6)
bpy.ops.object.transform_apply(scale=True)
win2.data.materials.append(get_mat("M_window", (0.35, 0.55, 0.75)))
parts.append(win2)
# chimney
bpy.ops.mesh.primitive_cube_add(size=1, location=(1.6, 0.8, 3.35))
ch = link_to(bpy.context.active_object, col)
ch.name = "Chimney"
ch.scale = (0.5, 0.5, 1.3)
bpy.ops.object.transform_apply(scale=True)
ch.data.materials.append(get_mat("M_stone", (0.55, 0.53, 0.5)))
parts.append(ch)
fit_bbox(parts, [5.6, 4.6, 4.6], col)
bpy.context.view_layer.update()
print("house built:", world_bbox(list(col.all_objects))[0])
