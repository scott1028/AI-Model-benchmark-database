extends SceneTree
# Binds the user-approved input map and saves project.godot.
# Usage: godot --headless --path godot -s res://tools/setup_inputs.gd


func _key(k: int) -> InputEventKey:
	var e := InputEventKey.new()
	e.physical_keycode = k
	return e


func _pad(b: int) -> InputEventJoypadButton:
	var e := InputEventJoypadButton.new()
	e.button_index = b
	return e


func _set_action(action: String, events: Array) -> void:
	# Persist into project settings so the input map survives to game runs.
	ProjectSettings.set_setting("input/" + action, {"deadzone": 0.5, "events": events})


func _init() -> void:
	_set_action("move_up", [_key(KEY_UP)])
	_set_action("move_down", [_key(KEY_DOWN)])
	_set_action("move_left", [_key(KEY_LEFT)])
	_set_action("move_right", [_key(KEY_RIGHT)])
	_set_action("accelerate", [_key(KEY_A), _pad(2)])  # pad X
	_set_action("brake", [_key(KEY_B), _pad(1)])  # pad B
	_set_action("jump", [_key(KEY_SPACE), _key(KEY_ENTER), _pad(0)])  # pad A
	var err := ProjectSettings.save()
	var report := {"save_error": err, "actions": {}}
	for a in ["move_up", "move_down", "move_left", "move_right", "accelerate", "brake", "jump"]:
		var evs: Array[String] = []
		var cfg = ProjectSettings.get_setting("input/" + a, {})
		for e in cfg.get("events", []):
			if e is InputEventKey:
				evs.append("key:%d" % (e as InputEventKey).physical_keycode)
			elif e is InputEventJoypadButton:
				evs.append("pad:%d" % (e as InputEventJoypadButton).button_index)
		report["actions"][a] = evs
	print("INPUT_JSON " + JSON.stringify(report))
	quit(0)
