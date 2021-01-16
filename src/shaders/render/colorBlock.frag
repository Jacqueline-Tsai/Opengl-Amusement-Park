#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D Texture;

void main(void)
{
    vec3 oriColor = vec3(texture2D(Texture, TexCoords));
    float gradient = 25.6;
    oriColor.x = int(oriColor.x * gradient)/gradient; oriColor.y = int(oriColor.y * gradient)/gradient; oriColor.z = int(oriColor.z * gradient)/gradient;
    color = vec4(oriColor, 1);
}
