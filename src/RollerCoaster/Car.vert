#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec2 TexCoords;
uniform mat4 model, view, projection;
uniform vec3 x, y, z, center; 

void main()
{
    vec3 pos = center + x * position.x + y * position.y + z * position.z;
    gl_Position = projection * view * model * vec4(pos, 1);
    vec3 coord = vec3(position.x / 2 + 0.5, position.y / 2 + 0.5, position.y / 2 + 0.5);
    if (coord.x == 1 || coord.x == 0)TexCoords = vec2(coord.y, coord.z);
    if (coord.y == 1 || coord.y == 0)TexCoords = vec2(coord.x, coord.z);
    if (coord.z == 1 || coord.z == 0)TexCoords = vec2(coord.x, coord.y);
}