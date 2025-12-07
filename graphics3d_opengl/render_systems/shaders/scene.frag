//
// scene.frag  — OpenGL 330 core
//

#version 330 core

#define PI 3.1415926535897932384626433832795
#define GREYSCALE_WEIGHT_VECTOR vec3(0.2126, 0.7152, 0.0722)

in vec3 fragmentWorldCoordinate;
in vec3 fragmentNormal;
in vec2 fragmentTextureCoordinate;
in vec4 fragmentColor;
in vec3 fragmentTangent;
in vec3 fragmentBitangent;

layout(location = 0) out vec4 outputColor;

struct PointLight {
    vec4 position;
    vec4 color;
};

layout(std140) uniform GlobalUbo 
{
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    vec3 cameraPosition;
    PointLight pointLights[10];
    int numLights; 
} globalUbo;

uniform int useTextureAlbedo;
uniform int useTextureNormal;
uniform int useAlphaMask;

uniform vec3 albedo; 
uniform float metallic;
uniform float roughness;
uniform float ambientOcclusion;
uniform vec3 emissive; 
uniform float alphaMaskCutoff;

uniform float bloomBrightnessCutoff;
uniform vec3 multiplier;

// ---------- Samplers (must be standalone uniforms in GL) ----------
uniform samplerCube diffuseIrradianceMap;
uniform samplerCube prefilteredEnvMap;
uniform sampler2D brdfConvolutionMap;

uniform sampler2D textureAlbedo;
uniform sampler2D textureNormal;

// If your GL version doesn't support textureQueryLevels, provide max LOD from app
uniform float prefilteredEnvMapMaxLod; // set to (mipCount - 1.0) from host

// Tonemapping/fresnel helpers should be provided somewhere in GL code.
// e.g. provide fresnelSchlick(NdotV, F0) and ACESFilm() if needed.
// If you have a tonemapping.glsl, paste the functions into a shared header for GL.

// Simple helpers
vec4 getAlbedo() {

    if (useTextureAlbedo != 0) {
        return texture(textureAlbedo, fragmentTextureCoordinate);
    }
    else
    {    return vec4(albedo, 1.0);
    }
}

const float DIRECT_LIGHT_INTENSITY = 0.25;
const float IBL_INTENSITY          = 1.0;
const float EXPOSURE               = 1.0;

//
// NOTE: fresnelSchlick and ACESFilm not defined here — add them to a shared GL shader header.
//
vec3 fresnelSchlick(float cosTheta, vec3 F0); // implement in shared header
vec3 srgbToLinear(vec3 c);                     // optional if you want linearization
vec3 ACESFilm(vec3 x);                         // optional tonemapping


// ACES Filmic Tone Mapping (Narkowicz 2015)
vec3 ACESFilm(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}
 
// gamma correction
vec3 linearToSrgb(vec3 c) { return pow(c, vec3(1.0 / 2.2)); }
vec3 srgbToLinear(vec3 c) { return pow(c, vec3(2.2)); }
 
 
float DistributionGGX(float NdotH, float roughness) {
    float a  = roughness * roughness;
    float a2 = a * a;
    float denom = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom);
}
 
float GeometrySmith(float NdotV, float NdotL, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float gv = NdotV / (NdotV * (1.0 - k) + k);
    float gl = NdotL / (NdotL * (1.0 - k) + k);
    return gv * gl;
}
 
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}



void main() {
    // Albedo & alpha mask
    vec4 texColor = getAlbedo() * fragmentColor;

    if (useAlphaMask != 0 && texColor.a < alphaMaskCutoff) {
        discard;
    }

    // TBN and normal mapping
    mat3 TBN = mat3(fragmentTangent, fragmentBitangent, fragmentNormal);

    vec3 N = normalize(fragmentNormal);
    if (useTextureNormal != 0) {
        vec3 nMap = texture(textureNormal, fragmentTextureCoordinate).xyz * 2.0 - 1.0;
        N = normalize(TBN * nMap);
    }

    // View vector
    vec3 cameraPos = globalUbo.cameraPosition;
    vec3 V = normalize(cameraPos - fragmentWorldCoordinate);
    float NdotV = max(dot(N, V), 0.0);

    // Direct lighting (ambient + point lights)
    vec3 ambient = globalUbo.ambientLightColor.rgb * texColor.rgb * globalUbo.ambientLightColor.a;
    vec3 lighting = ambient;

    for (int i = 0; i < globalUbo.numLights; ++i) {
        vec3 L = normalize(globalUbo.pointLights[i].position.xyz - fragmentWorldCoordinate);
        vec3 H = normalize(L + V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL <= 0.0) continue;

        vec3 lightCol = globalUbo.pointLights[i].color.rgb * globalUbo.pointLights[i].color.a;

        float specPower = 32.0;
        float spec = pow(max(dot(H, N), 0.0), specPower);

        vec3 diffuse = texColor.rgb * NdotL * lightCol;
        vec3 specular = spec * lightCol;

        lighting += diffuse + specular;
    }

    lighting *= DIRECT_LIGHT_INTENSITY;

    // Image-based lighting (IBL)
    float fMetallic  = metallic;
    float fRoughness = clamp(roughness, 0.04, 1.0);
    float fAmbientOcclusion        = ambientOcclusion;
    if (useAlphaMask != 0) { fAmbientOcclusion = 0.02; }

    vec3 albedo = texColor.rgb; // assume already linear or convert with srgbToLinear()

    vec3 F0 = mix(vec3(0.04), albedo, fMetallic);

    // Diffuse IBL
    vec3 irradiance = texture(diffuseIrradianceMap, N).rgb;
    vec3 diffuseIBL = irradiance * albedo;

    // Specular IBL
    vec3 R = reflect(-V, N);

    // Use prefilteredEnvMapMaxLod set by host instead of textureQueryLevels
    float maxLod = prefilteredEnvMapMaxLod;
    vec3 prefiltered = textureLod(prefilteredEnvMap, R, fRoughness * maxLod).rgb;

    vec2 brdf = texture(brdfConvolutionMap, vec2(NdotV, fRoughness)).rg;

    vec3 F = fresnelSchlick(NdotV, F0);
    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - fMetallic);

    vec3 specIBL = prefiltered * (kS * brdf.x + brdf.y);
    vec3 ambientIBL = (diffuseIBL * kD + specIBL) * fAmbientOcclusion * IBL_INTENSITY;

    // Final composite
    vec3 color = lighting + ambientIBL;
    // color = ACESFilm(color * EXPOSURE); // optionally apply tonemapping

    outputColor = vec4(color, texColor.a);
    ////outputColor = texColor;
    //outputColor = vec4(color, texColor.a);
    //outputColor = vec4(fragmentColor.xyz, 1.0);
}
