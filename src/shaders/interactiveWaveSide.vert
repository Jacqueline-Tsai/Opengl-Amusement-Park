#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 Position;
//out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec2 min = vec2(-50, -50), max = vec2(50, 50), range = max - min;
    vec3 pos = position ;
    gl_Position = projection * view * vec4(pos, 1.0f);
    Position = vec3(model * vec4(pos, 1.0f));

    Normal = normal;
}


