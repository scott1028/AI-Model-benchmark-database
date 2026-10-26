extends Node

# Builds the whole diorama from res://data/layout.json so the layout is
# reproducible and editable without touching hundreds of nodes by hand.

# ground instance offset is derived at runtime: GLB local top max must map
# onto the max terrain height from heights.json (hardcoding it was off 0.53 m)

var layout: Dictionary
var heights: Dictionary
var root: Node


func build(a_root: Node) -> Dictionary:
	root = a_root
	layout = JSON.parse_string(FileAccess.get_file_as_string("res://data/layout.json"))
	heights = JSON.parse_string(FileAccess.get_file_as_string("res://data/heights.json"))
	var world := Node3D.new()
	world.name = "World"
	root.add_child(world)

	var refs := {
		"ground_body": null,
		"coins": [],
		"flag": null,
		"trees": [],
		"bushes": [],
		"barrels": [],
		"clouds": [],
	}

	_world_env(world)
	_sun(world)
	refs.ground_body = _ground(world)
	_walls(world)
	for p: Dictionary in layout.props:
		_prop(world, p, refs)
	for c: Dictionary in layout.coins:
		_coin(world, c, refs)
	_flag(world, refs)
	return refs


func height_at(x: float, z: float) -> float:
	# heights.json: columns over bx = -22 + i*step, rows over by = -22 + j*step (Blender frame)
	# Godot: x = bx, z = -by
	var step: float = heights.step
	var base: float = heights.min
	var bx: float = x
	var by: float = -z
	var fi: float = (bx - base) / step
	var fj: float = (by - base) / step
	var cells: Array = heights.cells
	var n: int = cells.size()
	var i0: int = clampi(int(floor(fi)), 0, n - 2)
	var j0: int = clampi(int(floor(fj)), 0, n - 2)
	var u: float = clampf(fi - i0, 0.0, 1.0)
	var v: float = clampf(fj - j0, 0.0, 1.0)
	var a: float = cells[i0][j0]
	var b: float = cells[i0 + 1][j0]
	var c: float = cells[i0][j0 + 1]
	var d: float = cells[i0 + 1][j0 + 1]
	return lerp(lerp(a, b, u), lerp(c, d, u), v)


func _world_env(world: Node3D) -> void:
	var we := WorldEnvironment.new()
	we.name = "WorldEnv"
	var env := Environment.new()
	env.background_mode = Environment.BG_SKY
	var sky := Sky.new()
	var mat := ProceduralSkyMaterial.new()
	mat.sky_top_color = Color(0.45, 0.62, 0.85)
	mat.sky_horizon_color = Color(0.88, 0.82, 0.70)
	mat.ground_bottom_color = Color(0.35, 0.32, 0.30)
	mat.ground_horizon_color = Color(0.88, 0.82, 0.70)
	sky.sky_material = mat
	env.sky = sky
	env.ambient_light_source = Environment.AMBIENT_SOURCE_SKY
	env.ambient_light_energy = 0.7
	env.tonemap_mode = Environment.TONE_MAPPER_FILMIC
	env.glow_enabled = true
	env.glow_intensity = 0.25
	we.environment = env
	world.add_child(we)


func _sun(world: Node3D) -> void:
	var sun := DirectionalLight3D.new()
	sun.name = "Sun"
	sun.light_color = Color(1.0, 0.87, 0.66)
	sun.light_energy = 1.7
	sun.rotation_degrees = Vector3(-52, 118, 0)
	sun.shadow_enabled = true
	sun.directional_shadow_max_distance = 70.0
	world.add_child(sun)


