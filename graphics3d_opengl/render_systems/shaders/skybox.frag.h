// This file was automatically generated from a command line like:
// application_build_helper --inline-text "skybox.frag"
#pragma once


const char g_psz_skybox_frag[] = R"frag_text(#version 330 core

uniform samplerCube skybox;




in vec3 vDirection;
out vec4 outColor;

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
    int numLights;
};


uniform vec3 multiplier;

void main() {
    vec3 dir = normalize(vDirection);
    dir *= multiplier;
    //dir.y = -dir.y; // Flip Y if needed
    outColor = texture(skybox, dir);
}
)frag_text";




