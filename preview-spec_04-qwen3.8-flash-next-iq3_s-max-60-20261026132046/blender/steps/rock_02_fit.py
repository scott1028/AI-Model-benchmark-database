import bpy

# rock step 2: bake transforms first, then fit bbox to spec [1.2,1.0,0.9],
# origin bottom-center (object at world origin, mesh zmin=0)
rock = bpy.data.objects["Rock"]
bpy.ops.object.select_all(action="DESELECT")
rock.select_set(True)
bpy.context.view_layer.objects.active = rock
bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
me = rock.data
xs = [v.co.x for v in me.vertices]
ys = [v.co.y for v in me.vertices]
zs = [v.co.z for v in me.vertices]
sx = max(xs) - min(xs)
sy = max(ys) - min(ys)
sz = max(zs) - min(zs)
for v in me.vertices:
    v.co.x = (v.co.x - (max(xs) + min(xs)) / 2) * (1.2 / sx)
    v.co.y = (v.co.y - (max(ys) + min(ys)) / 2) * (1.0 / sy)
    v.co.z = (v.co.z - min(zs)) * (0.9 / sz)
for p in me.polygons:
    p.use_smooth = False
me.update()
print("rock fitted dims:", tuple(round(d, 3) for d in rock.dimensions), "loc:", tuple(rock.location))