func _ground(world: Node3D) -> StaticBody3D:
	var glb: PackedScene = load("res://assets/ground.glb")
	var inst := glb.instantiate()
	inst.name = "GroundInstance"
	var body := StaticBody3D.new()
	body.name = "GroundBody"
	body.add_child(inst)
	var shape_node := CollisionShape3D.new()
	var trimesh := ConcavePolygonShape3D.new()
	var faces := PackedVector3Array()
	for mi: MeshInstance3D in _mesh_instances(inst):
		var m: Mesh = mi.mesh
		var g: Transform3D = _global_xf(mi, inst)
		for s in m.get_surface_count():
			var arr := m.surface_get_arrays(s)
			var verts: PackedVector3Array = arr[Mesh.ARRAY_VERTEX]
			var idx: PackedInt32Array = arr[Mesh.ARRAY_INDEX]
			if idx.size() > 0:
				for k in range(0, idx.size(), 3):
					faces.append(g * verts[idx[k]])
					faces.append(g * verts[idx[k + 1]])
					faces.append(g * verts[idx[k + 2]])
			else:
				for k in range(0, verts.size(), 3):
					faces.append(g * verts[k])
					faces.append(g * verts[k + 1])
					faces.append(g * verts[k + 2])
	trimesh.set_faces(faces)
	shape_node.shape = trimesh
	# low ground friction: robot drive model owns its motion; barrels still
	# slow down (barrel material 0.6 pairs ~0.32 effective)
	body.physics_material_override = _phys_mat(0.05, 0.0)
	body.add_child(shape_node)
	var ground_y := _ground_offset(inst)
	inst.position = Vector3(0, ground_y, 0)
	shape_node.position = Vector3(0, ground_y, 0)
	world.add_child(body)
	return body


func _ground_offset(inst: Node) -> float:
	# align GLB local top surface with heights.json terrain field
	var hmax := -1e9
	for col: Array in heights.cells:
		for v: float in col:
			hmax = maxf(hmax, v)
	var zmax := -1e9
	for mi: MeshInstance3D in _mesh_instances(inst):
		var xf := _global_xf(mi, inst)
		var aabb := mi.get_aabb()
		for c in 8:
			var corner := aabb.position + Vector3(
				aabb.size.x * float(c & 1), aabb.size.y * float((c >> 1) & 1), aabb.size.z * float((c >> 2) & 1))
			zmax = maxf(zmax, (xf * corner).y)
	return hmax - zmax


func _walls(world: Node3D) -> void:
	var seg: PackedScene = load("res://assets/wall.glb")
	var half := float(layout.world.play_area) / 2.0
	var seg_len := 4.0
	var count := int(layout.world.play_area / seg_len)
	for side in 4:
		for k in count:
			var t := (k + 0.5) * seg_len - half
			var pos := Vector3()
			var rot := 0.0
			match side:
				0:
					pos = Vector3(t, 0, -half)
					rot = 0.0
				1:
					pos = Vector3(t, 0, half)
					rot = PI
				2:
					pos = Vector3(-half, 0, t)
					rot = PI / 2.0
				3:
					pos = Vector3(half, 0, t)
					rot = -PI / 2.0
			var body := StaticBody3D.new()
			body.name = "Wall_%d_%d" % [side, k]
			body.rotation.y = rot
			var inst := seg.instantiate()
			body.add_child(inst)
			var cs := CollisionShape3D.new()
			var box := BoxShape3D.new()
			box.size = Vector3(seg_len, 3.2, 0.5)
			cs.shape = box
			cs.position = Vector3(0, 1.6, 0)
			body.add_child(cs)
			body.position = Vector3(pos.x, height_at(pos.x, pos.z), pos.z)
			world.add_child(body)


