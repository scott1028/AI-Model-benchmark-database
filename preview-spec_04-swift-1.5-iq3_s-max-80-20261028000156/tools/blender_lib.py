"""Shared Blender modelling helpers for Coin Run assets.

Loaded into the Blender MCP namespace with:
    exec(open(ROOT + "/tools/blender_lib.py").read(), globals())
so the modelling snippet can call new_scene(), mat(), box(), cyl(), ico(), finish() ...
Conventions live in AGENTS.md §2/§4: 1 unit = 1 m, Z-up, origin = bottom centre,
character front = +Y, visual meshes named <id>*, collider mesh named <id>_collision.
"""
import math

import bpy

ROOT = globals().get("ROOT", "/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-swift-1.5-iq3_s-max-20261028000156")
_MATS = {}
CTX = {"id": None, "root": None, "parts": [], "mats": {}}


def _srgb_to_linear(c):
    c = c / 255.0
    return c / 12.92 if c <= 0.04045 else ((c + 0.055) / 1.055) ** 2.4


def hex_rgb(h):
    h = h.lstrip("#")
    return [_srgb_to_linear(int(h[i:i + 2], 16)) for i in (0, 2, 4)]


def new_scene(asset_id, fps=30):
    """Start a clean single-asset scene with an Empty root named <id>."""
    for o in list(bpy.data.objects):
        bpy.data.objects.remove(o, do_unlink=True)
    for block in (bpy.data.meshes, bpy.data.materials, bpy.data.actions, bpy.data.lights, bpy.data.cameras):
        for b in list(block):
            if b.users == 0:
                block.remove(b)
    scene = bpy.context.scene
    scene.render.fps = fps
    root = bpy.data.objects.new(asset_id, None)
    root.empty_display_size = 0.5
    scene.collection.objects.link(root)
    CTX.update({"id": asset_id, "root": root, "parts": [], "mats": {}})
    _MATS.clear()
    return root


def mat(name, hex_color, rough=0.85, emit=None, emit_strength=1.0, metallic=0.0):
    """Flat-shaded material named exactly as the spec wants; emit = emissive hex colour."""
    if name in CTX["mats"]:
        return CTX["mats"][name]
    m = bpy.data.materials.new(name)
    m.use_nodes = True
    b = next(n for n in m.node_tree.nodes if n.type == "BSDF_PRINCIPLED")
    b.inputs["Base Color"].default_value = (*hex_rgb(hex_color), 1.0)
    b.inputs["Roughness"].default_value = rough
    b.inputs["Metallic"].default_value = metallic
    if emit:
        b.inputs["Emission Color"].default_value = (*hex_rgb(emit), 1.0)
        b.inputs["Emission Strength"].default_value = emit_strength
    CTX["mats"][name] = m
    return m


def _link(obj, name, material=None, parent_to=None):
    obj.name = name
    obj.data.materials.clear()
    if material:
        obj.data.materials.append(material)
    root = parent_to if parent_to is not None else CTX["root"]
    obj.parent = root
    obj.matrix_parent_inverse = root.matrix_world.inverted()
    CTX["parts"].append(obj)
    return obj


def box(name, size, loc=(0, 0, 0), rot=(0, 0, 0), material=None, parent_to=None):
    bpy.ops.mesh.primitive_cube_add(size=1.0, location=loc, rotation=rot)
    o = bpy.context.active_object
    o.scale = (abs(size[0]), abs(size[1]), abs(size[2]))
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    return _link(o, name, material, parent_to)


def cyl(name, radius, depth, loc=(0, 0, 0), verts=12, axis="Z", material=None, parent_to=None):
    rot = {"Z": (0, 0, 0), "Y": (math.radians(90), 0, 0), "X": (0, math.radians(-90), 0)}[axis]
    bpy.ops.mesh.primitive_cylinder_add(vertices=verts, radius=radius, depth=depth, location=loc, rotation=rot)
    o = bpy.context.active_object
    bpy.ops.object.transform_apply(location=False, rotation=True, scale=False)
    return _link(o, name, material, parent_to)


def cone(name, radius, depth, loc=(0, 0, 0), verts=8, material=None, parent_to=None):
    bpy.ops.mesh.primitive_cone_add(vertices=verts, radius1=radius, radius2=0.0, depth=depth, location=loc)
    o = bpy.context.active_object
    return _link(o, name, material, parent_to)


