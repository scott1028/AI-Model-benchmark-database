import bpy
import math

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# tree step 3: Sway action on SwayRoot + fit bbox
col = bpy.data.collections["A_tree"]
sway = bpy.data.objects["SwayRoot"]
trunk = bpy.data.objects["Trunk"]


def make_sway(obj, action_name, amp_deg, frames=60):
    purge_actions(action_name)
    act = bpy.data.actions.new(action_name)
    act.use_fake_user = True
    obj.animation_data_create()
    obj.animation_data.action = act
    base = obj.rotation_euler[:]
    seq = [(1, 0), (frames * 0.25, amp_deg), (frames * 0.5, 0), (frames * 0.75, -amp_deg), (frames + 1, 0)]
    obj.rotation_euler = (base[0], math.radians(seq[0][1]), base[2])
    obj.keyframe_insert("rotation_euler", index=1, frame=seq[0][0])
    for f, a in seq[1:]:
        obj.rotation_euler = (base[0], math.radians(a), base[2])
        obj.keyframe_insert("rotation_euler", index=1, frame=f)
    for layer in act.layers:
        for strip in layer.strips:
            for cb in strip.channelbags:
                for fcv in cb.fcurves:
                    for kp in fcv.keyframe_points:
                        kp.interpolation = "LINEAR"


make_sway(sway, "Sway_tree", 5.0)
fit_bbox([trunk, sway], [3.4, 3.4, 4.3], col, measure=list(col.all_objects))
bpy.context.view_layer.update()
print("tree sway done:", world_bbox(list(col.all_objects))[0])
