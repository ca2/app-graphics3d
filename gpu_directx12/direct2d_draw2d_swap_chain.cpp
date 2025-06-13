// Created by camilo on 2025-06-04 23:11 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "approach.h"
#include "direct2d_draw2d_swap_chain.h"
#include "offscreen_render_target_view.h"
#include "shader.h"
#include "draw2d_direct2d/graphics.h"
#include "direct2d/direct2d.h"
#include "aura/windowing/window.h"
#include "gpu_directx12/context.h"
#include "gpu_directx12/renderer.h"
#include "gpu_directx12/swap_chain.h"
#include "gpu_directx12/texture.h"
#include "bred/gpu/graphics.h"
#include "bred/gpu/types.h"


namespace gpu_directx12
{



   direct2d_draw2d_swap_chain::direct2d_draw2d_swap_chain()
   {

   }


   direct2d_draw2d_swap_chain::~direct2d_draw2d_swap_chain()
   {


   }


   void direct2d_draw2d_swap_chain::initialize_gpu_swap_chain(::gpu::device* pgpudevice, ::windowing::window* pwindow)
   {

      ::gpu_directx12::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);

      m_pgpudevice = ::gpu::swap_chain::m_pgpudevice;

      ::cast < ::gpu_directx12::device > pdevice = m_pgpudevice;

      auto pd3d12device = pdevice->m_pdevice;

      HRESULT hrD3D11On12 = D3D11On12CreateDevice(
         pd3d12device,
         D3D11_CREATE_DEVICE_BGRA_SUPPORT,
         nullptr,
         0,
         (IUnknown* const*)pdevice->m_pcommandqueue.pp(),
         1,
         0,
         &m_pd3d11device,
         &m_pd3d11context,
         nullptr
      );

      ::defer_throw_hresult(hrD3D11On12);

      ::defer_throw_hresult(m_pd3d11device.as(m_pd3d11on12)); // Query interface

      ::defer_throw_hresult(m_pd3d11device.as(m_pdxgidevice_2));

      ::defer_throw_hresult(m_pdxgiswapchain3.as(m_pdxgiswapchain1));

      DXGI_SWAP_CHAIN_DESC swapchaindesc1{};

      int FrameCount = 2;

      if (SUCCEEDED(m_pdxgiswapchain3->GetDesc(&swapchaindesc1)))
      {

         FrameCount = swapchaindesc1.BufferCount;

      }

      m_frameIndex = m_pdxgiswapchain3->GetCurrentBackBufferIndex();

      // Create synchronization objects and wait until assets have been uploaded to the GPU.
      {
         ::defer_throw_hresult(pd3d12device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __interface_of(m_fence)));
         m_fenceValue = 1;

