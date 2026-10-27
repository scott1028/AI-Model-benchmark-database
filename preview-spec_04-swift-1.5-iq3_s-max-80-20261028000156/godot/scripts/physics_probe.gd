extends Node3D
# Physics test probe (PLAYBOOK Phase 8.2): drives the real player through the real world and
# prints one machine-readable line per measurement:
#
#   PHYS_TEST {"id": "...", "measured": x, "unit": "...", "note": "..."}
#
# Every id must exist in specs/physics_thresholds.json; tools/physics_test.py does the judging.
# Nothing is mocked here: the world comes from layout.json (WorldBuilder), so slopes, walls,
# barrels and bushes are the same ones the player meets in the game.
#
# Run: godot --headless --path godot --scene res://scenes/tests/physics_test.tscn -- --phys-test

const TAG := "PHYS_TEST"
const RUN_DIR := Vector3(-0.952, 0.0, -0.298)  # start plaza (10,13) along the road to (-6,8)
const START := Vector3(10.0, 0.0, 13.0)

@onready var world: Node3D = $World
@onready var player: CharacterBody3D = $Player

var steps: Array = []
var step_i := 0
var step_t := 0.0
var s: Dictionary = {}  # scratch state for the current step


func _ready() -> void:
	player.uses_input = false
	_build_steps()
	steps[0]["begin"].call()


func _build_steps() -> void:
	steps = [
		{"name": "speed_run", "dur": 4.0, "begin": _begin_run, "tick": _tick_run, "finish": _finish_run},
		{"name": "boost_run", "dur": 4.0, "begin": _begin_boost, "tick": _tick_boost, "finish": _finish_boost},
		{"name": "wheel_roll", "dur": 3.0, "begin": _begin_wheel, "tick": _tick_wheel, "finish": _finish_wheel},
		{"name": "brake", "dur": 4.5, "begin": _begin_brake, "tick": _tick_brake, "finish": _finish_brake},
		{"name": "coast", "dur": 4.5, "begin": _begin_coast, "tick": _tick_coast, "finish": _finish_coast},
		{"name": "jump", "dur": 2.5, "begin": _begin_jump, "tick": _tick_jump, "finish": _finish_jump},
		{"name": "barrel", "dur": 5.0, "begin": _begin_barrel, "tick": _tick_none, "finish": _finish_barrel},
		{"name": "tree", "dur": 4.0, "begin": _begin_tree, "tick": _tick_none, "finish": _finish_tree},
		{"name": "bush", "dur": 4.0, "begin": _begin_bush, "tick": _tick_none, "finish": _finish_bush},
		{"name": "slope", "dur": 3.5, "begin": _begin_slope, "tick": _tick_none, "finish": _finish_slope},
		{"name": "wall", "dur": 12.0, "begin": _begin_wall, "tick": _tick_wall, "finish": _finish_wall},
	]


func _physics_process(delta: float) -> void:
	var st: Dictionary = steps[step_i]
	step_t += delta
	st["tick"].call(delta)
	if step_t < float(st["dur"]):
		return
	st["finish"].call()
	step_i += 1
	if step_i >= steps.size():
		print("PHYSICS_PROBE finished %d steps in %.1f s" % [steps.size(), Engine.get_physics_frames() / 60.0])
		get_tree().quit(0)
		return
	step_t = 0.0
	s = {}
	steps[step_i]["begin"].call()
	print("PHYS_STEP %s" % steps[step_i]["name"])


# ---------------------------------------------------------------- step bodies

func _begin_run() -> void:
	spawn_at(START)
	drive(RUN_DIR, false)
	s = {"t0": 0.0, "accel_time": 0.0, "max_speed": 0.0}


func _tick_run(_delta: float) -> void:
	var v := speed()
	s["max_speed"] = max(float(s["max_speed"]), v)
	if float(s["accel_time"]) == 0.0 and v >= 0.95 * player.max_speed:
		s["accel_time"] = step_t


