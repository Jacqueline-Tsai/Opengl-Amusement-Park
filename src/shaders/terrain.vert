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
    gl_Position = projection * view * model * vec4(position, 1);
	Normal = vec3(0, 1, 0);
    TexCoords = vec2(position.x, position.z);
}