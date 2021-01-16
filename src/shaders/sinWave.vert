#version 330 core
layout (location = 0) in vec3 position;

out vec3 Normal;
out vec3 Position;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float height, amplitude, wavelength, time, speed;

void main()
{
    vec2 min = vec2((-1) * 200, -50), max = vec2(-100, 50), range = max - min;
    float k = 2 * 3.14159 / wavelength;    
    vec3 pos = vec3(position.x * range.x + min.x, 0, position.z * range.y + min.y);
    float f = k * (pos.x - speed * time);
    pos.y = amplitude * sin(f) + height;
    vec3 tangent = normalize(vec3(1,k*amplitude*cos(f),0));
    //Normal = normalize(vec3(-tangent.y, tangent.x, 0));
    //Normal = normalize(cross(vec3(0, 0, 1), vec3(1, cos(f), 0)));
    Normal = normalize(vec3(-cos(f), 1, 0));
    if(normalize(cross(vec3(0, 0, 1), vec3(1, cos(f), 0))) != normalize(vec3(-cos(f), 1, 0))){
        pos.y = 0;
    }
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    Position = vec3(model * vec4(pos, 1.0f));
    TexCoords = vec2(position.x, position.z);
}