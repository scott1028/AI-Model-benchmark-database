extends CharacterBody3D
# The single-wheel robot (PLAYBOOK 使用者需求: 4 m/s、力向量 + 摩擦力、有慣性、boost/brake/jump、
# 加速身傾斜、輪子轉動、撞樹搖晃掉葉、穿過草叢草叢晃).
#
# Movement is force-like: velocity is pulled toward `direction * target_speed` with a limited
# acceleration, and decays with friction when there is no input — so the robot has real inertia.
# The collider comes from the layout/spec (capsule), never hardcoded twice.

const WHEEL_RADIUS := 0.25
const ROBOT_ASSET := "robot"
const PUSH_GAIN := 0.35

signal bumped(asset: String, normal: Vector3)

var cam: Camera3D
var max_speed := 4.0
var boost_multiplier := 1.6
var accel := 6.5
var coast_friction := 4.0
var brake_decel := 12.0
var air_accel := 2.2
var jump_speed := 5.2
var gravity_scale := 1.6
var uses_input := true

# bot interface (bot mode ignores player input)
var drive_dir := Vector2.ZERO
var drive_boost := false
var drive_brake := false
var drive_jump := false

var lean_angle := 0.0
var wheel_turns := 0.0
var travel_demand := Vector3.ZERO  # where the controls are pushing (used to shove rigid bodies)
var collect_enabled := true

var _visual: Node3D
var _anim: AnimationPlayer
var _anim_len := 1.0
var _gravity := 9.8 * 1.6
var _jump_queued := false
var _last_speed := 0.0


func _ready() -> void:
	collision_layer = 1
	collision_mask = 1
	_build_collider()
	_build_visual()
	_build_sense_area()
	_gravity = 9.8 * gravity_scale


func configure_from_layout(player_cfg: Dictionary) -> void:
	max_speed = float(player_cfg.get("speed_mps", 4.0))


func ground_speed() -> float:
	return Vector2(velocity.x, velocity.z).length()


func jump() -> void:
	_jump_queued = true


func is_boosting() -> bool:
	return uses_input and Input.is_action_pressed("boost")


func is_braking() -> bool:
	return uses_input and Input.is_action_pressed("brake")


func _physics_process(delta: float) -> void:
	var dir := _input_dir()
	var boosting := is_boosting()
	var braking := is_braking()
	if not uses_input:
		boosting = drive_boost
		braking = drive_brake
		if drive_jump:
			_jump_queued = true
			drive_jump = false

	var target := dir * max_speed * (boost_multiplier if boosting else 1.0)
	if braking:
		target = Vector3.ZERO  # brake = fight the momentum, not a faster accelerate
	travel_demand = target
	var on_floor := is_on_floor()
	var current := Vector2(velocity.x, velocity.z)
	var desired := Vector2(target.x, target.z)
	var rate := _acceleration(dir, braking, on_floor)
	var step := (desired - current).limit_length(rate * delta)
	velocity.x += step.x
	velocity.z += step.y

	if _jump_queued and on_floor:
		velocity.y = jump_speed
		_jump_queued = false
	elif on_floor:
		velocity.y = -0.8  # stick to the undulating ground instead of hopping off tiny steps
	else:
		velocity.y -= _gravity * delta

	move_and_slide()
	_report_collisions()
	_apply_lean(dir, boosting, delta)
	_drive_wheel(delta)
	_last_speed = ground_speed()


func _acceleration(dir: Vector3, braking: bool, on_floor: bool) -> float:
	if not on_floor:
		return air_accel
	if dir.length() > 0.01:
		return brake_decel if braking else accel
	return brake_decel if braking else coast_friction


func _input_dir() -> Vector3:
	if not uses_input:
		var d := Vector3(drive_dir.x, 0.0, drive_dir.y)
		return d.limit_length(1.0)
	var v := Input.get_vector("move_left", "move_right", "move_up", "move_down")
	if cam == null:
		return Vector3(v.x, 0.0, -v.y)
	var f := -cam.global_transform.basis.z
	f.y = 0.0
	f = f.normalized()
	var r := cam.global_transform.basis.x
	r.y = 0.0
	r = r.normalized()
	return (f * -v.y + r * v.x).limit_length(1.0)


func _apply_lean(dir: Vector3, boosting: bool, delta: float) -> void:
	# lean into the direction of travel, more when fast / boosting (measurable: lean_angle)
	var speed := ground_speed()
	var moving := dir.length() > 0.01
	var want := 0.0
	if moving:
		want = clampf(speed / max_speed, 0.0, 1.2) * 0.16
		if boosting:
			want += 0.11
		var heading := atan2(dir.x, dir.z)
		# rotate the visual to face where we are going (robot front = -Z, AGENTS.md §2)
		_visual.rotation.y = lerp_angle(_visual.rotation.y, heading + PI, min(1.0, delta * 12.0))
	_lean_angle_to(want, delta)


