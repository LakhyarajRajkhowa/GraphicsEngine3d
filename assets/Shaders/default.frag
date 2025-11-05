#version 330 core

in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec4 baseColor;

uniform sampler2D albedoMap;
uniform int useTexture;

uniform int state;

void main()
{
    vec4 color = baseColor;

    if (useTexture == 1) {
        color = texture(albedoMap, TexCoord);
    }

    if (state == 1) {
        color.rgb += vec3(0.2);
    }
    else if (state == 2) {
        color = vec4(1.0, 0.85, 0.3, 1.0);
    }

    FragColor = color;
}
