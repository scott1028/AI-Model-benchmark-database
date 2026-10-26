extends Node3D
# Tree: wind sway on the trunk + periodic leaf particle bursts.

@export var push_gain := 0.25
@export var wind_amplitude := 0.025
@export var wind_frequency := 0.35

var _time := 0.0
var _phase := 0.0
var _kick := 0.0
var _leaves: Node
var _next_burst := 0.0

func _ready() -> void:
	_phase = randf() * TAU
	_leaves = _find_particles(self)
	if _leaves:
		_leaves.emitting = true
		_next_burst = 2.0 + randf() * 2.0

func _find_particles(node: Node) -> Node:
	for c in node.get_children():
		var cls: String = c.get_class()
		if cls == "CPUParticles3D" or cls == "GPUParticles3D":
			return c
		var found: Node = _find_particles(c)
		if found:
			return found
	return null

func push(_direction: Vector3, strength: float) -> void:
	_kick += clampf(strength, 0.0, 4.0) * push_gain
	if _leaves:
		_leaves.restart()
		_leaves.emitting = true

func _process(delta: float) -> void:
	_time += delta
	var t := _time + _phase
	var sway := sin(t * TAU * wind_frequency) * (wind_amplitude + _kick)
	_kick = maxf(_kick - delta * 1.5, 0.0)
	rotation.z = sway
	rotation.x = sin(t * TAU * wind_frequency * 0.7) * wind_amplitude * 0.5
	if _leaves and _time > _next_burst:
		_leaves.restart()
		_leaves.emitting = true
		_next_burst = _time + 4.0 + randf() * 3.0
