extends Node3D
# Spring-damper sway of $Model around the bottom pivot, plus a travelling wind gust.

const WIND := preload("res://scripts/wind.gd")

@export var stiffness := 40.0
@export var damping := 5.0
@export var push_gain := 0.15
@export var max_angle := 0.35
@export var wind_amplitude := 0.0
@export var wind_frequency := 0.4

var _angle := Vector3.ZERO
var _angular := Vector3.ZERO
var _time := 0.0

func _ready() -> void:
	pass  # kept so bush.gd can call super()

func push(direction: Vector3, strength: float) -> void:
	var local_dir := global_transform.basis.inverse() * direction
	local_dir.y = 0.0
	if local_dir.length() < 0.001:
		return
	_angular += local_dir.normalized() * strength * push_gain

func _physics_process(delta: float) -> void:
	_time += delta
	_angular += (-stiffness * _angle - damping * _angular) * delta
	_angle += _angular * delta
	var mag := _angle.length()
	if mag > max_angle:
		_angle *= max_angle / mag
	if _angle.length() < 0.001 and _angular.length() < 0.001:
		_angle = Vector3.ZERO
		_angular = Vector3.ZERO

	var tilt := _angle
	if wind_amplitude > 0.0:
		var phase := TAU * global_position.dot(WIND.DIRECTION) / WIND.GUST_WAVELENGTH
		var gust := 0.6 + 0.4 * sin(TAU * wind_frequency * _time - phase)
		var wind_local := global_transform.basis.inverse() * WIND.DIRECTION
		wind_local.y = 0.0
		if wind_local.length() > 0.001:
			tilt += wind_local.normalized() * wind_amplitude * gust

	var model := get_node_or_null("Model")
	if model and tilt.length_squared() > 0.0:
		var axis := Vector3.UP.cross(tilt)
		if axis.length_squared() > 0.000001:
			model.basis = Basis(axis.normalized(), tilt.length())
