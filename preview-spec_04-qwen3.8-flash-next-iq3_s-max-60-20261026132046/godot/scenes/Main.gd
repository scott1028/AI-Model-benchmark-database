extends Node3D

# Entry point. Modes: play (default) | bot | phys | shots, via `-- --mode X`.

const WorldSpawnerScript := preload("res://scripts/WorldSpawner.gd")

var mode := "play"
var spawner
var refs := {}
var robot  # RigidBody3D with Robot.gd attached (variant on purpose)
var camera: Camera3D
var hud: Label
var coins_taken := 0
var victory := false
var _shot_stage := -1
var _shot_t := 0.0


func _ready() -> void:
	var args := OS.get_cmdline_user_args()
	for i in args.size():
		if String(args[i]) == "--mode" and i + 1 < args.size():
			mode = String(args[i + 1])
	print("MAIN mode=%s args=%s" % [mode, str(args)])

	spawner = WorldSpawnerScript.new()
	add_child(spawner)
	refs = spawner.build(self)
	print("MAIN world built: props=%d coins=%d barrels=%d trees=%d bushes=%d" % [spawner.layout.props.size(), refs.coins.size(), refs.barrels.size(), refs.trees.size(), refs.bushes.size()])
	_spawn_robot()
	_make_camera()
	_make_hud()
	_wire_signals()

	match mode:
		"bot":
			var bot := Node.new()
			bot.name = "BotRunner"
			bot.set_script(load("res://scripts/BotRunner.gd"))
			add_child(bot)
			bot.setup(robot, spawner.layout.bot_path)
		"phys":
			var pr := Node.new()
			pr.name = "PhysRunner"
			pr.set_script(load("res://scripts/PhysRunner.gd"))
			add_child(pr)
			pr.setup(robot, spawner, refs)
		"shots":
			_shot_stage = 0
			_shot_t = 1.2
		"play":
			pass
		_:
			print("GAME_ERROR unknown_mode=%s" % mode)
			get_tree().quit(2)


func _spawn_robot() -> void:
	var s: Dictionary = spawner.layout.spawn
	robot = RigidBody3D.new()
	robot.set_script(load("res://scripts/Robot.gd"))
	add_child(robot)
	robot.build(load("res://assets/robot.glb"), Vector3(float(s.pos[0]), float(s.pos[1]), float(s.pos[2])), float(s.get("rot_y", 0.0)))


func _make_camera() -> void:
	var rig := Node3D.new()
	rig.name = "CameraRig"
	add_child(rig)
	camera = Camera3D.new()
	camera.name = "Camera"
	camera.far = 250.0
	rig.add_child(camera)
	rig.position = robot.position + Vector3(9, 11, 9)
	camera.look_at(robot.position + Vector3(0, 0.8, 0))


func _make_hud() -> void:
	var cl := CanvasLayer.new()
	cl.name = "HUD"
	add_child(cl)
	hud = Label.new()
	hud.position = Vector2(12, 10)
	hud.add_theme_color_override("font_color", Color(1, 1, 0.9))
	cl.add_child(hud)


func _wire_signals() -> void:
	for c in refs.coins:
		c.picked.connect(_on_coin)
	if refs.flag:
		refs.flag.touched.connect(_on_flag)


func _process(dt: float) -> void:
	_drift_clouds(dt)
	_follow_camera(dt)
	_update_hud()
	if mode == "shots":
		_shots(dt)
	if Input.is_action_just_pressed("ui_cancel"):
		get_tree().quit(0)


func _drift_clouds(dt: float) -> void:
	for c: Dictionary in refs.clouds:
		var n: Node3D = c.node
		n.position.x += float(c.speed) * dt
		if n.position.x > 24.0:
			n.position.x = -24.0


func _follow_camera(dt: float) -> void:
	if _shot_stage >= 1:
		return
	var rig := get_node("CameraRig")
	var want: Vector3 = robot.position + Vector3(9, 11, 9)
	var floor_y: float = spawner.height_at(want.x, want.z) + 2.5
	want.y = maxf(want.y, floor_y)
	rig.position = rig.position.lerp(want, 1.0 - exp(-3.0 * dt))
	camera.look_at(robot.position + Vector3(0, 0.8, 0))


func _update_hud() -> void:
	var txt := "Coin Run [%s]  coins %d/%d" % [mode, coins_taken, refs.coins.size()]
	if victory:
		txt += "   VICTORY!"
	hud.text = txt


func _on_coin(_coin_id: int) -> void:
	coins_taken += 1
	if mode == "bot":
		get_node("BotRunner").on_coin(_coin_id)


func _on_flag() -> void:
	victory = true
	if mode == "bot":
		get_node("BotRunner").on_victory()


func _shots(dt: float) -> void:
	if _shot_stage < 0:
		return
	_shot_t -= dt
	if _shot_t > 0.0:
		return
	var dir := OS.get_environment("SHOT_DIR")
	if dir.is_empty():
		dir = OS.get_user_data_dir().path_join("shots")
	DirAccess.make_dir_recursive_absolute(dir)
	if _shot_stage == 0:
		var img := get_viewport().get_texture().get_image()
		img.save_png(dir.path_join("gameplay_1.png"))
		print("SHOT_saved gameplay_1.png")
		# move to overhead overview
		var rig := get_node("CameraRig")
		rig.position = Vector3(0, 40, 0.01)
		camera.look_at(Vector3(0, 0, 0))
		_shot_stage = 1
		_shot_t = 0.5
	elif _shot_stage == 1:
		var img := get_viewport().get_texture().get_image()
		img.save_png(dir.path_join("overview_1.png"))
		print("SHOT_saved overview_1.png")
		print("SHOT_DONE")
		_shot_stage = -1
		get_tree().create_timer(0.2).timeout.connect(func() -> void: get_tree().quit(0))
