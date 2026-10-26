import bpy
import math

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# tree step 1: trunk + branches
col = asset_collection("tree")
parts = []
me = bpy.data.meshes.new("Trunk")
import bmesh
bm = bmesh.new()
rings = []
for z, r in [(0.0, 0.26), (0.8, 0.22), (1.7, 0.17), (2.3, 0.12)]:
    ring = [bm.verts.new((r * math.cos(i * math.tau / 7), r * math.sin(i * math.tau / 7), z)) for i in range(7)]
    rings.append(ring)
for a in range(3):
    for i in range(7):
        j = (i + 1) % 7
        bm.faces.new([rings[a][i], rings[a][j], rings[a + 1][j], rings[a + 1][i]])
bm.faces.new(list(reversed(rings[0])))
bm.faces.new(rings[-1])
bm.to_mesh(me)
bm.free()
trunk = bpy.data.objects.new("Trunk", me)
link_to(trunk, col)
trunk.data.materials.append(get_mat("M_trunk", (0.36, 0.24, 0.14)))
parts.append(trunk)
# two branches
for i, (bx, bz, ang) in enumerate([(0.28, 1.55, 1.0), (-0.24, 1.8, -1.1)]):
    bpy.ops.mesh.primitive_cylinder_add(vertices=5, radius=0.06, depth=0.75, location=(bx, 0, bz), rotation=(0, ang, 0))
    br = link_to(bpy.context.active_object, col)
    br.name = f"Branch_{i}"
    br.data.materials.append(get_mat("M_trunk", (0.36, 0.24, 0.14)))
    parts.append(br)
print("tree trunk built")
