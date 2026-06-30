#version 330 core
in vec4 vColor;
in vec4 vBrightness;
in vec2 vUV;

uniform sampler2D particleTex;
uniform bool useTexture;

out vec4 FragColor;

void main() {
    vec4 texColor = useTexture ? texture(particleTex, vUV) : vec4(1.0);
    FragColor = texColor * vColor * vBrightness;
    if (FragColor.a < 0.01) discard;
}