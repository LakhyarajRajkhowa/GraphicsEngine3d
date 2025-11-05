#version 330 core

out vec4 FragColor;

uniform vec4 color;   // we can set sphere color

void main()
{
    FragColor = vec4(color);
}
