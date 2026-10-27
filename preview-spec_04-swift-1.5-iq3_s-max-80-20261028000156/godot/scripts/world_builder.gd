extends Node3D
# WorldBuilder: instantiates the whole diorama from godot/data/layout.json (PLAYBOOK Phase 6).
#
# Why data-driven: the layout is generated + validated by tools/gen_layout.py, so the world is
# reproducible and re-composing it never means dragging hundreds of nodes by hand.
#
# Conventions (AGENTS.md §2):
#   layout coordinates are Godot space (x, z), y up; props sit on the height field.
#   spec sizes are (x, y_depth, z_height) -> Godot size (x, z_height, y_depth).
#   collision comes from layout["assets"][id]["collision"] (the spec is the source of truth):
#     box / cylinder / capsule / box_pair -> built here;  mesh -> the <id>_collision node in the glb;
#     heightfield -> HeightMapShape3D from the heightmap json.
#   mode: body -> StaticBody3D, rigid -> RigidBody3D, sensor -> Area3D, none -> no collider.
#   a shape with no offset_m is centred at half its height (origin = bottom centre).
#
# Usage: put this script on a Node3D; it builds on _ready().
# Headless check: godot --headless --path godot -- --world-check  (prints WORLD_BUILD json)

const LAYOUT_PATH := "res://data/layout.json"
const COLL_SUFFIX := "_collision"
const SWAY_SCRIPT := preload("res://scripts/sway.gd")
const TERRAIN_SURFACE_SHADER := preload("res://shaders/terrain_surface.gdshader")
const TERRAIN_SURFACE_TEX_PATH := "res://data/terrain_surface.png"
const TERRAIN_HALF_M := 22.0
const TERRAIN_SURFACE_SLOTS := 5  # mesh slots 0..4 = grass x3 / road / plaza; 5..7 = soil bands
# Cloud meshes are the cloud shadow (PLAYBOOK 附錄 B: 地上看得到雲的影子). Measured 2026-10-28: with
# casting on, ground under a cloud sits at 69 % of sunlit light; a fake shadow-blob shader added ~0.
const CLOUD_CAST_SHADOW := true

var layout: Dictionary = {}
var grid: Dictionary = {}
var report: Dictionary = {}
var _col_cache: Dictionary = {}
var _placed_count := 0


func _ready() -> void:
	build()
	if OS.get_cmdline_user_args().has("--world-check"):
		print("WORLD_BUILD " + JSON.stringify(report))
		get_tree().quit(0 if (report.get("missing", []) as Array).is_empty() else 1)


func build() -> Dictionary:
	layout = _read_json(LAYOUT_PATH)
	grid = _read_json(str(layout.get("heightmap", "res://data/heightmap.json")))
	_placed_count = 0
	_col_cache = {}
	report = {"props": 0, "walls": 0, "clouds": 0, "coins": 0, "colliders": 0, "missing": [],
			"prop_y_min": 0.0, "prop_y_max": 0.0, "groups": {}}
	_build_terrain()
	_build_walls()
	_build_props()
	_build_landmarks()
	_build_clouds()
	return report


func _read_json(path: String) -> Dictionary:
	var text := FileAccess.get_file_as_string(path)
	if text == "":
		push_error("cannot read %s" % path)
		return {}
	var data = JSON.parse_string(text)
	if data == null or not (data is Dictionary):
		push_error("bad json: %s" % path)
		return {}
	return data as Dictionary


# ---------------------------------------------------------------- height field

func height_at(x: float, z: float) -> float:
	var n := int(grid.get("size", 0))
	if n == 0:
		return 0.0
	var step := float(grid.get("step", 1.0))
	var ox := float((grid.get("origin") as Array)[0])
	var oz := float((grid.get("origin") as Array)[1])
	var fi := (x - ox) / step
	var fj := (z - oz) / step
	var i0 := clampi(int(floor(fi)), 0, n - 1)
	var j0 := clampi(int(floor(fj)), 0, n - 1)
	var i1 := mini(i0 + 1, n - 1)
	var j1 := mini(j0 + 1, n - 1)
	var ti := fi - float(i0)
	var tj := fj - float(j0)
	var rows: Array = grid["rows"]
	var row0: Array = rows[j0]
	var row1: Array = rows[j1]
	var a := lerpf(float(row0[i0]), float(row0[i1]), ti)
	var b := lerpf(float(row1[i0]), float(row1[i1]), ti)
	return lerpf(a, b, tj)


