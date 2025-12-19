#version 450

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


// UBO: matches fragment shader binding and structure
layout (set = 0, binding = 0, std140) uniform GlobalUbo 
{

    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    vec3 cameraPosition;
    PointLight pointLights[10];
    int numLights;

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
layout(set = 2, binding = 2) uniform sampler2D textureMetallicRoughness;
layout(set = 2, binding = 3) uniform sampler2D textureAmbientOcclusion;
layout(set = 2, binding = 4) uniform sampler2D textureEmissive;


// Push constants or UBO for camera + post params could be used; keep here as UBO-like values
// For simplicity we read cameraPosition from the GlobalUbo.viewPos (xyz) if you prefer.
// But keep dedicated uniform if you want:
layout(push_constant) uniform PushConsts 
{
    
    mat4 modelMatrix;
    mat4 normalMatrix; // inverse-transpose of model

    // booleans promoted to ints (std140 rules); use 0/1 in C++ when updating

    int useTextureAlbedo;
    int useTextureMetallicRoughness;
    int useTextureNormal;
    int useTextureAmbientOcclusion;
    int useTextureEmissive;

    vec3 albedo;
    float metallic;
    float roughness;
    float ambientOcclusion;
    vec3 emissive;

    //vec3 cameraPosition;
    float bloomBrightnessCutoff;
    vec3 multiplier;

} pushConsts;

// Helper wrappers
vec3 getAlbedo() {
    vec3 a = pushConsts.albedo;
    if (pushConsts.useTextureAlbedo != 0) {
        a = texture(textureAlbedo, fragmentTextureCoordinate).rgb;
    }
    return a;
}

void getMetallicRoughness(out float metallic, out float roughness) {
    metallic = pushConsts.metallic;
    roughness = pushConsts.roughness;
    if (pushConsts.useTextureMetallicRoughness != 0) {
        vec3 mr = texture(textureMetallicRoughness, fragmentTextureCoordinate).rgb;
        metallic = mr.b;
        roughness = mr.g;
    }
}

vec3 getNormal(vec3 interpNormal) {
    vec3 n = interpNormal;
    if (pushConsts.useTextureNormal != 0) {
        vec3 tangentNormal = texture(textureNormal, fragmentTextureCoordinate).rgb;
        // convert from [0,1] to [-1,1]
        vec3 norm = normalize(tangentNormal * 2.0 - 1.0);
        mat3 TBN = mat3(fragmentTangent, fragmentBitangent, fragmentNormal);
        n = normalize(TBN * norm);
    }
    return n;
}

float getAO() {
    float ao = pushConsts.ambientOcclusion;
    if (pushConsts.useTextureAmbientOcclusion != 0) {
        ao = texture(textureAmbientOcclusion, fragmentTextureCoordinate).r;
    }
    return ao;
}

vec3 getEmissive() {
    vec3 e = pushConsts.emissive;
    if (pushConsts.useTextureEmissive != 0) {
        e = texture(textureEmissive, fragmentTextureCoordinate).rgb;
    }
    return e;
}

// ---------- PBR helper functions ----------
vec3 fresnelSchlick(float cosTheta, vec3 f0) {
    return f0 + (1.0 - f0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 f0, float roughness) {
    return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float ndfTrowbridgeReitzGGX(vec3 n, vec3 h, float roughness) {
    float alpha = roughness * roughness;
    float alphaSquared = alpha * alpha;
    float nDotH = max(dot(n, h), 0.0);
    float nDotHSquared = nDotH * nDotH;
    float innerTerms = nDotHSquared * (alphaSquared - 1.0) + 1.0;
    float numerator = alphaSquared;
    float denominator = PI * innerTerms * innerTerms;
    denominator = max(denominator, 0.0001);
    return numerator / denominator;
}

float geometrySchlickGGX(vec3 n, vec3 v, float k) {
    float nDotV = max(dot(n, v), 0.0);
    float numerator = nDotV;
    float denominator = nDotV * (1.0 - k) + k;
    return numerator / denominator;
}

float geometrySmith(vec3 n, vec3 v, vec3 l, float roughness) {
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    return geometrySchlickGGX(n, v, k) * geometrySchlickGGX(n, l, k);
}

// ---------- main ----------
void main() {
    vec3 worldPos = fragmentWorldCoordinate;
    vec3 albedo = getAlbedo();

    float metallic;
    float roughness;
    getMetallicRoughness(metallic, roughness);

    vec3 n = getNormal(fragmentNormal);
    float ao = getAO();
    vec3 emissive = getEmissive();

    // camera position: prefer push constant, fallback to globalUbo.viewPos.xyz
    vec3 cameraPos = globalUbo.cameraPosition;
    // If you don't use push constants, you can use globalUbo.viewPos.xyz instead.
    // vec3 cameraPos = globalUbo.viewPos.xyz;

    vec3 v = normalize(cameraPos - worldPos);
    vec3 r = reflect(-v, n);//r *=pushConsts.multiplier;

    vec3 f0 = vec3(0.04);
    f0 = mix(f0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // Direct lights: use up to 4 (your loop used 4). Use min with provided numLights.
    int maxDirect = min(4, globalUbo.numLights);
    for (int i = 0; i < maxDirect; ++i) {
        vec3 lightPos = globalUbo.pointLights[i].position.xyz;
        vec3 lightColor = globalUbo.pointLights[i].color.rgb;

        vec3 l = normalize(lightPos - worldPos);
        vec3 h = normalize(v + l);

        float distance = length(lightPos - worldPos);
        float attenuation = 1.0 / max(distance * distance, 0.0001);
        vec3 radiance = lightColor * attenuation;

        float dTerm = ndfTrowbridgeReitzGGX(n, h, roughness);
        vec3 fTerm = fresnelSchlick(max(dot(h, v), 0.0), f0);
        float gTerm = geometrySmith(n, v, l, roughness);

        vec3 numerator = dTerm * fTerm * gTerm;
        float denominator = 4.0 * max(dot(v, n), 0.0) * max(dot(l, n), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        vec3 kSpecular = fTerm;
        vec3 kDiffuse = vec3(1.0) - kSpecular;
        kDiffuse *= 1.0 - metallic;

        vec3 diffuse = kDiffuse * albedo / PI;
        vec3 cookTorranceBrdf = diffuse + specular;
        float nDotL = max(dot(n, l), 0.0);

        Lo += cookTorranceBrdf * radiance * nDotL;
    }

    // Indirect lighting (IBL)
    vec3 kSpecular = fresnelSchlickRoughness(max(dot(n, v), 0.0), f0, roughness);
    vec3 kDiffuse = vec3(1.0) - kSpecular;
    kDiffuse *= 1.0 - metallic;

    vec3 irradiance = texture(diffuseIrradianceMap, n).rgb;
    vec3 diffuseIBL = irradiance * albedo;

    //vec3 r_fixed = vec3(r.x, r.y, -r.z);
    vec3 r_fixed = r;
    vec3 prefilteredEnvMapColor = textureLod(prefilteredEnvMap, r_fixed, roughness * 4.0).rgb; // PREFILTERED_ENV_MAP_LOD = 4.0
    float NdotV = max(dot(n, v), 0.0);
    vec2 brdf = texture(brdfConvolutionMap, vec2(NdotV, roughness)).rg;
    vec3 specularIBL = prefilteredEnvMapColor * (kSpecular * brdf.x + brdf.y);

    vec3 ambient = (kDiffuse * diffuseIBL + specularIBL) * ao;

    vec3 color = emissive + ambient + Lo;

    FragColor = vec4(color, 1.0);
    //FragColor = vec4(albedo, 1.0);
    //FragColor = vec4(float(0), float(pushConsts.useTextureAlbedo), float(0), 1.0);
    //FragColor = vec4(specularIBL, 1.0);
    //FragColor = vec4(vec3(ao), 1.0);
    //FragColor = vec4(fragmentTangent*0.5+0.5, 1.0); // visualize fragmentTangent

    //float greyscaleBrightness = dot(FragColor.rgb, GREYSCALE_WEIGHT_VECTOR);
    //BloomColor = greyscaleBrightness > pushConsts.bloomBrightnessCutoff ? FragColor : vec4(0.0);
}
