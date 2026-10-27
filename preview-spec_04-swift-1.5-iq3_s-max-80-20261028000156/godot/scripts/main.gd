extends Node3D
# Coin Run game root (PLAYBOOK Phase 7): wires world + player + camera + atmosphere, the
# collect-3-coins-then-touch-the-flag win rule, the HUD, the bot mode, and the screenshot presets.
#
#   --bot                 bot mode (bot.gd drives; input ignored)
#   --capture <preset> --out <path>   one screenshot then quit (tools/capture.py)
#     presets: play | overview | action | ground | road

const BOT_SCRIPT := preload("res://scripts/bot.gd")
const TAG := "COINRUN_BOT"
const SHOT_FRAMES := {"play": 100, "overview": 60, "action": 78, "ground": 70, "road": 90}
const SHOT_DIR := {"play": Vector3(-1.0, 0.0, -0.35), "action": Vector3(0.0, 0.0, 1.0),
		"road": Vector3(-1.0, 0.0, -0.28)}

@onready var world: Node3D = $World
@onready var player: CharacterBody3D = $Player
@onready var cam: Camera3D = $Camera
@onready var atmo: Node3D = $Atmosphere

var layout: Dictionary = {}
var coins_found := 0
var coin_total := 3
var won := false
var bot_mode := false
var elapsed := 0.0

var _hud: Label
var _status: Label
var _banner: Label
var _slab_focus: Node3D
var _coin_areas: Array[Area3D] = []
var _endpoint_areas: Array[Area3D] = []
var _collected: Dictionary = {}
var _args: PackedStringArray = []
var _capture_preset := ""
var _capture_out := ""
var _frames := 0
var _cam_check_frames := 0
var _drive_time := 0.0
var _status_time := 0.0


func _ready() -> void:
	_args = OS.get_cmdline_user_args()
	layout = world.layout
	coin_total = (layout.get("coins", []) as Array).size()
	atmo.follow = player
	atmo.cam = cam
	cam.target = player
	_slab_focus = Node3D.new()
	_slab_focus.name = "SlabFocus"
	world.add_child(_slab_focus)
	_spawn_player()
	_wire_triggers()
	_build_hud()
	_apply_shot_preset()
	if _args.has("--bot"):
		bot_mode = true
		var bot := BOT_SCRIPT.new()
		bot.name = "Bot"
		bot.main = self
		bot.player = player
		bot.world = world
		add_child(bot)
	print(TAG + " " + JSON.stringify({"event": "ready", "props": world.report.get("props", 0),
			"walls": world.report.get("walls", 0), "coins": coin_total, "bot": bot_mode}))


func _physics_process(delta: float) -> void:
	elapsed += delta
	if _args.has("--cam-check"):
		_cam_check_frames += 1
		if _cam_check_frames == 20:  # let the follow rig settle, then report its geometry
			_report_cam()
			return
	if _status_time > 0.0:
		_status_time = max(0.0, _status_time - delta)
		if _status_time == 0.0:
			_status.text = ""
	_spin_coins(delta)
	_scripted_drive(delta)
	_update_hud()
	if _capture_preset != "":
		_frames += 1
		if _frames == int(SHOT_FRAMES.get(_capture_preset, 100)):
			_take_shot()


# ---------------------------------------------------------------- gameplay

func _spawn_player() -> void:
	var spawn: Dictionary = (layout.get("player", {}) as Dictionary).get("spawn", {}) as Dictionary
	var x := float(spawn.get("x", 0.0))
	var z := float(spawn.get("z", 0.0))
	player.global_position = Vector3(x, world.height_at(x, z) + 0.06, z)
	player.configure_from_layout(layout.get("player", {}) as Dictionary)
	player.cam = cam


func _wire_triggers() -> void:
	for n in get_tree().get_nodes_in_group("coins"):
		if n is Area3D:
			_coin_areas.append(n as Area3D)
			(n as Area3D).body_entered.connect(_on_coin_area.bind(n as Area3D))
	for n in get_tree().get_nodes_in_group("endpoint"):
		if n is Area3D:
			_endpoint_areas.append(n as Area3D)
			(n as Area3D).body_entered.connect(_on_endpoint_area)


