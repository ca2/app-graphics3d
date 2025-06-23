// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "texture.h"
#include "renderer.h"


namespace gpu_directx12
{


   texture::texture()
   {

      m_bRenderTarget = false;
      m_bShaderResource = false;
      //m_rtvDescriptorSize = 0;

      new_texture.set_new_texture();

   }


   texture::~texture()
   {

   }


   void texture::initialize_gpu_texture(::gpu::renderer* prenderer, const ::int_rectangle & rectangleTarget) //, bool bCreateRenderTargetView, bool bCreateShaderResourceView)
   {

      auto size = m_rectangleTarget.size();

      if (rectangleTarget != m_rectangleTarget)
      {

         ::gpu::texture::initialize_gpu_texture(prenderer, rectangleTarget);

      }

      if (rectangleTarget.size() != size)
      {

         DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
         // 1. Create the texture resource
         D3D12_RESOURCE_DESC textureDesc = {};
         textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
         textureDesc.Width = rectangleTarget.width();
         textureDesc.Height = rectangleTarget.height();
         textureDesc.MipLevels = 1;
         textureDesc.DepthOrArraySize = 1;
         textureDesc.Format = format;
         textureDesc.SampleDesc.Count = 1;
         textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
         textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

         D3D12_CLEAR_VALUE clearValue = {};
         clearValue.Format = format;
         //clearValue.Color[0] = 0.5f * 0.5f;
         //clearValue.Color[1] = 0.75f * 0.5f;
         //clearValue.Color[2] = 0.9f * 0.5f;
         //clearValue.Color[3] = 0.5f;
         clearValue.Color[0] = 0.f * 0.5f;
         clearValue.Color[1] = 0.f * 0.5f;
         clearValue.Color[2] = 0. * 0.5f;
         clearValue.Color[3] = 0.f;

         //clearValue.Color = { 0.5f, 0.75f, 0.9f, 0.5f };

         ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

         CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

         D3D12_HEAP_FLAGS eheap;

         if (m_pgpurenderer->m_pgpucontext->m_bD3D11On12Shared)
         {

            eheap = D3D12_HEAP_FLAG_NONE;

            textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            //textureDesc.Width = width;
            //textureDesc.Height = height;
            textureDesc.DepthOrArraySize = 1;
            textureDesc.MipLevels = 1;
            textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // MUST be D3D11-compatible format
            textureDesc.SampleDesc.Count = 1;
            textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

            m_bRenderTarget = false;

            m_bShaderResource = false;

         }
         else
         {

            eheap = D3D12_HEAP_FLAG_NONE;

         }

         HRESULT hrCreateCommittedResource = pdevice->m_pdevice->CreateCommittedResource(
            &heapproperties,
            eheap,
            &textureDesc,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            &clearValue,
            __interface_of(m_presource));

         pdevice->defer_throw_hresult(hrCreateCommittedResource);

         m_estate = D3D12_RESOURCE_STATE_RENDER_TARGET;

         new_texture.set_new_texture();

         if (m_bRenderTarget || m_pheapRenderTargetView)
         {

            create_render_target();

         }

         if (m_bShaderResource || m_pheapRenderTargetView)
         {

            create_shader_resource();

         }

      }

   }


