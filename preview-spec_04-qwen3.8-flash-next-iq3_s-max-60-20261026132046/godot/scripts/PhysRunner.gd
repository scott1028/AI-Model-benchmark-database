extends Node

# Scripted physics scenarios in the east test strip (pad at 16,-2 r3.2).
# One scenario runs at a time; each prints `PHYS <name> <metric>=<value>`
# and all results are written to $RESULT_DIR/phys_result.txt.

var robot  # dynamic scripts on these nodes: keep them variant-typed
var spawner
var barrel
var tree
var bush

var _scenarios := ["brake", "push", "jump", "sway", "bush"]
var _idx := -1
var _phase := ""       # settle / run / done
var _t := 0.0
var _results := {}
var _report := ""

# scenario scratch
var _brake_started := false
var _brake_pos := Vector3.ZERO
var _path_len := 0.0
var _contact_yet := false
var _barrel_peak := 0.0
var _barrel_contact_pos := Vector3.ZERO
var _barrel_rest_t := 0.0
var _jump_base := 0.0
var _jump_fired := false
var _apex := 0.0
var _passed := false


func setup(r, sp, refs: Dictionary) -> void:
	robot = r
	spawner = sp
	barrel = null
	tree = null
	bush = null
	for b in refs.barrels:
		if String(b.name) == "barrel_test":
			barrel = b
	for tr in refs.trees:
		if String(tr.name) == "tree_test":
			tree = tr
	for bu in refs.bushes:
		if String(bu.name) == "bush_test":
			bush = bu
	if barrel == null or tree == null or bush == null:
		print("GAME_ERROR phys_test_props_missing")
		get_tree().quit(2)


func _physics_process(dt: float) -> void:
	if _phase == "":
		_next_scenario()
		return
	if _phase == "run" and _t > 14.0:
		print("GAME_ERROR phys_scenario_timeout %s" % String(_scenarios[_idx]))
		_put(String(_scenarios[_idx]), "timeout", -1.0)
		_reset_flags()
		_next_scenario()
		return
	_t += dt
	match _phase:
		"settle":
			robot.drive_throttle = 0.0
			if _t > 0.9:
				_phase = "run"
				_t = 0.0
				_run(dt)
		"run":
			_run(dt)
		"report":
			_finish()


func _next_scenario() -> void:
	_idx += 1
	if _idx >= _scenarios.size():
		_phase = "report"
		_t = 0.0
		return
	print("PHYS_SCENARIO %s" % String(_scenarios[_idx]))
	robot.use_input = false
	robot.drive_throttle = 0.0
	robot.drive_steer = 0.0
	_t = 0.0
	_phase = "settle"
	var sc: String = _scenarios[_idx]
	var start: Array = _start_pose(sc)  # [x, z, heading]
	robot.position = Vector3(float(start[0]), spawner.height_at(float(start[0]), float(start[1])) + 0.6, float(start[1]))
	robot.linear_velocity = Vector3.ZERO
	robot.angular_velocity = Vector3.ZERO
	robot.heading = float(start[2])
	if sc == "push":
		barrel.position = Vector3(16.0, spawner.height_at(16.0, -2.5) + 0.05, -2.5)
		barrel.linear_velocity = Vector3.ZERO
		barrel.rotation = Vector3.ZERO
	if sc == "sway":
		tree.reset_peak()


func _start_pose(sc: String) -> Array:
	match sc:
		"brake":
			return [13.0, -0.5, 0.5 * PI]
		"push":
			return [13.2, -2.5, 0.5 * PI]
		"jump":
			return [13.5, -4.0, 0.5 * PI]
		"sway":
			return [17.3, -4.5, 0.5 * PI]
		"bush":
			return [12.6, 1.2, 0.5 * PI]
	return [13.0, -0.5, 0.5 * PI]


func _run(_dt: float) -> void:
	var sc: String = _scenarios[_idx]
	match sc:
		"brake":
			_sc_brake()
		"push":
			_sc_push()
		"jump":
			_sc_jump()
		"sway":
			_sc_sway()
		"bush":
			_sc_bush()


func _speed() -> float:
	return Vector3(robot.linear_velocity.x, 0.0, robot.linear_velocity.z).length()


