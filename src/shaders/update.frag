#version 330 core

in vec2 TexCoords;

out vec4 fragcolor;

uniform sampler2D u_water;
const vec2 u_delta = vec2(1.0/512, 1.0/512);
const float speed = 0.01;

void main()
{
	vec4 info = texture(u_water, TexCoords);
	vec2 dx = vec2(u_delta.x, 0.0), dy = vec2(0.0, u_delta.y);
	float average = (texture(u_water, TexCoords - dx).r + texture(u_water, TexCoords - dy).r +
		texture(u_water, TexCoords + dx).r + texture(u_water, TexCoords + dy).r) * 0.25;
	//info.g += (average - info.r) * 2.0;
    //info.g *= 0.995;
    //info.r += info.g;
	vec2 tmp = vec2(TexCoords.x-0.5, TexCoords.y-0.5);
	float len = sqrt(tmp.x * tmp.x + tmp.y * tmp.y);
	if(len<0.01){
		fragcolor = vec4(0,0,0,1);
	}
	else{
		vec2 coord = vec2(tmp.x*(len-speed)/len+0.5, tmp.y*(len-speed)/len+0.5);
		fragcolor = texture(u_water, coord);
	}
}