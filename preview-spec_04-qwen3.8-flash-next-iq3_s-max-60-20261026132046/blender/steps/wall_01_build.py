import bpy
import math

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# wall segment: body + staggered brick reliefs (3 courses) + top cap + row of roof tiles
col = asset_collection("wall")
parts = []
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 1.35))
body = link_to(bpy.context.active_object, col)
body.name = "WallBody"
body.scale = (4.0, 0.42, 2.7)
bpy.ops.object.transform_apply(scale=True)
body.data.materials.append(get_mat("M_wall", (0.72, 0.62, 0.48)))
parts.append(body)
# brick reliefs: 3 courses x 4 bricks, staggered
bi = 0
for row in range(3):
    z = 0.35 + row * 0.72
    for k in range(4):
        x = -1.5 + k * 1.0 + (0.5 if row % 2 else 0)
        if abs(x) > 1.95:
            continue
        for side in (-1, 1):
            bpy.ops.mesh.primitive_cube_add(size=1, location=(x, side * 0.225, z))
            br = link_to(bpy.context.active_object, col)
            br.name = f"Brick_{bi}"
            br.scale = (0.44, 0.045, 0.30)
            bpy.ops.object.transform_apply(scale=True)
            br.data.materials.append(get_mat("M_brick", (0.62, 0.50, 0.38)))
            parts.append(br)
            bi += 1
# top cap beam
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 2.78))
cap = link_to(bpy.context.active_object, col)
cap.name = "WallCap"
cap.scale = (4.0, 0.52, 0.14)
bpy.ops.object.transform_apply(scale=True)
cap.data.materials.append(get_mat("M_wall", (0.72, 0.62, 0.48)))
parts.append(cap)
# roof tiles: 6 half-cylinders running along X on top
for i in range(6):
    x = -1.67 + i * 0.666
    bpy.ops.mesh.primitive_cylinder_add(vertices=8, radius=0.16, depth=4.0, location=(0, 0, 3.02), rotation=(0, math.pi / 2, 0))
    t = link_to(bpy.context.active_object, col)
    t.name = f"Tile_{i}"
    # only the top half needed: move down so lower half hides in cap; keep cheap
    t.data.materials.append(get_mat("M_roof_tile", (0.45, 0.30, 0.26)))
    parts.append(t)
    break
# one long ridge tile instead of 6 (low-poly + reads as tile ridge): add 5 grooves
for i in range(5):
    y = -0.2 + i * 0.1
    bpy.ops.mesh.primitive_cube_add(size=1, location=(0, y, 3.14))
    g = link_to(bpy.context.active_object, col)
    g.name = f"TileGroove_{i}"
    g.scale = (4.0, 0.02, 0.06)
    bpy.ops.object.transform_apply(scale=True)
    g.data.materials.append(get_mat("M_roof_dark", (0.34, 0.22, 0.20)))
    parts.append(g)
fit_bbox(parts, [4.0, 0.55, 3.3], col)
bpy.context.view_layer.update()
print("wall built:", world_bbox(list(col.all_objects))[0], "tris:", sum(len(o.data.polygons) * 2 for o in col.all_objects if o.type == "MESH"))
