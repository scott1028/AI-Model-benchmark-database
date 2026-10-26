import bpy
import bmesh

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# barrel: bulged 12-gon staves + 2 iron hoops + lid + col_barrel convex node
col = asset_collection("barrel")
parts = []
# staves: profile rings at z 0 / 0.25 / 0.65 / 0.9 with bulge
me = bpy.data.meshes.new("BarrelStaves")
bm = bmesh.new()
rings = []
for z, r in [(0.0, 0.30), (0.25, 0.37), (0.65, 0.37), (0.90, 0.30)]:
    ring = [bm.verts.new((r * __import__("math").cos(i * __import__("math").tau / 12), r * __import__("math").sin(i * __import__("math").tau / 12), z)) for i in range(12)]
    rings.append(ring)
for a in range(3):
    for i in range(12):
        j = (i + 1) % 12
        bm.faces.new([rings[a][i], rings[a][j], rings[a + 1][j], rings[a + 1][i]])
bm.faces.new(list(reversed(rings[0])))
bm.faces.new(rings[-1])
bm.normal_update()
bm.to_mesh(me)
bm.free()
staves = bpy.data.objects.new("BarrelStaves", me)
link_to(staves, col)
staves.data.materials.append(get_mat("M_wood", (0.62, 0.42, 0.22)))
parts.append(staves)
# hoops
import math
for z in (0.14, 0.76):
    r = 0.375 if 0.25 < z < 0.65 else 0.315
    bpy.ops.mesh.primitive_torus_add(major_radius=r, minor_radius=0.022, major_segments=12, minor_segments=5, location=(0, 0, z))
    hoop = link_to(bpy.context.active_object, col)
    hoop.name = f"BarrelHoop_{z}"
    hoop.data.materials.append(get_mat("M_iron", (0.22, 0.22, 0.25)))
    parts.append(hoop)
# col node: convex cylinder approximating barrel
bpy.ops.mesh.primitive_cylinder_add(vertices=12, radius=0.37, depth=0.9, location=(0, 0, 0.45))
cn = link_to(bpy.context.active_object, col)
cn.name = "col_barrel"
cn.display_type = "WIRE"
cn.hide_render = True
cn.data.materials.append(get_mat("M_col", (0.8, 0.2, 0.8)))
parts.append(cn)
fit_bbox(parts, [0.74, 0.74, 0.92], col)
print("barrel built:", world_bbox(parts)[0], "col nodes:", [o.name for o in col.objects if o.name.startswith("col_")])