func _prop(world: Node3D, p: Dictionary, refs: Dictionary) -> void:
	if p.get("ring", false):
		return  # walls handled separately
	var asset: String = p.asset
	var base_asset := asset.trim_suffix("_test")
	var is_test := asset != base_asset
	var scene: PackedScene = load("res://assets/%s.glb" % base_asset)
	var x := float(p.pos[0])
	var z := float(p.pos[2])
	var y := height_at(x, z)
	var scale_v := float(p.get("scale", 1.0))
	var rot := float(p.get("rot_y", 0.0))

	match base_asset:
		"cloud":
			var mi := scene.instantiate()
			mi.name = String(p.asset) + ("_T" if is_test else "")
			mi.position = Vector3(x, float(p.pos[1]), z)
			mi.rotation.y = rot
			mi.scale = Vector3.ONE * scale_v
			world.add_child(mi)
			refs.clouds.append({"node": mi, "speed": float(p.get("speed", 0.4))})
		"barrel":
			var body := RigidBody3D.new()
			body.name = String(asset)
			body.mass = 9.0
			var inst := scene.instantiate()
			# Godot's glTF importer keeps col_* nodes as plain meshes; the
			# collider is game-side per spec, so hide the debug hull.
			var hull := inst.find_child("col_barrel", true, false)
			if hull is MeshInstance3D:
				(hull as MeshInstance3D).hide()
			body.add_child(inst)
			var cs := CollisionShape3D.new()
			var cyl := CylinderShape3D.new()
			cyl.radius = 0.37
			cyl.height = 0.92
			cs.shape = cyl
			cs.position = Vector3(0, 0.46, 0)
			body.add_child(cs)
			body.physics_material_override = _phys_mat(0.6, 0.1)
			body.position = Vector3(x, y, z)
			body.rotation.y = rot
			body.linear_damp = 0.6
			body.angular_damp = 0.8
			world.add_child(body)
			refs.barrels.append(body)
		"bush":
			var holder := Node3D.new()
			holder.name = String(asset)
			holder.set_script(load("res://scripts/BushProp.gd"))
			holder.add_child(scene.instantiate())
			var area := Area3D.new()
			area.name = "Area3D"
			var cs := CollisionShape3D.new()
			var cyl := CylinderShape3D.new()
			cyl.radius = 0.75
			cyl.height = 1.1
			cs.shape = cyl
			cs.position = Vector3(0, 0.5, 0)
			area.add_child(cs)
			holder.add_child(area)
			holder.position = Vector3(x, y, z)
			holder.rotation.y = rot
			holder.scale = Vector3.ONE * scale_v
			world.add_child(holder)
			refs.bushes.append(holder)
		"tree":
			var holder := Node3D.new()
			holder.name = String(asset)
			holder.set_script(load("res://scripts/TreeProp.gd"))
			holder.add_child(scene.instantiate())
			var body := StaticBody3D.new()
			body.name = "TrunkCollider"
			var cs := CollisionShape3D.new()
			var cyl := CylinderShape3D.new()
			cyl.radius = 0.3
			cyl.height = 2.6
			cs.shape = cyl
			cs.position = Vector3(0, 1.3, 0)
			body.add_child(cs)
			holder.add_child(body)
			var hit := Area3D.new()
			hit.name = "HitArea"
			var hcs := CollisionShape3D.new()
			var hcyl := CylinderShape3D.new()
			hcyl.radius = 0.62
			hcyl.height = 2.2
			hcs.shape = hcyl
			hcs.position = Vector3(0, 1.1, 0)
			hit.add_child(hcs)
			hit.collision_mask = 1
			holder.add_child(hit)
			holder.position = Vector3(x, y, z)
			holder.rotation.y = rot
			holder.scale = Vector3.ONE * scale_v
			world.add_child(holder)
			refs.trees.append(holder)
		_:
			var body := StaticBody3D.new()
			body.name = String(asset)
			body.add_child(scene.instantiate())
			var cs := CollisionShape3D.new()
			var shape: Shape3D
			match base_asset:
				"house":
					shape = _box(Vector3(5.2, 4.0, 4.2))
				"ancient_tower":
					shape = _cyl(1.85, 7.4)
				"ancient_shrine":
					shape = _box(Vector3(4.6, 4.6, 3.6))
				"crate":
					shape = _box(Vector3(0.84, 0.84, 0.84))
				"rock":
					shape = _convex_from(scene)
				_:
					shape = _box(Vector3(1, 1, 1))
			cs.shape = shape
			if shape is CylinderShape3D:
				cs.position.y = (shape as CylinderShape3D).height / 2.0
			elif shape is BoxShape3D:
				cs.position.y = (shape as BoxShape3D).size.y / 2.0
			else:
				cs.position.y = 0.45
			body.add_child(cs)
			var py := y + (float(p.pos[1]) if base_asset == "crate" else 0.0)
			body.position = Vector3(x, py, z)
			body.rotation.y = rot
			body.scale = Vector3.ONE * scale_v
			world.add_child(body)


