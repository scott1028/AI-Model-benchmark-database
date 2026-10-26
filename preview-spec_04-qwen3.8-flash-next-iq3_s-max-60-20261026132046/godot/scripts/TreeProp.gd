extends Node3D

# Procedural wind sway on the visual instance; a robot hit spikes the sway
# and bursts leaves. sway_peak records the biggest angle since last reset
# (used by the physics test runner).

var sway_peak := 0.0
var leaf_bursts := 0

var _visual: Node3D
var _t := 0.0
var _wind_amp := 0.03
var _hit_amp := 0.0
var _leaves: GPUParticles3D


func _ready() -> void:
	_visual = get_child(0)
	_t = randf() * 10.0
	$HitArea.body_entered.connect(_on_hit)
	_leaves = _make_leaves()
	add_child(_leaves)


func _process(dt: float) -> void:
	_t += dt
	_hit_amp = maxf(0.0, _hit_amp - dt * 0.6)
	var amp := _wind_amp + _hit_amp
	var a := sin(_t * 1.3) * amp + sin(_t * 3.7 + 1.1) * amp * 0.35
	_visual.rotation.x = a
	_visual.rotation.z = cos(_t * 1.1 + 0.4) * amp * 0.8
	sway_peak = maxf(sway_peak, absf(a) + absf(_visual.rotation.z))


func reset_peak() -> void:
	sway_peak = 0.0
	leaf_bursts = 0


func _on_hit(body: Node3D) -> void:
	if not body.is_in_group("robot"):
		return
	var speed := 1.0
	if body is RigidBody3D:
		speed = (body as RigidBody3D).linear_velocity.length()
	_hit_amp = clampf(0.10 + speed * 0.05, 0.10, 0.35)
	_leaves.emitting = false
	_leaves.restart()
	leaf_bursts += 1


func _make_leaves() -> GPUParticles3D:
	var p := GPUParticles3D.new()
	p.amount = 14
	p.lifetime = 1.6
	p.one_shot = true
	p.emitting = false
	var mat := ParticleProcessMaterial.new()
	mat.direction = Vector3(0, 1, 0)
	mat.spread = 60.0
	mat.initial_velocity_min = 0.6
	mat.initial_velocity_max = 1.6
	mat.gravity = Vector3(0, -2.2, 0)
	p.process_material = mat
	var mi := BoxMesh.new()
	mi.size = Vector3(0.06, 0.015, 0.06)
	var m := StandardMaterial3D.new()
	m.albedo_color = Color(0.25, 0.55, 0.22)
	mi.material = m
	p.draw_pass_1 = mi
	p.position = Vector3(0, 2.1, 0)
	return p