func _build_terrain() -> void:
	var node := _instantiate("terrain")
	if node == null:
		return
	node.position = Vector3(0.0, float(grid.get("node_y", 0.0)), 0.0)
	add_child(node)
	for mi in _mesh_nodes(node):
		_apply_terrain_surface(mi)

	var n := int(grid["size"])
	var shape := HeightMapShape3D.new()
	shape.map_width = n
	shape.map_depth = n
	var data := PackedFloat32Array()
	data.resize(n * n)
	var rows: Array = grid["rows"]
	for j in n:
		for i in n:
			data[i + j * n] = float((rows[j] as Array)[i])
	shape.map_data = data
	var body := StaticBody3D.new()
	body.name = "TerrainCollision"
	var cs := CollisionShape3D.new()
	cs.shape = shape
	body.add_child(cs)
	add_child(body)
	add_to_group("terrain")
	report["colliders"] = int(report["colliders"]) + 1


func _apply_terrain_surface(mi: MeshInstance3D) -> void:
	# The Blender mesh paints each 1 m cell flat; sampling the baked palette map blends the same
	# colours, so the lawn does not read as a 1 m checkerboard (tools/gen_terrain_texture.py).
	var mesh := mi.mesh as ArrayMesh
	if mesh == null:
		return
	var palette := load(TERRAIN_SURFACE_TEX_PATH) as Texture2D
	if palette == null:
		report["missing"].append(TERRAIN_SURFACE_TEX_PATH)  # visuals fall back to the flat mesh colours
		return
	var mat := ShaderMaterial.new()
	mat.shader = TERRAIN_SURFACE_SHADER
	mat.set_shader_parameter("palette", palette)
	mat.set_shader_parameter("half_size", TERRAIN_HALF_M)
	for i in mini(TERRAIN_SURFACE_SLOTS, mesh.get_surface_count()):
		mi.set_surface_override_material(i, mat)  # Godot 4.7 name (not set_surface_material_override)


# ---------------------------------------------------------------- pieces

func _instantiate(asset: String) -> Node3D:
	var path := "res://assets/%s/%s.glb" % [asset, asset]
	if not ResourceLoader.exists(path):
		report["missing"].append(asset)
		return null
	var res = ResourceLoader.load(path, "", ResourceLoader.CACHE_MODE_REUSE)
	if res == null or not (res is PackedScene):
		report["missing"].append(asset)
		return null
	return (res as PackedScene).instantiate() as Node3D


func _place(node: Node3D, x: float, z: float, rot_deg: float, y_offset: float) -> float:
	var y := height_at(x, z) + y_offset
	node.position = Vector3(x, y, z)
	node.rotation_degrees = Vector3(0.0, rot_deg, 0.0)
	add_child(node)
	if _placed_count == 0:
		report["prop_y_min"] = y
		report["prop_y_max"] = y
	else:
		report["prop_y_min"] = min(float(report["prop_y_min"]), y)
		report["prop_y_max"] = max(float(report["prop_y_max"]), y)
	_placed_count += 1
	return y


func _build_walls() -> void:
	for w in layout.get("walls", []):
		var node := _instantiate(str(w["asset"]))
		if node == null:
			continue
		node.name = "wall_%d" % int(report["walls"])
		_place(node, float(w["x"]), float(w["z"]), float(w.get("rot_deg", 0.0)), 0.0)
		node.add_to_group("walls")
		_add_collision(node, _collision_of(str(w["asset"])))
		report["walls"] = int(report["walls"]) + 1


func _build_props() -> void:
	var fx: Dictionary = layout.get("fx", {})
	var sway_assets: Array = fx.get("sway_assets", []) as Array
	var winds: Dictionary = fx.get("wind", {}) as Dictionary
	for p in layout.get("props", []):
		var asset := str(p["asset"])
		var node := _instantiate(asset)
		if node == null:
			continue
		var label := "%s%d" % [str(p["id"]), int(p.get("n", 0))]
		node.name = label
		_place(node, float(p["x"]), float(p["z"]), float(p.get("rot_deg", 0.0)), float(p.get("y_offset", 0.0)))
		node.add_to_group(label)
		node.add_to_group(asset)
		node.set_meta("layout", p)
		_add_collision(node, _collision_of(asset))
		if asset in sway_assets:
			node.set_script(SWAY_SCRIPT)
			node.set_process(true)  # set_script on an in-tree node does not start _process by itself
			var w: Dictionary = winds.get(asset, {}) as Dictionary
			node.setup(float(w.get("ref_height_m", 2.0)), float(w.get("strength_m", 0.06)), asset == "tree")
		var key := str(p["id"])
		report["groups"][key] = int(report["groups"].get(key, 0)) + 1
		report["props"] = int(report["props"]) + 1


