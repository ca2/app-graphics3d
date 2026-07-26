// This file was automatically generated from a command line like:
// application_build_helper --inline-text "texture.frag"
#pragma once


const char g_psz_texture_frag[] = R"frag_text(#version 330 core

in vec2 fragTexCoord;

out vec4 outColor;



struct PointLight {
    vec4 position;
    vec4 color;
};
 
// Must match the same binding point as in your C++ side using glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboBuffer)
layout(std140) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    mat4 invView;
    vec4 ambientLightColor;
	 vec3 cameraPosition;
    PointLight pointLights[10];
    int numLights; // Needs to be padded to 16 bytes in std140 layout
    // Add padding to align to 16 bytes
} globalUbo;

 
// Instead of push constants, use a second uniform block or separate uniforms
uniform mat4 modelMatrix;


uniform sampler2D textureSampler;


void main()
{

    if (gl_FrontFacing)
    {
        outColor = texture(textureSampler, fragTexCoord);
    }
    else
    {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    //outColor = texture(textureSampler, fragTexCoord);
        //outColor = vec4(
        //fragTexCoord.x,
        //fragTexCoord.y,
        //0.0,
        //1.0);
})frag_text";




