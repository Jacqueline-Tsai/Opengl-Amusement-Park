#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 centerPosition;
layout (location = 3) in float velocity;
layout (location = 4) in float duration;

out vec2 fragTexCoords;
flat out int valid;

uniform mat4 view;
uniform mat4 inverseView;
uniform mat4 projection;
uniform vec2 billboardSize;
uniform vec3 cameraPosition;
uniform float maxVisibleDistance;

uniform vec3 direction;

void main()
{
    float distance = sqrt(pow(cameraPosition.x - centerPosition.x, 2) + pow(cameraPosition.y - centerPosition.y, 2) + pow(cameraPosition.z - centerPosition.z, 2));
    if (distance > maxVisibleDistance) {

        valid = 0;
    }
    else {

        valid = 1;
    }
    vec3 CameraRight_worldspace = { inverseView[0][0], inverseView[0][1], inverseView[0][2] };
    vec3 CameraUp_worldspace = { inverseView[1][0], inverseView[1][1], inverseView[1][2] };
    vec3 final_position = centerPosition + position.x * CameraRight_worldspace + position.y * CameraUp_worldspace;
    final_position = final_position + direction * velocity * duration;
    //final_position.y = position.y + velocity * duration;
    gl_Position = projection * view * vec4(final_position, 1);
    fragTexCoords = texCoords;
}
