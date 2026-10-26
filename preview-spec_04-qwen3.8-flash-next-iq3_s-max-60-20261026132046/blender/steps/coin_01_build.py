import bpy
import math

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# coin: flat disc + rim + emboss star (flat low-poly gold)
col = asset_collection("coin")
parts = []
bpy.ops.mesh.primitive_cylinder_add(vertices=16, radius=0.30, depth=0.10, location=(0, 0, 0.05))
disc = link_to(bpy.context.active_object, col)
disc.name = "CoinDisc"
disc.data.materials.append(get_mat("M_coin", (0.85, 0.62, 0.12), 0.35))
parts.append(disc)
bpy.ops.mesh.primitive_cylinder_add(vertices=16, radius=0.24, depth=0.115, location=(0, 0, 0.05))
rim = link_to(bpy.context.active_object, col)
rim.name = "CoinRim"
rim.data.materials.append(get_mat("M_coin_dark", (0.62, 0.44, 0.08), 0.4))
parts.append(rim)
# 6-point star emboss on top face
bpy.ops.mesh.primitive_cylinder_add(vertices=6, radius=0.14, depth=0.02, location=(0, 0, 0.105))
star = link_to(bpy.context.active_object, col)
star.name = "CoinStar"
star.data.materials.append(get_mat("M_coin", (0.85, 0.62, 0.12), 0.35))
parts.append(star)
for p in parts:
    for pl in p.data.polygons:
        pl.use_smooth = False
fit_bbox(parts, [0.36, 0.36, 0.07], col)
print("coin built:", world_bbox(parts)[0])
