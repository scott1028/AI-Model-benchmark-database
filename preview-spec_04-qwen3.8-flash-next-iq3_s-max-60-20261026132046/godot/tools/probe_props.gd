extends SceneTree
func _init() -> void:
	for cls in ["CollisionShape3D", "RigidBody3D"]:
		var c: Object = ClassDB.instantiate(cls)
		var names: Array[String] = []
		for p in c.get_property_list():
			if "material" in String(p.name) or "friction" in String(p.name):
				names.append(String(p.name))
		print(cls, " -> ", names)
	quit(0)
