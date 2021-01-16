#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D Texture;

void main(void)
{
    float step_w = 1.0/1200, step_h = 1.0/600, threshold=0.1;
    vec4 result = vec4(0.0);

    if(TexCoords.y < 0.7){
        vec2 offset[25]; float kernel[25];
        for(int i=-2; i<3; i++){
            for(int j=-2; j<3; j++){
                offset[(i+2)*5+j+2] = vec2(i*step_w, j*step_h);
            }
        }
        kernel[0] = -1.0;    kernel[1] = -1.0;    kernel[2] = -1.0; kernel[3] = -1.0;    kernel[4] = -1.0;
        kernel[5] = -1.0;    kernel[6] = -2.0;     kernel[7] = -2.0;    kernel[8] = -2.0;   kernel[9] = -1.0;
        kernel[10] = -1.0;    kernel[11] = -2.0;     kernel[12] = 32.0;
        for(int i=13; i<25; i++){
            kernel[i] = kernel[24-i];
        }
        for(int i=13; i<25; i++){
            kernel[i] = kernel[24-i];
        }
        for(int i=0; i<25; i++ )  {
            vec4 tmp = texture2D(Texture, TexCoords + offset[i]);
            result += tmp * kernel[i];
        }
        result /= 25;
    }
    else{
        vec2 offset[9]; float kernel[9];
        for(int i=-1; i<2; i++){
            for(int j=-1; j<2; j++){
                offset[(i+1)*3+j+1] = vec2(i*step_w, j*step_h);
            }
        }
        kernel[0] = -1.0;    kernel[1] = -1.0;    kernel[2] = -1.0;
        kernel[3] = -1.0;    kernel[4] = 8.0;
        for(int i=5; i<9; i++) kernel[i] = kernel[8-i]; 
        for(int i=0; i<9; i++ ) result += texture2D(Texture, TexCoords + offset[i]) * kernel[i];    
        result /= 9;
    }
    

    if(result.x + result.y + result.z > threshold) result = vec4(0, 0, 0, 1);
    else result = vec4(1, 1, 1, 1);
    color = result;
}
