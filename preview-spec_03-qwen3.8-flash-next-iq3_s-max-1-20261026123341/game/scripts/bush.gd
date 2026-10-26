extends "res://scripts/sway.gd"
# Bush: pushes itself when a body runs through its trigger.

func _ready() -> void:
	super()
	var trigger := get_node_or_null("Trigger")
	if trigger:
		trigger.body_entered.connect(_on_body)
		trigger.body_exited.connect(_on_body)

func _on_body(body: Node) -> void:
	var v := Vector3.ZERO
	if body is CharacterBody3D:
		v = body.velocity
	elif body is RigidBody3D:
		v = body.linear_velocity
	v.y = 0.0
	if v.length() >= 0.1:
		push(v.normalized(), v.length())
