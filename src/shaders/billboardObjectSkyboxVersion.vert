#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 fragTexCoords;
flat out int valid;

uniform mat4 view;
uniform mat4 projection;
uniform vec2 billboardSize;

void main()
{
    gl_Position = projection * mat4(mat3(view)) * vec4(position, 1);
    fragTexCoords = texCoords;
    valid = 1;
}
