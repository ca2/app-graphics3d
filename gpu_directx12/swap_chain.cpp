// Created by camilo on 2025-06-11 02:34 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "renderer.h"
#include "swap_chain.h"


#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
const char* fullscreen_vertex_shader = R"shader(// fullscreen_vs.hlsl
      struct VSOut {
         float4 pos : SV_POSITION;
         float2 uv : TEXCOORD0;
      };

      VSOut main(uint vid : SV_VertexID) {
         float2 verts[3] = {
             float2(-1, -1),
             float2(-1, +3),
             float2(+3, -1)
         };
         float2 uvs[3] = {
             float2(0, 1),
             float2(0, -1),
             float2(2, 1)
         };

         VSOut o;
         o.pos = float4(verts[vid], 0, 1);
         o.uv = 0.5 * (verts[vid] + 1.0);
         return o;
      }
)shader";

const char* fullscreen_pixel_shader = R"shader(// fullscreen_ps.hlsl
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
    return tex.Sample(samp, uv);
}
)shader";



namespace gpu_directx12
{


   swap_chain::swap_chain()
   {


	}


   swap_chain::~swap_chain()
   {

   }


	void swap_chain::initialize_render_target(::gpu::renderer* pgpurenderer, const ::int_size& size, ::pointer <::gpu::render_target>previous)
	{

		render_target_view::initialize_render_target(pgpurenderer, size, previous);

	}


	void swap_chain::on_init()
	{


	}


   void swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   {




   }


   void swap_chain::present()
   {


   }


} // namespace gpu_directx12



