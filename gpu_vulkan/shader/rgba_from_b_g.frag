#version 450
layout(binding = 0) uniform sampler2D metallicTex;
layout(binding = 1) uniform sampler2D roughnessTex;

layout(push_constant) uniform Push {
    int mipLevel;
} pc;


layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

void main() {
   float metallic  = textureLod(metallicTex,  fragUV, pc.mipLevel).r;
    float roughness = textureLod(roughnessTex, fragUV, pc.mipLevel).r;

    outColor = vec4(0.0, roughness, metallic, 1.0);
}
