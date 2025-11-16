#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs;

out vec4 FragColor;

uniform vec3 cameraPos;

// material inputs
uniform vec4 baseColor;
uniform sampler2D albedoMap;
uniform int useTexture;

// light inputs (directional)
uniform vec3 lightDir;    // direction the light is coming FROM (e.g. normalize(vec3(0.2,-1,0.3)))
uniform vec3 lightColor;  // color/intensity
uniform vec3 ambient;     // small ambient term, e.g. vec3(0.03)
uniform float shininess;  // specular exponent
uniform float specularStrength; // specular multiplier

uniform int state; // your existing state for selection

void main()
{
    // base color / albedo
    vec3 albedo = baseColor.rgb;
    if (useTexture == 1) {
        albedo = texture(albedoMap, fs.TexCoord).rgb;
    }

    // normal and view vector
    vec3 N = normalize(fs.Normal);
    vec3 V = normalize(cameraPos - fs.FragPos);

    // directional light: L points *towards* the light source;
    // we treat lightDir as direction the light is coming FROM, so flip:
    vec3 L = normalize(-lightDir);

    // Ambient
    vec3 ambientTerm = albedo * ambient;

    // Diffuse (Lambert)
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = albedo * NdotL;

    // Specular (Blinn-Phong)
    vec3 H = normalize(V + L); // half-vector
    float NdotH = max(dot(N, H), 0.0);
    float spec = pow(NdotH, shininess);
    vec3 Fspec = lightColor * specularStrength * spec;

    // Compose final color (Lambert + specular), modulated by lightColor
    vec3 radiance = lightColor;
    vec3 color = (diffuse + Fspec) * radiance + ambientTerm;

    // selection states 
    vec4 outColor = vec4(color, baseColor.a);
    if (state == 1) {
        outColor.rgb += vec3(0.2); // brighten
    } else if (state == 2) {
        outColor = vec4(1.0, 0.85, 0.3, 1.0);
    }

    FragColor = outColor;
}
