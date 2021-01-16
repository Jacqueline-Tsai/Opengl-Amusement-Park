#version 330 core

in vec3 Normal;
in vec2 TexCoords;

out vec4 color;
uniform sampler2D surface;

void main()
{
    color = texture(surface, TexCoords);
}