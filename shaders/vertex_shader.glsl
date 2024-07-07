#version 330 core
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

uniform mat4 uProjection;
uniform mat4 uModel;

out vec3 fragNormal;
out vec2 fragTexCoords;

void main() {
    gl_Position = uProjection * uModel * vec4(inPosition, 1.0);
    fragNormal = inNormal;
    fragTexCoords = inTexCoords;
}
