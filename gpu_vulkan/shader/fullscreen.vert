#version 450

layout(location = 0) out vec2 uv;

void main() {
    vec2 verts[3] = vec2[](
        vec2(-1.0, -1.0),
        vec2(-1.0,  3.0),
        vec2( 3.0, -1.0)
    );

    vec2 v = verts[gl_VertexIndex];
    gl_Position = vec4(v, 0.0, 1.0);
    uv = 0.5 * (v + 1.0); // Remap from NDC to [0,1]
}