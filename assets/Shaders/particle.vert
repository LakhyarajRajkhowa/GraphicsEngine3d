#version 330 core
layout(location = 0) in vec2 quadCorner;
layout(location = 1) in vec4 instPosSize; // xyz = world pos, w = size
layout(location = 2) in vec4 instColor;
layout(location = 3) in float instRotation;

uniform mat4 view;
uniform mat4 projection;

out vec4 vColor;
out vec2 vUV;

void main() {
    vec3 worldPos = instPosSize.xyz;
    float size    = instPosSize.w;

    // Camera-facing billboard basis (right/up extracted from view matrix)
    vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camUp    = vec3(view[0][1], view[1][1], view[2][1]);

    float c = cos(instRotation);
    float s = sin(instRotation);
    vec2 rotated = vec2(
        quadCorner.x * c - quadCorner.y * s,
        quadCorner.x * s + quadCorner.y * c
    );

    vec3 offset = (camRight * rotated.x + camUp * rotated.y) * size;

    gl_Position = projection * view * vec4(worldPos + offset, 1.0);
    vColor = instColor;
    vUV = quadCorner + 0.5; // 0..1
}