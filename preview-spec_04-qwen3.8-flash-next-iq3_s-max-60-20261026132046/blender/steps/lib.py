# shared helpers for asset steps (exec'd by each step file)
import bpy
import mathutils


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


def asset_collection(name):
    key = "A_" + name
    if key in bpy.data.collections:
        col = bpy.data.collections[key]
        for o in list(col.objects):
            me = o.data
            bpy.data.objects.remove(o)
            if me and me.users == 0:
                bpy.data.meshes.remove(me)
    else:
        col = bpy.data.collections.new(key)
        bpy.context.scene.collection.children.link(col)
    return col


def link_to(o, col):
    for c in o.users_collection:
        c.objects.unlink(o)
    col.objects.link(o)
    return o


def world_bbox(objs):
    vmin = mathutils.Vector((1e9, 1e9, 1e9))
    vmax = mathutils.Vector((-1e9, -1e9, -1e9))
    for ob in objs:
        if ob.data is None:  # empties report viewport widget bounds, not geometry
            continue
        for corner in ob.bound_box:
            w = ob.matrix_world @ mathutils.Vector(corner)
            for i in range(3):
                vmin[i] = min(vmin[i], w[i])
                vmax[i] = max(vmax[i], w[i])
    return [round(vmax[i] - vmin[i], 3) for i in range(3)], [round(v, 3) for v in vmin], [round(v, 3) for v in vmax]


def purge_actions(prefix):
    used = set()
    for o in bpy.data.objects:
        if o.animation_data and o.animation_data.action:
            used.add(o.animation_data.action)
    for old in list(bpy.data.actions):
        if old.name.startswith(prefix) and old not in used:
            bpy.data.actions.remove(old)


def fit_bbox(objs, size, col, center_xy=True, zmin=0.0, measure=None):
    """Parent objects to an AssetRoot empty (inside col) and iterate its
    scale/translation until the measured world bbox matches size. Works with
    nested parents (e.g. SwayRoot chains); export_apply bakes the result."""
    mobjs = list(measure) if measure is not None else objs
    root = bpy.data.objects.new("AssetRoot", None)
    link_to(root, col)
    for ob in objs:
        if ob.parent is None:
            ob.parent = root
    for _ in range(8):
        _, vmin, vmax = world_bbox(mobjs)
        cur = [vmax[i] - vmin[i] for i in range(3)]
        if any(c < 1e-6 for c in cur):
            raise RuntimeError("degenerate bbox")
        root.scale.x *= size[0] / cur[0]
        root.scale.y *= size[1] / cur[1]
        root.scale.z *= size[2] / cur[2]
        bpy.context.view_layer.update()
        _, vmin, vmax = world_bbox(mobjs)
        if center_xy:
            root.location.x -= (vmin[0] + vmax[0]) / 2
            root.location.y -= (vmin[1] + vmax[1]) / 2
        root.location.z += zmin - vmin[2]
        bpy.context.view_layer.update()
    dims, _, _ = world_bbox(mobjs)
    print("fit_bbox target:", size, "got:", tuple(round(d, 3) for d in dims))
