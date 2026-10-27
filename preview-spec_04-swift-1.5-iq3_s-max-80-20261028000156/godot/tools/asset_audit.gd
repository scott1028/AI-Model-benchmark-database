extends SceneTree
# Godot-side asset audit: loads the imported .glb and measures it (no judgement here;
# comparison happens in tools/check_asset.py).
#
# Convention (AGENTS.md §2): Blender (x, y, z-up) -> Godot (x, z, -y).
#   spec x = Godot size.x, spec y (depth) = Godot size.z, spec z (height) = Godot size.y
#   origin "bottom center" => AABB min.y == 0 and XZ centred on the origin
# Mesh nodes named *_collision are colliders: excluded from visual size/tris.
#
# Measurement runs on the first processed frame: node global transforms are identity
# inside _init(), which silently collapsed every bbox to the raw mesh bounds.
#
# Usage: godot --headless --path godot --script res://tools/asset_audit.gd -- --id <id> --json-out <path>

const COL_SUFFIX := "_collision"  # avoids Godot's _col auto-converter; WorldBuilder builds bodies

var id := ""
var json_out := ""
var subject: Node = null
var load_error := ""
var visual_nodes: Array[Node3D] = []
var col_nodes: Array[Node3D] = []
var all_names: Array[String] = []


func _init() -> void:
	var argv := OS.get_cmdline_user_args()
	var i := 0
	while i < argv.size():
		if argv[i] == "--id":
			id = argv[i + 1]
			i += 2
		elif argv[i] == "--json-out":
			json_out = argv[i + 1]
			i += 2
		else:
			i += 1

	var path := "res://assets/%s/%s.glb" % [id, id]
	if not ResourceLoader.exists(path):
		load_error = "glb not found / not imported: " + path
		return
	var res = ResourceLoader.load(path, "", ResourceLoader.CACHE_MODE_REPLACE)
	if res == null or not (res is PackedScene):
		load_error = "load failed: " + path
		return
	subject = (res as PackedScene).instantiate(PackedScene.GEN_EDIT_STATE_MAIN)
	get_root().add_child(subject)


func _process(_delta: float) -> bool:
	# first frame done: global transforms are now valid
	var report := {"id": id, "path": "res://assets/%s/%s.glb" % [id, id], "warnings": [] as Array[String]}
	if load_error != "":
		_emit(dict_merge(report, {"loaded": false, "error": load_error}))
		return true
	if subject == null:
		_emit(dict_merge(report, {"loaded": false, "error": "not instantiated"}))
		return true

	_collect(subject)
	var vb := _bbox(visual_nodes)
	var tris := 0
	var mats := {}
	for n in visual_nodes:
		if not (n is MeshInstance3D):
			continue
		var mi := n as MeshInstance3D
		var m := mi.mesh
		if m == null:
			report["warnings"].append("empty mesh node: " + String(mi.name))
			continue
		for s in m.get_surface_count():
			tris += _surface_tri_count(m, s)
			var mat = mi.get_active_material(s)
			if mat is StandardMaterial3D:
				var sm := mat as StandardMaterial3D
				mats[String(sm.resource_name)] = "#" + sm.albedo_color.to_html(false).to_upper()
			elif mat != null:
				mats[String(mat.resource_name)] = "n/a"

	var col_tris := 0
	var col_names: Array[String] = []
	var meta_keys: Array[String] = []
	var col_flags: Array[bool] = []
	for n in col_nodes:
		col_names.append(String(n.name))
		var flagged := false
		for k in n.get_meta_list():
			var key := String(k)
			meta_keys.append(key)
			var v = n.get_meta(k)
			if key == "crcollider":
				flagged = true
			elif typeof(v) == TYPE_DICTIONARY and (v as Dictionary).has("crcollider"):
				flagged = true
			elif typeof(v) == TYPE_STRING and String(v).contains("crcollider"):
				flagged = true
		col_flags.append(flagged)
		if n is MeshInstance3D and (n as MeshInstance3D).mesh != null:
			for s in (n as MeshInstance3D).mesh.get_surface_count():
				col_tris += _surface_tri_count((n as MeshInstance3D).mesh, s)

	var anims: Array = []
	var ap := _find_anim_player(subject)
	if ap != null:
		for a in ap.get_animation_list():
			anims.append({"name": String(a), "length": ap.get_animation(a).length})

	var nodes_detail: Array = []
	for n in visual_nodes:
		if not (n is MeshInstance3D) or (n as MeshInstance3D).mesh == null:
			continue
		var g := _global_vertex_aabb(n as MeshInstance3D)
		nodes_detail.append({"name": String(n.name),
			"dims_godot": {"x": g.size.x, "y": g.size.y, "z": g.size.z},
			"pos": [g.position.x, g.position.y, g.position.z]})

	report["loaded"] = true
	report["node_count"] = all_names.size()
	report["node_names"] = all_names.slice(0, 24)
	report["nodes_detail"] = nodes_detail
	report["visual_mesh_count"] = visual_nodes.filter(func(n): return n is MeshInstance3D).size()
	report["tris"] = tris
	report["tris_col"] = col_tris
	report["dims_godot"] = {"x": vb.size.x, "y": vb.size.y, "z": vb.size.z}
	report["dims_blender_equivalent"] = {"x": vb.size.x, "y": vb.size.z, "z": vb.size.y}
	report["origin"] = {"min_y": vb.position.y,
		"xy_center_offset": [vb.position.x + vb.size.x * 0.5, vb.position.z + vb.size.z * 0.5]}
	report["materials"] = mats.keys().map(func(k): return {"name": k, "color": mats[k]})
	report["collision_nodes"] = col_names
	report["collision_meta_keys"] = meta_keys
	report["collision_flags"] = col_flags
	report["collision_meta_ok"] = col_nodes.size() > 0 and not col_flags.has(false)
	report["animations"] = anims
	_emit(report)
	return true


