#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 FragPos;       // world-space position
    vec3 Normal;        // world-space normal
    vec2 TexCoord;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vs_out.FragPos = worldPos.xyz;

    // correct normal transform to world space
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vs_out.Normal = normalize(normalMatrix * aNormal);

    vs_out.TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);

    gl_Position = projection * view * worldPos;
}
