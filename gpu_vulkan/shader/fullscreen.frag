
#version 450

layout(binding = 0) uniform sampler2D tex;

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

void main() {
    // Optional: flip Y here if needed
    outColor = texture(tex, vec2(uv.x, 1.0 - uv.y));
    //outColor=vec4(0.5*0.5,0.75*0.5,0.95*0.5,0.5);

}