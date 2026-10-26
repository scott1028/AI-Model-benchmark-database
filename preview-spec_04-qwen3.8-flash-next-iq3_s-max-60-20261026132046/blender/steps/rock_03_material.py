import bpy

# rock step 3: flat material (gray-blue), no textures
if "M_rock" in bpy.data.materials:
    bpy.data.materials.remove(bpy.data.materials["M_rock"])
mat = bpy.data.materials.new("M_rock")
mat.use_nodes = True
bsdf = next(n for n in mat.node_tree.nodes if n.type == "BSDF_PRINCIPLED")
bsdf.inputs["Base Color"].default_value = (0.42, 0.46, 0.52, 1.0)
bsdf.inputs["Roughness"].default_value = 0.85
bsdf.inputs["Metallic"].default_value = 0.0
rock = bpy.data.objects["Rock"]
rock.data.materials.clear()
rock.data.materials.append(mat)
print("rock material set")
