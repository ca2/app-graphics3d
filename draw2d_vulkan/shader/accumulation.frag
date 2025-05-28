#version 450

layout(location = 0) in vec4 fragColor; // Input color with alpha

layout(location = 0) out vec4 outAccum; // RGB * alpha in RGB, 1.0 in A
layout(location = 1) out float outAlpha; // Just alpha

void main() {
    // Accumulate premultiplied color
    outAccum.rgb = fragColor.rgb * fragColor.a;
    outAccum.a = 1.0; // Dummy, not used

    // Accumulate alpha (denominator for resolve)
    outAlpha = fragColor.a;
}