func _finish_run() -> void:
	emit("top_speed", float(s["max_speed"]), "m/s", "measured with the real terrain + wall ring")
	emit("accel_time_s", float(s["accel_time"]), "s", "0 -> 95%% of %.1f m/s" % player.max_speed)


func _begin_boost() -> void:
	spawn_at(START)
	drive(RUN_DIR, true)
	s = {"max_speed": 0.0, "max_lean": 0.0, "settle": 1.6}


func _tick_boost(_delta: float) -> void:
	if step_t < float(s["settle"]):
		return  # lean and speed both need a moment to build up
	s["max_speed"] = max(float(s["max_speed"]), speed())
	s["max_lean"] = max(float(s["max_lean"]), abs(player.lean_angle))


func _finish_boost() -> void:
	emit("boost_speed", float(s["max_speed"]), "m/s", "boost multiplier 1.6 x top speed")
	emit("lean_rad", float(s["max_lean"]), "rad", "body lean while boosting at speed")


func _begin_wheel() -> void:
	spawn_at(START)
	drive(RUN_DIR, false)
	s = {"turns0": -1.0, "t0": 2.0}  # sample the last second of the step, at steady speed


func _tick_wheel(_delta: float) -> void:
	if float(s["turns0"]) < 0.0 and step_t >= float(s["t0"]):
		s["turns0"] = float(player.wheel_turns)
		s["t0"] = step_t


func _finish_wheel() -> void:
	var turns := float(player.wheel_turns) - float(s["turns0"])
	var dt := step_t - float(s["t0"])
	var rps := turns / maxf(dt, 0.05)
	var want: float = player.max_speed / (2.0 * PI * player.WHEEL_RADIUS)
	emit("wheel_rps", rps, "rev/s", "expected %.2f rev/s at %.1f m/s (wheel radius %.2f m)"
		% [want, player.max_speed, player.WHEEL_RADIUS])


func _begin_brake() -> void:
	spawn_at(START)
	drive(RUN_DIR, false)
	s = {"braking": false, "start": Vector3.ZERO, "stopped": -1.0}


func _tick_brake(_delta: float) -> void:
	if step_t < 2.2:
		return
	if not bool(s["braking"]):
		s["braking"] = true
		s["start"] = pos()
		player.drive_boost = false
		player.drive_brake = true
	if float(s["stopped"]) < 0.0 and speed() < 0.06:
		var start: Vector3 = s["start"]
		s["stopped"] = Vector2(pos().x - start.x, pos().z - start.z).length()


func _finish_brake() -> void:
	var start: Vector3 = s["start"]
	var d: float = float(s["stopped"]) if float(s["stopped"]) >= 0.0 else Vector2(pos().x - start.x, pos().z - start.z).length()
	player.drive_brake = false
	emit("brake_stop_m", d, "m", "full speed -> standstill while braking")


func _begin_coast() -> void:
	spawn_at(START)
	drive(RUN_DIR, false)
	s = {"released": false, "start": Vector3.ZERO, "stopped": -1.0}


func _tick_coast(_delta: float) -> void:
	if step_t < 2.2:
		return
	if not bool(s["released"]):
		s["released"] = true
		s["start"] = pos()
		player.drive_dir = Vector2.ZERO  # let go: inertia should carry the robot
	if float(s["stopped"]) < 0.0 and speed() < 0.2:
		var start: Vector3 = s["start"]
		s["stopped"] = Vector2(pos().x - start.x, pos().z - start.z).length()


func _finish_coast() -> void:
	var start: Vector3 = s["start"]
	var d: float = float(s["stopped"]) if float(s["stopped"]) >= 0.0 else Vector2(pos().x - start.x, pos().z - start.z).length()
	emit("inertia_slide_m", d, "m", "distance coasted after releasing the controls at full speed")


func _begin_jump() -> void:
	spawn_at(START)
	drive(Vector3.ZERO, false)
	s = {"y0": 0.0, "peak": 0.0, "jumped": false}


