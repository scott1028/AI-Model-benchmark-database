extends SceneTree
# Physics checks for Coin Run.
# Usage: godot --headless --path game --fixed-fps 60 -s res://tools/physics_test.gd
# Prints "PASS/FAIL <label>" per check, then "PHYSICS_TEST: PASS|FAIL"; exit 0/1.

const PlayerScript := preload("res://scripts/player.gd")
const DT := 1.0 / 60.0
const G := 9.8

var _fails := 0

func _report(ok: bool, label: String) -> void:
	print("%s %s" % ["PASS" if ok else "FAIL", label])
	if not ok:
		_fails += 1

func _init() -> void:
	_static_checks()
	# Scene checks run across physics frames.
	await physics_frame
	var main: Node = (load("res://scenes/main.tscn") as PackedScene).instantiate()
	root.add_child(main)
	await physics_frame
	await physics_frame
	await _scene_checks(main)
	main.free()
	print("PHYSICS_TEST: %s" % ["PASS" if _fails == 0 else "FAIL"])
	quit(0 if _fails == 0 else 1)

func _static_checks() -> void:
	# 1: hold direction from rest 1.5 s → speed in (3.8, 4.05)
	var v := Vector3.ZERO
	for i in 90:
		v = PlayerScript.step_velocity(v, Vector3(0.0, 0.0, 1.0), true, 4.0, false, G, DT)
	_report(v.length() > 3.8 and v.length() < 4.05, "1 top speed after 1.5s = %.3f" % v.length())
	# 2: release for 1 s → speed 0
	for i in 60:
		v = PlayerScript.step_velocity(v, Vector3.ZERO, true, 4.0, false, G, DT)
	_report(v.length() < 1e-6, "2 coast stop after 1s = %.6f" % v.length())
	# 3: first 0.1 s, air vs ground
	var vg := Vector3.ZERO
	var va := Vector3.ZERO
	for i in 6:
		vg = PlayerScript.step_velocity(vg, Vector3(0.0, 0.0, 1.0), true, 4.0, false, G, DT)
		va = PlayerScript.step_velocity(va, Vector3(0.0, 0.0, 1.0), false, 4.0, false, G, DT)
	_report(va.length() < vg.length() * 0.5, "3 air %.3f < ground %.3f * 0.5" % [va.length(), vg.length()])
	# 4: air at 4 m/s holding direction 1 s → ≤ 4.01
	var v4 := Vector3(0.0, 0.0, 4.0)
	for i in 60:
		v4 = PlayerScript.step_velocity(v4, Vector3(0.0, 0.0, 1.0), false, 4.0, false, G, DT)
	_report(v4.length() <= 4.01, "4 air 4m/s after 1s = %.3f" % v4.length())
	# 5: boost from rest 2 s → in (6.7, 7.05)
	var v5 := Vector3.ZERO
	for i in 120:
		v5 = PlayerScript.step_velocity(v5, Vector3(0.0, 0.0, 1.0), true, 7.0, false, G, DT)
	_report(v5.length() > 6.7 and v5.length() < 7.05, "5 boost after 2s = %.3f" % v5.length())
	# 6: braking (holding direction) vs release coast, both from 7 m/s; frames to stop
	var stop_brake := _frames_to_stop(7.0, true, true)
	var stop_coast := _frames_to_stop(7.0, false, false)
	_report(
		stop_brake <= 30 and stop_brake < stop_coast * 0.7,
		"6 brake %d frames ≤30 and < coast %d * 0.7" % [stop_brake, stop_coast]
	)

func _frames_to_stop(speed: float, braking: bool, hold_dir: bool) -> int:
	var v := Vector3(0.0, 0.0, speed)
	var dir := Vector3(0.0, 0.0, 1.0) if hold_dir else Vector3.ZERO
	for i in 600:
		v = PlayerScript.step_velocity(v, dir, true, 4.0, braking, G, DT)
		if Vector3(v.x, 0.0, v.z).length() < 1e-6:
			return i + 1
	return 601

func _scene_checks(main: Node) -> void:
	var player := main.get_node("Player") as CharacterBody3D
	var fwd := -(Basis(Vector3.UP, PI / 4.0) * Basis(Vector3.RIGHT, -0.698)).z
	fwd.y = 0.0
	fwd = fwd.normalized()

	# checks 7-8: nearest tree to (-13.5, 0, -2)
	var tree := _nearest(main.get_node("World/Trees").get_children(), Vector3(-13.5, 0, -2))
	_spawn_at(main, player, fwd, tree)
	Input.action_press("ui_up")
	var max_sway := 0.0
	var emitting_seen := false
	for i in 120:
		await physics_frame
		max_sway = maxf(max_sway, absf(tree.rotation.z) + absf(tree.rotation.x))
		var leaves: Array = tree.find_children("*", "CPUParticles3D", true, false)
		if not leaves.is_empty() and (leaves[0] as CPUParticles3D).emitting:
			emitting_seen = true
	Input.action_release("ui_up")
	_report(emitting_seen, "7 tree '%s' leaves emitting=true" % tree.name)
	_report(max_sway > 0.005, "8 tree '%s' sway max %.4f rad > 0.005" % [tree.name, max_sway])

	# check 9: nearest barrel to (14, 0, -3), push 120 frames → > 0.3 m
	var barrel := _nearest(main.get_node("World/Barrels").get_children(), Vector3(14, 0, -3))
	_spawn_at(main, player, fwd, barrel)
	var start_pos: Vector3 = barrel.global_position
	Input.action_press("ui_up")
	for i in 120:
		await physics_frame
	Input.action_release("ui_up")
	var moved: Vector3 = barrel.global_position - start_pos
	_report(Vector3(moved.x, 0.0, moved.z).length() > 0.3, "9 barrel '%s' pushed %.3f m > 0.3" % [barrel.name, moved.length()])

func _spawn_at(main: Node, player: CharacterBody3D, fwd: Vector3, target: Node3D) -> void:
	var pos := target.global_position - fwd * 1.6
	pos.y = target.global_position.y + 0.5
	player.global_transform.origin = pos
	player.velocity = Vector3.ZERO

func _nearest(nodes: Array, p: Vector3) -> Node3D:
	var best: Node3D = null
	var best_d := INF
	for n in nodes:
		var n3 := n as Node3D
		if n3 == null:
			continue
		var d: float = (n3.global_position - p).length()
		if d < best_d:
			best_d = d
			best = n3
	return best
