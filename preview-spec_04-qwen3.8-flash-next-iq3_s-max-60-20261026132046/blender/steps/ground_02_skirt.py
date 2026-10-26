import bpy
import bmesh
import json
import math

ROOT = "/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046"
exec(open(ROOT + "/blender/steps/lib.py").read())

layout = json.load(open(ROOT + "/godot/data/layout.json"))
W = layout["world"]
ISLE = W["island_size"]
DEPTH = W["island_depth"]
CELLS = 40
STEP = ISLE / CELLS
half = ISLE / 2

top = bpy.data.objects["GroundTop"]
bm = bmesh.new()
bm.from_mesh(top.data)
bm.verts.ensure_lookup_table()


def pts_on_edge():
    pts = {}
    for v in bm.verts:
        x, y, z = v.co
        onx = abs(abs(x) - half) < 1e-4
        ony = abs(abs(y) - half) < 1e-4
        if onx or ony:
            pts[(round(x, 3), round(y, 3))] = v
    return pts


pts = pts_on_edge()
ring0 = []
for i in range(CELLS + 1):
    ring0.append(pts[(round(-half + i * STEP, 3), round(half, 3))])
for j in range(1, CELLS + 1):
    ring0.append(pts[(round(half, 3), round(half - j * STEP, 3))])
for i in range(1, CELLS + 1):
    ring0.append(pts[(round(half - i * STEP, 3), round(-half, 3))])
for j in range(1, CELLS):
    ring0.append(pts[(round(-half, 3), round(-half + j * STEP, 3))])

ring1 = [bm.verts.new((v.co.x * 0.996, v.co.y * 0.996, v.co.z - 0.35)) for v in ring0]
ring2 = [bm.verts.new((v.co.x - math.copysign(1.4, v.co.x), v.co.y - math.copysign(1.4, v.co.y), -1.8)) for v in ring1]
ring3 = [bm.verts.new((v.co.x - math.copysign(2.6, v.co.x), v.co.y - math.copysign(2.6, v.co.y), -DEPTH)) for v in ring2]

n = len(ring0)
for k in range(n):
    a, b = k, (k + 1) % n
    bm.faces.new([ring0[a], ring0[b], ring1[b], ring1[a]])
    bm.faces.new([ring1[a], ring1[b], ring2[b], ring2[a]])
    bm.faces.new([ring2[a], ring2[b], ring3[b], ring3[a]])
bm.faces.new(list(reversed(ring3)))
bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
bm.to_mesh(top.data)
bm.free()
print("skirt built; total polys:", len(top.data.polygons))
