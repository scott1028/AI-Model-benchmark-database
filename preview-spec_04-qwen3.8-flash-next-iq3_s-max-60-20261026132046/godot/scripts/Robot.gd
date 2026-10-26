extends RigidBody3D

# Force/drag drive model: dv/dt = A0*throttle - K*v  ->  v_max = A0/K = 4 m/s.
# Angular response is locked so the capsule never tips over; chassis lean is
# visual only. Wheels are driven by phase accumulation + AnimationPlayer seek.

const A0 := 12.0        # drive acceleration, m/s^2
const K_DRAG := 3.0     # linear drag coefficient
const VMAX := 4.0       # terminal speed from A0/K_DRAG
const JUMP_V := 4.4     # apex ~= 0.54 m with gravity 18
const TURN_RATE := 2.8  # rad/s at full speed
const WHEEL_R := 0.3    # single-wheel radius

var heading := 0.0
var wheel_phase := 0.0
var grounded := false
var use_input := true
var drive_throttle := 0.0
var drive_steer := 0.0
var lean := 0.0

var _visual: Node3D
var _ap: AnimationPlayer
var _anim_len := 1.0
var _jump_queued := false
var _jump_hold := 0.0
var jump_lock := 0.0  # set by scripted runners to avoid re-jump spam
var _prev_speed := 0.0

var is_robot := true


func build(scene: PackedScene, pos: Vector3, rot_y: float) -> void:
	name = "Robot"
	position = pos
	heading = rot_y
	mass = 15.0
	axis_lock_angular_x = true
	axis_lock_angular_y = true
	axis_lock_angular_z = true
	linear_damp = 0.0
	angular_damp = 10.0
	continuous_cd = true
	add_to_group("robot")

	_visual = scene.instantiate()
	add_child(_visual)

	var cs := CollisionShape3D.new()
	var capsule := CapsuleShape3D.new()
	capsule.radius = 0.34
	capsule.height = 1.15
	cs.shape = capsule
	cs.position = Vector3(0, 0.575, 0)
	var pm := PhysicsMaterial.new()
	# near-zero friction: the drive/drag model and lateral grip decay in
	# _physics_process own the motion; ground pair friction stays low too
	pm.friction = 0.0
	pm.bounce = 0.0
	physics_material_override = pm
	add_child(cs)

	_ap = _find_anim_player(_visual)
	if _ap and _ap.has_animation("WheelSpin"):
		_anim_len = _ap.get_animation("WheelSpin").length
		_ap.play("WheelSpin")
		_ap.seek(0.0, true)


func request_jump() -> void:
	_jump_queued = true


func forward_dir() -> Vector3:
	# measured with tools/verify_facing.gd: imported robot face sits at +Z
	return Vector3(sin(heading), 0.0, cos(heading))


func _physics_process(dt: float) -> void:
	var throttle := 0.0
	var steer := 0.0
	if use_input:
		if Input.is_action_pressed("accelerate"):
			throttle = 1.0
		if Input.is_action_pressed("brake"):
			throttle = -1.0
		if Input.is_action_pressed("move_left"):
			steer -= 1.0
		if Input.is_action_pressed("move_right"):
			steer += 1.0
		if Input.is_action_just_pressed("jump"):
			_jump_queued = true
	else:
		throttle = drive_throttle
		steer = drive_steer

	var hv := Vector3(linear_velocity.x, 0.0, linear_velocity.z)
	var speed := hv.length()
	var fwd := forward_dir()
	# turn slower when nearly stopped
	var grip := clampf(0.35 + 0.65 * minf(1.0, speed / VMAX), 0.0, 1.0)
	heading -= steer * TURN_RATE * grip * dt
	if steer != 0.0:
		heading = wrapf(heading, 0.0, TAU)

	# arcade rolling: accelerate along fwd, kill lateral slide quickly
	var accel := 0.0
	if throttle > 0.0:
		accel = A0
	elif throttle < 0.0:
		accel = -A0 * 0.6
	var fwd_comp := hv.dot(fwd)
	var perp := hv - fwd * fwd_comp
	perp *= exp(-10.0 * dt)
	fwd_comp += accel * dt
	fwd_comp -= fwd_comp * K_DRAG * dt
	if absf(fwd_comp) > VMAX * 1.6:
		fwd_comp = signf(fwd_comp) * VMAX * 1.6
	hv = fwd * fwd_comp + perp
	linear_velocity = Vector3(hv.x, linear_velocity.y, hv.z)

	grounded = test_move(global_transform, Vector3(0, -0.3, 0))
	# jump with a small input buffer so a queued press lands when grounded
	if jump_lock > 0.0:
		jump_lock -= dt
	if _jump_queued:
		_jump_queued = false
		if jump_lock <= 0.0:
			_jump_hold = 0.3
			jump_lock = 0.7
	if _jump_hold > 0.0:
		_jump_hold -= dt
		if grounded:
			linear_velocity.y = JUMP_V
			_jump_hold = 0.0

	# keep any picked-up angular motion dead (locks already, belt + braces)
	angular_velocity = Vector3.ZERO

	# wheel phase + animation seek
	var fwd_speed := hv.dot(fwd)
	wheel_phase = fmod(wheel_phase + fwd_speed * dt / WHEEL_R, TAU)
	if _ap and _ap.has_animation("WheelSpin"):
		_ap.seek(fmod(wheel_phase / TAU, 1.0) * _anim_len, true)

	# visual lean from forward acceleration
	var accel_now := (hv.length() * signf(fwd_speed) - _prev_speed) / maxf(dt, 1e-4)
	_prev_speed = hv.length() * signf(fwd_speed)
	lean = lerp(lean, clampf(accel_now * 0.02, -0.14, 0.14), 1.0 - exp(-6.0 * dt))
	_visual.rotation.x = lean
	_visual.rotation.y = heading


func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	# keep the body axis-aligned; the visual child does the lean/turn
	var xf := state.transform
	xf.basis = Basis()
	state.transform = xf


func _find_anim_player(n: Node) -> AnimationPlayer:
	for c in n.get_children():
		if c is AnimationPlayer:
			return c
		var r := _find_anim_player(c)
		if r:
			return r
	return null
