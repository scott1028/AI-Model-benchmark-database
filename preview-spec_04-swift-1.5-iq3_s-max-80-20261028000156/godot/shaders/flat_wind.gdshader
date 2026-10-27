shader_type spatial;
// Flat-shaded low-poly surface with a wind sway (PLAYBOOK: 樹與草叢隨微風擺動).
// Replaces the imported StandardMaterial3D on tree/bush meshes; the albedo comes from that material.

uniform vec4 base_color : source_color = vec4(0.5, 0.7, 0.4, 1.0);
uniform float wind_strength = 0.06;      // metres of tip displacement
uniform float wind_speed = 1.25;
uniform float phase = 0.0;               // per-instance offset so the grove is not in lockstep
uniform float gust = 0.0;                // extra amplitude (player hit / bush brush)
uniform float sway_ref_height = 2.0;     // height (above the instance origin) of full sway

void vertex() {
	float h = clamp(VERTEX.y / max(sway_ref_height, 0.05), 0.0, 1.6);
	h *= h; // cantilever: base barely moves, tips move most
	float t = TIME * wind_speed + phase;
	float s = sin(t) * 0.62 + sin(t * 2.3 + 1.7) * 0.28 + sin(t * 0.47 + 0.9) * 0.1;
	float amp = (wind_strength + gust) * h;
	VERTEX.x += s * amp;
	VERTEX.z += cos(t * 0.83 + phase) * amp * 0.7;
}

void fragment() {
	ALBEDO = base_color.rgb;
	ROUGHNESS = 0.92;
	METALLIC = 0.0;
}
