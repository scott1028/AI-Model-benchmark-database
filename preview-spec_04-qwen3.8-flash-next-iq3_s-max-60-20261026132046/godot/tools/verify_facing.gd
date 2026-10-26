extends SceneTree

# Where do the cyan face/eye materials sit in the imported robot?
# Prints mean local axis coordinate of face-colored verts vs whole mesh.

func _init() -> void:
	var inst: Node = (load("res://assets/robot.glb") as PackedScene).instantiate()
	var face := PackedVector3Array()
	var all_c := PackedVector3Array()
	var all_w := PackedVector3Array()
	_walk(inst, Transform3D(), face, all_c, all_w)
	var cm := _mean(face if face.size() > 0 else all_c)
	var mc := _mean(all_c)
	var mw := _mean(all_w)
	print("face_verts=", face.size())
	print("face_mean=", "%.3f %.3f %.3f" % [cm.x, cm.y, cm.z])
	print("cyan_mean=", "%.3f %.3f %.3f" % [mc.x, mc.y, mc.z])
	print("warm_mean=", "%.3f %.3f %.3f" % [mw.x, mw.y, mw.z])
	var cx: float = cm.x - mc.x
	var cz: float = cm.z - mc.z
	print("FACING=", "x_offset=%.3f z_offset=%.3f" % [cx, cz])
	var ab := _bbox(all_c)
	var fb := _bbox(face)
	print("ALL bbox min=%.2f %.2f %.2f max=%.2f %.2f %.2f" % [ab[0].x, ab[0].y, ab[0].z, ab[1].x, ab[1].y, ab[1].z])
	print("FACE bbox min=%.2f %.2f %.2f max=%.2f %.2f %.2f" % [fb[0].x, fb[0].y, fb[0].z, fb[1].x, fb[1].y, fb[1].z])
	quit(0)


func _walk(n: Node, xf: Transform3D, face: PackedVector3Array, all_c: PackedVector3Array, all_w: PackedVector3Array) -> void:
	if n is MeshInstance3D:
		var m: Mesh = (n as MeshInstance3D).mesh
		for s in m.get_surface_count():
			var arr := m.surface_get_arrays(s)
			var verts: PackedVector3Array = arr[Mesh.ARRAY_VERTEX]
			var mat: Material = m.surface_get_material(s)
			var col := Color(1, 1, 1, 1)
			if mat is StandardMaterial3D:
				col = (mat as StandardMaterial3D).albedo_color
			for v in verts:
				var p := xf * v
				all_c.append(p)
				if col.b > 0.5 and col.b > col.r and col.g > col.r:
					face.append(p)
				elif col.r > 0.7 and col.g > 0.6:
					all_w.append(p)
	for c in n.get_children():
		_walk(c, xf * (c as Node3D).transform, face, all_c, all_w)


func _bbox(a: PackedVector3Array) -> Array:
	if a.size() == 0:
		return [Vector3.ZERO, Vector3.ZERO]
	var lo := a[0]
	var hi := a[0]
	for v in a:
		lo = Vector3(minf(lo.x, v.x), minf(lo.y, v.y), minf(lo.z, v.z))
		hi = Vector3(maxf(hi.x, v.x), maxf(hi.y, v.y), maxf(hi.z, v.z))
	return [lo, hi]


func _mean(a: PackedVector3Array) -> Vector3:
	var s := Vector3.ZERO
	for v in a:
		s += v
	return s / maxf(1.0, float(a.size()))