def ico(name, radius, loc=(0, 0, 0), subdiv=1, material=None, squash=(1, 1, 1), seed=0, jitter=0.0, parent_to=None):
    bpy.ops.mesh.primitive_ico_sphere_add(subdivisions=subdiv, radius=radius, location=loc)
    o = bpy.context.active_object
    if jitter:
        import random
        rnd = random.Random(seed)
        for v in o.data.vertices:
            v.co += v.co.normalized() * jitter * rnd.uniform(-1.0, 1.0)
    o.scale = squash
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    return _link(o, name, material, parent_to)


def uv_sphere(name, radius, loc=(0, 0, 0), segments=14, rings=8, material=None, squash=(1, 1, 1), parent_to=None):
    bpy.ops.mesh.primitive_uv_sphere_add(segments=segments, ring_count=rings, radius=radius, location=loc)
    o = bpy.context.active_object
    o.scale = squash
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    return _link(o, name, material, parent_to)


def prism(name, verts, radius, depth, loc=(0, 0, 0), axis="Z", material=None, parent_to=None):
    """Regular-n-gon prism; used for stylised columns and crystals."""
    return cyl(name, radius, depth, loc=loc, verts=verts, axis=axis, material=material, parent_to=parent_to)


def assign(obj, material):
    obj.data.materials.clear()
    obj.data.materials.append(material)
    return obj


def collider(shape_fn, kind="box"):
    """Build the mesh collider <id>_collision (crcollider=1, hidden from render)."""
    idn = CTX["id"]
    objs = shape_fn()
    if isinstance(objs, bpy.types.Object):
        objs = [objs]
    for o in objs:
        o.parent = None
    if len(objs) > 1:
        bpy.ops.object.select_all(action="DESELECT")
        for o in objs:
            o.select_set(True)
        bpy.context.view_layer.objects.active = objs[0]
        bpy.ops.object.join()
        merged = bpy.context.active_object
    else:
        merged = objs[0]
    merged.name = "%s_collision" % idn
    merged["crcollider"] = 1
    merged.hide_render = True
    merged.parent = CTX["root"]
    if merged not in CTX["parts"]:
        CTX["parts"].append(merged)
    return merged


def _world_verts(objs):
    """World-space vertices (not bound_box, which inflates rotated objects)."""
    from mathutils import Vector
    xs, ys, zs = [], [], []
    for o in objs:
        if o.type != "MESH":
            continue
        for v in o.data.vertices:
            w = o.matrix_world @ v.co
            xs.append(w.x)
            ys.append(w.y)
            zs.append(w.z)
    return xs, ys, zs


def set_origin_bottom():
    """Shift every part so the asset sits on z=0 and its footprint is centred on the origin."""
    bpy.context.view_layer.update()
    visual = [o for o in CTX["parts"] if not o.name.endswith("_collision")]
    xs, ys, zs = _world_verts(visual)
    if not zs:
        raise RuntimeError("no parts modelled")
    dz = min(zs)
    cx = (min(xs) + max(xs)) / 2.0
    cy = (min(ys) + max(ys)) / 2.0
    for o in CTX["parts"]:
        o.location.x -= cx
        o.location.y -= cy
        o.location.z -= dz
    bpy.context.view_layer.update()
    return {"dz": dz, "cx": cx, "cy": cy}


def report(materials_expected=()):
    bpy.context.view_layer.update()
    visual = [o for o in CTX["parts"] if not o.name.endswith("_collision")]
    cols = [o for o in CTX["parts"] if o.name.endswith("_collision")]
    xs, ys, zs = _world_verts(visual)
    tris = 0
    for o in visual:
        o.data.calc_loop_triangles()
        tris += len(o.data.loop_triangles)
    names = sorted({m.name for o in visual for m in o.data.materials})
    return {
        "id": CTX["id"],
        "dims_blender": {"x": round(max(xs) - min(xs), 3), "y": round(max(ys) - min(ys), 3), "z": round(max(zs) - min(zs), 3)},
        "min_z": round(min(zs), 4),
        "tris_visual": tris,
        "materials": names,
        "missing_materials": [n for n in materials_expected if n not in names],
        "parts": len(visual),
        "collider": [o.name for o in cols],
    }
