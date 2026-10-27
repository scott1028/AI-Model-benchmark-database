extends Camera3D
# Isometric 45° follow camera (PLAYBOOK 鏡頭: 等角 45° 平滑跟隨；方向鍵相對鏡頭).
# The fixed view direction is what makes movement camera-relative: player.gd projects its
# input onto this camera's ground plane.
#
# The environment's near/far DOF band (atmosphere.gd) straddles this camera distance, so the
# robot stays sharp while the near and far ground blurs -> miniature-model (tilt-shift) look.

const ISO_DEFAULT := Vector3(1.0, 1.4142, 1.0)  # 45° elevation, from the +X +Z corner

var iso_dir := ISO_DEFAULT.normalized()
var target: Node3D
var follow_offset := Vector3(0.0, 0.9, 0.0)
var distance := 21.0
var follow_rate := 5.5

var _focus := Vector3.ZERO
var _started := false


func _process(delta: float) -> void:
	if target == null:
		return
	var want_focus := target.global_position + follow_offset
	# keep the robot in the lower-centre of frame: bias the look-at toward the camera a little
	var framed := want_focus - iso_dir * 1.6
	_focus = framed if not _started else _focus.lerp(framed, 1.0 - exp(-follow_rate * delta))
	global_position = _focus + iso_dir * distance
	look_at(want_focus, Vector3.UP)
	_started = true
