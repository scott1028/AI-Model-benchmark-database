extends SceneTree
# 用法：godot --headless --path game -s res://tools/inspect_assets.gd -- res://assets/a.glb ...

func _init() -> void:
	var result := {}
	for path in OS.get_cmdline_user_args():
		result[path] = _inspect(path)
	print("INSPECT_JSON:" + JSON.stringify(result))
	quit()

func _inspect(path: String) -> Dictionary:
	if not ResourceLoader.exists(path):
		return {"error": "not_found_or_not_imported"}
	var packed := load(path) as PackedScene
	if packed == null:
		return {"error": "load_failed"}
	var root := packed.instantiate()
	var state := {"aabb": AABB(), "first": true, "collision": false, "anims": []}
	_walk(root, Transform3D.IDENTITY, state)
	root.free()
	if state["first"]:
		return {"error": "no_mesh"}
	var box: AABB = state["aabb"]
	return {
		"aabb_min": [box.position.x, box.position.y, box.position.z],
		"aabb_size": [box.size.x, box.size.y, box.size.z],
		"has_collision": state["collision"],
		"anims": state["anims"],
	}

func _walk(node: Node, parent_xf: Transform3D, state: Dictionary) -> void:
	var xf := parent_xf
	if node is Node3D:
		xf = parent_xf * (node as Node3D).transform
	if node is MeshInstance3D and (node as MeshInstance3D).mesh != null:
		var box: AABB = xf * (node as MeshInstance3D).mesh.get_aabb()
		if state["first"]:
			state["aabb"] = box
			state["first"] = false
		else:
			var cur: AABB = state["aabb"]
			state["aabb"] = cur.merge(box)
	if node is CollisionObject3D or node is CollisionShape3D:
		state["collision"] = true
	if node is AnimationPlayer:
		for anim_name in (node as AnimationPlayer).get_animation_list():
			state["anims"].append(String(anim_name))
	for child in node.get_children():
		_walk(child, xf, state)
