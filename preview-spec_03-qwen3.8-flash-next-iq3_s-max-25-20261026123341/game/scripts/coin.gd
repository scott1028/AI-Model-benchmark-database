extends Area3D
signal collected

func _ready() -> void:
	body_entered.connect(_on_body_entered)

func _process(delta: float) -> void:
	rotate_y(deg_to_rad(180.0) * delta)

func _on_body_entered(body: Node3D) -> void:
	if body.name == "Player":
		emit_signal("collected")
		queue_free()