         // Create an event handle to use for frame synchronization.
         m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
         if (m_fenceEvent == nullptr)
         {
            ::defer_throw_hresult(HRESULT_FROM_WIN32(GetLastError()));
         }

      }

      //_defer_d3d11on12_wrapped_resources();
      // Create descriptor heaps.

      {
         // Describe and create a render target view (RTV) descriptor heap.
         D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
         rtvHeapDesc.NumDescriptors = FrameCount;
         rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
         rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
         ::defer_throw_hresult(pd3d12device->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_rtvHeap)));

         m_rtvDescriptorSize = pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

      }

      // Create frame resources.
      {

         CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

         // Create a RTV for each frame.
         for (UINT n = 0; n < FrameCount; n++)
         {

            auto & prendertarget = m_renderTargets[n];

            ::defer_throw_hresult(
               m_pdxgiswapchain1->GetBuffer(
                  n, __interface_of(prendertarget)));

            pd3d12device->CreateRenderTargetView(prendertarget, nullptr, rtvHandle);

            rtvHandle.Offset(1, m_rtvDescriptorSize);

         }

      }

      ::draw2d_direct2d::swap_chain::initialize_gpu_swap_chain(pgpudevice, pwindow);

   }


   ::gpu_directx12::texture* direct2d_draw2d_swap_chain::current_texture()
   {

      return offscreen_render_target_view::current_texture();
      //return m_ptextureRenderTargetBackBuffer;

   }


   void direct2d_draw2d_swap_chain::on_before_begin_draw_frame(::draw2d_gpu::graphics* pgraphics)
   {

      ::cast < ::draw2d_direct2d::graphics > pdirect2dgraphics = pgraphics;

      if (pdirect2dgraphics)
      {

         auto ptexture = this->current_texture();

         if (ptexture)
         {

            auto pd3d11 = ptexture->m_pd3d11;

            if (pd3d11)
            {

               ID3D11Resource* ppResources[] = { pd3d11->wrappedResource };

               m_pd3d11on12->AcquireWrappedResources(
                  ppResources,
                  1
               );

               ptexture->m_estate = D3D12_RESOURCE_STATE_RENDER_TARGET;

               d2dContext->SetTarget(pd3d11->d2dBitmap);

               pdirect2dgraphics->__attach(d2dContext);

            }

         }

      }

   }


   void direct2d_draw2d_swap_chain::on_after_end_draw_frame(::draw2d_gpu::graphics* pgraphics)
   {

      ::cast < ::draw2d_direct2d::graphics > pdirect2dgraphics = pgraphics;

      if (pdirect2dgraphics)
      {

         auto ptexture = this->current_texture();

         if (ptexture)
         {

            auto pd3d11 = ptexture->m_pd3d11;

            if (pd3d11)
            {

               ID3D11Resource* ppResources[] = { pd3d11->wrappedResource };

               m_pd3d11on12->ReleaseWrappedResources(
                  ppResources,
                  1
               );

               ptexture->m_estate = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

            }

         }

      }

   }


   ::gpu_directx12::depth_stencil* direct2d_draw2d_swap_chain::current_depth_stencil()
   {

      return offscreen_render_target_view::current_depth_stencil();

   }



   //void direct2d_draw2d_swap_chain::_defer_d3d11on12_wrapped_resources()
   //{

   //   //HRESULT hrGetBuffer = m_pdxgiswapchain1->GetBuffer(
   //   //   0, __interface_of(m_pd3d12resourceBackBuffer));

   //   //D3D11_RESOURCE_FLAGS d3d11Flags = { };

   //   //d3d11Flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

   //   //::defer_throw_hresult(m_pd3d11on12->CreateWrappedResource(
   //   //   m_pd3d12resourceBackBuffer,
   //   //   &d3d11Flags,
   //   //   D3D12_RESOURCE_STATE_RENDER_TARGET,
   //   //   D3D12_RESOURCE_STATE_PRESENT,
   //   //   __interface_of(m_pd3d11resourceWrapped)
   //   //));

   //   //::defer_throw_hresult(m_pd3d11resourceWrapped.as(m_pdxgisurface));

   //}


   void direct2d_draw2d_swap_chain::endDraw(::draw2d_gpu::graphics* pgraphics, ::user::interaction* puserinteraction, ::gpu::renderer* prendererSrc)
   {

      ::cast < renderer > pgpurendererSrc = prendererSrc;

      ::cast < ::draw2d_direct2d::graphics > pdirect2dgraphics = pgraphics;

      //::comptr<ID3D12Resource> pd3d12resourceBackBuffer;

    /*  if (pdirect2dgraphics)
      {

         ::cast < texture > ptexture = pdirect2dgraphics->m_pSwapChainForBlitting;

         if (ptexture)
         {

            pd3d12resourceBackBuffer = ptexture->m_presource;

         }

      }*/

      ::cast <offscreen_render_target_view> poffscreenrendertargetview = pgpurendererSrc->m_pgpurendertarget;

      auto ptexture = poffscreenrendertargetview->m_texturea[pgpurendererSrc->get_frame_index()];

      auto pd3d11 = ptexture->m_pd3d11;

      ::comptr<ID3D12Resource> pd3d12resourceBackBuffer = ptexture->m_presource;

      //   ::cast < renderer > pgpurendererSrc = prendererSrc;

      //   //ID3D11Resource* resources[] = { m_pd3d11resourceWrapped.m_p };

      //   //m_pd3d11on12->AcquireWrappedResources(resources, _countof(resources));

      //   try
      //   {

      //      ::draw2d_direct2d::swap_chain::endDraw(pgraphics, puserinteraction, prendererSrc);

      //   }
      //   catch (...)
      //   {


      //   }

      //   //m_pd3d11on12->ReleaseWrappedResources(resources, _countof(resources));

      //}


      //void direct2d_draw2d_swap_chain::present()
      //{

      ::cast < ::gpu_directx12::device> pgpudevice = m_pgpudevice;

      auto pdevice = pgpudevice->m_pdevice;

      if (!m_commandAllocator)
      {

         ::defer_throw_hresult(pdevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT, __interface_of(m_commandAllocator)));

      }

      if (!m_commandList)
      {

         ::defer_throw_hresult(
            pdevice->CreateCommandList(
               0, D3D12_COMMAND_LIST_TYPE_DIRECT,
               m_commandAllocator,
               nullptr,
               __interface_of(m_commandList)));

         // Command lists are created in the recording state, but there is nothing
         // to record yet. The main loop expects it to be closed, so close it now.
         ::defer_throw_hresult(m_commandList->Close());

      }

      if (!m_srvHeap)
      {

         D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
         srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
         srvHeapDesc.NumDescriptors = pgpurendererSrc->get_frame_count(); // Just one texture
         srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
         srvHeapDesc.NodeMask = 0;

         HRESULT hr = pdevice->CreateDescriptorHeap(
            &srvHeapDesc,
            __interface_of(m_srvHeap));

         ::defer_throw_hresult(hr);

         m_srvDescriptorSize = pdevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

         for (int iFrameIndex = 0; iFrameIndex < pgpurendererSrc->get_frame_count(); iFrameIndex++)
         {

            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Format = pd3d12resourceBackBuffer->GetDesc().Format;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = pd3d12resourceBackBuffer->GetDesc().MipLevels;

            CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
               m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
               iFrameIndex,
               m_srvDescriptorSize);

            auto presource = poffscreenrendertargetview->m_texturea[iFrameIndex]->m_presource;

            pdevice->CreateShaderResourceView(
               presource,
               &srvDesc,
               srvHandle
            );

         }

      }

      if (!m_samplerHeap)
      {

         D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
         samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
         samplerHeapDesc.NumDescriptors = 1; // Just one sampler
         samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
         samplerHeapDesc.NodeMask = 0;

         HRESULT hr = pdevice->CreateDescriptorHeap(
            &samplerHeapDesc, __interface_of(m_samplerHeap));

         ::defer_throw_hresult(hr);

         D3D12_SAMPLER_DESC samplerDesc = {};
         samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
         samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
         samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
         samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
         samplerDesc.MinLOD = 0.0f;
         samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
         samplerDesc.MipLODBias = 0.0f;
         samplerDesc.MaxAnisotropy = 1;
         samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

         pdevice->CreateSampler(
            &samplerDesc,
            m_samplerHeap->GetCPUDescriptorHandleForHeapStart()
         );

      }

      if (!m_rootSignature)
      {

         CD3DX12_DESCRIPTOR_RANGE srvRange;
         srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0

         CD3DX12_DESCRIPTOR_RANGE samplerRange;
         samplerRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0); // s0

         CD3DX12_ROOT_PARAMETER rootParams[2];
         rootParams[0].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL);
         rootParams[1].InitAsDescriptorTable(1, &samplerRange, D3D12_SHADER_VISIBILITY_PIXEL);

         CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
         rootSigDesc.Init(_countof(rootParams), rootParams, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

         ::comptr<ID3DBlob> sigBlob, errBlob;
         
         HRESULT hrSerializeRootSignature = D3D12SerializeRootSignature(
            &rootSigDesc,
            D3D_ROOT_SIGNATURE_VERSION_1, &sigBlob, &errBlob);

         ::defer_throw_hresult(hrSerializeRootSignature);

         HRESULT hrCreateRootSignature = pdevice->CreateRootSignature(
            0,
            sigBlob->GetBufferPointer(),
            sigBlob->GetBufferSize(),
            __interface_of(m_rootSignature));

         ::defer_throw_hresult(hrCreateRootSignature);

      }

      if (!m_pipelineState)
      {
         
         ::comptr<ID3DBlob> vertexShader;
         
         ::comptr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
         
         // Enable better shader debugging with the graphics debugging tools.
         UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else

         UINT compileFlags = 0;

#endif
         
         ::string strVs(_fullscreen_vertex_shader_hlsl());

         ::defer_throw_hresult(D3DCompile(
            strVs.c_str(), strVs.size(),
            nullptr, nullptr, nullptr, "main", "vs_5_0",
            compileFlags, 0, &vertexShader, nullptr));
         
         ::string strPs(_fullscreen_pixel_shader_hlsl());

         ::defer_throw_hresult(D3DCompile(
            strPs.c_str(), strPs.size(),
            nullptr, nullptr, nullptr, "main", "ps_5_0",
            compileFlags, 0, &pixelShader, nullptr));

         D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

         psoDesc.pRootSignature = m_rootSignature;

         // Shaders
         psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
         psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };

         // No vertex buffer used — SV_VertexID only
         psoDesc.InputLayout = { nullptr, 0 };

         // Rasterizer state
         psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
         psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // Often good for fullscreen quads

         // Blend state
         psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

         // No depth or stencil
         psoDesc.DepthStencilState.DepthEnable = FALSE;
         psoDesc.DepthStencilState.StencilEnable = FALSE;

         // Primitive type
         psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

         // Render target
         psoDesc.NumRenderTargets = 1;
         psoDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
         psoDesc.SampleDesc.Count = 1;

         psoDesc.SampleMask = UINT_MAX;

         HRESULT hr = pdevice->CreateGraphicsPipelineState(
            &psoDesc, __interface_of(m_pipelineState));
         
         ::defer_throw_hresult(hr);

      }

      // Command list allocators can only be reset when the associated 
      // command lists have finished execution on the GPU; apps should use 
      // fences to determine GPU execution progress.
      ::defer_throw_hresult(m_commandAllocator->Reset());

      // However, when ExecuteCommandList() is called on a particular command 
      // list, that command list can then be reset at any time and must be before 
      // re-recording.
      ::defer_throw_hresult(
         m_commandList->Reset(
            m_commandAllocator,
            m_pipelineState));

      m_commandList->SetPipelineState(m_pipelineState);
      m_commandList->SetGraphicsRootSignature(m_rootSignature);

      ID3D12DescriptorHeap* heaps[] = { m_srvHeap, m_samplerHeap };
      m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

      CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(
         m_srvHeap->GetGPUDescriptorHandleForHeapStart(), 
         m_pgpurenderer->get_frame_index(),
         m_srvDescriptorSize);
      m_commandList->SetGraphicsRootDescriptorTable(0, srvHandle);     // t0
      m_commandList->SetGraphicsRootDescriptorTable(1, m_samplerHeap->GetGPUDescriptorHandleForHeapStart()); // s0

      D3D12_VIEWPORT viewport = {};
      viewport.TopLeftX = 0.0f;
      viewport.TopLeftY = 0.0f;
      viewport.Width = static_cast<float>(m_pgpurenderer->m_pgpucontext->m_rectangle.width());
      viewport.Height = static_cast<float>(m_pgpurenderer->m_pgpucontext->m_rectangle.height());
      viewport.MinDepth = 0.0f;
      viewport.MaxDepth = 1.0f;

      D3D12_RECT scissorRect = {};
      scissorRect.left = 0;
      scissorRect.top = 0;
      scissorRect.right = m_pgpurenderer->m_pgpucontext->m_rectangle.width();
      scissorRect.bottom = m_pgpurenderer->m_pgpucontext->m_rectangle.height();

      m_commandList->RSSetViewports(1, &viewport);
      m_commandList->RSSetScissorRects(1, &scissorRect);

      auto& prendertarget = m_renderTargets[m_frameIndex];

      auto barrierToRender = CD3DX12_RESOURCE_BARRIER::Transition(prendertarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
      m_commandList->ResourceBarrier(1, &barrierToRender);

      CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
      m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

      const float clearColor[] = { 0.5f * 0.5f, 0.5f * 0.75f, 0.90f * 0.5f, 0.5f };
      m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

      m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      m_commandList->DrawInstanced(3, 1, 0, 0); 

      auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(prendertarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
      m_commandList->ResourceBarrier(1, &barrierToPresent);

      ::defer_throw_hresult(m_commandList->Close());

      ID3D12CommandList* ppCommandLists[] = { m_commandList };
      pgpudevice->m_pcommandqueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

      m_pdxgiswapchain1->Present(1, 0);

      // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
      // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
      // sample illustrates how to use fences for efficient resource usage and to
      // maximize GPU utilization.

      // Signal and increment the fence value.
      const UINT64 fence = m_fenceValue;
      
      ::defer_throw_hresult(pgpudevice->m_pcommandqueue->Signal(m_fence, fence));

      m_fenceValue++;

      // Wait until the previous frame is finished.
      if (m_fence->GetCompletedValue() < fence)
      {
         
         ::defer_throw_hresult(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
         
         ::WaitForSingleObject(m_fenceEvent, INFINITE);

      }

      m_frameIndex = m_pdxgiswapchain3->GetCurrentBackBufferIndex();

   }


   void direct2d_draw2d_swap_chain::present()
   {


   }


   bool context::create_offscreen_graphics_for_swap_chain_blitting(::draw2d_gpu::graphics* pgraphics, const ::int_size& size)
   {

      ::cast < ::draw2d_direct2d::graphics > pdirect2dgraphics = pgraphics;

      if (pdirect2dgraphics)
      {

         ::cast < direct2d_draw2d_swap_chain > pswapchain = m_pgpudevice->m_pswapchain;

         pswapchain->_create_offscreen_graphics_for_swap_chain_blitting(
            this,
            pdirect2dgraphics,
            size);

         return true;

      }

      return false;

   }


   void direct2d_draw2d_swap_chain::_create_offscreen_graphics_for_swap_chain_blitting(::gpu_directx12::context* pcontext, ::draw2d_direct2d::graphics* pgraphics, const ::int_size& size)
   {

      pcontext->set_placement(size);

      pcontext->m_eoutput = pcontext->eoutput_for_begin_draw();

      ::cast < renderer > prenderer = pcontext->get_renderer(::gpu::e_scene_2d);

      prenderer->m_bDisableDepthStencil = true;

      initialize_render_target_view(prenderer, size, nullptr);

      init();

      ::cast < device > pgpudevice = m_pgpudevice;

      auto pdevice = pgpudevice->m_pdevice;

      _get_d2d1_device()->CreateDeviceContext(
         D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
         &d2dContext);

      for (int i = 0; i < m_texturea.size(); i++)
      {

         //m_ptextureRenderTargetBackBuffer = __create_new < swap_chain_texture>();
         auto ptexture = m_texturea[i];

         __defer_construct_new(ptexture->m_pd3d11);

         //pgraphics->m_pSwapChainForBlitting = m_ptextureRenderTargetBackBuffer;

         auto pd3d11 = ptexture->m_pd3d11.m_p;

         //CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

         //D3D12_RESOURCE_DESC texDesc = {};
         //texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
         //texDesc.Width = size.cx();
         //texDesc.Height = size.cy();
         //texDesc.DepthOrArraySize = 1;
         //texDesc.MipLevels = 1;
         //texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // D2D-compatible
         //texDesc.SampleDesc.Count = 1;
         //texDesc.SampleDesc.Quality = 0;
         //texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
         //texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

         //FLOAT clearValues[4] = { 0.5f * 0.5f, 0.75f * 0.5f, 0.90f * 0.5f, 0.5f };
         //CD3DX12_CLEAR_VALUE clearValue(
         //   DXGI_FORMAT_B8G8R8A8_UNORM, clearValues);

         //pdevice->CreateCommittedResource(
         //   &heapProps,
         //   D3D12_HEAP_FLAG_NONE,
         //   &texDesc,
         //   D3D12_RESOURCE_STATE_RENDER_TARGET, // or COPY_DEST if uploading
         //   &clearValue,
         //   __interface_of(ptexture->m_presource));


         //ComPtr<ID3D11Device> d3d11Device;
         //ComPtr<ID3D11DeviceContext> d3d11Context;
         //ComPtr<ID3D11On12Device> d3d11on12;

         //D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
         //D3D11On12CreateDevice(
         //   device12,
         //   0,
         //   levels, _countof(levels),
         //   nullptr, 0, 0,
         //   &d3d11Device,
         //   &d3d11Context,
         //   nullptr);

         D3D11_RESOURCE_FLAGS flags = {};
         //flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
         flags.BindFlags = D3D11_BIND_RENDER_TARGET;

         HRESULT hrCreateWrappedResource = m_pd3d11on12->CreateWrappedResource(
            ptexture->m_presource,
            &flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
            __interface_of(pd3d11->wrappedResource));

         ::defer_throw_hresult(hrCreateWrappedResource);

         //ComPtr<ID2D1Factory3> d2dFactory;
         //D2D1_FACTORY_OPTIONS factoryOptions = {};
         //D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, &d2dFactory);

         //ComPtr<IDXGIDevice> dxgiDevice;
         //d3d11Device.As(&dxgiDevice);

         //ComPtr<ID2D1Device2> d2dDevice;
         //d2dFactory->CreateDevice(dxgiDevice.Get(), &d2dDevice);




         pd3d11->wrappedResource.as(pd3d11->dxgiSurface);

         D2D1_BITMAP_PROPERTIES1 bmpProps = {};
         bmpProps.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
         bmpProps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
         bmpProps.dpiX = 96;
         bmpProps.dpiY = 96;
         bmpProps.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

         //::comptr<ID2D1Bitmap1> d2dBitmap;
         d2dContext->CreateBitmapFromDxgiSurface(
            pd3d11->dxgiSurface, &bmpProps, &pd3d11->d2dBitmap);
         //d2dContext->SetTarget(pd3d11->d2dBitmap);

         


         //   if (m_iType != 0)
         //   {

         //      destroy();

         //   }

   //});

      }



      //::user::interaction* puserinteraction = m_puserinteraction;

      //if (::is_null(puserinteraction))
      //{

      //   puserinteraction = dynamic_cast <::user::interaction*>(m_papplication->m_pacmeuserinteractionMain.m_p);

      //}

      //auto pwindow = puserinteraction->window();

      //auto rectanglePlacement = pwindow->get_window_rectangle();

      //auto pdevice = m_papplication->get_gpu()->get_device();

      //if (__defer_construct(m_pgpucontext))
      //{

      //   m_pgpucontext->start_gpu_context(
      //      ::gpu::start_gpu_output_context_t
      //      {
      //         this,
      //         pdevice,
      //         ::gpu::e_output_gpu_buffer,
      //         rectanglePlacement
      //      });

      //}

      //m_pgpucontext->_send([this, size]()
      //   {

      //      /*::direct2d::direct2d() = __allocate ::draw2d_direct2d::plugin();

      //      ::direct2d::get()->initialize();*/

      //      HRESULT hr;

      //      ::user::interaction* puserinteraction = m_puserinteraction;

      //      if (!puserinteraction)
      //      {

      //         puserinteraction = dynamic_cast <::user::interaction*>(m_papplication->m_pacmeuserinteractionMain.m_p);

      //      }

      //      auto pwindow = puserinteraction->window();

      //      auto rectanglePlacement = pwindow->get_window_rectangle();

      //      ::direct2d_lock lock;

      //      auto pdirect2d = ::direct2d::get();

      //      auto pdevicecontextDefault = pdirect2d->default_d2d1_device_context(pwindow, rectanglePlacement);

      //      //comptr < ID2D1DeviceContext > pdevicecontextTemplate;

      //      //hr = m_pdevice->CreateDeviceContext(
      //      //   //D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
      //      //   D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
      //      //   &pdevicecontextTemplate);

      //      //if (FAILED(hr))
      //      //{

      //      //   warning() << "graphics::CreateCompatibleDC, CreateDeviceContext (1) " << hresult_text(hr);

      //      //   //return false;

      //      //   throw ::exception(error_failed);

      //      //}

      //      auto psession = session();

      //      auto paurasession = psession;

      //      auto puser = paurasession->user();

      //      auto pwindowing = system()->windowing();

      //      auto pdisplay = pwindowing->display();

      //      auto dpi = pdisplay->get_dpi();

      //      if (dpi <= 0.0)
      //      {

      //         ASSERT(false);

      //         throw ::exception(error_failed);

      //      }

      //      pdevicecontextDefault->SetDpi(dpi, dpi);

      //      //comptr < ID2D1RenderTarget > prendertargetTemplate;

      //      //hr = pdevicecontextDefault->QueryInterface(IID_ID2D1RenderTarget, (void**)&prendertargetTemplate);

      //      //if (FAILED(hr))
      //      //{

      //      //   warning() << "graphics::CreateCompatibleDC, QueryInterface (2) " << hresult_text(hr);

      //      //   throw ::exception(error_failed);

      //      //}

      //      D2D1_SIZE_U sizeu = D2D1::SizeU(size.cx(), size.cy());

      //      if (sizeu.width <= 0)
      //      {

      //         sizeu.width = 800;

      //      }

      //      if (sizeu.width <= 0)
      //      {

      //         sizeu.width = 600;

      //      }

      //      D2D1_PIXEL_FORMAT pixelformat;

      //      pixelformat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

      //      pixelformat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //      if (m_pbitmaprendertarget)
      //      {

      //         ID2D1Bitmap* pbitmap;



      //         hr = m_pbitmaprendertarget->GetBitmap(&pbitmap);
      //         if (SUCCEEDED(hr) &&

      //            pbitmap)
      //         {


      //            auto s = pbitmap->GetSize();

      //            if (s.width == size.width()
      //               && s.height == size.height())
      //            {

      //               return;

      //            }

      //         }

      //      }

      //      hr = pdevicecontextDefault->CreateCompatibleRenderTarget(
      //         nullptr,
      //         &sizeu,
      //         &pixelformat,
      //         D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
      //         &m_pbitmaprendertarget);

      //      if (FAILED(hr))
      //      {

      //         warning() << "graphics::CreateCompatibleDC, CreateCompatibleRenderTarget (3) " << hresult_text(hr);

      //         throw ::exception(error_failed);

      //      }

      //      hr = m_pbitmaprendertarget.as(m_pd2d1rendertarget);

      //      if (FAILED(hr))
      //      {

      //         m_pbitmaprendertarget = nullptr;

      //         throw ::exception(error_failed);

      //      }

      //      hr = m_pbitmaprendertarget.as(m_pdevicecontext);

      //      m_pdevicecontext.as(m_pdevicecontext1);

      //      if (FAILED(hr))
      //      {

      //         m_pd2d1rendertarget = nullptr;

      //         m_pbitmaprendertarget = nullptr;

      //         throw ::exception(error_failed);

      //      }

      //      __defer_construct(m_pbitmap);

      //      ID2D1Bitmap* pbitmap;

      //      hr = m_pbitmaprendertarget->GetBitmap(&pbitmap);

      //      if (FAILED(hr))
      //      {

      //         m_pbitmaprendertarget = nullptr;

      //         throw ::exception(error_failed);

      //      }

      //      m_pbitmap->attach(pbitmap);

      //      m_iType = 3;

      //      m_osdata[data_device_context] = m_pdevicecontext;

      //      m_osdata[data_render_target] = m_pd2d1rendertarget;

      //   });
   }

} // namespace gpu_directx12



