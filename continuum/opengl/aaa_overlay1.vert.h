// This file was automatically generated from a command line like:
// application_build_helper --inline-text "aaa_overlay1.vert"
#pragma once


const char g_psz_aaa_overlay1_vert[] = R"vert_text(#version 330 core

out vec2 viewportUv;

void main()
{
    const vec2 positions[3] = vec2[3](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    const vec2 textureCoordinates[3] = vec2[3](
        vec2(0.0, 0.0),
        vec2(2.0, 0.0),
        vec2(0.0, 2.0)
    );

    gl_Position = vec4(
        positions[gl_VertexID],
        0.0,
        1.0);

    viewportUv = textureCoordinates[gl_VertexID];
})vert_text";




