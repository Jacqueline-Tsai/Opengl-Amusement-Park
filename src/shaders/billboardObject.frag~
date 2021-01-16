#version 430 core
in vec2 fragTexCoords;
flat in int valid;
out vec4 f_color;

uniform sampler2D billboardTexture;
uniform vec4 billboardColor;
uniform int mode;

void main()
{
    if (valid == 0) {

        discard;
    }
    if (mode == 1) {

        f_color = texture(billboardTexture, fragTexCoords);
    }
    else {

	f_color = billboardColor;
    }
}
