#version 450 core

in VS_OUT {
    vec3 WorldPos;
    vec3 Normal;
    vec2 UV;
} fs;

out vec4 FragColor;

uniform vec3 cameraPos;

// Material inputs
uniform vec4 baseColor;
uniform float metallic;
uniform float roughness;
uniform int useTexture;
uniform sampler2D albedoMap;

// Directional light
uniform vec3 lightDir;
uniform vec3 lightColor;

// =========================
//  Helper functions
// =========================
float DistributionGGX(vec3 N, vec3 H, float r) {
    float a = r * r;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265 * denom * denom;
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float r) {
    float k = ((r + 1.0) * (r + 1.0)) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float r) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, r);
    float ggx1 = GeometrySchlickGGX(NdotL, r);
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// =========================
//       MAIN PBR
// =========================
void main() {
    vec3 N = normalize(fs.Normal);
    vec3 V = normalize(cameraPos - fs.WorldPos);

    vec3 albedo = baseColor.rgb;
    if (useTexture == 1)
        albedo = texture(albedoMap, fs.UV).rgb;

    // Base reflectivity
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 L = normalize(-lightDir);
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);

    // PBR components
    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator   = D * G * F;
    float denom      = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.001;
    vec3 specular    = numerator / denom;

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    vec3 diffuse = albedo / 3.14159265;

    vec3 radiance = lightColor;

    vec3 color = (kD * diffuse + specular) * radiance * NdotL;

    // Small ambient
    vec3 ambient = albedo * 0.03;

    FragColor = vec4(color + ambient, 1.0);
}
