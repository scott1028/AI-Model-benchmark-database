extends Node3D

func _ready() -> void:
	var sp = load("res://scripts/WorldSpawner.gd").new()
	add_child(sp)
	var refs = sp.build(self)
	await get_tree().physics_frame
	await get_tree().physics_frame
	var space := get_world_3d().direct_space_state
	for pt in [[0, -15], [13, -0.5], [16, -2.5], [19, -4.5], [14.2, 1.2], [8, 14], [-6.2, -4.6]]:
		var q := PhysicsRayQueryParameters3D.create(Vector3(pt[0], 10, pt[1]), Vector3(pt[0], -10, pt[1]))
		var hit := space.intersect_ray(q)
		var hy: float = -999.0
		if hit:
			hy = hit.position.y
		print("PROBE x=%.1f z=%.1f ray_y=%.3f height_at=%.3f collider=%s" % [pt[0], pt[1], hy, sp.height_at(pt[0], pt[1]), str(hit.get("collider").name) if hit else "none"])
	get_tree().quit(0)