func _on_coin_area(body: Node, area: Area3D) -> void:
	if body != player or won:
		return
	var index := int(area.get_meta("coin_index", -1))
	if _collected.has(index):
		return
	_collected[index] = true
	coins_found = _collected.size()
	var visual := _visual_of(area)
	if visual != null:
		_pop(visual as Node3D)
	_status.text = "COIN %d / %d" % [coins_found, coin_total]
	_status_time = 1.6
	print(TAG + " " + JSON.stringify({"event": "coin", "index": index, "t": snappedf(elapsed, 0.01),
			"coins": coins_found, "x": snappedf(player.global_position.x, 0.1), "z": snappedf(player.global_position.z, 0.1)}))
	if coins_found == coin_total:
		_status.text = "ALL COINS - REACH THE FLAG"
		_status_time = 2.5


func _on_endpoint_area(body: Node) -> void:
	if body != player:
		return
	if coins_found < coin_total:
		_status.text = "NEED %d MORE COIN(S) FIRST" % (coin_total - coins_found)
		_status_time = 2.0
		if bot_mode:
			print(TAG + " " + JSON.stringify({"event": "progress", "note": "flag locked", "coins": coins_found,
					"t": snappedf(elapsed, 0.01)}))
		return
	win()


func win() -> void:
	if won:
		return
	won = true
	_banner.text = "YOU WIN\n3 coins + flag"
	_status.text = ""
	print(TAG + " " + JSON.stringify({"event": "win", "message": "All %d coins collected and the flag reached" % coin_total,
			"coins": coin_total, "elapsed": snappedf(elapsed, 0.01), "x": snappedf(player.global_position.x, 0.1),
			"z": snappedf(player.global_position.z, 0.1)}))
	if bot_mode:
		get_tree().create_timer(0.2).timeout.connect(func() -> void: get_tree().quit(0))


func _spin_coins(delta: float) -> void:
	for area in _coin_areas:
		var visual := _visual_of(area)
		if visual == null:
			continue
		visual.rotate_y(delta * 1.7)


func _visual_of(node: Node) -> Node:
	if not node.has_meta("visual"):
		return null
	return get_node_or_null(node.get_meta("visual") as NodePath)


func _pop(node: Node3D) -> void:
	var tw := create_tween()
	tw.set_parallel(true)
	tw.tween_property(node, "scale", Vector3(1.9, 1.9, 1.9), 0.22)
	tw.tween_property(node, "position:y", node.position.y + 0.7, 0.22)
	tw.chain().tween_callback(func() -> void: node.visible = false)


# ---------------------------------------------------------------- HUD

func _build_hud() -> void:
	var layer := CanvasLayer.new()
	layer.name = "HUD"
	layer.layer = 10  # above the tilt-shift post (layer 5), so the HUD stays sharp
	add_child(layer)
	_hud = Label.new()
	_hud.position = Vector2(16, 12)
	_hud.add_theme_font_size_override("font_size", 18)
	layer.add_child(_hud)
	_status = Label.new()
	_status.position = Vector2(16, 40)
	_status.add_theme_font_size_override("font_size", 20)
	layer.add_child(_status)
	_banner = Label.new()
	_banner.add_theme_font_size_override("font_size", 46)
	_banner.set_anchors_preset(Control.PRESET_CENTER)
	_banner.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	layer.add_child(_banner)
	if _args.has("--no-hud"):
		layer.visible = false


func _update_hud() -> void:
	var mode := "BOT" if bot_mode else "PLAYER"
	_hud.text = "COINS %d/%d   SPEED %.1f m/s   MODE %s" % [coins_found, coin_total, player.ground_speed(), mode]


# ---------------------------------------------------------------- camera check
# Headless evidence for the camera requirement (isometric 45, no roll, camera-relative input):
#   godot --headless --path godot --scene res://scenes/main.tscn -- --cam-check

