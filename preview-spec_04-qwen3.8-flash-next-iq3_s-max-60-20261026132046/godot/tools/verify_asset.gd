extends SceneTree
# Headless asset verification: loads an imported glb and reports world AABB,
# triangles, animation names, and collision nodes.
# Usage: godot --headless --path godot -s res://tools/verify_asset.gd -- res://assets/coin.glb


func _init() -> void:
	var args := OS.get_cmdline_user_args()
	if args.is_empty():
		print("VERIFY_JSON {\"error\":\"missing glb path arg\"}")
		quit(1)
		return
	var path: String = args[0]
	var res := ResourceLoader.load(path)
	if res == null:
		print("VERIFY_JSON " + JSON.stringify({"error": "load failed", "path": path}))
		quit(1)
		return
	var scene := res as PackedScene
	if scene == null:
		print("VERIFY_JSON " + JSON.stringify({"error": "not a PackedScene", "path": path}))
		quit(1)
		return
	var inst := scene.instantiate()
	root.add_child(inst)

	var ctx := {
		"vmin": [1e9, 1e9, 1e9],
		"vmax": [-1e9, -1e9, -1e9],
		"tris": 0,
		"anims": {},
		"collision_nodes": [],
		"node_names": [],
	}
	_scan(inst, Transform3D(), ctx)

	var vmin := Vector3(ctx.vmin[0], ctx.vmin[1], ctx.vmin[2])
	var vmax := Vector3(ctx.vmax[0], ctx.vmax[1], ctx.vmax[2])
	var dims := vmax - vmin
	var out := {
		"path": path,
		"bbox_min": [vmin.x, vmin.y, vmin.z],
		"bbox_max": [vmax.x, vmax.y, vmax.z],
		"dims": [dims.x, dims.y, dims.z],
		"tris": ctx.tris,
		"animations": ctx.anims.keys(),
		"collision_nodes": ctx.collision_nodes,
		"node_names": ctx.node_names,
	}
	print("VERIFY_JSON " + JSON.stringify(out))
	quit(0)


func _scan(n: Node, xform: Transform3D, ctx: Dictionary) -> void:
	var t := xform
	if n is Node3D:
		t = xform * (n as Node3D).transform
	if n is MeshInstance3D:
		var mi := n as MeshInstance3D
		var m := mi.mesh
		if m != null:
			for i in m.get_surface_count():
				var arrays := m.surface_get_arrays(i)
				var idx = arrays[Mesh.ARRAY_INDEX]
				var vt = arrays[Mesh.ARRAY_VERTEX]
				if idx != null:
					ctx.tris = int(ctx.tris) + int(idx.size()) / 3
				elif vt != null:
					ctx.tris = int(ctx.tris) + int(vt.size()) / 3
				var aabb := mi.get_aabb()
				for cx in [aabb.position, aabb.position + aabb.size]:
					var p: Vector3 = t * cx
					for k in 3:
						ctx.vmin[k] = min(ctx.vmin[k], p[k])
						ctx.vmax[k] = max(ctx.vmax[k], p[k])
	elif n is CollisionShape3D or n is StaticBody3D or String(n.name).begins_with("col_"):
		ctx.collision_nodes.append(String(n.name))
	if n is AnimationPlayer:
		for a in (n as AnimationPlayer).get_animation_list():
			ctx.anims[a] = true
	ctx.node_names.append(String(n.name) + ":" + n.get_class())
	for c in n.get_children():
		_scan(c, t, ctx)
