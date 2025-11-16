#version 330 core

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform vec3 lightColor;
uniform vec3 lightPos;  


uniform sampler2D albedoMap;
uniform int useTexture;

uniform int state;

void main()
{
    vec3 color = objectColor;
    // for texture use/unuse
    if (useTexture == 1) {
        color = texture(albedoMap, TexCoord).rgb;
    }

    // for object select/unselect
    if (state == 1) {
        color.rgb += vec3(0.1, 0.2, 0.1);
    }
   
    // ambient lightning
    vec3 ambient = ambientStrength * lightColor;
    

    // Diffusion lightning
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // final output color
    color = (ambient + diffuse) * color;

    
    FragColor = vec4(color,1.0);
}
