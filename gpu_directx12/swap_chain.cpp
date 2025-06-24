// Created by camilo on 2025-06-11 02:34 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include "windowing_win32/window.h"


#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")



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


   //void swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   //{




   //}


   void swap_chain::present(::gpu::texture * pgputexture)
   {

      ::cast < renderer > pgpurenderer = ::gpu::swap_chain::m_pgpurenderer;

      ::cast < context > pgpucontext = pgpurenderer->m_pgpucontext;

      m_size = pgpucontext->m_rectangle.size();

      if (m_iSwapChainCount < 0)
      {

         DXGI_SWAP_CHAIN_DESC desc = {};
         //ComPtr<IDXGISwapChain> swapChain; // or swapChain3 as IDXGISwapChain

         HRESULT hrSwapChainGetDesc = m_pdxgiswapchain->GetDesc(&desc);
         ::defer_throw_hresult(hrSwapChainGetDesc);
         m_iSwapChainCount = desc.BufferCount;
         // bufferCount now holds how many backbuffers the swap chain uses

      }

      //if (!m_pdxgiswapchain3)
      //{
      //   m_pdxgiswapchain1.as(m_pdxgiswapchain3);

      //}

      //UINT currentBackBufferIndex = m_pdxgiswapchain->GetCurrentBackBufferIndex();

      auto& ptextureSwapChain = m_textureaSwapChain.element_at_grow(m_iSwapChainIndex);

      if (!ptextureSwapChain)
      {

         __construct_new(ptextureSwapChain);

         ptextureSwapChain->m_bRenderTarget = true;

         ptextureSwapChain->m_bShaderResource = false;

         ptextureSwapChain->m_bDepthStencil = false;

         ptextureSwapChain->_initialize_gpu_texture(
            pgpurenderer,
            m_iSwapChainIndex,
            m_pdxgiswapchain);

         //m_pdxgiswapchain1->GetBuffer(0, __interface_of(m_ptextureSwapChain));

      }

      //if (!m_pblendstateDisabled)
      //{

      //   ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      //   D3D11_BLEND_DESC blendDesc = { 0 };
      //   blendDesc.RenderTarget[0].BlendEnable = FALSE;  // Disable blending
      //   blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

      //   HRESULT hr = pgpudevice->m_pdevice->CreateBlendState(&blendDesc, &m_pblendstateDisabled);
      //   ::defer_throw_hresult(hr);

      //}

      //{

      //   float blendFactor[4] = { 0, 0, 0, 0 }; // Not used
      //   UINT sampleMask = 0xFFFFFFFF;
      //   pgpucontext->m_pcontext->OMSetBlendState(m_pblendstateDisabled, blendFactor, sampleMask);

      //}

      //if (!m_prendertargetviewSwapChain)
      //{
      // 
      //   ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      //   pgpudevice->m_pdevice->CreateRenderTargetView(
      //      m_ptextureSwapChain, nullptr, &m_prendertargetviewSwapChain);

      //}

      //ID3D11RenderTargetView* rendertargetviewa[] = 
      //{
      //   m_ptextureSwapChain->m_prendertargetview
      //};
      //
      //pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, nullptr);

      // 2. Set viewport

      if (!m_pshaderPresent)
      {

         __construct_new(m_pshaderPresent);

         m_pshaderPresent->m_bTextureAndSampler = true;
         m_pshaderPresent->m_bDisableDepthTest = true;
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
    
//if(uv.x >0.5)
//{
  // return float4(0.1*0.5, 0.8*0.5, 0.98*0.5, 0.5); // test if the shader pipeline is running
//}
//else
//{
return tex.Sample(samp, float2(uv.x, 1.0 - uv.y));
//}
}
)shader";

         m_pshaderPresent->initialize_shader_with_block(
            pgpurenderer,
            as_block(fullscreen_vertex_shader),
            as_block(fullscreen_pixel_shader));

      }

      auto pcommandbuffer = pgpurenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      ptextureSwapChain->_new_state(pcommandlist, D3D12_RESOURCE_STATE_RENDER_TARGET);

      if (!ptextureSwapChain->m_pheapRenderTargetView)
      {

         ptextureSwapChain->create_render_target();
      }

      //{
      //   FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };
      //   D3D12_RECT r[1];
      //   r[0].left = 100;
      //   r[0].top = 100;
      //   r[0].right = 200;
      //   r[0].bottom = 200;
      //   pcommandlist->ClearRenderTargetView(ptextureSwapChain->m_handleRenderTargetView,
      //      colorRGBA2, 1, r);

      //}


      ::cast < texture > ptextureSrc = pgputexture;

      ptextureSrc->_new_state(pcommandlist, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

      m_pshaderPresent->bind(ptextureSwapChain, ptextureSrc);
      //pgpucontext->m_pcontext->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      //pgpucontext->m_pcontext->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

      //pgpucontext->m_pcontext->PSSetShaderResources(
      //   0, 1, m_ptextureSwapChain->m_pshaderresourceview.pp());
      //pgpucontext->m_pcontext->PSSetSamplers(
      //   0, 1, m_ptextureSwapChain->m_psamplerstate.pp());

 /*     D3D11_VIEWPORT vp = {};
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      vp.Width = static_cast<float>(m_size.cx());
      vp.Height = static_cast<float>(m_size.cy());
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      pgpucontext->m_pcontext->RSSetViewports(1, &vp);*/

      D3D12_VIEWPORT viewport = {};
      viewport.TopLeftX = 0;
      viewport.TopLeftY = 0;
      viewport.Width = static_cast<float>(m_size.cx());
      viewport.Height = static_cast<float>(m_size.cy());
      viewport.MinDepth = 0.0f;
      viewport.MaxDepth = 1.0f;

      D3D12_RECT scissorRect = {};
      scissorRect.left = 0;
      scissorRect.top = 0;
      scissorRect.right = static_cast<float>(m_size.cx());
      scissorRect.bottom = static_cast<float>(m_size.cy());

      //::cast < renderer > prenderer = m_pgpurenderer;


      //// 4. Set the viewport and scissor
      pcommandlist->RSSetViewports(1, &viewport);
      pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      pcommandlist->DrawInstanced(3, 1, 0, 0);


      m_pshaderPresent->unbind();


      //FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

      //pcommandlist->ClearRenderTargetView(ptextureSwapChain->m_handleRenderTargetView, 
        // colorRGBA2, 0, nullptr);

      //{
      //   FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };
      //   D3D12_RECT r[1];
      //   r[0].left = 200;
      //   r[0].top = 100;
      //   r[0].right = 300;
      //   r[0].bottom = 200;
      //   pcommandlist->ClearRenderTargetView(ptextureSwapChain->m_handleRenderTargetView,
      //      colorRGBA2, 1, r);

      //}

   }


   void swap_chain::set_present_state()
   {

      ::cast < renderer > pgpurenderer = ::gpu::swap_chain::m_pgpurenderer;

      if (pgpurenderer)
      {

         auto& ptextureSwapChain = m_textureaSwapChain[m_iSwapChainIndex];

         auto pcommandbuffer = pgpurenderer->getCurrentCommandBuffer2();

         auto pcommandlist = pcommandbuffer->m_pcommandlist;

         ptextureSwapChain->_new_state(pcommandlist, D3D12_RESOURCE_STATE_PRESENT);

      }

   }


   void swap_chain::swap_buffers()
   {

      m_pdxgiswapchain->Present(1, 0);

   }

   
   void swap_chain::get_new_swap_chain_index()
   {

      m_iSwapChainIndex = m_pdxgiswapchain->GetCurrentBackBufferIndex();

   }


   void swap_chain::initialize_swap_chain_window(::gpu::context* pgpucontext, ::windowing::window* pwindow)
   {

      ::gpu::swap_chain::initialize_swap_chain_window(pgpucontext, pwindow);

      ::cast < ::windowing_win32::window > pwin32window = pwindow;

      ::cast < context > pcontext = pgpucontext;

      auto& pdcompositiondevice = m_pdcompositiondevice;
      auto& pdcompositiontarget = m_pdcompositiontarget;
      auto& pdcompositionvisual = m_pdcompositionvisual;

      auto pdxgidevice = pcontext->_get_dxgi_device();

      ::defer_throw_hresult(DCompositionCreateDevice(
         pdxgidevice,
         __interface_of(pdcompositiondevice)));


      ::defer_throw_hresult(pdcompositiondevice->CreateTargetForHwnd(pwin32window->m_hwnd,
         true,
         &pdcompositiontarget));

      ::defer_throw_hresult(pdcompositiondevice->CreateVisual(&pdcompositionvisual));
      ::defer_throw_hresult(pdcompositionvisual->SetContent(m_pdxgiswapchain));
      ::defer_throw_hresult(pdcompositiontarget->SetRoot(pdcompositionvisual));
      ::defer_throw_hresult(pdcompositiondevice->Commit());

   }



} // namespace gpu_directx12



