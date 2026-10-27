extends Node3D
# Atmosphere (PLAYBOOK 氣氛): warm sun + soft shadows, ambient light, AO, tilt-shift depth of
# field, drifting clouds (that cast shadows onto the ground).

const SUN_ANGLES := Vector3(-44.0, -34.0, 0.0)
const TILT_SCRIPT := preload("res://scripts/tilt_shift.gd")

var follow: Node3D
var cam: Camera3D
var tilt: CanvasLayer
var cloud_speed_scale := 1.0
var sun: DirectionalLight3D
var environment: Environment


func _ready() -> void:
	environment = Environment.new()
	environment.background_mode = Environment.BG_SKY
	var sky_mat := ProceduralSkyMaterial.new()
	# the diorama floats in the sky: the background must read as sky all around it, not mud
	sky_mat.sky_top_color = Color(0.24, 0.52, 0.86)
	sky_mat.sky_horizon_color = Color(0.74, 0.86, 0.95)
	sky_mat.ground_bottom_color = Color(0.52, 0.66, 0.82)  # seen from above: soft haze under the slab
	sky_mat.ground_horizon_color = Color(0.80, 0.87, 0.94)
	sky_mat.sky_energy_multiplier = 1.0
	var sky := Sky.new()
	sky.sky_material = sky_mat
	environment.sky = sky
	environment.ambient_light_source = Environment.AMBIENT_SOURCE_SKY
	# blue-sky ambient at 1.0 washed the pale plaza to near white, and shaded grass read blue
	# (2026-10-28 screenshots): mix the sky fill with a warm neutral colour so shadows stay neutral.
	environment.ambient_light_energy = 0.6
	environment.ambient_light_color = Color(0.62, 0.60, 0.55)
	environment.ambient_light_sky_contribution = 0.45
	environment.tonemap_mode = Environment.TONE_MAPPER_FILMIC
	environment.tonemap_white = 5.0
	environment.glow_enabled = true
	environment.glow_intensity = 0.28
	environment.ssao_enabled = true
	environment.ssao_intensity = 0.75
	environment.ssao_radius = 1.1
	# tilt-shift: a narrow sharp band around the robot, blur in front of and behind it
	# (Godot 4.7 has no Environment DOF -> screen-space pass, see shaders/tilt_shift.gdshader)
	tilt = TILT_SCRIPT.new()
	tilt.name = "TiltShiftLayer"
	add_child(tilt)

	var we := WorldEnvironment.new()
	we.name = "WorldEnvironment"
	we.environment = environment
	add_child(we)

	sun = DirectionalLight3D.new()
	sun.name = "Sun"
	sun.rotation_degrees = SUN_ANGLES
	sun.light_energy = 1.35
	sun.light_color = Color(1.0, 0.93, 0.79)
	sun.light_angular_distance = 12.0  # wide sun disc -> soft shadow edges
	sun.shadow_enabled = true
	# One orthogonal map over 68 m gave ~1 m shadow texels: tree shadows aliased into 1 m blue
	# speckle on the lawn (2026-10-28 截圖). 4 splits keep near-field texels ~8x smaller.
	sun.directional_shadow_mode = DirectionalLight3D.SHADOW_PARALLEL_4_SPLITS
	sun.directional_shadow_max_distance = 60.0
	sun.directional_shadow_fade_start = 0.85
	sun.shadow_bias = 0.03
	sun.shadow_normal_bias = 0.12
	add_child(sun)


func _process(delta: float) -> void:
	if follow != null:
		sun.global_position = follow.global_position + Vector3(0.0, 6.0, 0.0)
		if cam != null:
			# the sharp band follows the robot on screen (canvas_item cannot read depth -> drive it here)
			var sp := cam.unproject_position(follow.global_position)
			var vp := get_viewport().get_visible_rect().size
			tilt.set_focus_screen_y(sp.y / maxf(vp.y, 1.0))
	for c in get_tree().get_nodes_in_group("clouds"):
		var n := c as Node3D
		n.position.x += float(n.get_meta("drift_mps", 0.3)) * cloud_speed_scale * delta
		if n.position.x > 34.0:
			n.position.x = -34.0
