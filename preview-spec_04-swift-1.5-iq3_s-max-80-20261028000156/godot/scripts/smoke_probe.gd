extends Node3D
# Smoke probe: proves imported assets load, render, and report their measured size.
# Prints SMOKE lines then quits so `godot --headless` / Godot MCP run_project returns.

const ASSETS := ["robot", "rock", "coin", "tree", "bush", "barrel", "wooden_crate",
	"endpoint_flag", "brick_wall", "house", "fantasy_shrine", "fantasy_arch", "cloud"]

var _frames := 0


func _ready() -> void:
	var rows: Array[String] = []
	for id in ASSETS:
		var ps = load("res://assets/%s/%s.glb" % [id, id])
		if ps == null:
			print("SMOKE FAIL missing asset " + id)
			get_tree().quit(1)
			return
		var inst: Node3D = ps.instantiate()
		inst.position = Vector3(float(rows.size()) * 3.0 - 18.0, 0.0, 0.0)
		add_child(inst)
		var sz := _aabb(inst).size
		rows.append("%s=%.2f/%.2f/%.2f" % [id, sz.x, sz.y, sz.z])
	print("SMOKE assets " + ", ".join(rows))


func _process(_d: float) -> void:
	_frames += 1
	if _frames == 30:
		print("SMOKE_OK frames=%d" % _frames)
		get_tree().quit(0)


func _aabb(root: Node3D) -> AABB:
	var total := AABB()
	var inited := false
	var stack: Array[Node] = [root]
	while not stack.is_empty():
		var n: Node = stack.pop_back()
		if n is MeshInstance3D and (n as MeshInstance3D).mesh != null and not String(n.name).ends_with("_collision"):
			var a := (n as Node3D).global_transform * (n as MeshInstance3D).mesh.get_aabb()
			total = a if not inited else total.merge(a)
			inited = true
		for c in n.get_children():
			stack.append(c)
	return total
