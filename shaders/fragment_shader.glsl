#version 330 core
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 outColor;

uniform vec3 lightDir; // Light direction
uniform float brightness; // Brightness factor

void main() {
    // Simple diffuse lighting
    float diff = max(dot(normalize(fragNormal), normalize(lightDir)), 0.0);
    vec3 color = vec3(1.0, 1.0, 1.0) * diff * brightness; // Apply brightness
    outColor = vec4(color, 1.0);
}
