extends Node3D

func _ready() -> void:
	var sp = load("res://scripts/WorldSpawner.gd").new()
	add_child(sp)
	var refs = sp.build(self)
	await get_tree().physics_frame
	await get_tree().physics_frame
	var walls: Array[String] = []
	var q := Node3D.new()
	add_child(q)
	var names: Array[String] = []
	var world := get_node("World")
	for c in world.get_children():
		if String(c.name).begins_with("Wall_"):
			names.append(String(c.name))
	print("WALL_COUNT %d" % names.size())
	var space := get_world_3d().direct_space_state
	for pt in [[20, -0.96], [20.5, -0.96], [25, -0.96], [30, -0.96], [19.9, -0.96]]:
		var r := PhysicsRayQueryParameters3D.create(Vector3(pt[0], 5, pt[1]), Vector3(pt[0], -40, pt[1]))
		var hit := space.intersect_ray(r)
		print("WALLPROBE x=%.1f z=%.1f y=%.2f hit=%s" % [pt[0], pt[1], hit.position.y if hit else -999.0, str(hit.get("collider").name) if hit else "none"])
	# wall segment world positions for side 3 (x=+half)
	for c in world.get_children():
		if String(c.name) == "Wall_3_0" or String(c.name) == "Wall_3_4" or String(c.name) == "Wall_3_9":
			print("SEG ", String(c.name), " pos=", c.position)
	get_tree().quit(0)
