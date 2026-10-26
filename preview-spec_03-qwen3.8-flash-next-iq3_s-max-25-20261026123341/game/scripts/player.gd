extends CharacterBody3D
# One-wheeled bot: force-vector drive + friction, boost/brake, jump, push, lean, wheel spin.

const MAX_SPEED := 4.0
const BOOST_SPEED := 7.0
const MASS := 2.0
const DRAG := 4.0
const ROLLING_FRICTION := 0.15
const BRAKE_FRICTION := 0.8
const AIR_CONTROL := 0.3
const JUMP_VELOCITY := 4.5
const REACH_DISTANCE := 0.3
const WHEEL_RADIUS := 0.16
const LEAN_PER_ACCEL := 0.04
const MAX_LEAN := 0.3
const LEAN_RESPONSE := 10.0
const HIT_MIN_SPEED := 0.5

var bot_mode := false
var bot_points: Array[Vector3] = []
var bot_index := 0

var _gravity := 9.8
var _top_speed := MAX_SPEED
var _yaw := 0.0
var _lean := 0.0
var _prev_velocity := Vector3.ZERO
var _wheel_anim: AnimationPlayer
var _wheel_length := 1.0

@onready var _visual: Node3D = $Visual

static func get_drive_force(top_speed: float, g: float) -> float:
	return MASS * (ROLLING_FRICTION * g + DRAG * top_speed)

static func step_velocity(v: Vector3, dir: Vector3, on_floor: bool, top_speed: float, is_braking: bool, g: float, dt: float) -> Vector3:
	if is_braking:
		dir = Vector3.ZERO
	var drive: float
	var drag: float
	if on_floor:
		drive = get_drive_force(top_speed, g)
		drag = DRAG
	else:
		drive = get_drive_force(top_speed, g) * AIR_CONTROL
		drag = drive / (MASS * top_speed)
	v += (dir * drive / MASS - v * drag) * dt
	if on_floor:
		var mu := BRAKE_FRICTION if is_braking else ROLLING_FRICTION
		var hv := Vector3(v.x, 0.0, v.z)
		var speed := hv.length()
		var dec := mu * g * dt
		if speed > 0.0:
			var new_speed := maxf(speed - dec, 0.0)
			hv = hv * (new_speed / speed)
			v.x = hv.x
			v.z = hv.z
	return v

func _ready() -> void:
	_gravity = float(ProjectSettings.get_setting("physics/3d/default_gravity", 9.8))
	bot_mode = "--bot" in OS.get_cmdline_user_args()
	if bot_mode:
		for p in ["../Coins/Coin1", "../Coins/Coin2", "../Coins/Coin3", "../Goal"]:
			var n := get_node_or_null(p)
			if n:
				bot_points.append(n.global_position)
	var ap := _visual.find_child("AnimationPlayer", true, false) as AnimationPlayer
	if ap and ap.has_animation("wheel_spin"):
		_wheel_anim = ap
		var anim := ap.get_animation("wheel_spin")
		if anim.track_get_key_count(0) > 0:
			_wheel_length = anim.track_get_key_time(0, anim.track_get_key_count(0) - 1)
		anim.length = _wheel_length
		anim.loop_mode = Animation.LOOP_LINEAR
		ap.play("wheel_spin")

func _physics_process(delta: float) -> void:
	if not is_on_floor():
		velocity.y -= _gravity * delta

	var dir := _input_direction()
	var is_braking := Input.is_action_pressed("brake") and not bot_mode
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	_top_speed = BOOST_SPEED if (Input.is_action_pressed("boost") and not bot_mode) else MAX_SPEED
	velocity = step_velocity(velocity, dir, is_on_floor(), _top_speed, is_braking, _gravity, delta)

	_prev_velocity = velocity
	move_and_slide()
	_handle_collisions(delta)

	_update_visual(delta)

func _input_direction() -> Vector3:
	if bot_mode:
		if bot_index < bot_points.size():
			var to := bot_points[bot_index] - global_position
			to.y = 0.0
			if to.length() < 0.3:
				bot_index += 1
				if bot_index >= bot_points.size():
					return Vector3.ZERO
				to = bot_points[bot_index] - global_position
				to.y = 0.0
			return to.normalized() if to.length() > 0.001 else Vector3.ZERO
		return Vector3.ZERO
	var camera := get_viewport().get_camera_3d()
	if camera == null:
		return Vector3.ZERO
	var in_vec := Input.get_vector("ui_left", "ui_right", "ui_down", "ui_up")
	if in_vec.length() < 0.01:
		return Vector3.ZERO
	var cam_basis := camera.global_basis
	var fwd := Vector3(-cam_basis.z.x, 0.0, -cam_basis.z.z).normalized()
	var right := Vector3(cam_basis.x.x, 0.0, cam_basis.x.z).normalized()
	return (right * in_vec.x + fwd * in_vec.y).normalized()

func _handle_collisions(delta: float) -> void:
	for i in get_slide_collision_count():
		var collision := get_slide_collision(i)
		var collider := collision.get_collider()
		if collider == null:
			continue
		var normal := collision.get_normal()
		if collider is RigidBody3D:
			var rb := collider as RigidBody3D
			var push_dir := -normal
			push_dir.y = 0.0
			if push_dir.length() < 0.001:
				continue
			push_dir = push_dir.normalized()
			var approach := velocity.dot(push_dir)
			approach = maxf(approach, 0.0)
			var reduced := MASS * rb.mass / (MASS + rb.mass)
			var impulse := push_dir * (reduced * approach + get_drive_force(_top_speed, _gravity) * delta)
			rb.apply_central_impulse(impulse)
		else:
			var impact := velocity.dot(-normal)
			if impact >= HIT_MIN_SPEED:
				var node: Node = collider
				while node != null:
					if node.has_method("push"):
						node.push(-normal, impact)
						break
					node = node.get_parent()

func _update_visual(delta: float) -> void:
	var hv := Vector3(velocity.x, 0.0, velocity.z)
	if hv.length() > 0.2:
		_yaw = atan2(hv.x, hv.z)
	var accel := (hv - Vector3(_prev_velocity.x, 0.0, _prev_velocity.z)) / maxf(delta, 0.0001)
	var lean_target := clampf(accel.length() * LEAN_PER_ACCEL * signf(accel.dot(hv)), -MAX_LEAN, MAX_LEAN)
	_lean = lerpf(_lean, lean_target, 1.0 - exp(-LEAN_RESPONSE * delta))
	_visual.basis = Basis(Vector3.UP, _yaw) * Basis(Vector3(1, 0, 0), _lean)

	if _wheel_anim:
		var forward_speed := hv.length() * signf(hv.dot(Vector3(sin(_yaw), 0.0, cos(_yaw))))
		_wheel_anim.speed_scale = forward_speed / (2.0 * PI * WHEEL_RADIUS) * _wheel_length
