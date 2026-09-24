// Created by camilo on 2026-08 with ChatGPT
#version 450




layout(set = 0, binding = 0) uniform sampler2D uTexture;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 outColor;

void main()
{

   outColor = texture(uTexture, uv);
   //outColor = texture(0.1, 0.4, 0.5, 0.5);

}
