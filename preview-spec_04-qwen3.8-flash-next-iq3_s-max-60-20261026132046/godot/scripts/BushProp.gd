extends Node3D

# Passable bush: no collider, just a shake when something walks through.

var _visual: Node3D
var _t := 0.0
var _shake := 0.0


func _ready() -> void:
	_visual = get_child(0)
	_t = randf() * 10.0
	var area: Area3D = get_node_or_null("Area3D")
	if area:
		area.body_entered.connect(_on_body_entered)


func _process(dt: float) -> void:
	_t += dt
	_shake = maxf(0.0, _shake - dt * 2.2)
	var idle := 0.015
	_visual.rotation.x = sin(_t * 2.1) * idle + sin(_t * 22.0) * _shake * 0.09
	_visual.rotation.z = cos(_t * 1.7) * idle + cos(_t * 19.0) * _shake * 0.09


func _on_body_entered(body: Node3D) -> void:
	if body.is_in_group("robot"):
		_shake = 1.0
