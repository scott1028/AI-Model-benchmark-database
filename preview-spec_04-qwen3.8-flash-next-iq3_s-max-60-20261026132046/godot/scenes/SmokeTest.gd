extends Node

# Phase 3 smoke: verify imported rock renders in a lit scene, write a result
# marker (godot-mcp's get_debug_output does not surface stdout on this box),
# then auto-quit.


func _ready() -> void:
	var rock := get_node_or_null("World/Rock")
	var ok := rock != null and (rock as Node3D).get_child_count() > 0
	var msg := "OK rock at " + str((rock as Node3D).global_position) if ok else "FAIL rock instance missing"
	var f := FileAccess.open("/home/scott/workspace/AI-Model-benchmark-database/preview-spec_04-qwen3.8-flash-next-iq3_s-max-60-20261026132046/pipeline/checks/smoke_result.txt", FileAccess.WRITE)
	if f != null:
		f.store_string(msg + "\n")
		f.close()
	if not ok:
		get_tree().quit(1)
		return
	await get_tree().create_timer(2.0).timeout
	get_tree().quit(0)