func _tick_jump(_delta: float) -> void:
	if not bool(s["jumped"]):
		s["jumped"] = true
		s["y0"] = pos().y
		player.jump()
	s["peak"] = max(float(s["peak"]), pos().y)


func _finish_jump() -> void:
	emit("jump_height_m", float(s["peak"]) - float(s["y0"]), "m", "apex above the launch height")


func _begin_barrel() -> void:
	var body := _first_body_in_group("barrel")
	if body == null:
		s = {"missing": true}
		return
	var at: Vector3 = body.global_position
	var to_centre := _flat(at, Vector3.ZERO)
	spawn_at(at + to_centre * 1.1)
	s = {"body": body, "from": at, "dir": -to_centre}
	drive(s["dir"], false)
	print("PHYS_NOTE barrel body=%s at %s, player at %s, push dir %s" % [body.name, at, pos(), s["dir"]])


func _finish_barrel() -> void:
	if bool(s.get("missing", false)):
		emit("barrel_push_m", -1.0, "m", "no barrel body found")
		return
	var body: RigidBody3D = s["body"]
	var from: Vector3 = s["from"]
	var moved := Vector2(body.global_position.x - from.x, body.global_position.z - from.z).length()
	print("PHYS_NOTE barrel end at %s (from %s), player at %s" % [body.global_position, from, pos()])
	emit("barrel_push_m", moved, "m", "barrel %s slid after being pushed" % body.name)


func _begin_tree() -> void:
	_begin_sway_target("tree1", "tree")


func _finish_tree() -> void:
	var node := _sway_node("tree1")
	var sway_m := float(node.get_meta("shake_max", 0.0)) if node != null else 0.0
	var bursts := int(node.get_meta("leaf_bursts", 0)) if node != null else 0
	print("PHYS_NOTE tree end player=%s sway gust=%s shake=%s bursts=%s" % [
		pos(), node.get("gust"), node.get("shake_max"), node.get("leaf_bursts")])
	emit("tree_sway_m", sway_m, "m", "wind gust amplitude added to tree1 by the hit")
	emit("leaf_bursts", bursts, "count", "leaf bursts from tree1 after the hit")


func _begin_bush() -> void:
	_begin_sway_target("bush1", "bush")


func _finish_bush() -> void:
	var node := _sway_node("bush1")
	emit("bush_sway_m", float(node.get_meta("shake_max", 0.0)) if node != null else 0.0, "m",
		 "gust amplitude on bush1 after the robot pushed through (bush is passable)")


func _begin_slope() -> void:
	var cell := _steepest_open_cell()
	if cell.is_empty():
		s = {"missing": true}
		return
	var at: Vector3 = cell["at"]
	var dir: Vector3 = cell["dir"]
	spawn_at(at - dir * 2.5)
	s = {"slope": float(cell["slope"]), "start": pos()}
	drive(dir, false)


func _finish_slope() -> void:
	if bool(s.get("missing", false)):
		emit("climb_slope", 0.0, "m/m", "no slope cell found in the heightmap")
		emit("climb_progress_m", 0.0, "m", "no slope cell found")
		return
	var start: Vector3 = s["start"]
	var travelled := Vector2(pos().x - start.x, pos().z - start.z).length()
	var slope := float(s["slope"])
	emit("climb_slope", slope if travelled >= 2.5 else 0.0, "m/m", "steepest 1 m terrain gradient the robot climbed")
	emit("climb_progress_m", travelled, "m", "distance covered climbing that slope")


func _begin_wall() -> void:
	spawn_at(Vector3(0.0, 0.0, 0.0))
	drive(Vector3.RIGHT, true)
	s = {"min_x": INF, "max_x": -INF, "flip_at": 6.0, "flipped": false}


func _tick_wall(_delta: float) -> void:
	var x := pos().x
	s["min_x"] = min(float(s["min_x"]), x)
	s["max_x"] = max(float(s["max_x"]), x)
	if not bool(s["flipped"]) and step_t >= float(s["flip_at"]):
		s["flipped"] = true
		drive(Vector3.LEFT, true)


