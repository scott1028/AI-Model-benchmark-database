import bpy
import math

# robot step 1: single wheel (tire + hub + spokes), center (0,0,0.30), axis X
if "A_robot" in bpy.data.collections:
    col = bpy.data.collections["A_robot"]
    for o in list(col.objects):
        me = o.data
        bpy.data.objects.remove(o)
        if me and me.users == 0:
            bpy.data.meshes.remove(me)
else:
    col = bpy.data.collections.new("A_robot")
    bpy.context.scene.collection.children.link(col)


def link_to(o):
    for c in o.users_collection:
        c.objects.unlink(o)
    col.objects.link(o)
    return o


def get_mat(name, rgba):
    m = bpy.data.materials.get(name)
    if m is None:
        m = bpy.data.materials.new(name)
        m.use_nodes = True
        b = next(n for n in m.node_tree.nodes if n.type == "BSDF_PRINCIPLED")
        b.inputs["Base Color"].default_value = (*rgba, 1.0)
        b.inputs["Roughness"].default_value = 0.8
        b.inputs["Metallic"].default_value = 0.0
    return m


parts = []
# tire: cylinder axis along X
bpy.ops.mesh.primitive_cylinder_add(vertices=16, radius=0.30, depth=0.16, location=(0, 0, 0.30), rotation=(0, math.pi / 2, 0))
tire = bpy.context.active_object
tire.name = "Wheel"
tire.data.materials.append(get_mat("M_tire", (0.13, 0.13, 0.15)))
link_to(tire)
parts.append(tire)
# hub cap on both sides (orange disc)
for sx in (-0.09, 0.09):
    bpy.ops.mesh.primitive_cylinder_add(vertices=8, radius=0.13, depth=0.03, location=(sx, 0, 0.30), rotation=(0, math.pi / 2, 0))
    hub = bpy.context.active_object
    hub.name = "WheelHub_" + ("L" if sx < 0 else "R")
    hub.data.materials.append(get_mat("M_hub", (0.85, 0.45, 0.15)))
    link_to(hub)
    parts.append(hub)
# spokes: 4 boxes crossing the wheel face (visible rotation)
for i in range(4):
    a = math.radians(45 + i * 90)
    y = 0.16 * math.cos(a) / 2
    z = 0.16 * math.sin(a) / 2
    bpy.ops.mesh.primitive_cube_add(size=1, location=(0.10, y, 0.30 + z))
    sp = bpy.context.active_object
    sp.name = f"Spoke_{i}"
    sp.data.materials.append(get_mat("M_hub", (0.85, 0.45, 0.15)))
    sp.scale = (0.012, 0.045, 0.28)
    sp.rotation_euler = (a, 0, 0)
    bpy.ops.object.transform_apply(scale=True, rotation=True)
    link_to(sp)
    parts.append(sp)
    bpy.ops.mesh.primitive_cube_add(size=1, location=(-0.10, y, 0.30 + z))
    sp2 = bpy.context.active_object
    sp2.name = f"SpokeL_{i}"
    sp2.data.materials.append(get_mat("M_hub", (0.85, 0.45, 0.15)))
    sp2.scale = (0.012, 0.045, 0.28)
    sp2.rotation_euler = (a, 0, 0)
    bpy.ops.object.transform_apply(scale=True, rotation=True)
    link_to(sp2)
    parts.append(sp2)
# join hub+spokes into Wheel so one rotation animates them all
bpy.ops.object.select_all(action="DESELECT")
for p in parts:
    p.select_set(True)
bpy.context.view_layer.objects.active = tire
bpy.ops.object.join()
print("Wheel built:", tire.name, tuple(round(d, 3) for d in tire.dimensions))
