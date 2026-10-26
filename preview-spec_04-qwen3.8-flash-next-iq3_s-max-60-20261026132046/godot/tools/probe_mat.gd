extends SceneTree
func _init() -> void:
	var c = PhysicsMaterial.new()
	print("CLS=", c.get_class())
	for p in c.get_property_list():
		if "combine" in String(p.name):
			print("PROP ", p.name)
	quit(0)