func _build_landmarks() -> void:
	for c in layout.get("coins", []):
		var node := _instantiate(str(c["asset"]))
		if node == null:
			continue
		node.name = "coin_%d" % int(c["index"])
		_place(node, float(c["x"]), float(c["z"]), 0.0, float(c.get("y_offset", 0.0)))
		node.add_to_group("coins")
		node.set_meta("coin_index", int(c["index"]))
		var area := _add_collision(node, _collision_of(str(c["asset"])), true)
		if area != null:
			area.set_meta("coin_index", int(c["index"]))  # the game reads the index from the sensor
		report["coins"] = int(report["coins"]) + 1

	var ep: Dictionary = layout.get("endpoint", {})
	if ep.is_empty():
		return
	var flag := _instantiate(str(ep["asset"]))
	if flag == null:
		return
	flag.name = "endpoint_flag"
	_place(flag, float(ep["x"]), float(ep["z"]), float(ep.get("rot_deg", 0.0)), float(ep.get("y_offset", 0.0)))
	flag.add_to_group("endpoint")
	_add_collision(flag, _collision_of(str(ep["asset"])), true)


func _build_clouds() -> void:
	for i in layout.get("clouds", []):
		var asset := str(i["asset"])
		var node := _instantiate(asset)
		if node == null:
			continue
		node.name = "cloud_%d" % int(report["clouds"])
		var at := Vector3(float(i["x"]), float(i["y"]), float(i["z"]))
		node.position = at
		var s := float(i.get("scale", 1.0))
		node.scale = Vector3(s, s, s)
		node.add_to_group("clouds")
		_set_cast(node, CLOUD_CAST_SHADOW)
		node.set_meta("drift_mps", float(i.get("drift_mps", 0.3)))
		add_child(node)
		report["clouds"] = int(report["clouds"]) + 1


func _set_cast(n: Node, on: bool) -> void:
	if n is GeometryInstance3D:
		(n as GeometryInstance3D).cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_ON if on else GeometryInstance3D.SHADOW_CASTING_SETTING_OFF
	for c in n.get_children():
		_set_cast(c, on)


# ---------------------------------------------------------------- collision

func _collision_of(asset: String) -> Dictionary:
	if not _col_cache.has(asset):
		var assets: Dictionary = layout.get("assets", {})
		var entry: Dictionary = assets.get(asset, {}) as Dictionary
		_col_cache[asset] = (entry.get("collision", {}) as Dictionary)
	return _col_cache[asset]


func _add_collision(node: Node3D, col: Dictionary, as_area := false) -> Node3D:
	# hide the baked collider mesh (it is a marker, never a visual), then build real physics
	for mi in _mesh_nodes(node):
		if String(mi.name).ends_with(COLL_SUFFIX):
			mi.visible = false
	var mode := str(col.get("mode", "none"))
	# passable props that must react to the player (bush) still need a sensor volume
	if mode == "none":
		if node.has_meta("layout") and _is_sway_trigger(node):
			_add_sway_sensor(node)
		return null

	var body: Node3D
	if as_area or mode == "sensor":
		body = Area3D.new()
	elif mode == "rigid":
		var rb := RigidBody3D.new()
		rb.mass = 6.0
		rb.can_sleep = true
		rb.angular_damp = 1.2
		rb.linear_damp = 0.5
		var pm := PhysicsMaterial.new()
		pm.friction = 0.9
		pm.bounce = 0.05
		rb.physics_material_override = pm
		body = rb
	else:
		body = StaticBody3D.new()
	body.name = String(node.name) + "_body"
	body.transform = node.transform
	_mirror_groups(node, body)
	add_child(body)

	var shape := str(col.get("shape", ""))
	if shape == "box_pair":
		for b in col.get("boxes", []):
			_add_shape(body, _box_shape(b["size_m"]), _godot_offset(b.get("offset_m", [0, 0, 0])))
	elif shape == "mesh":
		_add_mesh_collider(node, body)
	else:
		var built := _primitive_shape(shape, col)
		if built == null:
			return null
		_add_shape(body, built, _shape_center(shape, col))
	report["colliders"] = int(report["colliders"]) + 1
	return body


