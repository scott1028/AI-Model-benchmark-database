extends SceneTree
# Reads the saved input map (fresh process) and reports bindings.
# Usage: godot --headless --path godot -s res://tools/verify_inputs.gd


func _init() -> void:
	var report := {}
	var expected := {
		"move_up": ["key:4194320"],
		"move_down": ["key:4194322"],
		"move_left": ["key:4194319"],
		"move_right": ["key:4194321"],
		"accelerate": ["key:65", "pad:2"],
		"brake": ["key:66", "pad:1"],
		"jump": ["key:32", "key:4194309", "pad:0"],
	}
	var ok := true
	for a in expected:
		if not InputMap.has_action(a):
			report[a] = "MISSING"
			ok = false
			continue
		var got: Array[String] = []
		for e in InputMap.action_get_events(a):
			if e is InputEventKey:
				got.append("key:%d" % (e as InputEventKey).physical_keycode)
			elif e is InputEventJoypadButton:
				got.append("pad:%d" % (e as InputEventJoypadButton).button_index)
		report[a] = got
		for want in expected[a]:
			if want not in got:
				ok = false
				report[a + "_missing"] = want
	print("INPUT_VERIFY " + JSON.stringify(report) + " OK=" + str(ok))
	quit(0 if ok else 1)
