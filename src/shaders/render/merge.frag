#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D edgeTexture;
uniform sampler2D colorBlockTexture;

void main(void)
{
    float step_w = 1.0/1200, step_h = 1.0/600;
    vec4 result = texture(colorBlockTexture, TexCoords);

    /*vec2 offset[9];
    for(int i=-1; i<2; i++){
        for(int j=-1; j<2; j++){
            if(texture(edgeTexture, TexCoords + vec2(i*step_w, j*step_h)).x == 0) {
                result = vec4(0, 0, 0, 1);
                break;
            }
        }
    }*/

    if(texture(edgeTexture, TexCoords).x == 0) result = vec4(0, 0, 0, 1);
    
    color = result;
}
