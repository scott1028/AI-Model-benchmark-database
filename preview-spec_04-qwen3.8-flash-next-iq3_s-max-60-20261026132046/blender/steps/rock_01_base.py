import bpy
import random
from mathutils import Vector

# rock step 1: icosphere subdiv2, radial noise, flattened bottom
random.seed(23)
if "Rock" in bpy.data.objects:
    old = bpy.data.objects["Rock"]
    me_old = old.data
    old.location = (0, 0, 0)
    bpy.data.objects.remove(old)
    if me_old and me_old.users == 0:
        bpy.data.meshes.remove(me_old)
col = bpy.data.collections.new("A_rock") if "A_rock" not in bpy.data.collections else bpy.data.collections["A_rock"]
if col.name not in bpy.context.scene.collection.children:
    bpy.context.scene.collection.children.link(col)
bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=2, radius=0.55, location=(0, 0, 0.45))
rock = bpy.context.active_object
rock.name = "Rock"
for c in rock.users_collection:
    c.objects.unlink(rock)
col.objects.link(rock)
me = rock.data
for v in me.vertices:
    n = Vector((
        random.uniform(-0.11, 0.11),
        random.uniform(-0.09, 0.09),
        random.uniform(-0.09, 0.09),
    ))
    v.co = v.co + v.co.normalized() * n.length * 0.9 + n * 0.3
floor = -0.24
for v in me.vertices:
    if v.co.z < floor:
        v.co.z = floor + (v.co.z - floor) * 0.25
print("rock base verts:", len(me.vertices), "polys:", len(me.polygons))
