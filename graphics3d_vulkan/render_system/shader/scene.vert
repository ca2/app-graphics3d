//
// scene.vert Vulkan
//

#version 450


// Vertex inputs
layout(location = 0) in vec3 inputWorldCoordinate;
layout(location = 1) in vec3 inputNormal;
layout(location = 2) in vec2 inputTextureCoordinate;
layout(location = 3) in vec4 inputColor;
layout(location = 4) in vec4 inputTangent;   // .xyz = tangent, .w = bitangent sign


// Outputs to fragment shader
layout (location = 0) out vec3 fragmentWorldCoordinate;
layout (location = 1) out vec3 fragmentNormal;
layout (location = 2) out vec2 fragmentTextureCoordinate;
layout (location = 3) out vec4 fragmentColor;
layout (location = 4) out vec3 fragmentTangent;
layout (location = 5) out vec3 fragmentBitangent;


// Must match fragment shader
struct PointLight 
{

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
    int padding1;
    int padding2;
    int padding3;

} globalUbo;


// Push constants for model transforms (Vulkan-style)
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
    vec3 multiplier;

} pushConsts;


void main() 
{

    vec4 worldPosition = pushConsts.modelMatrix * vec4(inputWorldCoordinate, 1.0);
    gl_Position   = globalUbo.projection * globalUbo.view * worldPosition;


    mat3 normalMat = mat3(pushConsts.normalMatrix);


    vec3 normal = normalize(normalMat * inputNormal);
    vec3 tangent = normalize(normalMat * inputTangent.xyz);
    vec3 bitangent = cross(normal, tangent) * inputTangent.w;


    fragmentWorldCoordinate         = worldPosition.xyz;
    fragmentNormal                  = normal;
    fragmentTextureCoordinate       = inputTextureCoordinate;
    fragmentColor                   = inputColor;
    fragmentTangent                 = tangent;
    fragmentBitangent               = bitangent;

}



