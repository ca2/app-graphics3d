#version 450

// Vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec4 aTangent;

// Outputs to fragment shader
layout (location = 0) out vec3 worldCoordinates;
layout (location = 1) out vec2 textureCoordinates;
layout (location = 2) out vec3 tangent;
layout (location = 3) out vec3 bitangent;
layout (location = 4) out vec3 normal;

// Must match fragment shader
struct PointLight {
    vec4 position;
    vec4 color;
};

// UBO: matches fragment shader binding and structure
layout (set = 0, binding = 0, std140) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    vec3 cameraPosition;
    PointLight pointLights[10];
    int numLights;
    int padding1;
    int padding2;
    int padding3;
} ubo;

// Push constants for model transforms (Vulkan-style)
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

} pushConsts;

void main() {
    // Transform position to world space
    vec4 worldPos = pushConsts.modelMatrix * vec4(aPos, 1.0);
    worldCoordinates = worldPos.xyz;

    // Final clip-space position
    gl_Position = ubo.projection * ubo.view * worldPos;

    // Pass through texture coordinates
    textureCoordinates = aTextureCoordinates;

    // Normal, tangent, bitangent in world space
    mat3 normalMat = mat3(pushConsts.normalMatrix);

    vec3 N = normalize(normalMat * aNormal);
    vec3 T = normalize(normalMat * aTangent.xyz);
    vec3 B = cross(N, T) * aTangent.w;

    normal = N;
    tangent = T;
    bitangent = B;
}