func _sc_brake() -> void:
	if not _brake_started:
		robot.drive_throttle = 1.0
		if _speed() >= 3.0 or _t > 4.0:
			_brake_started = true
			_brake_pos = robot.position
			robot.drive_throttle = -1.0
	else:
		_path_len += _speed() * get_physics_process_delta_time()
		if _speed() < 0.15 or _t > 8.0:
			_put("brake", "stop_distance", _path_len)
			_reset_flags()
			_next_scenario()
	if _t > 10.0:
		_put("brake", "stop_distance", -1.0)
		_reset_flags()
		_next_scenario()


func _sc_push() -> void:
	robot.drive_throttle = 1.0
	var bs: float = barrel.linear_velocity.length()
	if not _contact_yet:
		if bs > 0.25:
			_contact_yet = true
			_barrel_contact_pos = barrel.position
			_barrel_peak = bs
			_barrel_rest_t = 0.0
	else:
		_barrel_peak = maxf(_barrel_peak, bs)
		if bs < 0.08:
			_barrel_rest_t += get_physics_process_delta_time()
			if _barrel_rest_t > 0.5:
				var slide: float = Vector3(barrel.position - _barrel_contact_pos).length()
				_put("push", "barrel_speed", _barrel_peak)
				_put("friction", "barrel_slide", slide)
				_reset_flags()
				_next_scenario()
				return
	if _t > 10.0:
		_put("push", "barrel_speed", _barrel_peak if _contact_yet else -1.0)
		_put("friction", "barrel_slide", -1.0)
		_reset_flags()
		_next_scenario()


func _sc_jump() -> void:
	robot.drive_throttle = 1.0
	if not _jump_fired:
		if _speed() > 2.5 and robot.grounded:
			robot.request_jump()
			_jump_fired = true
			_jump_base = robot.position.y
	else:
		_apex = maxf(_apex, robot.position.y - _jump_base)
		if robot.grounded and _t > 1.0:
			_put("jump", "apex", _apex)
			_reset_flags()
			_next_scenario()


func _sc_sway() -> void:
	robot.drive_throttle = 1.0
	if _speed() < 0.3 and _t > 1.5:
		_put("sway", "tree_sway", tree.sway_peak)
		_put("leaves", "leaf_bursts", float(tree.leaf_bursts))
		_reset_flags()
		_next_scenario()
	elif _t > 8.0:
		_put("sway", "tree_sway", tree.sway_peak)
		_put("leaves", "leaf_bursts", float(tree.leaf_bursts))
		_reset_flags()
		_next_scenario()


func _sc_bush() -> void:
	robot.drive_throttle = 1.0
	if not _passed and robot.position.x > 16.0:
		_passed = true
		_put("bush", "pass_speed", _speed())
		_reset_flags()
		_next_scenario()
	elif _t > 8.0:
		_put("bush", "pass_speed", _speed())
		_reset_flags()
		_next_scenario()


func _reset_flags() -> void:
	_brake_started = false
	_path_len = 0.0
	_contact_yet = false
	_barrel_peak = 0.0
	_jump_base = 0.0
	_jump_fired = false
	_apex = 0.0
	_passed = false
	robot.drive_throttle = 0.0


func _put(scenario: String, metric: String, value: float) -> void:
	var line := "PHYS %s %s=%.3f" % [scenario, metric, value]
	_results[scenario + "." + metric] = value
	print(line)
	robot.drive_throttle = 0.0
	_phase = ""


func _finish() -> void:
	var dir := OS.get_environment("RESULT_DIR")
	var path := "phys_result.txt"
	if dir.is_empty():
		path = OS.get_user_data_dir().path_join("phys_result.txt")
	else:
		path = dir.path_join("phys_result.txt")
	var f := FileAccess.open(path, FileAccess.WRITE)
	if f:
		for k: String in _results:
			f.store_line("PHYS %s %s=%.3f" % [k.get_slice(".", 0), k.get_slice(".", 1), _results[k]])
		f.close()
		print("PHYS_RESULT_FILE %s" % path)
	var failed := false
	for k: String in _results:
		if _results[k] < 0.0:
			failed = true
	get_tree().quit(1 if failed else 0)
