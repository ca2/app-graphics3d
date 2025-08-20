#version 450

layout(location = 0) out vec2 TexCoords;

layout(push_constant) uniform PushConstants {
    //mat4 projection;
    vec4 quad;        // l, t, r, b
    vec4 texcoords;   // l, t, r, b
    vec4 textColor;
} pc;

void main() {
    // 4 vertices: 0–3
    vec2 positions[4] = vec2[](
        vec2(pc.quad.x, pc.quad.y),
        vec2(pc.quad.z, pc.quad.y),
        vec2(pc.quad.x, pc.quad.w),
        vec2(pc.quad.z, pc.quad.w)
    );

    vec2 uvs[4] = vec2[](
        vec2(pc.texcoords.x, pc.texcoords.y),
        vec2(pc.texcoords.z, pc.texcoords.y),
        vec2(pc.texcoords.x, pc.texcoords.w),
        vec2(pc.texcoords.z, pc.texcoords.w)
    );

    int vid = gl_VertexIndex;
    //gl_Position = pc.projection * vec4(positions[vid], 0.0, 1.0);
    gl_Position =  vec4(positions[vid], 0.0, 1.0);
    TexCoords = uvs[vid];
}