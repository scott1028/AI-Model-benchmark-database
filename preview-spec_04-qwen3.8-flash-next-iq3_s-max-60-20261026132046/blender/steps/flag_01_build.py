import bpy

exec(open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/blender/steps/lib.py").read())

# flag: stone base + wooden pole + swallowtail pennant
col = asset_collection("flag")
parts = []
bpy.ops.mesh.primitive_cube_add(size=1, location=(0, 0, 0.12))
base = link_to(bpy.context.active_object, col)
base.name = "FlagBase"
base.scale = (0.42, 0.42, 0.24)
bpy.ops.object.transform_apply(scale=True)
base.data.materials.append(get_mat("M_flagbase", (0.42, 0.46, 0.52)))
parts.append(base)
bpy.ops.mesh.primitive_cylinder_add(vertices=8, radius=0.05, depth=2.75, location=(0, 0, 1.5))
pole = link_to(bpy.context.active_object, col)
pole.name = "FlagPole"
pole.data.materials.append(get_mat("M_pole", (0.45, 0.30, 0.16)))
parts.append(pole)
bpy.ops.mesh.primitive_uv_sphere_add(segments=8, ring_count=6, radius=0.09, location=(0, 0, 2.93))
finial = link_to(bpy.context.active_object, col)
finial.name = "FlagFinial"
finial.data.materials.append(get_mat("M_coin", (0.85, 0.62, 0.12), 0.35))
parts.append(finial)
# pennant: extruded V-notch triangle facing -X... flag hangs to +X from pole top
mesh = bpy.data.meshes.new("FlagCloth")
verts = [(-0.0, -0.30, 2.62), (-0.0, 0.30, 2.62), (1.05, 0.30, 2.05), (0.75, 0.0, 2.32), (1.05, -0.30, 2.05)]
faces = [(0, 1, 2, 3), (0, 3, 4)]
mesh.from_pydata(verts, [], faces)
mesh.update()
cloth = bpy.data.objects.new("FlagCloth", mesh)
link_to(cloth, col)
sol = cloth.modifiers.new("Sol", "SOLIDIFY")
sol.thickness = 0.03
bpy.ops.object.select_all(action="DESELECT")
cloth.select_set(True)
bpy.context.view_layer.objects.active = cloth
bpy.ops.object.modifier_apply(modifier="Sol")
cloth.data.materials.append(get_mat("M_cloth", (0.78, 0.25, 0.22)))
parts.append(cloth)
fit_bbox(parts, [1.1, 0.6, 3.0], col)
print("flag built:", world_bbox(parts)[0])
