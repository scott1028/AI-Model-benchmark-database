shader_type canvas_item;
// Tilt-shift (miniature-model) depth of field: keep a band sharp, blur above and below it, so the
// diorama reads like a model on a table (PLAYBOOK 氣氛: 移軸).
// Godot 4.7 has no Environment DOF, and canvas_item shaders cannot read depth
// ("hint_depth_texture is not supported in canvas_item shaders", 2026-10-28 實測), so the focus
// band is driven from GDScript: tilt_shift.gd feeds the robot's projected screen Y every frame.

uniform sampler2D scene_tex : hint_screen_texture, filter_linear;
uniform float focus_y = 0.60;      // screen-space centre of the sharp band (0 top, 1 bottom)
uniform float band = 0.11;         // half width of the sharp band
uniform float strength = 1.0;      // 0 = effect off
uniform float max_radius = 0.012;  // blur radius in UV units
uniform float falloff = 1.35;      // >1 = blur ramps up faster away from the band

void fragment() {
	vec2 uv = SCREEN_UV;
	float dist = max(0.0, (abs(uv.y - focus_y) - band) / max(band, 0.01));
	float coc = pow(clamp(dist, 0.0, 1.0), falloff) * clamp(strength, 0.0, 2.0);

	vec3 col = texture(scene_tex, uv).rgb;
	if (coc > 0.003) {
		float r = max_radius * coc;
		vec3 sum = col;
		float w = 1.0;
		for (int i = 0; i < 8; i++) {
			float a = float(i) * 2.39996;  // golden angle -> even ring coverage
			vec2 dir = vec2(cos(a), sin(a));
			sum += texture(scene_tex, uv + dir * r).rgb * 0.6;
			sum += texture(scene_tex, uv + dir * r * 0.5).rgb * 0.6;
			w += 1.2;
		}
		col = sum / w;
	}
	COLOR = vec4(col, 1.0);
}