func _coin(world: Node3D, c: Dictionary, refs: Dictionary) -> void:
	var scene: PackedScene = load("res://assets/coin.glb")
	var holder := Node3D.new()
	holder.name = "Coin_%d" % int(c.id)
	holder.set_script(load("res://scripts/CoinPickup.gd"))
	holder.set("coin_id", int(c.id))
	holder.add_child(scene.instantiate())
	var area := Area3D.new()
	area.name = "Area3D"
	var cs := CollisionShape3D.new()
	var cyl := CylinderShape3D.new()
	# generous pickup volume (game feel; asset collision is game-side anyway)
	# Phase 9: coin shrunk to 0.36 -> magnet radius from 1.2 to 1.0
	cyl.radius = 1.0
	cyl.height = 1.4
	cs.shape = cyl
	area.add_child(cs)
	area.collision_mask = 1
	holder.add_child(area)
	holder.position = Vector3(float(c.pos[0]), height_at(float(c.pos[0]), float(c.pos[2])) + float(c.pos[1]), float(c.pos[2]))
	world.add_child(holder)
	refs.coins.append(holder)


func _flag(world: Node3D, refs: Dictionary) -> void:
	var f: Dictionary = layout.flag
	var scene: PackedScene = load("res://assets/flag.glb")
	var holder := Node3D.new()
	holder.name = "FinishFlag"
	holder.set_script(load("res://scripts/FinishFlag.gd"))
	holder.add_child(scene.instantiate())
	var area := Area3D.new()
	area.name = "Area3D"
	var cs := CollisionShape3D.new()
	var cyl := CylinderShape3D.new()
	cyl.radius = 1.1
	cyl.height = 3.2
	cs.shape = cyl
	cs.position.y = 1.6
	area.add_child(cs)
	area.collision_mask = 1
	holder.add_child(area)
	var x := float(f.pos[0])
	var z := float(f.pos[2])
	holder.position = Vector3(x, height_at(x, z), z)
	holder.rotation.y = float(f.get("rot_y", 0.0))
	world.add_child(holder)
	refs.flag = holder


func _mesh_instances(n: Node) -> Array:
	var out: Array = []
	for c in n.get_children():
		if c is MeshInstance3D:
			out.append(c)
		out.append_array(_mesh_instances(c))
	return out


func _global_xf(n: Node3D, under: Node3D) -> Transform3D:
	var t := Transform3D()
	var cur: Node = n
	while cur != null and cur != under:
		t = (cur as Node3D).transform * t
		cur = cur.get_parent()
	return t


func _box(s: Vector3) -> BoxShape3D:
	var b := BoxShape3D.new()
	b.size = s
	return b


func _cyl(r: float, h: float) -> CylinderShape3D:
	var c := CylinderShape3D.new()
	c.radius = r
	c.height = h
	return c


func _convex_from(scene: PackedScene) -> ConvexPolygonShape3D:
	var pts := PackedVector3Array()
	var inst := scene.instantiate()
	for mi: MeshInstance3D in _mesh_instances(inst):
		var arr := mi.mesh.surface_get_arrays(0)
		var verts: PackedVector3Array = arr[Mesh.ARRAY_VERTEX]
		for v in verts:
			pts.append(mi.transform.origin + v)
	inst.free()
	var s := ConvexPolygonShape3D.new()
	s.points = pts
	return s


func _phys_mat(friction: float, bounce: float) -> PhysicsMaterial:
	var m := PhysicsMaterial.new()
	m.friction = friction
	m.bounce = bounce
	return m
