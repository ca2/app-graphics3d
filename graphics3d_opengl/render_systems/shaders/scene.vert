#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inColor;
layout(location = 4) in vec4 inTangent;   // .xyz = tangent, .w = bitangent sign

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
};

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;  

out vec3 fragNormal;
out vec4 fragColor;
out vec2 fragUV;
out vec3 fragViewVec;
out vec3 fragWorldPos;
out vec4 fragTangent;

void main() 
{

    // world-space
    vec4 worldPos = modelMatrix * vec4(inPos, 1.0);
    gl_Position   = projection * view * worldPos;

    // world-space
    fragNormal   = normalize(mat3(normalMatrix) * inNormal);
    vec3 tangentWS = normalize(mat3(normalMatrix) * inTangent.xyz);
    fragTangent = vec4(tangentWS, inTangent.w);

    fragColor    = inColor;
    fragUV       = inUV;

    vec3 camPos = cameraPosition.xyz;
    fragWorldPos = worldPos.xyz;

    fragViewVec  = camPos - worldPos.xyz;

}
