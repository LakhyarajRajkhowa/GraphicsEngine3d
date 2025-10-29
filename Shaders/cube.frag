#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 viewPos ;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
   
    FragColor = vec4(0.54, 0.54, 0.54, 1.0);
}
