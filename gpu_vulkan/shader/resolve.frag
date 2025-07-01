#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec2 uv;

layout(binding = 0) uniform sampler2D accumTex;
layout(binding = 1) uniform sampler2D alphaTex;

void main() {
    vec3 accum = texture(accumTex, uv).rgb;
    float alpha = texture(alphaTex, uv).r;
    outColor = vec4(accum / max(alpha, 0.0001), 1.0);
}