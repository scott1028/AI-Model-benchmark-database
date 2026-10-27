extends Node3D
# Wind sway + hit reaction for trees and bushes (PLAYBOOK: 撞樹搖晃掉葉、穿過草叢會晃、微風擺動).
# WorldBuilder attaches this to every tree / bush instance (layout.fx.sway_assets).
#
# The wind is a vertex-shader sway (flat colour preserved). A hit adds a decaying "gust"
# amplitude, and trees drop a one-shot burst of leaves. Peak gust and leaf bursts are written
# to node metadata so the headless physics test can measure them (shaders are not evaluated
# in headless mode, so the amplitude lives here, not in the shader).

const SHADER_PATH := "res://shaders/flat_wind.gdshader"

var gust := 0.0
var shake_max := 0.0
var leaf_bursts := 0
var wind_strength := 0.06
var is_tree := false

var _mats: Array[ShaderMaterial] = []
var _leaves: CPUParticles3D


func _ready() -> void:
	set_process(true)  # the wind and the hit decay are driven here


func setup(ref_height: float, wind: float, tree: bool) -> void:
	is_tree = tree
	wind_strength = wind
	var shader = load(SHADER_PATH)
	for mi in _meshes(self):
		var src := mi.get_active_material(0) as StandardMaterial3D
		var m := ShaderMaterial.new()
		m.shader = shader
		m.set_shader_parameter("base_color", src.albedo_color if src != null else Color(0.35, 0.6, 0.3))
		m.set_shader_parameter("wind_strength", wind)
		m.set_shader_parameter("sway_ref_height", ref_height)
		m.set_shader_parameter("phase", float((name.hash() % 627) % 100) / 31.0)
		m.set_shader_parameter("wind_speed", 1.15 + float(name.hash() % 7) * 0.05)
		_mats.append(m)
		mi.material_override = m

	var top := _canopy_centre()
	if is_tree:
		_leaves = _make_leaves(top)


func hit(_dir: Vector3) -> void:
	gust = 0.30
	if is_tree and _leaves != null:
		leaf_bursts += 1
		_leaves.emitting = true
		_leaves.restart()


func brush() -> void:
	gust = max(gust, 0.26)


func _process(delta: float) -> void:
	if gust > 0.0:
		gust = max(0.0, gust - delta * 0.55)
		shake_max = max(shake_max, gust)
		for m in _mats:
			m.set_shader_parameter("gust", gust)
	set_meta("shake_max", shake_max)
	set_meta("leaf_bursts", leaf_bursts)


func _make_leaves(at: Vector3) -> CPUParticles3D:
	var p := CPUParticles3D.new()
	p.name = "leaves"
	p.one_shot = true
	p.emitting = false
	p.amount = 16
	p.lifetime = 1.7
	p.explosiveness = 0.85
	p.direction = Vector3.DOWN
	p.spread = 62.0
	p.gravity = Vector3(0.0, -2.6, 0.0)
	p.initial_velocity_min = 0.3
	p.initial_velocity_max = 1.0
	p.scale_amount_min = 0.055
	p.scale_amount_max = 0.10
	var leaf := BoxMesh.new()
	leaf.size = Vector3(0.12, 0.02, 0.09)
	var mat := StandardMaterial3D.new()
	mat.albedo_color = Color("#6FAE4C")
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
	leaf.material = mat
	p.mesh = leaf
	p.visibility_aabb = AABB(Vector3(-2.2, -4.5, -2.2), Vector3(4.4, 6.0, 4.4))
	add_child(p)
	p.position = at
	return p


func _canopy_centre() -> Vector3:
	var sum := Vector3.ZERO
	var n := 0
	for mi in _meshes(self):
		var a := mi.global_transform * mi.mesh.get_aabb() if mi.mesh != null else AABB()
		sum += a.get_center()
		n += 1
	return (sum / float(n) - global_position) if n > 0 else Vector3(0.0, 2.4, 0.0)


func _meshes(root: Node) -> Array[MeshInstance3D]:
	var out: Array[MeshInstance3D] = []
	var stack: Array[Node] = [root]
	while not stack.is_empty():
		var nd: Node = stack.pop_back()
		if nd is MeshInstance3D and not String(nd.name).ends_with("_collision"):
			out.append(nd as MeshInstance3D)
		for c in nd.get_children():
			stack.append(c)
	return out
