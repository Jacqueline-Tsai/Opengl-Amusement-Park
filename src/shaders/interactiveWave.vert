#version 330 core
layout (location = 0) in vec3 position;

out vec3 Normal;
out vec3 Position;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightmap;


void main()
{
    vec2 min = vec2(-50, -50), max = vec2(50, 50), range = max - min;
	vec3 pos = vec3(position.x * range.x + min.x, 0, position.z * range.y + min.y);
	TexCoords = vec2(position.x, position.z);
    float height = texture(heightmap, TexCoords).r;
    pos.y = height * 20 + 40;
    gl_Position = projection * view * vec4(pos, 1.0f);
    Position = vec3(model * vec4(pos, 1.0f));

    Normal = vec3(0, 1, 0);
}


