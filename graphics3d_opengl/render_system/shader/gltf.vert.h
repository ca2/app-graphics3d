// This file was automatically generated from a command line like:
// application_build_helper --inline-text "gltf.vert"
#pragma once


const char g_psz_gltf_vert[] = R"vert_text(#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoordinates;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec4 aTangent;

out vec3 worldCoordinates;
out vec2 textureCoordinates;
out vec3 tangent;
out vec3 bitangent;
out vec3 normal;

struct PointLight {
    vec4 position;
    vec4 color;
};

layout(std140) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
    vec3 cameraPosition;
    PointLight pointLights[10];
    int numLights;
} globalUbo;

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;   // inverse-transpose of model

void main() {
    worldCoordinates = (modelMatrix * vec4(aPos, 1.0f)).xyz;
    gl_Position = globalUbo.projection * globalUbo.view * modelMatrix * vec4(aPos, 1.0f);
    textureCoordinates = aTextureCoordinates;

    mat3 normalMat = mat3(normalMatrix);

    vec3 N = normalize(normalMat * aNormal);
    vec3 T = normalize(normalMat * aTangent.xyz);
    vec3 B = cross(N, T) * aTangent.w;

    normal    = N;
    tangent   = T;
    bitangent = B;
}



)vert_text";




