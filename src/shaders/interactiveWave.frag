#version 330 core

in vec3 Normal;
in vec3 Position;
//in vec2 TexCoords;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform samplerCube tile;
int nn(float f){
    if(f==0)return 1;
    else if(f>0)return 2;
    else return 0;
}
vec3 n(vec3 v){
    return vec3(nn(v.x), nn(v.y), nn(v.z));
}
void main()
{
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(Position - cameraPos);
    vec4 reflection = texture(skybox, reflect(I, normalize(Normal)));

    vec3 R2 = refract(I, normalize(Normal), ratio);
    
    vec3 p0, p1, p2, target;
    if(R2.x > 0) p0 = Position + R2 / R2.x * (50 - Position.x);
    else p0 = Position + R2 / R2.x * (- 50 - Position.x);
    if(R2.y > 0) p1 = Position + R2 / R2.y * (50 - Position.y);
    else p1 = Position + R2 / R2.y * (- 50 - Position.y);
    if(R2.z > 0) p2 = Position + R2 / R2.z * (50 - Position.z);
    else p2 = Position + R2 / R2.z * (- 50 - Position.z);
    if(p0.y<50 &&p0.y>-50 && p0.z<50 && p0.z>-50) target=p0;
    if(p1.x<50 &&p1.x>-50 && p1.z<50 && p1.z>-50) target=p1;
    if(p2.y<50 &&p2.y>-50 && p2.x<50 && p2.x>-50) target=p2;

	vec4 refraction = texture(tile, target);
    vec4 blue = vec4(0.14, 0.3, 0.35, 1);
    color = mix(mix(reflection, refraction, 0.7), blue, 0.3);

    vec3 pNormal = vec3(abs(Normal.x), abs(Normal.y), abs(Normal.z)) * 50;
    vec3 pCam = vec3(abs(cameraPos.x), abs(cameraPos.y), abs(cameraPos.z));

    /*if(pNormal.x > pCam.x || pNormal.y > pCam.y || pNormal.z > pCam.z ||
        length(n(Normal) - n(cameraPos))>=3 || 1>0){
        color.a = 0;
    }*/    
}
