extends Node
# Bot mode (PLAYBOOK Phase 7 / bot 測試): ignores player input, walks the validated route to
# coin 0 -> coin 1 -> coin 2 -> endpoint and prints COINRUN_BOT events for tools/bot_test.py.
#
# The route comes from layout.json (tools/gen_layout.py validated that it is clear of blockers),
# so the bot is a test of the real world, not of a hand-written path.

const TAG := "COINRUN_BOT"
const WP_REACH_M := 3.0
const STUCK_AFTER := 2.2

var main: Node
var player: Node
var world: Node

var route: Array = []
var targets: Array = []
var target_indices: Array = []
var cursor := 0
var stage := 0
var elapsed := 0.0
var sidestep := 0.0

var _progress_t := 0.0
var _stuck_t := 0.0


func _ready() -> void:
	player.uses_input = false
	_build()
	var spawn_at: Vector3 = player.global_position
	cursor = mini(_nearest_index(spawn_at) + 1, route.size() - 1)  # the spawn is route[0]
	var pts: Array = []
	for t in targets:
		pts.append([snappedf(t.x, 0.1), snappedf(t.z, 0.1)])
	_log({"event": "start", "spawn": [snappedf(spawn_at.x, 0.01), snappedf(spawn_at.z, 0.01)],
		"route_points": route.size(), "coins": targets.size() - 1, "targets": pts, "wp": cursor})


func _build() -> void:
	route = world.layout.get("routes", {}).get("bot", []) as Array
	targets.clear()
	target_indices.clear()
	var coins: Array = world.layout.get("coins", []) as Array
	coins.sort_custom(func(a, b): return int(a["index"]) < int(b["index"]))
	for c in coins:
		targets.append(_pt(float(c["x"]), float(c["z"])))
		target_indices.append(_nearest_index(_pt(float(c["x"]), float(c["z"]))))
	var ep: Dictionary = world.layout.get("endpoint", {}) as Dictionary
	targets.append(_pt(float(ep["x"]), float(ep["z"])))
	target_indices.append(route.size() - 1)


func _pt(x: float, z: float) -> Vector3:
	return Vector3(x, 0.0, z)


func _route_pt(i: int) -> Vector3:
	var p: Array = route[clampi(i, 0, route.size() - 1)]
	return Vector3(float(p[0]), 0.0, float(p[1]))


func _nearest_index(at: Vector3) -> int:
	var best := 0
	var best_d := INF
	for i in route.size():
		var d := _dist_xz(at, _route_pt(i))
		if d < best_d:
			best_d = d
			best = i
	return best


func _dist_xz(a: Vector3, b: Vector3) -> float:
	return Vector2(a.x - b.x, a.z - b.z).length()


func _physics_process(delta: float) -> void:
	elapsed += delta
	var pos: Vector3 = player.global_position

	# collect / reach bookkeeping
	while stage < targets.size() - 1 and int(main.coins_found) > stage:
		_log({"event": "reach", "stage": stage, "target": [snappedf(targets[stage].x, 0.1), snappedf(targets[stage].z, 0.1)],
			"t": snappedf(elapsed, 0.01), "speed": snappedf(player.ground_speed(), 0.01)})
		stage += 1
		cursor = maxi(cursor, int(target_indices[mini(stage, target_indices.size() - 1)]) - 1)

	var goal: Vector3 = targets[mini(stage, targets.size() - 1)]
	var tidx := int(target_indices[mini(stage, target_indices.size() - 1)])
	while cursor < tidx and _dist_xz(pos, _route_pt(cursor)) < WP_REACH_M:
		cursor += 1  # waypoint reached -> take the next one (roads are wide, so be generous)

	var aim := _route_pt(cursor)
	if _dist_xz(pos, goal) < 7.0:
		aim = goal  # final approach: head straight for the coin / flag
	var dir := aim - pos
	dir.y = 0.0
	if dir.length() > 0.05:
		var want := dir.normalized()
		if sidestep > 0.0:
			want = want.rotated(Vector3.UP, 0.9)
		player.drive_dir = Vector2(want.x, want.z)
	player.drive_boost = _is_straight(aim)
	_steer_stuck(delta, pos)

	_progress_t += delta
	if _progress_t >= 2.0:
		_progress_t = 0.0
		_log({"event": "progress", "stage": stage, "coins": int(main.coins_found), "wp": cursor,
			"speed": snappedf(player.ground_speed(), 0.01), "x": snappedf(pos.x, 0.1), "z": snappedf(pos.z, 0.1),
			"t": snappedf(elapsed, 0.1), "goal": [snappedf(goal.x, 0.1), snappedf(goal.z, 0.1)]})


func _is_straight(aim: Vector3) -> bool:
	var at: Vector3 = player.global_position
	var a := (aim - at).normalized()
	var seg := (_route_pt(mini(cursor + 2, route.size() - 1)) - _route_pt(cursor)).normalized()
	return a.distance_to(seg) < 0.25


func _steer_stuck(delta: float, pos: Vector3) -> void:
	sidestep = max(0.0, sidestep - delta)
	if player.ground_speed() > 0.45:
		_stuck_t = 0.0
		return
	_stuck_t += delta
	if _stuck_t < STUCK_AFTER:
		return
	_stuck_t = 0.0
	sidestep = 0.7
	player.jump()
	_log({"event": "stuck", "stage": stage, "wp": cursor, "t": snappedf(elapsed, 0.1),
		"x": snappedf(pos.x, 0.1), "z": snappedf(pos.z, 0.1), "action": "jump+sidestep"})


func _log(payload: Dictionary) -> void:
	print(TAG + " " + JSON.stringify(payload))