func _shape_center(shape: String, col: Dictionary) -> Vector3:
	if col.has("offset_m"):
		return _godot_offset(col["offset_m"])
	match shape:
		"box":
			return Vector3(0.0, float((col["size_m"] as Array)[2]) * 0.5, 0.0)
		"cylinder", "capsule":
			return Vector3(0.0, float(col["height_m"]) * 0.5, 0.0)
	return Vector3.ZERO


func _godot_offset(off: Array) -> Vector3:
	# spec offsets are Blender (x, y_depth, z_up) -> Godot (x, z_up, -y_depth)
	return Vector3(float(off[0]), float(off[2]), -float(off[1]))


func _box_shape(size_m: Array) -> BoxShape3D:
	var s := BoxShape3D.new()
	s.size = Vector3(float(size_m[0]), float(size_m[2]), float(size_m[1]))
	return s


func _primitive_shape(shape: String, col: Dictionary) -> Shape3D:
	match shape:
		"box":
			return _box_shape(col["size_m"])
		"cylinder":
			var c := CylinderShape3D.new()
			c.radius = float(col["radius_m"])
			c.height = float(col["height_m"])
			return c
		"capsule":
			var k := CapsuleShape3D.new()
			k.radius = float(col["radius_m"])
			k.height = float(col["height_m"])
			return k
	return null


func _add_shape(body: Node3D, shape: Shape3D, local_pos: Vector3) -> void:
	var cs := CollisionShape3D.new()
	cs.shape = shape
	cs.position = local_pos
	body.add_child(cs)


func _add_mesh_collider(node: Node3D, body: Node3D) -> void:
	# irregular shapes (rock): the glb carries <id>_collision -> convex hull, placed like the marker
	for mi in _mesh_nodes(node):
		if not String(mi.name).ends_with(COLL_SUFFIX) or mi.mesh == null:
			continue
		var verts: PackedVector3Array = mi.mesh.surface_get_arrays(0)[Mesh.ARRAY_VERTEX]
		if verts.is_empty():
			continue
		var shape := ConvexPolygonShape3D.new()
		shape.points = verts
		var cs := CollisionShape3D.new()
		cs.shape = shape
		cs.transform = body.global_transform.affine_inverse() * mi.global_transform
		body.add_child(cs)


func _mesh_nodes(root: Node) -> Array[MeshInstance3D]:
	var out: Array[MeshInstance3D] = []
	var stack: Array[Node] = [root]
	while not stack.is_empty():
		var n: Node = stack.pop_back()
		if n is MeshInstance3D:
			out.append(n as MeshInstance3D)
		for c in n.get_children():
			stack.append(c)
	return out


func _mirror_groups(node: Node, body: Node) -> void:
	# so a slide collision / area overlap can tell "this was tree14" (the visual node is the label)
	for g in node.get_groups():
		body.add_to_group(g)
	body.set_meta("visual", node.get_path())
	body.set_meta("asset", node.get_meta("layout", {}).get("asset", "") if node.has_meta("layout") else "")


func _is_sway_trigger(node: Node3D) -> bool:
	var fx: Dictionary = layout.get("fx", {}) as Dictionary
	var p: Dictionary = node.get_meta("layout", {})
	return str(p.get("asset", "")) in (fx.get("sensor_assets", []) as Array)


func _add_sway_sensor(node: Node3D) -> void:
	# passable props (bush) need a sensor volume so the player can feel they went through it
	var p: Dictionary = node.get_meta("layout", {})
	var dims: Dictionary = (layout.get("assets", {}) as Dictionary).get(str(p.get("asset", "")), {}).get("dims_m", {})
	if dims.is_empty():
		return
	var area := Area3D.new()
	area.name = String(node.name) + "_sensor"
	area.transform = node.transform
	area.monitoring = false  # the player's sense area does the detecting; this volume is the target
	var cs := CollisionShape3D.new()
	var cyl := CylinderShape3D.new()
	cyl.radius = float(dims.get("x", 1.0)) * 0.5
	cyl.height = float(dims.get("z", 1.0))
	cs.shape = cyl
	cs.position = Vector3(0.0, cyl.height * 0.5, 0.0)
	area.add_child(cs)
	_mirror_groups(node, area)
	add_child(area)
	report["sway_sensors"] = int(report.get("sway_sensors", 0)) + 1
