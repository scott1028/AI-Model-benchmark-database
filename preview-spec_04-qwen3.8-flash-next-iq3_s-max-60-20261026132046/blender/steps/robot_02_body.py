import bpy


# robot step 2: body, face panel, eyes, antenna, arms
col = bpy.data.collections["A_robot"]


def link_to(o):
    for c in o.users_collection:
        c.objects.unlink(o)
    col.objects.link(o)
    return o


def get_mat(name, rgba, rough=0.8):
    m = bpy.data.materials.get(name)
    if m is None:
        m = bpy.data.materials.new(name)
        m.use_nodes = True
        b = next(n for n in m.node_tree.nodes if n.type == "BSDF_PRINCIPLED")
        b.inputs["Base Color"].default_value = (*rgba, 1.0)
        b.inputs["Roughness"].default_value = rough
        b.inputs["Metallic"].default_value = 0.0
    return m


for nm in ["RobotBody", "FacePanel", "Eye_L", "Eye_R", "Antenna", "AntennaTip", "Arm_L", "Arm_R"]:
    if nm in bpy.data.objects:
        o = bpy.data.objects[nm]
        me = o.data
        bpy.data.objects.remove(o)
        if me and me.users == 0:
            bpy.data.meshes.remove(me)

# body: rounded warm-white box on top of the wheel
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 0.80))
body = link_to(bpy.context.active_object)
body.name = "RobotBody"
body.scale = (0.60, 0.42, 0.46)
bev = body.modifiers.new("Bevel", "BEVEL")
bev.width = 0.09
bev.segments = 2
bpy.ops.object.select_all(action="DESELECT")
body.select_set(True)
bpy.context.view_layer.objects.active = body
bpy.ops.object.modifier_apply(modifier="Bevel")
body.data.materials.append(get_mat("M_body", (0.88, 0.85, 0.78)))

# face panel (front = -Y), cyan
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, -0.235, 0.84))
panel = link_to(bpy.context.active_object)
panel.name = "FacePanel"
panel.scale = (0.34, 0.05, 0.24)
bpy.ops.object.transform_apply(scale=True)
panel.data.materials.append(get_mat("M_face", (0.15, 0.62, 0.70)))

# eyes
for nm, ex in [("Eye_L", -0.08), ("Eye_R", 0.08)]:
    bpy.ops.mesh.primitive_uv_sphere_add(segments=10, ring_count=6, radius=0.042, location=(ex, -0.268, 0.88))
    eye = link_to(bpy.context.active_object)
    eye.name = nm
    eye.data.materials.append(get_mat("M_eye", (0.05, 0.06, 0.08), 0.4))

# antenna: rod + tip (top must reach z=1.15)
bpy.ops.mesh.primitive_cylinder_add(vertices=6, radius=0.014, depth=0.10, location=(0, 0.05, 1.05))
rod = link_to(bpy.context.active_object)
rod.name = "Antenna"
rod.data.materials.append(get_mat("M_arm", (0.45, 0.47, 0.5)))
bpy.ops.mesh.primitive_uv_sphere_add(segments=10, ring_count=6, radius=0.06, location=(0, 0.05, 1.09))
tip = link_to(bpy.context.active_object)
tip.name = "AntennaTip"
tip.data.materials.append(get_mat("M_tip", (0.85, 0.25, 0.2)))

# arms (set overall width to 0.80)
for nm, ax in [("Arm_L", -0.365), ("Arm_R", 0.365)]:
    bpy.ops.mesh.primitive_cube_add(size=1, location=(ax, 0, 0.78))
    arm = link_to(bpy.context.active_object)
    arm.name = nm
    arm.scale = (0.07, 0.14, 0.30)
    bpy.ops.object.transform_apply(scale=True)
    arm.data.materials.append(get_mat("M_arm", (0.45, 0.47, 0.5)))

print("robot body built; objects:", sorted(o.name for o in col.objects))
