extends SceneTree
# Verifies the input map matches the keys the user fixed in the playbook:
# arrows + dpad, jump = Space/Enter/pad A, boost = key A / pad X, brake = key B / pad B.
# Run: godot --headless --path godot --script res://tools/verify_input.gd

const EXPECTED_KEYS := {
	"move_up": [4194320],
	"move_down": [4194322],
	"move_left": [4194319],
	"move_right": [4194321],
	"jump": [32, 4194309],
	"boost": [97],
	"brake": [98],
}
const EXPECTED_PAD_BUTTONS := {
	"move_up": [11],
	"move_down": [12],
	"move_left": [13],
	"move_right": [14],
	"jump": [0],
	"boost": [2],
	"brake": [1],
}
const EXPECTED_PAD_AXES := {
	"move_up": [[7, -1.0]],
	"move_down": [[7, 1.0]],
	"move_left": [[6, -1.0]],
	"move_right": [[6, 1.0]],
}

func _init() -> void:
	var detail := {}
	var failures: Array[String] = []
	for action in EXPECTED_KEYS.keys():
		if not InputMap.has_action(action):
			failures.append("missing action: " + action)
			continue
		var keys: Array[int] = []
		var pad_buttons: Array[int] = []
		var pad_axes: Array = []
		for ev in InputMap.action_get_events(action):
			if ev is InputEventKey:
				keys.append(int(ev.physical_keycode))
			elif ev is InputEventJoypadButton:
				pad_buttons.append(int(ev.button_index))
			elif ev is InputEventJoypadMotion:
				pad_axes.append([int(ev.axis), float(ev.axis_value)])
		var row := {
			"keys": keys,
			"pad_buttons": pad_buttons,
			"pad_axes": pad_axes,
			"ok": true,
		}
		for k in EXPECTED_KEYS[action]:
			if not keys.has(k):
				row["ok"] = false
				failures.append("%s: missing key %d" % [action, k])
		for b in EXPECTED_PAD_BUTTONS[action]:
			if not pad_buttons.has(b):
				row["ok"] = false
				failures.append("%s: missing pad button %d" % [action, b])
		if EXPECTED_PAD_AXES.has(action):
			for ax in EXPECTED_PAD_AXES[action]:
				if not pad_axes.has(ax):
					row["ok"] = false
					failures.append("%s: missing pad axis %s" % [action, str(ax)])
		detail[action] = row
	var extra: Array[String] = []
	for a in InputMap.get_actions():
		if not EXPECTED_KEYS.has(a) and not String(a).begins_with("ui_"):
			extra.append(a)
	var out := {"pass": failures.is_empty() and extra.is_empty(), "detail": detail, "failures": failures, "unexpected_actions": extra}
	print("INPUT_VERIFY " + JSON.stringify(out))
	quit(0 if out["pass"] else 1)
