extends CanvasLayer
# Tilt-shift post pass owner (PLAYBOOK 氣氛: 移軸 / 微縮模型感).
# Godot 4.7 has no Environment DOF, so shaders/tilt_shift.gdshader runs as a screen-space pass.
# atmosphere.gd creates this and updates the focus band from the camera distance every frame.

const SHADER := preload("res://shaders/tilt_shift.gdshader")

var rect: ColorRect
var effect_on := true

var focus_y := 0.60
var band := 0.15   # wide enough that the robot and its surroundings stay readable (附錄 B)
var strength := 0.8
var depth_range := 16.0


func _ready() -> void:
	layer = 5
	rect = ColorRect.new()
	rect.name = "TiltShift"
	rect.mouse_filter = Control.MOUSE_FILTER_IGNORE
	rect.set_anchors_preset(Control.PRESET_FULL_RECT)
	var m := ShaderMaterial.new()
	m.shader = SHADER
	rect.material = m
	add_child(rect)
	_apply()


func set_effect(on: bool) -> void:
	effect_on = on
	rect.visible = on
	_apply()


func configure(focus_y_want: float, band_want: float, strength_want: float) -> void:
	focus_y = focus_y_want
	band = band_want
	strength = strength_want
	_apply()


func set_focus_screen_y(screen_y: float) -> void:
	if not effect_on:
		return
	var p: ShaderMaterial = rect.material as ShaderMaterial
	p.set_shader_parameter("focus_y", clampf(screen_y, 0.0, 1.0))


func _apply() -> void:
	var p: ShaderMaterial = rect.material as ShaderMaterial
	if p == null:
		return
	p.set_shader_parameter("focus_y", focus_y)
	p.set_shader_parameter("band", band)
	p.set_shader_parameter("strength", strength if effect_on else 0.0)
	p.set_shader_parameter("depth_range", depth_range)