func _report_cam() -> void:
	var view: Vector3 = -cam.global_transform.basis.z
	var pitch := rad_to_deg(asin(clampf(-view.y, -1.0, 1.0)))
	var right: Vector3 = cam.global_transform.basis.x  # a rolled camera would tilt this off the ground
	var d := cam.global_position.distance_to(player.global_position)
	var rel: bool = player.cam == cam
	var no_roll := absf(right.y) < 0.01
	print("CAM_REPORT " + JSON.stringify({
		"pitch_deg": snappedf(pitch, 0.01),
		"azimuth_deg": snappedf(rad_to_deg(atan2(view.x, view.z)), 0.01),
		"distance_m": snappedf(d, 0.01),
		"no_roll": no_roll,
		"camera_relative_input": rel,
		"follow_rate": cam.follow_rate}))
	get_tree().quit(0 if pitch > 40.0 and pitch < 55.0 and rel and no_roll else 1)


# ---------------------------------------------------------------- screenshots

func _arg_value(flag: String) -> String:
	var i := _args.find(flag)
	if i < 0 or i + 1 >= _args.size():
		return ""
	return str(_args[i + 1])


func _apply_shot_preset() -> void:
	_capture_preset = _arg_value("--capture")
	_capture_out = _arg_value("--out")
	if _capture_preset == "":
		return
	player.uses_input = false
	match _capture_preset:
		"overview":
			# 附錄 B: the whole slab in frame, no big empty margins
			cam.target = _slab_focus
			cam.distance = 52.0
			cam.iso_dir = Vector3(1.0, 1.18, 1.0).normalized()
			cam.follow_rate = 30.0
			atmo.tilt.set_effect(false)
			atmo.cloud_speed_scale = 0.0
			_place_shot(Vector3(10.0, 0.0, 13.0), Vector3.ZERO, false)
		"ground":
			cam.distance = 26.0
			cam.iso_dir = Vector3(1.0, 0.42, 1.0).normalized()
			atmo.tilt.configure(0.55, 0.2, 0.5)  # keep the ground readable, keep the miniature feel
			atmo.sun.rotation_degrees = Vector3(-15.0, -32.0, 0.0)
			atmo.sun.light_color = Color(1.0, 0.84, 0.62)
			atmo.sun.light_energy = 1.5
			_place_shot(Vector3(6.0, 0.0, 6.0), SHOT_DIR.get("play", Vector3.LEFT), false)
		"action":
			# 附錄 B: show a pushable prop doing its job — the robot pushes barrel5 toward the flag
			# (open ground at 1.4,-4.4: no tall prop on the camera->player line, so the robot stays visible)
			cam.distance = 9.5
			_place_shot(Vector3(1.4, 0.0, -4.4), SHOT_DIR.get("action", Vector3.LEFT), false)
		"road":
			cam.distance = 17.0
			_place_shot(Vector3(-2.0, 0.0, 6.0), SHOT_DIR.get("road", Vector3.LEFT), true)
		_:
			# 附錄 B: general play shot — the robot must read clearly, so keep the camera close
			cam.distance = 9.5
			_place_shot(Vector3(10.0, 0.0, 13.0), SHOT_DIR.get("play", Vector3.LEFT), true)
	if not _capture_out.is_absolute_path():
		_capture_out = ProjectSettings.globalize_path("res://") + _capture_out


func _place_shot(at: Vector3, dir: Vector3, boost: bool) -> void:
	player.global_position = Vector3(at.x, world.height_at(at.x, at.z) + 0.06, at.z)
	player.drive_dir = Vector2(dir.x, dir.z)
	player.drive_boost = boost
	_drive_time = 3.0


func _scripted_drive(delta: float) -> void:
	if _capture_preset == "" or _drive_time <= 0.0:
		return
	_drive_time -= delta
	if _drive_time <= 0.0:
		player.drive_dir = Vector2.ZERO
		player.drive_boost = false


func _take_shot() -> void:
	await RenderingServer.frame_post_draw
	var img := get_viewport().get_texture().get_image()
	var err := img.save_png(_capture_out)
	print("CAPTURE " + JSON.stringify({"preset": _capture_preset, "out": _capture_out, "ok": err == OK,
			"width": img.get_width(), "height": img.get_height(), "frames": _frames}))
	get_tree().quit(0 if err == OK else 1)
