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
    vec2 min = vec2(0, 0), max = vec2(50, 50), range = max - min;
	vec3 pos = vec3(position.x * range.x + min.x, 0, position.z * range.y + min.y);
	TexCoords = vec2(position.x, position.z);
    float height = texture(heightmap, TexCoords).r;
    pos.y = height * 20;
    gl_Position = projection * view * vec4(pos, 1.0f);
    Position = vec3(model * vec4(pos, 1.0f));

    vec2 vecSize = vec2(1.0/512, 1.0/512);
    ivec3 off = ivec3(-1,0,1);
    float s11 = height;
    float s01 = textureOffset(heightmap, TexCoords, off.xy).x;
    float s21 = textureOffset(heightmap, TexCoords, off.zy).x;
    float s10 = textureOffset(heightmap, TexCoords, off.yx).x;
    float s12 = textureOffset(heightmap, TexCoords, off.yz).x;
    vec3 va = normalize(vec3(vecSize.xy, s21 - s01));
    vec3 vb = normalize(vec3(vecSize.yx, s12 - s10));
    vec4 bump = vec4(cross(va, vb), s11 );

    Normal = mat3(transpose(inverse(model))) * bump.xyz;
    
    /*vec4 eyeVec = vec4(view * vec4(Position, 1));
    Normal = normalize(cross(dFdx(eyeVec.xyz), dFdy(eyeVec.xyz)));*/
    Normal = vec3(0,1,0);//
}