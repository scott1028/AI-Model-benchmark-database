import bpy
import math

# robot step 3: WheelSpin action - Wheel rotates 360deg around X over 24 frames, linear
wheel = bpy.data.objects["Wheel"]
# bake wheel object rotation into mesh so axle = world X and the anim channel is clean
bpy.ops.object.select_all(action="DESELECT")
wheel.select_set(True)
bpy.context.view_layer.objects.active = wheel
bpy.ops.object.transform_apply(location=False, rotation=True, scale=False)
if "WheelSpin" in bpy.data.actions:
    bpy.data.actions.remove(bpy.data.actions["WheelSpin"])
act = bpy.data.actions.new("WheelSpin")
act.use_fake_user = True
wheel.animation_data_create()
wheel.animation_data.action = act
wheel.rotation_euler = (0.0, 0.0, 0.0)
wheel.keyframe_insert("rotation_euler", index=0, frame=1)
wheel.rotation_euler = (math.tau, 0.0, 0.0)
wheel.keyframe_insert("rotation_euler", index=0, frame=25)
# make interpolation LINEAR on whatever fcurve the layered system created
# Blender 5.2 layered actions: act.layers[].strips[].channelbags[].fcurves
fcurves = []
for layer in act.layers:
    for strip in layer.strips:
        for cb in strip.channelbags:
            fcurves.extend(cb.fcurves)
count = 0
for fc in fcurves:
    for kp in fc.keyframe_points:
        kp.interpolation = "LINEAR"
        count += 1
scene = bpy.context.scene
scene.frame_start = 1
scene.frame_end = 24
print("WheelSpin created, keyframes set linear:", count)
