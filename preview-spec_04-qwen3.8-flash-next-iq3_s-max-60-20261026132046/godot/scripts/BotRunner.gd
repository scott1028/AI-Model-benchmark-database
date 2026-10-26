extends Node

# Drives the robot along layout bot_path with virtual input presses and
# speaks the test protocol on stdout (BOT_STAGE / BOT_COIN / BOT_VICTORY).

var robot  # Robot.gd on RigidBody3D (variant so dynamic members resolve at runtime)
var waypoints: Array = []
var stage := 0
var coins_expected := 0

var _t := 0.0
var _stuck_t := 0.0
var _finished := false


func setup(r, wps: Array) -> void:
	robot = r
	waypoints = wps


func _process(dt: float) -> void:
	if _finished:
		return
	_t += dt
	if _t > 120.0:
		print("GAME_ERROR bot_timeout")
		get_tree().quit(1)
		return

	var wp: Array = waypoints[stage]
	var target := Vector3(float(wp[0]), robot.position.y, float(wp[1]))
	var to: Vector3 = target - robot.position
	to.y = 0.0
	if to.length() < 1.15:
		stage += 1
		print("BOT_STAGE %d" % stage)
		if stage >= waypoints.size():
			# last waypoint is the flag; wait for its area signal
			Input.action_release("accelerate")
			return

	var fwd: Vector3 = robot.forward_dir()
	var cross: float = fwd.cross(to).y / maxf(0.3, to.length())
	var near := to.length() < 3.0
	if cross > (0.06 if near else 0.12):
		Input.action_press("move_left")
		Input.action_release("move_right")
	elif cross < -(0.06 if near else 0.12):
		Input.action_press("move_right")
		Input.action_release("move_left")
	else:
		Input.action_release("move_left")
		Input.action_release("move_right")
	Input.action_press("accelerate")

	var speed := Vector3(robot.linear_velocity.x, 0.0, robot.linear_velocity.z).length()
	if speed < 0.35:
		_stuck_t += dt
		if _stuck_t > 6.0:
			print("GAME_ERROR bot_stuck stage=%d" % stage)
			get_tree().quit(1)
	else:
		_stuck_t = 0.0


func on_coin(coin_id: int) -> void:
	print("BOT_COIN %d" % coin_id)


func on_victory() -> void:
	if _finished:
		return
	_finished = true
	print("BOT_VICTORY")
	print("BOT_DONE")
	get_tree().create_timer(0.8).timeout.connect(func() -> void: get_tree().quit(0))
