extends Node3D
# Drifting low-poly clouds placed so their shadows land on the map (appendix C-15).

@export var sun: DirectionalLight3D

const WIND := preload("res://scripts/wind.gd")
const CLOUD_GLB := preload("res://assets/cloud.glb")
const HALF_RANGE := 34.0
const AVG_HEIGHT := 15.0

var _area_center := Vector3.ZERO
var _clouds: Array[Node3D] = []

func _ready() -> void:
	if sun == null:
		sun = get_node_or_null("../DirectionalLight3D") as DirectionalLight3D
	var rng := RandomNumberGenerator.new()
	rng.seed = 2026
	var ray := -sun.global_transform.basis.z
	var horizontal := Vector3(ray.x, 0.0, ray.z)
	_area_center = -horizontal * AVG_HEIGHT / maxf(-ray.y, 0.001)
	for i in range(10):
		var c := CLOUD_GLB.instantiate() as Node3D
		var x := _area_center.x + rng.randf_range(-HALF_RANGE, HALF_RANGE)
		var z := _area_center.z + rng.randf_range(-HALF_RANGE, HALF_RANGE)
		var y := rng.randf_range(13.0, 17.0)
		c.position = Vector3(x, y, z)
		c.scale = Vector3.ONE * rng.randf_range(0.9, 1.7)
		c.rotation.y = rng.randf_range(-0.4, 0.4)
		add_child(c)
		_clouds.append(c)

func _physics_process(delta: float) -> void:
	var move := WIND.DIRECTION * WIND.CLOUD_SPEED * delta
	for c in _clouds:
		c.position += move
		var rel := c.position - _area_center
		if absf(rel.x) > HALF_RANGE:
			c.position.x -= signf(rel.x) * 2.0 * HALF_RANGE
		if absf(rel.z) > HALF_RANGE:
			c.position.z -= signf(rel.z) * 2.0 * HALF_RANGE