func _collect(root: Node) -> void:
	var stack: Array[Node] = [root]
	while not stack.is_empty():
		var n: Node = stack.pop_back()
		all_names.append(String(n.name))
		if n is Node3D:
			if String(n.name).ends_with(COL_SUFFIX):
				col_nodes.append(n as Node3D)
			else:
				visual_nodes.append(n as Node3D)
		for c in n.get_children():
			stack.append(c)


func _bbox(nodes: Array[Node3D]) -> AABB:
	var total := AABB()
	var inited := false
	for n in nodes:
		if not (n is MeshInstance3D):
			continue
		if (n as MeshInstance3D).mesh == null:
			continue
		var aabb := _global_vertex_aabb(n as MeshInstance3D)
		total = aabb if not inited else total.merge(aabb)
		inited = true
	return total


func _global_vertex_aabb(mi: MeshInstance3D) -> AABB:
	# Real vertices, not Mesh.get_aabb(): a rotated node's mesh AABB inflates (AABB of AABB).
	var xf := mi.global_transform
	var m := mi.mesh
	var total := AABB()
	var inited := false
	for s in m.get_surface_count():
		var arrays: Array = m.surface_get_arrays(s)
		if arrays.size() <= Mesh.ARRAY_VERTEX:
			continue
		var verts: PackedVector3Array = arrays[Mesh.ARRAY_VERTEX]
		for v in verts:
			var p := AABB(xf * v, Vector3.ZERO)
			total = p if not inited else total.merge(p)
			inited = true
	return total


func _surface_tri_count(m: Mesh, s: int) -> int:
	var idx := 0
	var vl := 0
	if m is ArrayMesh:
		idx = int((m as ArrayMesh).surface_get_array_index_len(s))
		vl = int((m as ArrayMesh).surface_get_array_len(s))
	if idx == 0 and vl == 0:
		var arrays: Array = m.surface_get_arrays(s)
		var verts: PackedVector3Array = arrays[Mesh.ARRAY_VERTEX] if arrays.size() > Mesh.ARRAY_VERTEX else PackedVector3Array()
		vl = verts.size()
	return maxi(idx, vl) / 3


func _find_anim_player(root: Node) -> AnimationPlayer:
	var stack: Array[Node] = [root]
	while not stack.is_empty():
		var n: Node = stack.pop_back()
		if n is AnimationPlayer:
			return n as AnimationPlayer
		for c in n.get_children():
			stack.append(c)
	return null


func dict_merge(a: Dictionary, b: Dictionary) -> Dictionary:
	var out := a.duplicate(true)
	for k in b:
		out[k] = b[k]
	return out


func _emit(report: Dictionary) -> void:
	var text := JSON.stringify(report, "\t")
	print("ASSET_AUDIT " + JSON.stringify(report))
	if json_out != "":
		var f := FileAccess.open(json_out, FileAccess.WRITE)
		if f != null:
			f.store_string(text + "\n")
			f.close()
