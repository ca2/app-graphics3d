// This file was automatically generated from a command line like:
// application_build_helper --inline-text "scene.vert"
#pragma once


const char g_psz_scene_vert[] = R"vert_text(//
// scene.vert OpenGL
//

#version 330 core

// Vertex inputs
layout(location = 0) in vec3 inputWorldCoordinate;
layout(location = 1) in vec3 inputNormal;
layout(location = 2) in vec2 inputTextureCoordinate;
layout(location = 3) in vec4 inputColor;
layout(location = 4) in vec4 inputTangent;   // .xyz = tangent, .w = bitangent sign

out vec3 fragmentWorldCoordinate;
out vec3 fragmentNormal;
out vec2 fragmentTextureCoordinate;
out vec4 fragmentColor;
out vec3 fragmentTangent;
out vec3 fragmentBitangent;


struct PointLight 
{
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

uniform mat4 modelMatrix;
uniform mat4 normalMatrix; // inverse-transpose of model


void main() 
{

    
    vec4 worldPosition = modelMatrix * vec4(inputWorldCoordinate, 1.0);
    gl_Position = globalUbo.projection * globalUbo.view * worldPosition;


    mat3 normalMat = mat3(normalMatrix);


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
)vert_text";




