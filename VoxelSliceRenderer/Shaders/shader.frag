#version 330 core

//#define NONLINEAR_SAMPLING
#define MAX_DEPTH 5
#define MIN_DEPTH 0.02
//#define DEPTH_FOG
#define ALPHA_SAMPLE_ADJUST 150

in vec4 gl_FragCoord;

out vec4 color;

uniform sampler3D scene;
uniform float val;
uniform int width;
uniform int height;
uniform vec3 cam_pos;
uniform vec3 cam_dir;
uniform vec3 cam_right;
uniform vec3 cam_up;


void main() {
	//color = vec4(1, 0.8, 0.7, 1);
	vec2 texco = vec2(gl_FragCoord.x / width - 0.5, gl_FragCoord.y / height - 0.5);
	float aspect = float(width) / float(height);
	//color = vec4(texco.x, texco.y, 0.7, 1);

	vec4 col;
	float alpha_rem = 1.0f; // alpha remaining
	for (float depth = MIN_DEPTH; depth <= MAX_DEPTH;
#ifdef NONLINEAR_SAMPLING
		depth *= max(1.01, val+1)
#else
		depth += max(0.002, val)
#endif
		) {
		vec3 lookup = cam_pos + (cam_right * texco.x * depth * aspect) + (cam_up * texco.y * depth) + (cam_dir * depth);
		vec4 tex = texture(scene, lookup);
		float a = tex.a;
#ifdef ALPHA_SAMPLE_ADJUST
		a *= val * ALPHA_SAMPLE_ADJUST;
#endif
#ifdef DEPTH_FOG
		a *= (1 - (depth / MAX_DEPTH));
#endif
		col += tex*min(alpha_rem, a);
		alpha_rem -= a;
		if (alpha_rem <= 0) break;
	}
	col += max(alpha_rem, 0) * vec4(1, 0.8, 0.7, 1);

	color = col;
}