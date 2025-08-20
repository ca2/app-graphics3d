#version 450

layout(location = 0) in vec2 TexCoords;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    //mat4 projection;
    vec4 quad;        // l, t, r, b
    vec4 texcoords;   // l, t, r, b
    vec4 textColor;
} pc;

layout(set = 0, binding = 0) uniform sampler2D text;

void main() {
    vec4 sampled = texture(text, TexCoords).rgba;
    vec4 c = pc.textColor * sampled;
    outColor = vec4(c.rgb, c.a);
}
