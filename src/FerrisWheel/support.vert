#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat4 test = mat4(1.0f);//mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,0);
    test[0].x=0.2;
    test[0].y=0.1;
    test[2].z=0.1;
    gl_Position = projection * view * model * vec4(position, 1); //
}