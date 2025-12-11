// This file was automatically generated from a command line like:
// application_build_helper --inline-text "scene_frag.vert"
#pragma once


const char g_psz_scene_frag_vert[] = R"vert_text(#version 330 core

// Vertex inputs
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inColor;
layout(location = 4) in vec4 inTangent;   // .xyz = tangent, .w = bitangent sign

// Scene UBO
struct PointLight {
    vec4 position;
    vec4 color;
};

layout(std140) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
	 vec4 viewPos;
    PointLight pointLights[10];
    int numLights; // Needs to be padded to 16 bytes in std140 layout
    // Add padding to align to 16 bytes
    int padding1;
    int padding2;
    int padding3;
};

// Per-node UBO

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;           // inverse-transpose of model

// Outputs to fragment
out vec3 fragNormal;
out vec4 fragColor;
out vec2 fragUV;
out vec3 fragViewVec;
out vec3 fragWorldPos;
out vec4 fragTangent;

void main() {
    // world-space position
    vec4 worldPos = modelMatrix * vec4(inPos, 1.0);
    gl_Position   = projection * view * worldPos;

    // normals & tangents in world-space
    fragNormal   = normalize(mat3(normalMatrix) * inNormal);
    vec3 tangentWS = normalize(mat3(normalMatrix) * inTangent.xyz);
    fragTangent = vec4(tangentWS, inTangent.w);

    // color & UV
    fragColor    = inColor;
    fragUV       = inUV;

    // view & light vectors
    vec3 camPos = viewPos.xyz;
    fragWorldPos = worldPos.xyz;

    fragViewVec  = camPos - worldPos.xyz;
}
)vert_text";




