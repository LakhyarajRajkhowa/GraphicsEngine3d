#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;   // e.g. vec3(1.0, 0.95, 0.5)

void main()
{

    FragColor = vec4(vec3(1.0, 0.95, 0.5), 1.0);
}
