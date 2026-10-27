shader_type spatial;
// Terrain top surface: samples the baked palette map (tools/gen_terrain_texture.py) with linear
// filtering. The imported mesh paints each 1 m cell with one flat colour, which reads as a hard
// checkerboard; sampling the same palette smoothly keeps the layout's roads/plaza where they are
// and makes the lawn look like lawn (PLAYBOOK 附錄 B: 草坪不單調、不假).
// UV comes from object-space x/z (the slab spans +/- half_size), so it does not depend on mesh UVs.
// The glTF material is imported double-sided; a shader material defaults to cull_back, and the slab's
// top faces do not all share one winding -> culled faces showed the sky as grid-aligned blue patches.
render_mode cull_disabled;

uniform sampler2D palette : filter_linear;
uniform float half_size = 22.0;
// Matte earth: at the grazing angles of the isometric camera, a glossy ground mirrors the blue sky
// (2026-10-28 截圖: road / plaza read as wet puddles). F0 0.02 + roughness 1.0 keeps it dry soil.
uniform float rough = 1.0;
uniform float spec = 0.02;

varying vec2 surf_uv;

void vertex() {
	surf_uv = (VERTEX.xz + vec2(half_size)) / (2.0 * half_size);
}

void fragment() {
	ALBEDO = texture(palette, surf_uv).rgb;
	ROUGHNESS = rough;
	SPECULAR = spec;
}
