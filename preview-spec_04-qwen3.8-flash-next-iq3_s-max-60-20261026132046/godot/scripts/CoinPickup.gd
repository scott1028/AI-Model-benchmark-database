extends Node3D

signal picked(coin_id: int)

var coin_id := 0
var taken := false
var _visual: Node3D
var _bob_t := 0.0
var _base_y := 0.0


func _ready() -> void:
	_visual = get_child(0)
	_base_y = position.y
	_bob_t = randf() * TAU
	var area: Area3D = $Area3D
	area.body_entered.connect(_on_body_entered)


func _process(dt: float) -> void:
	if taken:
		return
	_bob_t += dt * 2.2
	_visual.rotation.y += dt * 2.4
	position.y = _base_y + sin(_bob_t) * 0.08


func _on_body_entered(body: Node3D) -> void:
	if taken or not body.is_in_group("robot"):
		return
	taken = true
	set_process(false)
	area_off()
	hide()
	picked.emit(coin_id)


func area_off() -> void:
	$Area3D.set_deferred("monitoring", false)
