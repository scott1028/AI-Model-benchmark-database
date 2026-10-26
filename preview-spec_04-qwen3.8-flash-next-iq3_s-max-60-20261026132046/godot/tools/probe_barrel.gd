extends SceneTree
func _init() -> void:
	var inst = (load("res://assets/barrel.glb") as PackedScene).instantiate()
	_dump(inst, 0)
	quit(0)
func _dump(n: Node, d: int) -> void:
	print("  ".repeat(d), n.get_class(), " '", n.name, "' visible=", n.visible if "visible" in n else "-")
	for c in n.get_children():
		_dump(c, d+1)