func _finish_wall() -> void:
	var half := float((world.layout.get("world", {}) as Dictionary).get("play_half", 20.5))
	var overflow := maxf(0.0, maxf(absf(float(s["max_x"])), absf(float(s["min_x"]))) - half)
	emit("wall_overflow_m", overflow, "m", "how far the robot got past the 40x40 m play area (0 = contained)")


# ---------------------------------------------------------------- helpers

func emit(id: String, measured: float, unit: String, note: String) -> void:
	print(TAG + " " + JSON.stringify({"id": id, "measured": snappedf(measured, 0.001), "unit": unit, "note": note}))


func _tick_none(_delta: float) -> void:
	pass


func pos() -> Vector3:
	return player.global_position


func speed() -> float:
	return player.ground_speed()


func drive(dir: Vector3, boost: bool) -> void:
	player.drive_dir = Vector2(dir.x, dir.z)
	player.drive_boost = boost


func spawn_at(at: Vector3) -> void:
	player.velocity = Vector3.ZERO
	player.global_position = Vector3(at.x, world.height_at(at.x, at.z) + 0.06, at.z)


func _flat(a: Vector3, b: Vector3) -> Vector3:
	var d := Vector3(b.x - a.x, 0.0, b.z - a.z)
	return d.normalized() if d.length() > 0.001 else Vector3.RIGHT


func _first_body_in_group(group: String) -> RigidBody3D:
	for n in get_tree().get_nodes_in_group(group):
		if n is RigidBody3D:
			return n as RigidBody3D
	return null


func _sway_node(label: String) -> Node:
	return world.get_node_or_null(NodePath(label))


func _begin_sway_target(label: String, group: String) -> void:
	var node := _sway_node(label)
	if node == null:
		push_error("missing prop " + label)
		return
	var at: Vector3 = (node as Node3D).global_position
	var to_centre := _flat(at, Vector3.ZERO)
	spawn_at(at + to_centre * 4.0)
	s = {"dir": -to_centre}
	drive(s["dir"], false)
	print("PHYS_NOTE %s at %s, player at %s, dir %s" % [label, at, pos(), s["dir"]])


# Steepest 1 m gradient in the height map that is open ground (clear of blockers, inside the walls).
func _steepest_open_cell() -> Dictionary:
	var n := int(world.grid.get("size", 0))
	var step := float(world.grid.get("step", 1.0))
	var origin: Array = world.grid.get("origin", [-22.0, -22.0])
	var rows: Array = world.grid.get("rows", [])
	if n < 3:
		return {}
	var blockers: Array = []
	for p in world.layout.get("props", []):
		if str(p.get("mode", "none")) != "none":
			blockers.append(p)
	var best := {}
	for j in range(1, n - 1):
		for i in range(1, n - 1):
			var x := float(origin[0]) + i * step
			var z := float(origin[1]) + j * step
			if maxf(absf(x), absf(z)) > 17.0:
				continue
			if _near_blocker(x, z, blockers):
				continue
			var h := float((rows[j] as Array)[i])
			var worst := 0.0
			var gi := 0
			var gj := 0
			for cand in [[1, 0], [-1, 0], [0, 1], [0, -1]]:
				var dh: float = float((rows[j + int(cand[1])] as Array)[i + int(cand[0])]) - h
				if absf(dh) > worst:
					worst = absf(dh)
					gi = int(cand[0])
					gj = int(cand[1])
			var slope := worst / step
			if best.is_empty() or slope > float(best["slope"]):
				var dir := Vector3(float(gi) * step, 0.0, float(gj) * step).normalized()
				best = {"at": Vector3(x, h, z), "slope": slope, "dir": dir}
	return best


func _near_blocker(x: float, z: float, blockers: Array) -> bool:
	for p in blockers:
		var need := float(p.get("col_r", 0.5)) + 1.5
		if Vector2(x - float(p["x"]), z - float(p["z"])).length() < need:
			return true
	return false
