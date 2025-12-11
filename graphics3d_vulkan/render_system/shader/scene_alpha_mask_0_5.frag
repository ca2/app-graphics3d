#version 450
#extension GL_KHR_vulkan_glsl : enable
#extension GL_GOOGLE_include_directive : enable
#include "tonemapping.glsl"

#define PI 3.1415926535897932384626433832795
#define GREYSCALE_WEIGHT_VECTOR vec3(0.2126, 0.7152, 0.0722)

layout(location = 0) out vec4 FragColor; // regular output
//layout(location = 1) out vec4 BloomColor; // output to be used by bloom shader

// vertex inputs (match your vertex shader locations)
layout (location = 0) in vec3 fragmentWorldCoordinate;
layout (location = 1) in vec3 fragmentNormal;
layout (location = 2) in vec2 fragmentTextureCoordinate;
layout (location = 3) in vec4 fragmentColor;
layout (location = 4) in vec3 fragmentTangent;
layout (location = 5) in vec3 fragmentBitangent;


struct PointLight {
    vec4 position;
    vec4 color;
};


// ---------- Global UBO (set 0 binding 0) ----------
layout(std140, set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    vec3 cameraPosition;
    // pointLights array
    PointLight pointLights[10];
    int numLights;
    int padding1;
    int padding2;
    int padding3;
} globalUbo;

//// ---------- Material scalar properties (no samplers) (set 0 binding 1) ----------
//layout(std140, set = 0, binding = 1) uniform MaterialProps {
//    // booleans promoted to ints (std140 rules); use 0/1 in C++ when updating
//    int useTextureAlbedo;
//    int useTextureMetallicRoughness;
//    int useTextureNormal;
//    int useTextureAmbientOcclusion;
//    int useTextureEmissive;
//
//    vec3 albedo;
//    float metallic;
//    float roughness;
//    float ambientOcclusion;
//    vec3 emissive;
//
//    // pad to 16-byte boundary
//    float _pad0;
//    vec4 _pad1;
//} pushConsts;
//
//
// IBL maps
layout(set = 1, binding = 0) uniform samplerCube diffuseIrradianceMap;
layout(set = 1, binding = 1) uniform samplerCube prefilteredEnvMap;
layout(set = 1, binding = 2) uniform sampler2D brdfConvolutionMap;

// ---------- Combined image samplers (set 1) ----------
layout(set = 2, binding = 0) uniform sampler2D textureAlbedo;
layout(set = 2, binding = 1) uniform sampler2D textureNormal;


// Push constants or UBO for camera + post params could be used; keep here as UBO-like values
// For simplicity we read cameraPosition from the GlobalUbo.viewPos (xyz) if you prefer.
// But keep dedicated uniform if you want:
layout(push_constant) uniform PushConsts 
{
    
    mat4 modelMatrix;
    mat4 normalMatrix; // inverse-transpose of model

    // booleans promoted to ints (std140 rules); use 0/1 in C++ when updating

    int useTextureAlbedo;
    int useTextureNormal;
    //int useTextureMetallicRoughness;
    //int useTextureAmbientOcclusion;
    //int useTextureEmissive;

    vec3 albedo;
    float metallic;
    float roughness;
    float ambientOcclusion;
    vec3 emissive;

    //vec3 cameraPosition;
    float bloomBrightnessCutoff;
    //int     useAlphaMask;
    //float alphaMaskCutOff;
    vec3 multiplier;

} pushConsts;

// Helper wrappers
vec4 getAlbedo() {
    vec4 a = vec4(pushConsts.albedo, 1.0);
    if (pushConsts.useTextureAlbedo != 0) {
        a = texture(textureAlbedo, fragmentTextureCoordinate);
    }
    return a;
}
const bool  ALPHA_MASK = true;
const float ALPHA_MASK_CUTOFF = 0.5;

// Constants
const float METALLIC_VALUE  = 1.0;
const float ROUGHNESS_VALUE = 1.0;
const float AO_VALUE        = 0.3;

const float DIRECT_LIGHT_INTENSITY = 0.25;
const float IBL_INTENSITY          = 1.0;
const float EXPOSURE               = 1.0; 



void main() {

    vec4 texColor = getAlbedo() * fragmentColor;

    if (ALPHA_MASK && texColor.a < ALPHA_MASK_CUTOFF) 
    {
        discard;
    }

    // Normal Mapping
    //vec3 N = normalize(inNormal);
    vec3 N = fragmentNormal;
    //vec3 T = normalize(inTangent.xyz);
    vec3 T = fragmentTangent;
    //vec3 B = cross(N, T) * inTangent.w;
    vec3 B = fragmentBitangent; 
    mat3 TBN = mat3(T, B, N);

    vec3 nMap = texture(textureNormal, fragmentTextureCoordinate).xyz * 2.0 - 1.0;
    N = normalize(TBN * nMap);

    //vec3 V = normalize(inViewVec);

    // camera position: prefer push constant, fallback to globalUbo.viewPos.xyz
    vec3 cameraPos = globalUbo.cameraPosition;
    // If you don't use push constants, you can use globalUbo.viewPos.xyz instead.
    // vec3 cameraPos = globalUbo.viewPos.xyz;

    vec3 V = normalize(cameraPos - fragmentWorldCoordinate);

    float NdotV = max(dot(N, V), 0.0);


    // Direct Light
    vec3 lighting = vec3(0.0);

    for (int i = 0; i < globalUbo.numLights; ++i) {

        vec3 L = normalize(globalUbo.pointLights[i].position.xyz - fragmentWorldCoordinate);
        vec3 H = normalize(L + V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL <= 0.0) continue;

        float NdotH = max(dot(N, H), 0.0);

        vec3 lightCol = globalUbo.pointLights[i].color.rgb * globalUbo.pointLights[i].color.a;

        // Simple Blinn-Phong for direct light
        float specPower = 32.0;
        float spec = pow(max(dot(H, N), 0.0), specPower);

        vec3 diffuse = texColor.rgb * NdotL * lightCol;
        vec3 specular = spec * lightCol;

        lighting += diffuse + specular;
    }

    lighting *= DIRECT_LIGHT_INTENSITY;


    // Image-Based Lighting
    float metallic  = METALLIC_VALUE;
    float roughness = clamp(ROUGHNESS_VALUE, 0.04, 1.0);
    float ao        = AO_VALUE;
    if (ALPHA_MASK) { ao = 0.02f; }
    //vec3 albedo = srgbToLinear(texColor.rgb);
    vec3 albedo = texColor.rgb;

    // Base reflectance
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // Diffuse IBL
    vec3 irradiance = texture(diffuseIrradianceMap, N).rgb;
    vec3 diffuseIBL = irradiance * albedo;

    // Specular IBL
    vec3 R = reflect(-V, N);

    int mipCount = textureQueryLevels(prefilteredEnvMap);
    float maxLod = float(max(0, mipCount - 1));

    vec3 prefiltered = textureLod(prefilteredEnvMap, R, roughness * maxLod).rgb;
    vec2 brdf = texture(brdfConvolutionMap, vec2(NdotV, roughness)).rg;

    vec3 F = fresnelSchlick(NdotV, F0);
    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    vec3 specIBL = prefiltered * (kS * brdf.x + brdf.y);

    vec3 ambientIBL = (diffuseIBL * kD + specIBL) * ao * IBL_INTENSITY;

    // Final Composite
    //vec3 color = lighting + ambientIBL + texColor.rgb * globalUbo.ambientLightColor.rgb;
    vec3 color = lighting + ambientIBL;
    //color = ACESFilm(color * EXPOSURE);

    FragColor = vec4(color, texColor.a);

}