func _lean_angle_to(want: float, delta: float) -> void:
	lean_angle = lerpf(lean_angle, want, min(1.0, delta * 8.0))
	_visual.rotation.x = -lean_angle


func _drive_wheel(delta: float) -> void:
	if _anim == null or not _anim.has_animation("wheel_roll"):
		return
	if not _anim.is_playing():
		_anim.play("wheel_roll")
	var turns_per_sec := ground_speed() / (2.0 * PI * WHEEL_RADIUS)
	wheel_turns += turns_per_sec * delta
	_anim.speed_scale = turns_per_sec * _anim_len if turns_per_sec > 0.01 else 0.02


func _report_collisions() -> void:
	for i in get_slide_collision_count():
		var c := get_slide_collision(i)
		var col: Node = c.get_collider()
		if col == null:
			continue
		_push_rigid_body(col)
		for g in ["tree", "bush", "barrel"]:
			if col.is_in_group(g):
				bumped.emit(g, c.get_normal())
				var node := _visual_of(col)
				if node != null and node.has_method("hit"):
					node.hit(c.get_normal())
				break


func _push_rigid_body(col: Node) -> void:
	# CharacterBody3D does not shove RigidBody3D by itself. Push with what the controls *ask for*
	# (velocity is ~0 while pressed against the barrel), so the barrel follows at 85 % and friction
	# / damping slow it again after the push.
	var rb := col as RigidBody3D
	if rb == null or travel_demand.length() < 0.1:
		return
	var want := Vector3(travel_demand.x, rb.linear_velocity.y, travel_demand.z) * 0.85
	rb.apply_impulse((want - rb.linear_velocity) * rb.mass * PUSH_GAIN)


func _visual_of(col: Node) -> Node:
	if not col.has_meta("visual"):
		return null
	return get_node_or_null(col.get_meta("visual") as NodePath)


func _build_collider() -> void:
	for old in get_children():
		if old is CollisionShape3D:
			old.free()
	var cs := CollisionShape3D.new()
	var cap := CapsuleShape3D.new()
	var cfg := _robot_collision_spec()
	cap.radius = float(cfg.get("radius_m", 0.31))
	cap.height = float(cfg.get("height_m", 1.05))
	cs.shape = cap
	cs.position = Vector3(0.0, cap.height * 0.5, 0.0)
	add_child(cs)


func _robot_collision_spec() -> Dictionary:
	# the layout carries the spec's collision block, so the shape is defined in one place
	var text := FileAccess.get_file_as_string("res://data/layout.json")
	if text == "":
		return {}
	var data = JSON.parse_string(text)
	if data == null:
		return {}
	var assets: Dictionary = (data as Dictionary).get("assets", {})
	var entry: Dictionary = assets.get(ROBOT_ASSET, {}) as Dictionary
	return entry.get("collision", {}) as Dictionary


func _build_visual() -> void:
	var path := "res://assets/%s/%s.glb" % [ROBOT_ASSET, ROBOT_ASSET]
	if not ResourceLoader.exists(path):
		push_error("robot glb missing: " + path)
		return
	_visual = (ResourceLoader.load(path) as PackedScene).instantiate() as Node3D
	add_child(_visual)
	for n in _anim_players(_visual):
		_anim = n as AnimationPlayer
		_anim_len = _anim.get_animation("wheel_roll").length
		break


func _build_sense_area() -> void:
	var area := Area3D.new()
	area.name = "BrushSense"
	area.monitorable = false
	var cs := CollisionShape3D.new()
	var sph := SphereShape3D.new()
	sph.radius = 0.55
	cs.shape = sph
	cs.position = Vector3(0.0, 0.45, 0.0)
	area.add_child(cs)
	add_child(area)
	area.area_entered.connect(_on_brush_area)


func _on_brush_area(area: Area3D) -> void:
	if not area.is_in_group("bush"):
		return
	var node := _visual_of(area)
	if node != null and node.has_method("brush"):
		node.brush()
		bumped.emit("bush", Vector3.ZERO)


func _anim_players(root: Node) -> Array[Node]:
	var out: Array[Node] = []
	var stack: Array[Node] = [root]
	while not stack.is_empty():
		var n: Node = stack.pop_back()
		if n is AnimationPlayer:
			out.append(n)
		for c in n.get_children():
			stack.append(c)
	return out
