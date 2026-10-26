extends Node3D

signal touched

var reached := false
var _visual: Node3D
var _t := 0.0
var _excite := 0.0


func _ready() -> void:
	_visual = get_child(0)
	var area: Area3D = $Area3D
	area.body_entered.connect(_on_body_entered)


func _process(dt: float) -> void:
	_t += dt
	_excite = maxf(0.0, _excite - dt * 0.8)
	var amp := 0.04 + _excite * 0.12
	_visual.rotation.z = sin(_t * 2.0) * amp


func _on_body_entered(body: Node3D) -> void:
	_excite = 1.0
	if reached or not body.is_in_group("robot"):
		return
	reached = true
	touched.emit()
