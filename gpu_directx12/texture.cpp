// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "texture.h"
#include "renderer.h"
#include "acme/graphics/image/pixmap.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/frame.h"


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


   void texture::create_image(const ::pointer_array < ::image::image > *pimagea)
   {

      DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
      // 1. Create the texture resource
      D3D12_RESOURCE_DESC textureDesc = {};
      textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      textureDesc.Width = m_rectangleTarget.width();
      textureDesc.Height = m_rectangleTarget.height();
      if (m_etype == e_type_cube_map)
      {
         if (textureDesc.Width != textureDesc.Height)
         {

            throw ::exception(error_wrong_state);

         }
         textureDesc.DepthOrArraySize = 6;
      }
      else
      {
         textureDesc.DepthOrArraySize = 1;

      }

      textureDesc.MipLevels = 1;
      //textureDesc.DepthOrArraySize = 1;
      textureDesc.Format = format;
      textureDesc.SampleDesc.Count = 1;
      textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

      if (m_bRenderTarget)
      {

         textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

      }

      D3D12_CLEAR_VALUE* pclearvalue = nullptr;

      D3D12_CLEAR_VALUE clearValue = {};

      if (m_bRenderTarget)
      {

         pclearvalue = &clearValue;

         clearValue.Format = format;
         //clearValue.Color[0] = 0.5f * 0.5f;
         //clearValue.Color[1] = 0.75f * 0.5f;
         //clearValue.Color[2] = 0.9f * 0.5f;
         //clearValue.Color[3] = 0.5f;
         clearValue.Color[0] = 0.f * 0.5f;
         clearValue.Color[1] = 0.f * 0.5f;
         clearValue.Color[2] = 0. * 0.5f;
         clearValue.Color[3] = 0.f;

      }

      //clearValue.Color = { 0.5f, 0.75f, 0.9f, 0.5f };

      ::cast < ::gpu_directx12::device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

      D3D12_HEAP_FLAGS eheap;

      if (m_pgpurenderer->m_pgpucontext->m_bD3D11On12Shared)
      {

         eheap = D3D12_HEAP_FLAG_NONE;

         textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
         //textureDesc.Width = width;
         //textureDesc.Height = height;
            //textureDesc.DepthOrArraySize = 1;
         textureDesc.MipLevels = 1;
         textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // MUST be D3D11-compatible format
         textureDesc.SampleDesc.Count = 1;
         textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

         m_bRenderTarget = false;

         m_bShaderResource = false;

      }
      else
      {

         eheap = D3D12_HEAP_FLAG_NONE;

      }

      D3D12_RESOURCE_STATES stateInitial;

      if (m_bRenderTarget)
      {

         stateInitial = D3D12_RESOURCE_STATE_RENDER_TARGET;

      }
      else
      {

         stateInitial = D3D12_RESOURCE_STATE_COPY_DEST;

      }

      HRESULT hrCreateCommittedResource = pdevice->m_pdevice->CreateCommittedResource(
         &heapproperties,
         eheap,
         &textureDesc,
         stateInitial,
         pclearvalue,
         __interface_of(m_presource));

      pdevice->defer_throw_hresult(hrCreateCommittedResource);

      m_estate = stateInitial;

      if (::is_set(pimagea) && pimagea->has_element())
      {

         int iCount;


         if (m_etype == e_type_cube_map)
         {
            iCount = 6;
            if (pimagea->first()->width() != m_rectangleTarget.width()
               || pimagea->first()->height() != m_rectangleTarget.height())
            {

               throw ::exception(error_failed);

            }

         }
         else
         {
            iCount = 1;
            if (pimagea->first()->size() != m_rectangleTarget.size())
            {

               throw ::exception(error_failed);

            }

         }

         ::comptr<ID3D12Resource> presourceUpload;

         const UINT64 presourceUploadBufferSize = GetRequiredIntermediateSize(m_presource, 0, iCount);

         CD3DX12_HEAP_PROPERTIES propertiesUpload(D3D12_HEAP_TYPE_UPLOAD);

         auto descUpload = CD3DX12_RESOURCE_DESC::Buffer(presourceUploadBufferSize);

         pdevice->m_pdevice->CreateCommittedResource(
            &propertiesUpload,
            D3D12_HEAP_FLAG_NONE,
            &descUpload,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            __interface_of(presourceUpload));


         // 3. Prepare subresources
         D3D12_SUBRESOURCE_DATA subresources[6];
         if (m_etype == e_type_cube_map)
         {
            for (int i = 0; i < 6; ++i) {
               auto ppixmap = (*pimagea)[i];
               subresources[i].pData = ppixmap->data(); // Your CPU data pointer
               subresources[i].RowPitch = ppixmap->m_iScan;  // 512 * 4
               subresources[i].SlicePitch = textureDesc.Width * textureDesc.Height * 4;
               //subresources[i].SlicePitch = 0;
            }
         }
         else
         {
            auto ppixmap = pimagea->first();
            subresources[0].pData = ppixmap->data();            // pointer to your bitmap data (RGBA8, etc.)
            subresources[0].RowPitch = ppixmap->m_iScan;
            subresources[0].SlicePitch = subresources[0].RowPitch * ppixmap->height();
         }

         ::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getLoadAssetsCommandBuffer();

         if (!pcommandbuffer)
         {

            pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());

         }

         UpdateSubresources(
            pcommandbuffer->m_pcommandlist,
            m_presource,
            presourceUpload, 0, 0, iCount,
            subresources);

         comptr < IUnknown > punknownResourceUpdate(presourceUpload);

         pcommandbuffer->m_comptraHold.add(punknownResourceUpdate);

      }

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


   // void texture::initialize_image_texture(::gpu::renderer* prenderer, const ::int_rectangle& rectangleTarget, bool bWithDepth, const ::pointer_array < ::image::image >& imagea, enum_type etype)
   // {
   //
   //    auto size = m_rectangleTarget.size();
   //
   //    if (rectangleTarget != m_rectangleTarget)
   //    {
   //
   //       ::gpu::texture::initialize_image_texture(prenderer, rectangleTarget, bWithDepth, imagea, etype);
   //
   //    }
   //
   //    if (rectangleTarget.size() != size)
   //    {
   //
   //       DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
   //       // 1. Create the texture resource
   //       D3D12_RESOURCE_DESC textureDesc = {};
   //       textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
   //       textureDesc.Width = rectangleTarget.width();
   //       textureDesc.Height = rectangleTarget.height();
   //       if (m_etype == e_type_cube_map)
   //       {
   //          if (textureDesc.Width != textureDesc.Height)
   //          {
   //
   //             throw ::exception(error_wrong_state);
   //
   //          }
   //          textureDesc.DepthOrArraySize = 6;
   //       }
   //       else
   //       {
   //          textureDesc.DepthOrArraySize = 1;
   //
   //       }
   //
   //       textureDesc.MipLevels = 1;
   //       //textureDesc.DepthOrArraySize = 1;
   //       textureDesc.Format = format;
   //       textureDesc.SampleDesc.Count = 1;
   //       textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
   //
   //       if (m_bRenderTarget)
   //       {
   //
   //          textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
   //
   //       }
   //
   //       D3D12_CLEAR_VALUE* pclearvalue = nullptr;
   //
   //       D3D12_CLEAR_VALUE clearValue = {};
   //
   //       if (m_bRenderTarget)
   //       {
   //
   //          pclearvalue = &clearValue;
   //
   //          clearValue.Format = format;
   //          //clearValue.Color[0] = 0.5f * 0.5f;
   //          //clearValue.Color[1] = 0.75f * 0.5f;
   //          //clearValue.Color[2] = 0.9f * 0.5f;
   //          //clearValue.Color[3] = 0.5f;
   //          clearValue.Color[0] = 0.f * 0.5f;
   //          clearValue.Color[1] = 0.f * 0.5f;
   //          clearValue.Color[2] = 0. * 0.5f;
   //          clearValue.Color[3] = 0.f;
   //
   //       }
   //
   //       //clearValue.Color = { 0.5f, 0.75f, 0.9f, 0.5f };
   //
   //       ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;
   //
   //       CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);
   //
   //       D3D12_HEAP_FLAGS eheap;
   //
   //       if (m_pgpurenderer->m_pgpucontext->m_bD3D11On12Shared)
   //       {
   //
   //          eheap = D3D12_HEAP_FLAG_NONE;
   //
   //          textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
   //          //textureDesc.Width = width;
   //          //textureDesc.Height = height;
   //             //textureDesc.DepthOrArraySize = 1;
   //          textureDesc.MipLevels = 1;
   //          textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // MUST be D3D11-compatible format
   //          textureDesc.SampleDesc.Count = 1;
   //          textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
   //
   //          m_bRenderTarget = false;
   //
   //          m_bShaderResource = false;
   //
   //       }
   //       else
   //       {
   //
   //          eheap = D3D12_HEAP_FLAG_NONE;
   //
   //       }
   //
   //       D3D12_RESOURCE_STATES stateInitial;
   //
   //       if (m_bRenderTarget)
   //       {
   //
   //          stateInitial = D3D12_RESOURCE_STATE_RENDER_TARGET;
   //
   //       }
   //       else
   //       {
   //
   //          stateInitial = D3D12_RESOURCE_STATE_COPY_DEST;
   //
   //       }
   //
   //       HRESULT hrCreateCommittedResource = pdevice->m_pdevice->CreateCommittedResource(
   //          &heapproperties,
   //          eheap,
   //          &textureDesc,
   //          stateInitial,
   //          pclearvalue,
   //          __interface_of(m_presource));
   //
   //       pdevice->defer_throw_hresult(hrCreateCommittedResource);
   //
   //       m_estate = stateInitial;
   //
   //       if (imagea.has_element())
   //       {
   //
   //          int iCount;
   //
   //
   //          if (m_etype == e_type_cube_map)
   //          {
   //             iCount = 6;
   //             if (imagea.first()->width() != rectangleTarget.width()
   //                || imagea.first()->height() != rectangleTarget.height())
   //             {
   //
   //                throw ::exception(error_failed);
   //
   //             }
   //
   //          }
   //          else
   //          {
   //             iCount = 1;
   //             if (imagea.first()->size() != rectangleTarget.size())
   //             {
   //
   //                throw ::exception(error_failed);
   //
   //             }
   //
   //          }
   //
   //          ::comptr<ID3D12Resource> presourceUpload;
   //
   //          const UINT64 presourceUploadBufferSize = GetRequiredIntermediateSize(m_presource, 0, iCount);
   //
   //          CD3DX12_HEAP_PROPERTIES propertiesUpload(D3D12_HEAP_TYPE_UPLOAD);
   //
   //          auto descUpload = CD3DX12_RESOURCE_DESC::Buffer(presourceUploadBufferSize);
   //
   //          pdevice->m_pdevice->CreateCommittedResource(
   //             &propertiesUpload,
   //             D3D12_HEAP_FLAG_NONE,
   //             &descUpload,
   //             D3D12_RESOURCE_STATE_GENERIC_READ,
   //             nullptr,
   //             __interface_of(presourceUpload));
   //
   //
   //          // 3. Prepare subresources
   //          D3D12_SUBRESOURCE_DATA subresources[6];
   //          if (m_etype == e_type_cube_map)
   //          {
   //             for (int i = 0; i < 6; ++i) {
   //                auto ppixmap = imagea[i];
   //                subresources[i].pData = ppixmap->data(); // Your CPU data pointer
   //                subresources[i].RowPitch = ppixmap->m_iScan;  // 512 * 4
   //                subresources[i].SlicePitch = textureDesc.Width * textureDesc.Height * 4;
   //                //subresources[i].SlicePitch = 0;
   //             }
   //          }
   //          else
   //          {
   //             auto ppixmap = imagea.first();
   //             subresources[0].pData = ppixmap->data();            // pointer to your bitmap data (RGBA8, etc.)
   //             subresources[0].RowPitch = ppixmap->m_iScan;
   //             subresources[0].SlicePitch = subresources[0].RowPitch * ppixmap->height();
   //          }
   //
   //          ::cast < command_buffer > pcommandbuffer = m_pgpurenderer->getLoadAssetsCommandBuffer();
   //
   //          if (!pcommandbuffer)
   //          {
   //
   //             pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_frame());
   //
   //          }
   //
   //          UpdateSubresources(
   //             pcommandbuffer->m_pcommandlist,
   //             m_presource,
   //             presourceUpload, 0, 0, iCount,
   //             subresources);
   //
   //          comptr < IUnknown > punknownResourceUpdate(presourceUpload);
   //
   //          pcommandbuffer->m_comptraHold.add(punknownResourceUpdate);
   //
   //       }
   //
   //       new_texture.set_new_texture();
   //
   //       if (m_bRenderTarget || m_pheapRenderTargetView)
   //       {
   //
   //          create_render_target();
   //
   //       }
   //
   //       if (m_bShaderResource || m_pheapRenderTargetView)
   //       {
   //
   //          create_shader_resource();
   //
   //       }
   //
   //    }
   //
   // }


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

      if (m_bRenderTarget)
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
      if (m_etype == e_type_cube_map)
      {

         srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
      }
      else
      {
         srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      }
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
      if (m_etype == e_type_cube_map)
      {

         samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;

      }
      else
      {

         samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

      }
      samplerDesc.MinLOD = 0;
      samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

      m_handleSampler = m_pheapSampler->GetCPUDescriptorHandleForHeapStart();

      pdevice->m_pdevice->CreateSampler(&samplerDesc, m_handleSampler);

   }


   void texture::create_depth_resources()
   {

      if (!m_handleDepthStencilView.ptr)
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

         m_handleDepthStencilView = m_pheapDepthStencilView->GetCPUDescriptorHandleForHeapStart();
         //::cast < device>pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

         pdevice->m_pdevice->CreateDepthStencilView(
            m_presourceDepthStencilView, &dsvDesc,
            m_handleDepthStencilView);

      }


   }


   class texture::d3d11* texture::d3d11()
   {

      if (!m_pd3d11)
      {

         øconstruct_new(m_pd3d11);

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


   //texture::upload_buffer* texture::_get_upload_buffer()
   //{


   //}


//   ID3D12Resource* texture::_get_upload_buffer()
//   {
//      if (m_presourceUpload && m_presourceUpload->GetDesc().Width >= iCapacity)
//         return m_presourceUpload; // already big enough
//
//      // Release old
////      m_presourceUploadReset();
//
//      // Create new (round up for fewer reallocations, e.g. 256KB align)
//      auto requiredBytes = (iCapacity + 0xFFFF) & ~0xFFFFull;
//
//      CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
//      CD3DX12_RESOURCE_DESC bufDesc = CD3DX12_RESOURCE_DESC::Buffer(requiredBytes);
//      ::cast < renderer > prenderer = m_pgpurenderer;
//
//      ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;
//
//      pdevice->m_pdevice->CreateCommittedResource(
//         &heapProps,
//         D3D12_HEAP_FLAG_NONE,
//         &bufDesc,
//         D3D12_RESOURCE_STATE_GENERIC_READ,
//         nullptr,
//         IID_PPV_ARGS(&m_presourceUpload)
//      );
//
//      return m_presourceUpload;
//
//   }
//

   //void texture::set_pixels_problem_to_maintain_upload_buffer_active_while_command_list_is_not_yet_executed(const ::int_rectangle& rectangle, const void* data)
   //{

   //   ::cast < renderer > prenderer = m_pgpurenderer;
   //   
   //   ::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_frame());

   //   ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

   //   _new_state(
   //      pcommandbuffer->m_pcommandlist,
   //      D3D12_RESOURCE_STATE_COPY_DEST
   //      );

   //   // Dimensions of update
   //   UINT updateWidth = rectangle.right - rectangle.left;
   //   UINT updateHeight = rectangle.bottom - rectangle.top;

   //   // Describe destination
   //   D3D12_RESOURCE_DESC texDesc = m_presource->GetDesc();

   //   // Compute layout
   //   D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
   //   UINT64 totalBytes = 0;
   //   pdevice->m_pdevice->GetCopyableFootprints(
   //      &texDesc, 0, 1, 0,
   //      &footprint, nullptr, nullptr, &totalBytes
   //   );

   //   // Create upload buffer

   //   auto  presourceUploadBuffer = _get_upload_buffer(totalBytes);
   //   //CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
   //   //CD3DX12_RESOURCE_DESC bufDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);

   //   //pdevice->m_pdevice->CreateCommittedResource(
   //   //   &heapProps,
   //   //   D3D12_HEAP_FLAG_NONE,
   //   //   &bufDesc,
   //   //   D3D12_RESOURCE_STATE_GENERIC_READ,
   //   //   nullptr,
   //   //   IID_PPV_ARGS(&presourceUploadBuffer)
   //   //);

   //   // Map and copy row by row
   //   BYTE* pData = nullptr;
   //   presourceUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pData));

   //   auto bytesPerPixel = 4;

   //   UINT srcRowPitch = updateWidth * bytesPerPixel;
   //   BYTE* dst = pData + footprint.Offset;

   //   auto cpuPixels = data;

   //   for (UINT y = 0; y < updateHeight; y++)
   //   {
   //      memcpy(
   //         dst + y * footprint.Footprint.RowPitch,
   //         (const BYTE*)cpuPixels + y * srcRowPitch,
   //         srcRowPitch
   //      );
   //   }

   //   presourceUploadBuffer->Unmap(0, nullptr);

   //   // Copy into texture
   //   D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
   //   dstLoc.pResource = m_presource;
   //   dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
   //   dstLoc.SubresourceIndex = 0;

   //   D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
   //   srcLoc.pResource = presourceUploadBuffer;
   //   srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
   //   srcLoc.PlacedFootprint = footprint;

   //   CD3DX12_BOX srcBox(0, 0, 0, updateWidth, updateHeight, 1);

   //   pcommandbuffer->m_pcommandlist->CopyTextureRegion(
   //      &dstLoc,
   //      rectangle.left, rectangle.top, 0, // destination position
   //      &srcLoc,
   //      &srcBox
   //   );

   //   // ⚠️ caller is responsible for:
   //   // - Transitioning dstTexture into COPY_DEST before this call
   //   // - Transitioning dstTexture back to usable state after this call

   //}


   //void texture::set_pixels_upload_buffer_is_big_exclamation_exclamation_lets_again_try_to_keep_it_around(const ::int_rectangle& rectangle, const void* data)
   //{

   //   ::cast < renderer > prenderer = m_pgpurenderer;

   //   ::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_frame());

   //   ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;


   //   // 1. Describe the subresource we want to update
   //   D3D12_SUBRESOURCE_DATA subresource = {};
   //   subresource.pData = data;
   //   subresource.RowPitch = rectangle.width() * 4;          // bytes per row in CPU buffer
   //   subresource.SlicePitch = rectangle.width() * 4 * rectangle.height();  // total size of region

   //   // 2. Create an intermediate UPLOAD buffer big enough for this region
   //   const UINT64 uploadBufferSize =
   //      GetRequiredIntermediateSize(m_presource, 0, 1);

   //   ::comptr<ID3D12Resource> presourceUpload;
   //   CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
   //   CD3DX12_RESOURCE_DESC   bufferDesc =
   //      CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

   //   pdevice->m_pdevice->CreateCommittedResource(
   //      &heapProps,
   //      D3D12_HEAP_FLAG_NONE,
   //      &bufferDesc,
   //      D3D12_RESOURCE_STATE_GENERIC_READ,
   //      nullptr,
   //      IID_PPV_ARGS(&presourceUpload));

   //   // 3. Transition the texture into COPY_DEST
   //   _new_state(pcommandbuffer->m_pcommandlist, 
   //      D3D12_RESOURCE_STATE_COPY_DEST);

   //   // 4. Copy CPU data → upload buffer → texture
   //   //    UpdateSubresources takes care of mapping & copying
   //   UpdateSubresources(
   //      pcommandbuffer->m_pcommandlist,
   //      m_presource, 
   //      presourceUpload,
   //      0, 0, 1, &subresource);

   //}

   texture::upload_buffer::upload_buffer()
   {

      m_pMap = nullptr;
   }


   texture::upload_buffer::~upload_buffer()
   {

      unmap();


   }


   void texture::upload_buffer::initialize_upload_buffer(texture* ptexture)
   {

      ::cast < renderer > prenderer = ptexture->m_pgpurenderer;

      ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      m_ptexture = ptexture;

      auto presource = ptexture->m_presource.m_p;

      // 2. Create an intermediate UPLOAD buffer big enough for this region
      const UINT64 uploadBufferSize =
         GetRequiredIntermediateSize(presource, 0, 1);

      CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
      CD3DX12_RESOURCE_DESC   bufferDesc =
         CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

      HRESULT hrCreateCommittedResource = 
         pdevice->m_pdevice->CreateCommittedResource(
         &heapProps,
         D3D12_HEAP_FLAG_NONE,
         &bufferDesc,
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&m_presourceUpload));

      ::defer_throw_hresult(hrCreateCommittedResource);

      m_descTexture = presource->GetDesc();

      pdevice->m_pdevice->GetCopyableFootprints(
         &m_descTexture,      // texture description
         0,             // first subresource
         1,             // num subresources
         0,             // base offset
         &m_footprint,    // out: layout for subresource
         &m_uNumRows,      // out: number of rows
         &m_uRowSizeInBytes, // out: bytes per row (unpadded)
         &m_uUploadBufferSize); // out: required buffer size


   }



   texture::upload_buffer* texture::_get_upload_buffer()
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      auto pframestorage = pdevice->current_frame_storage();

      auto& pobject = pframestorage->m_mapObject[this][::gpu::e_resource_upload_buffer];

      if (!pobject)
      {

         auto puploadbuffer = øallocate upload_buffer();

         puploadbuffer->initialize_upload_buffer(this);

         pobject = puploadbuffer;

      }

      ::cast < texture::upload_buffer > puploadbuffer = pobject;

      return puploadbuffer;

   }


   void texture::upload_buffer::on_end_frame()
   {

      upload_damaged_regions();

   }


   void texture::upload_buffer::damage::update_copyable_region(ID3D12GraphicsCommandList* pcommandlist)
   {

      pcommandlist->CopyTextureRegion(
         &m_copylocationTarget,
         m_iLeft, m_iTop, 0, // destination position
         &m_copylocationSource,
         &m_boxSource
      );

   }


   void texture::upload_buffer::map()
   {

      if (!m_pMap)
      {

         if (!m_presourceUpload)
         {

            throw ::exception(error_wrong_state);

         }

         // 4. Copy CPU data → upload buffer → texture
//    UpdateSubresources takes care of mapping & copying
// Map and copy row by row
//BYTE* pData = nullptr;
         HRESULT hrUploadResourceMap = m_presourceUpload->Map(0, nullptr, reinterpret_cast<void**>(&m_pMap));

         ::defer_throw_hresult(hrUploadResourceMap);

      }

   }


   void texture::upload_buffer::unmap()
   {

      if (m_pMap)
      {

         m_presourceUpload->Unmap(0, nullptr);

         m_pMap = nullptr;

      }

   }


   void texture::upload_buffer::upload_damaged_regions()
   {

      if (m_damagea.has_element())
      {

         //unmap();


         ::cast < renderer > prenderer = m_ptexture->m_pgpurenderer;

         ::cast < context > pcontext = prenderer->m_pgpucontext;

         auto pgpucommandbuffer = pcontext->beginSingleTimeCommands(nullptr);

         ::cast < command_buffer > pcommandbuffer = pgpucommandbuffer;

         {

            texture_guard guard(
               pcommandbuffer->m_pcommandlist,
               m_ptexture,
               D3D12_RESOURCE_STATE_COPY_DEST);

            for (auto& damage : m_damagea)
            {

               try
               {

                  damage.update_copyable_region(pcommandbuffer->m_pcommandlist);

               }
               catch (...)
               {

               }

            }

            m_damagea.clear();

         }

         pcontext->endSingleTimeCommands(pcommandbuffer);

      }

   }


   void texture::upload_buffer::update_pixels(const ::int_rectangle& rectangle, const void* data)
   {

      map();

      auto bytesPerPixel = 4;

      //// 1. Describe the subresource we want to update
      //D3D12_SUBRESOURCE_DATA subresource = {};
      //subresource.pData = data;
      //subresource.RowPitch = rectangle.width() * 4;          // bytes per row in CPU buffer
      //subresource.SlicePitch = rectangle.width() * 4 * rectangle.height();  // total size of region

      UINT srcRowPitch = rectangle.width() * bytesPerPixel;
      BYTE* dst = (BYTE *) m_pMap + m_footprint.Offset + 
         rectangle.left * bytesPerPixel
         + rectangle.top * m_footprint.Footprint.RowPitch;

      auto cpuPixels = data;

      for (UINT y = 0; y < rectangle.height(); y++)
      {
         memcpy(
            dst + y * m_footprint.Footprint.RowPitch,
            (const BYTE*)cpuPixels + y * srcRowPitch,
            srcRowPitch
         );
      }

      auto & damageNew = m_damagea.add_new();
      //damage damage;
      // Copy into texture
      //D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
      damageNew.m_copylocationTarget.pResource = m_ptexture->m_presource;
      damageNew.m_copylocationTarget.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
      damageNew.m_copylocationTarget.SubresourceIndex = 0;

      //D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
      damageNew.m_copylocationSource.pResource = m_presourceUpload;
      damageNew.m_copylocationSource.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
      damageNew.m_copylocationSource.PlacedFootprint = m_footprint;

      damageNew.m_boxSource = CD3DX12_BOX(
         rectangle.left, rectangle.top,
         0, rectangle.right, rectangle.bottom, 1);

      damageNew.m_iLeft = rectangle.left;
      damageNew.m_iTop = rectangle.top;

      // ⚠️ caller is responsible for:
      // - Transitioning dstTexture into COPY_DEST before this call
      // - Transitioning dstTexture back to usable state after this call


   }


   void texture::set_pixels(const ::int_rectangle& rectangle, const void* data)
   {

      //::cast < renderer > prenderer = m_pgpurenderer;

      //::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_frame());

      //::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      auto puploadbuffer = _get_upload_buffer();

      //D3D12_RESOURCE_DESC texDesc = m_presource->GetDesc();


      puploadbuffer->update_pixels(rectangle, data);

      //puploadbuffer->map();


   }


} // namespace gpu_directx12




