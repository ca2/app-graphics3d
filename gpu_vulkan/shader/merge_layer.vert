// Created by camilo on 2026-08 with ChatGPT
#version 450

layout(push_constant) uniform PushConstants
{
   vec4 quad;
   // left, bottom, right, top
} pc;

layout(location = 0) out vec2 uv;

void main()
{

   const vec2 pos[3] = vec2[](
      vec2(-1.0, -1.0),
      vec2(-1.0,  3.0),
      vec2( 3.0, -1.0)
   );

   const vec2 tex[3] = vec2[](
      vec2(0.0, 0.0),
      vec2(0.0, 2.0),
      vec2(2.0, 0.0)
   );

   gl_Position = vec4(pos[gl_VertexIndex], 0.0, 1.0);

   vec2 uvFull = tex[gl_VertexIndex];

   uv = vec2(
      mix(pc.quad.x, pc.quad.z, uvFull.x),
      mix(pc.quad.y, pc.quad.w, uvFull.y)
   );

}
