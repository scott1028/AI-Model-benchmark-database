extends Node3D
# Follow camera, DOF, coins/goal bookkeeping, screenshots.

const CAM_YAW := PI / 4
const CAM_PITCH := -0.698
const FOLLOW_DISTANCE := 18.0

var _coins_collected := 0
var _cam_target := Vector3.ZERO
var _overview := false
var _follow_distance := FOLLOW_DISTANCE

@onready var _camera: Camera3D = $Camera3D
@onready var _player: Node3D = $Player

func _ready() -> void:
	for i in range(1, 4):
		var coin := get_node("Coins/Coin%d" % i)
		coin.connect("collected", _on_coin_collected)
	get_node("Goal").connect("reached", _on_goal_reached)

	_overview = "--overview" in OS.get_cmdline_user_args()
	if _overview:
		_follow_distance = 64.0
		_cam_target = Vector3(4, 0, 4)
		var sun := get_node_or_null("DirectionalLight3D")
		if sun:
			sun.directional_shadow_max_distance = 128.0
	else:
		_cam_target = _player.global_position

	_setup_dof()
	if DisplayServer.get_name() != "headless":
		var target := "screenshot_overview.png" if _overview else "screenshot.png"
		_take_screenshot.call_deferred(target)

func _setup_dof() -> void:
	var attrs := CameraAttributesPractical.new()
	attrs.dof_blur_far_enabled = true
	attrs.dof_blur_near_enabled = true
	attrs.dof_blur_amount = 0.1
	var d := _follow_distance
	attrs.dof_blur_near_distance = d * 0.89
	attrs.dof_blur_near_transition = d * 0.17
	attrs.dof_blur_far_distance = d * 1.22
	attrs.dof_blur_far_transition = d * 0.55
	_camera.attributes = attrs
	_camera.fov = 40.0

func _take_screenshot(target: String) -> void:
	await get_tree().create_timer(3.0).timeout
	await RenderingServer.frame_post_draw
	var path := ProjectSettings.globalize_path("res://../pipeline/" + target)
	var img := get_viewport().get_texture().get_image()
	var err := img.save_png(path)
	print("SCREENSHOT %s (%s)" % [path, "OK" if err == OK else error_string(err)])

func _physics_process(delta: float) -> void:
	if not _overview:
		_cam_target = _cam_target.lerp(_player.global_position, 1.0 - exp(-4.0 * delta))
	var cam_basis := Basis(Vector3.UP, CAM_YAW) * Basis(Vector3.RIGHT, CAM_PITCH)
	_camera.global_basis = cam_basis
	_camera.global_position = _cam_target + Vector3.UP * 0.6 + cam_basis.z * _follow_distance

func _on_coin_collected() -> void:
	_coins_collected += 1
	print("COIN %d/3" % _coins_collected)

func _on_goal_reached() -> void:
	if _coins_collected >= 3:
		print("WIN")
		if "--bot" in OS.get_cmdline_user_args():
			get_tree().quit()
	else:
		print("NEED_COINS")
