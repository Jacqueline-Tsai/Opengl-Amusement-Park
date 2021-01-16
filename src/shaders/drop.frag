#version 330 core

in vec2 TexCoords;

out vec4 fragcolor;

const float PI = 3.141592653589793;
const vec2 u_center = vec2(0.5, 0.5);
const float u_radius = 0.03, u_strength = 0.05;

uniform sampler2D u_water;

void main()
{
	vec4 info = texture2D(u_water, TexCoords);
	float len = length(u_center - TexCoords) - 0.1;
	if(len < 0) len = 0 - len;
	float drop = max(0.0, 1.0 - len / u_radius);
	drop = 0.5 - cos (drop * PI) * 0.5;
	info.r += drop * u_strength;
	fragcolor = info;
}

