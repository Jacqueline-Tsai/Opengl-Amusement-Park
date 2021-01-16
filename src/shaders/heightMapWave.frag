#version 330 core

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(Position - cameraPos);
    vec4 reflection = texture(skybox, reflect(I, normalize(Normal)));
	//vec4 refraction = texture(skybox, refract(I, normalize(Normal), ratio));
    vec4 blue = vec4(0.14, 0.3, 0.35, 1);
    color = mix(reflection, blue, 0.6);
    color.a = 0.7;
}

/*in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 fragcolor;

const float PI = 3.141592653589793;
const vec2 center = vec2(0.5, 0.5);
const float radius = 0.2;
const float strength = 1;

uniform sampler2D surface;
//uniform vec2 center;
//uniform float radius;
//uniform float strength;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec4 reflection = texture(skybox, reflect(I, normalize(Normal)));
    vec4 blue = vec4(0.14, 0.3, 0.35, 1);
    vec4 info = mix(reflection, blue, 0.7);

    float drop = max(0.0, 1.0 - length(center - TexCoords) / radius);
    drop = 0.5 - cos(drop * PI) * 0.5;
    info.r += drop * strength;

    fragcolor = info;
}*/