   void texture::_new_state(ID3D12GraphicsCommandList* pcommandlist, D3D12_RESOURCE_STATES estateNew)
   {

      if (m_estate != estateNew)
      {

         // Transition to copy source
         D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            m_presource,
            m_estate, // Adjust if needed
            estateNew
         );

         pcommandlist->ResourceBarrier(1, &barrier);

         m_estate = estateNew;

         //pcommandBuffer->submit_command_buffer();

      }

   }


   void texture::create_render_target()
   {

      if (m_pgpurenderer->m_pgpucontext->m_bD3D11On12Shared)
      {

         return;

      }

      ::cast < device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;
         // 2. Create RTV descriptor heap
         D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
         rtvHeapDesc.NumDescriptors = 1;
         rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
         rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
         HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_pheapRenderTargetView));

         pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

         //// 3. Create RTV
         m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
         //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart());

         pdevice->m_pdevice->CreateRenderTargetView(m_presource, nullptr, m_handleRenderTargetView);



   }


   void texture::create_shader_resource()
   {

      //if (m_pgpurenderer->m_pgpucontext->m_bD3D11On12Shared)
      //{

      //   return;

      //}

      ::cast < device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      // 4. Create SRV descriptor heap
      D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
      srvHeapDesc.NumDescriptors = 1;
      srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
   
      HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(
         &srvHeapDesc, __interface_of(m_pheapShaderResourceView));

      pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      // 5. Create SRV
      DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
      D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Format = format;
      srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      srvDesc.Texture2D.MostDetailedMip = 0;
      srvDesc.Texture2D.MipLevels = 1;
      srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

      m_handleShaderResourceView = m_pheapShaderResourceView->GetCPUDescriptorHandleForHeapStart();

      pdevice->m_pdevice->CreateShaderResourceView(m_presource, &srvDesc, m_handleShaderResourceView);

      // Descriptor heap for Sampler
      D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
      samplerHeapDesc.NumDescriptors = 1;
      samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
      samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      pdevice->m_pdevice->CreateDescriptorHeap(&samplerHeapDesc, __interface_of(m_pheapSampler));

      D3D12_SAMPLER_DESC samplerDesc = {};
      samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
      samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
      samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
      samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
      samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
      samplerDesc.MinLOD = 0;
      samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

      m_handleSampler = m_pheapSampler->GetCPUDescriptorHandleForHeapStart();

      pdevice->m_pdevice->CreateSampler(&samplerDesc, m_handleSampler);

   }


   void texture::create_depth_resources()
   {

      if (m_pgpurenderer->m_pgpucontext->m_bD3D11On12Shared)
      {

         return;

      }
      ::cast < device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;


      // 2. Describe depth stencil resource
      D3D12_RESOURCE_DESC depthDesc = {};
      depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      depthDesc.Width = m_rectangleTarget.width();
      depthDesc.Height = m_rectangleTarget.height();
      depthDesc.MipLevels = 1;
      depthDesc.DepthOrArraySize = 1;
      depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
      depthDesc.SampleDesc.Count = 1;
      depthDesc.SampleDesc.Quality = 0;
      depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
      depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

      D3D12_CLEAR_VALUE depthClearValue = {};
      depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
      depthClearValue.DepthStencil.Depth = 1.0f;
      depthClearValue.DepthStencil.Stencil = 0;

      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

      // 3. Create depth stencil resource
      pdevice->m_pdevice->CreateCommittedResource(
         &heapproperties,
         D3D12_HEAP_FLAG_NONE,
         &depthDesc,
         D3D12_RESOURCE_STATE_DEPTH_WRITE,
         &depthClearValue,
         __interface_of(m_presourceDepthStencilView));

      // Describe and create a depth stencil view (DSV) descriptor heap.
      D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
      dsvHeapDesc.NumDescriptors = 1;
      dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
      dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      HRESULT hrCreateDescriptorHeapDsv = pdevice->m_pdevice->CreateDescriptorHeap(
         &dsvHeapDesc, __interface_of(m_pheapDepthStencilView));
      pdevice->defer_throw_hresult(hrCreateDescriptorHeapDsv);

      // 4. Create DSV
      D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
      dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
      dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
      dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

      //m_handle = m_pheap->GetCPUDescriptorHandleForHeapStart();
      //::cast < device>pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      pdevice->m_pdevice->CreateDepthStencilView(
         m_presourceDepthStencilView, &dsvDesc, 
         m_pheapDepthStencilView->GetCPUDescriptorHandleForHeapStart());




   }


   class texture::d3d11* texture::d3d11()
   {

      if (!m_pd3d11)
      {

         __construct_new(m_pd3d11);

      }

      return m_pd3d11;

   }


   void texture::blend(::gpu::texture* ptexture)
   {

      

   }


   void texture::_initialize_gpu_texture(::gpu::renderer* prenderer, UINT uCurrentBufferIndex, IDXGISwapChain3* pdxgiswapchain)
   {

      m_pgpurenderer = prenderer;

      if (!m_presource)
      {

         auto hrSwapChainGetBuffer = pdxgiswapchain->GetBuffer(
            uCurrentBufferIndex, __interface_of(m_presource));

         ::defer_throw_hresult(hrSwapChainGetBuffer);

      }

//      ::cast < ::gpu_directx12::device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;
//
//
//      // Create descriptor heaps.
//      {
//         // Describe and create a render target view (RTV) descriptor heap.
//         D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
//         rtvHeapDesc.NumDescriptors = 1;
//         rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//         rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
//         auto hrCreateDescriptorHeap = pgpudevice->m_pdevice->CreateDescriptorHeap(
//            &rtvHeapDesc, __interface_of(m_pheapRenderTargetView));
//
////         m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//      }
//
//      // Create frame resources.
//      {
//         
//         //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
//         m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
//         // Create a RTV for each frame.
//         //for (UINT n = 0; n < FrameCount; n++)
//         {
//         pgpudevice->m_pdevice->CreateRenderTargetView(
//            m_presource, nullptr, m_handleRenderTargetView);
//            //rtvHandle.Offset(1, m_rtvDescriptorSize);
//         }
//      }
//
      //auto pdevice = pgpudevice->m_pdevice;

      //HRESULT hrCreateTexture = pdxgiswapchain->GetBuffer(0, __interface_of(m_ptextureOffscreen));

      //if (FAILED(hrCreateTexture))
      //{

      //   throw ::hresult_exception(hrCreateTexture, "Failed to create offscreen texture");

      //}

      //if (m_bRenderTarget)
      //{

      //   create_render_target_view();

      //}

      //if (m_bShaderResourceView)
      //{

      //   create_shader_resource_view();

      //}

      ////HRESULT hrCreateShaderResourceView = pdevice->CreateShaderResourceView(m_ptextureOffscreen, nullptr, &m_pshaderresourceview);

      ////if (FAILED(hrCreateShaderResourceView))
      ////{

      ////   throw ::hresult_exception(hrCreateShaderResourceView, "Failed to create offscreen shader resource view");

      ////}

      ////if (bCreateRenderTargetView)
      ////{

      ////   //// 2. Create RTV descriptor heap
      ////   //D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
      ////   //rtvHeapDesc.NumDescriptors = 1;
      ////   //rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
      ////   //rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      ////   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_pheapRenderTargetView));

      ////   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      ////   //// 3. Create RTV
      ////   //m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
      ////   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

      ////   pdevice->m_pdevice->CreateRenderTargetView(m_presource, nullptr, m_handleRenderTargetView);

      ////}

      ////if (bCreateShaderResourceView)
      ////{

      ////   //// 4. Create SRV descriptor heap
      ////   //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
      ////   //srvHeapDesc.NumDescriptors = 1;
      ////   //srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      ////   //srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
      ////   //
      ////   //HRESULT hrCreateDescriptorHeap = pdevice->m_pdevice->CreateDescriptorHeap(&srvHeapDesc, __interface_of(m_pheapShaderResourceView));

      ////   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      ////   //// 5. Create SRV
      ////   //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      ////   //srvDesc.Format = format;
      ////   //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      ////   //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      ////   //srvDesc.Texture2D.MostDetailedMip = 0;
      ////   //srvDesc.Texture2D.MipLevels = 1;

      ////   //m_handleShaderResourceView = m_pheapShaderResourceView->GetCPUDescriptorHandleForHeapStart();

      ////   //pdevice->m_pdevice->CreateShaderResourceView(m_presource, &srvDesc, m_handleShaderResourceView);

      ////}

   }



} // namespace gpu_directx12




