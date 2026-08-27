// Created by camilo on 2025-06-11 02:34 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "command_buffer.h"
#include "window_attachment.h"
#include "frame.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include "windowing_win32/window.h"
#include "bred/gpu/binding.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/texture_site.h"
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


   void swap_chain::initialize_render_target(::gpu::renderer * pgpurenderer, const ::i32_size & size, ::pointer <::gpu::render_target>previous)
   {

      render_target_view::initialize_render_target(pgpurenderer, size, previous);

   }


   void swap_chain::on_init()
   {


   }


   void swap_chain::on_new_frame()
   {

      // DXGI owns back-buffer acquisition.  Refresh the index before ca2
      // selects per-frame layer textures, command buffers and descriptor heaps.
      swap_chain_frame_index();

   }


   bool swap_chain::create_frame_sync(frame_sync & frame)
   {

      if (!::gpu::swap_chain::create_frame_sync(frame))
      {

         return false;

      }

      // DXGI acquires back buffers implicitly and cannot signal this Vulkan-style semaphore.
      frame.m_pgpusemaphoreImageAvailable.release();

      return true;

   }


   //void swap_chain::endDraw(::draw2d_gpu::graphics* pdraw2dgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   //{




   //}


   void swap_chain::present(::gpu::texture_site * pgputexturesite, ::gpu::command_buffer * pgpucommandbuffer)
   {

      ::cast < renderer > pgpurenderer = ::gpu::swap_chain::m_pgpurenderer;

      ::cast < context > pgpucontext = pgpurenderer->m_pgpucontext;

      m_size = pgpucontext->size();

      if (m_iSwapChainCount < 0)
      {

         m_iSwapChainCount = swap_chain_frame_count();

         //DXGI_SWAP_CHAIN_DESC desc = {};
         ////ComPtr<IDXGISwapChain> swapChain; // or swapChain3 as IDXGISwapChain

         //HRESULT hrSwapChainGetDesc = m_pdxgiswapchain->GetDesc(&desc);
         //::defer_throw_hresult(hrSwapChainGetDesc);
         //m_iSwapChainCount = desc.BufferCount;
         //// bufferCount now holds how many backbuffers the swap chain uses

      }

      //if (!m_pdxgiswapchain3)
      //{
      //   m_pdxgiswapchain1.as(m_pdxgiswapchain3);

      //}

      //UINT currentBackBufferIndex = m_pdxgiswapchain->GetCurrentBackBufferIndex();

      defer_construct_newø(m_ptexturesiteaSwapChain);

      auto & ptexturesiteSwapChain = m_ptexturesiteaSwapChain->element_at_grow(m_iSwapChainIndex);

      defer_construct_newø(ptexturesiteSwapChain);

      ::pointer < ::gpu_directx12::texture > ptextureSwapChain;

      ptextureSwapChain = ptexturesiteSwapChain->gpu_texture();

      //auto & ptextureSwapChain = m_textureaSwapChain.element_at_grow(m_iSwapChainIndex);

      if (!ptextureSwapChain)
      {

         constructø(ptexturesiteSwapChain->m_pgputextureSite);

         ptextureSwapChain = ptexturesiteSwapChain->gpu_texture();

         ptextureSwapChain->m_textureflags.m_bRenderTarget = true;

         ptextureSwapChain->m_textureflags.m_bShaderResource = false;

         //ptextureSwapChain->m_bDepthStencil = false;

         ptextureSwapChain->_initialize_gpu_texture(
            pgpurenderer->m_pgpucontext,
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

      //   HRESULT hr = pgpudevice->m_pd3d12device->CreateBlendState(&blendDesc, &m_pblendstateDisabled);
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

      //   pgpudevice->m_pd3d12device->CreateRenderTargetView(
      //      m_ptextureSwapChain, nullptr, &m_prendertargetviewSwapChain);

      //}

      //ID3D11RenderTargetView* rendertargetviewa[] = 
      //{
      //   m_ptextureSwapChain->m_prendertargetview
      //};
      //
      //pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, nullptr);

      dummy_model_buffer();

      present_shader();

      // 2. Set viewport

//      if (!m_pshaderPresent)
//      {
//
//         construct_newø(m_pshaderPresent);
//
//         auto pbindingSampler = m_pshaderPresent->binding();
//         pbindingSampler->m_ebinding = ::gpu::e_binding_sampler2d;
//         m_pshaderPresent->m_bDisableDepthTest = true;
//         m_pshaderPresent->m_ecullmode = ::gpu::e_cull_mode_none;
//         const char* fullscreen_vertex_shader = R"shader(// fullscreen_vs.hlsl
//      struct VSOut {
//         float4 pos : SV_POSITION;
//         float2 uv : TEXCOORD0;
//      };
//
//      VSOut main(uint vid : SV_VertexID) {
//         float2 verts[3] = {
//             float2(-1, -1),
//             float2(-1, +3),
//             float2(+3, -1)
//         };
//         float2 uvs[3] = {
//             float2(0, 1),
//             float2(0, -1),
//             float2(2, 1)
//         };
//
//         VSOut o;
//         o.pos = float4(verts[vid], 0, 1);
//         o.uv = 0.5 * (verts[vid] + 1.0);
//         return o;
//      }
//)shader";
//
//         const char* fullscreen_pixel_shader = R"shader(// fullscreen_ps.hlsl
//Texture2D tex : register(t0);
//SamplerState samp : register(s0);
//
//float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_Target {
//    
////if(uv.x >0.5)
////{
//// return float4(0.1*0.5, 0.8*0.5, 0.98*0.5, 0.5); // test if the shader pipeline is running
////}
////else
//{
//return tex.Sample(samp, float2(uv.x, 1.0 - uv.y));
//}
//}
//)shader";
//
//         m_pshaderPresent->initialize_shader_with_block(
//            pgpurenderer,
//            as_block(fullscreen_vertex_shader),
//            as_block(fullscreen_pixel_shader));
//
//      }

      //::cast < command_buffer > pcommandbuffer = pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      ptextureSwapChain->set_state(pcommandbuffer, ::gpu::e_texture_state_color_attachment);

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

      ::cast<::gpu_directx12::texture> ptextureSrc = pgputexturesite->gpu_texture();

      // ptextureSrc->set_state(pcommandbuffer, ::gpu::e_texture_state_color_attachment);

      //{

      //   FLOAT colorRGBA2[] = {0.5f * 0.5f, 0.95f * 0.5f, 0.75f * 0.5f, 0.5f};

      //   D3D12_RECT r[1];

      //   r[0].left = 500;
      //   r[0].top = 100;


      //   r[0].right = r[0].left + 100;
      //   r[0].bottom = 100 + 100;

      //   pcommandlist->ClearRenderTargetView(ptextureSrc->current_layer().m_handleRenderTargetView, colorRGBA2, 1,
      //                                       r);
      //}

      ptextureSrc->set_state(pcommandbuffer, ::gpu::e_texture_state_shader_read);

      _present(ptexturesiteSwapChain, pgputexturesite, pgpucommandbuffer);

      //     //m_pshaderPresent->bind(pcommandbuffer, ptextureSwapChain, ptextureSrc);
      //     m_pshaderPresent->bind(pcommandbuffer, ptexturesiteSwapChain);
      //     m_pshaderPresent->bind_source(pcommandbuffer, pgputexturesite, 0);
      //     //pgpucontext->m_pcontext->VSSetShader(m_pvertexshaderFullscreen, nullptr, 0);
      //     //pgpucontext->m_pcontext->PSSetShader(m_ppixelshaderFullscreen, nullptr, 0);

      //     //pgpucontext->m_pcontext->PSSetShaderResources(
      //     //   0, 1, m_ptextureSwapChain->m_pshaderresourceview.pp());
      //     //pgpucontext->m_pcontext->PSSetSamplers(
      //     //   0, 1, m_ptextureSwapChain->m_psamplerstate.pp());

      ///*     D3D11_VIEWPORT vp = {};
      //     vp.TopLeftX = 0;
      //     vp.TopLeftY = 0;
      //     vp.Width = static_cast<float>(m_size.cx);
      //     vp.Height = static_cast<float>(m_size.cy);
      //     vp.MinDepth = 0.0f;
      //     vp.MaxDepth = 1.0f;
      //     pgpucontext->m_pcontext->RSSetViewports(1, &vp);*/

      //     D3D12_VIEWPORT viewport = {};
      //     viewport.TopLeftX = 0;
      //     viewport.TopLeftY = 0;
      //     viewport.Width = static_cast<float>(m_size.cx);
      //     viewport.Height = static_cast<float>(m_size.cy);
      //     viewport.MinDepth = 0.0f;
      //     viewport.MaxDepth = 1.0f;

      //     D3D12_RECT scissorRect = {};
      //     scissorRect.left = 0;
      //     scissorRect.top = 0;
      //     scissorRect.right = m_size.cx;
      //     scissorRect.bottom = m_size.cy;

      //     //::cast < renderer > prenderer = m_pgpurenderer;


      //     //// 4. Set the viewport and scissor
      //     pcommandlist->RSSetViewports(1, &viewport);
      //     pcommandlist->RSSetScissorRects(1, &scissorRect);
      //     pcommandlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      //     pcommandlist->DrawInstanced(3, 1, 0, 0);


      //     m_pshaderPresent->unbind(pcommandbuffer);


           //FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

           //pcommandlist->ClearRenderTargetView(ptextureSwapChain->m_handleRenderTargetView, 
           //   colorRGBA2, 0, nullptr);

           //{

           //   FLOAT colorRGBA2[] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

           //   D3D12_RECT r[1];

           //   r[0].left = 200;
           //   r[0].top = 100;


           //   r[0].right = 200 + 100;
           //   r[0].bottom = 100 + 100;

           //   pcommandlist->ClearRenderTargetView(ptextureSwapChain->current_layer().m_handleRenderTargetView,
           //      colorRGBA2, 1, r);

           //}

   }


   void swap_chain::set_present_state(::gpu::command_buffer * pgpucommandbuffer)
   {

      ::cast < renderer > pgpurenderer = ::gpu::swap_chain::m_pgpurenderer;

      if (pgpurenderer)
      {

         auto & ptexturesiteSwapChain = m_ptexturesiteaSwapChain->element_at(m_iSwapChainIndex);

         //::cast < command_buffer > pcommandbuffer = pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());

         ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

         auto pcommandlist = pcommandbuffer->m_pcommandlist;

         ptexturesiteSwapChain->gpu_texture()->set_state(pcommandbuffer, ::gpu::e_texture_state_present);

      }

   }


   void swap_chain::swap_buffers()
   {

      auto hrPresent = m_pdxgiswapchain->Present(1, 0);

      ::defer_throw_hresult(hrPresent);

      // Keep the cached index current between Present and the next frame.
      swap_chain_frame_index();

   }


   int swap_chain::swap_chain_frame_index()
   {

      m_iSwapChainIndex = m_pdxgiswapchain->GetCurrentBackBufferIndex();

      if (m_iSwapChainIndex < 0
         || (m_iSwapChainCount > 0 && m_iSwapChainIndex >= m_iSwapChainCount))
      {

         throw ::exception(error_failed);

      }

      m_iCurrentSwapChainFrame = m_iSwapChainIndex;

      m_iCurrentSwapChainImage = m_iSwapChainIndex;

      return m_iSwapChainIndex;

   }


   //int swap_chain::get_frame_index()
   //{
   //   return swap_chain_frame_index();

   //}


   void swap_chain::initialize_swap_chain_window(::gpu::context * pgpucontext, ::acme::windowing::window * pacmewindowingwindow)
   {

      ::gpu::swap_chain::initialize_swap_chain_window(pgpucontext, pacmewindowingwindow);

      ::cast < ::windowing_win32::window > pwin32window = pacmewindowingwindow;

      ::cast < context > pcontext = pgpucontext;

      auto & pdcompositiondevice = m_pdcompositiondevice;
      auto & pdcompositiontarget = m_pdcompositiontarget;
      auto & pdcompositionvisual = m_pdcompositionvisual;

      ::cast < device > pgpudevice = pcontext->m_pgpudevice;

      auto pdxgidevice = pgpudevice->_get_dxgi_device();

      ::defer_throw_hresult(DCompositionCreateDevice(
         pdxgidevice,
         __interface_of(pdcompositiondevice)));


      ::defer_throw_hresult(
         pdcompositiondevice->CreateTargetForHwnd((HWND)pwin32window->_HWND(),
            true,
            &pdcompositiontarget));

      ::defer_throw_hresult(pdcompositiondevice->CreateVisual(&pdcompositionvisual));
      ::defer_throw_hresult(pdcompositionvisual->SetContent(m_pdxgiswapchain));
      ::defer_throw_hresult(pdcompositiontarget->SetRoot(pdcompositionvisual));
      ::defer_throw_hresult(pdcompositiondevice->Commit());

   }


   int swap_chain::swap_chain_frame_count()
   {

      UINT frame_count = 0;

      DXGI_SWAP_CHAIN_DESC desc;

      HRESULT hr = m_pdxgiswapchain->GetDesc(&desc);

      if (SUCCEEDED(hr))
      {

         frame_count = desc.BufferCount;
         
      }
      else
      {

         throw ::exception(error_failed);

      }

      return frame_count;

   }


} // namespace gpu_directx12



