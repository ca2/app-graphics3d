// Created by camilo on 2025-06-08 18:14 < 3ThomasBorregaardSørensen!!
#include "platform.h"
#include "command_buffer.h"
#include "window_attachment.h"
#include "texture.h"
#include "renderer.h"
#include "acme/graphics/image/pixmap.h"
#include "aura/graphics/image/image.h"
#include "bred/gpu/layer.h"
#include <stb/stb_image.h>


namespace gpu_directx12
{

   ::comptr<ID3DBlob> create_computer_shader_blob(const ::block &block);

   texture::texture()
   {

      //m_bRenderTarget = false;
      m_textureflags.m_bShaderResource = false;
      //m_rtvDescriptorSize = 0;

      new_texture.set_new_texture();

      if (m_iTextureSerial == 0xd)
      {

         ::warning("m_iTextureSerial == 0xd");

      }

      //m_dxgiformat = DXGI_FORMAT_UNKNOWN;

   }


   texture::~texture()
   {

   }


   void texture::_create_texture(const ::gpu::texture_data & texturedata)
   {

      DXGI_FORMAT format;
      // 1. Create the texture resource

      auto &textureDesc = m_resourcedesc;
      ::zero(textureDesc);
      textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      textureDesc.Width = this->raw_width();
      textureDesc.Height = this->raw_height();
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
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

      if (m_textureattributes.m_iChannelCount == 2)
      {

          format = DXGI_FORMAT_R8G8_UNORM;

      }
      else
      {

          format = DXGI_FORMAT_B8G8R8A8_UNORM;

      }

      textureDesc.MipLevels = m_textureattributes.m_iMipCount;
      //textureDesc.DepthOrArraySize = 1;
      textureDesc.Format = format;
      textureDesc.SampleDesc.Count = 1;
      textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

      ::cast < ::gpu_directx12::device > pdevice = m_pgpucontext->m_pgpudevice;

      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

      D3D12_HEAP_FLAGS eheap;

      if (m_pgpucontext->m_bD3D11On12Shared)
      {

         //eheap = D3D12_HEAP_FLAG_NONE;
         eheap = D3D12_HEAP_FLAG_SHARED;

         textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
         //textureDesc.Width = width;
         //textureDesc.Height = height;
         //textureDesc.DepthOrArraySize = 1;
         textureDesc.MipLevels = 1;
         textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;  // MUST be D3D11-compatible format
         textureDesc.SampleDesc.Count = 1;
         textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

         //m_textureflags.m_bRenderTarget = false;

         informationf("m_textureflags.m_bShaderResource %d", m_textureflags.m_bShaderResource);
         //m_textureflags.m_bShaderResource = false;

         //textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

      }
      else
      {

         eheap = D3D12_HEAP_FLAG_NONE;

      }

      D3D12_RESOURCE_STATES stateInitial;

      if (m_textureflags.m_bRenderTarget)
      {

         stateInitial = D3D12_RESOURCE_STATE_RENDER_TARGET;

      }
      else if (m_textureflags.m_bShaderResource)
      {

         stateInitial = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

      }
      else 
      {

         stateInitial = D3D12_RESOURCE_STATE_COPY_DEST;

      }

      if (m_textureflags.m_bRenderTarget)
      {

         textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

      }

      if (textureDesc.MipLevels > 1)
      {

         textureDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

      }

      D3D12_CLEAR_VALUE * pclearvalue = nullptr;

      D3D12_CLEAR_VALUE clearValue = {};

      if (m_textureflags.m_bRenderTarget)
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

      ::cast<::gpu_directx12::context> pcontext = m_pgpucontext;

      pcontext->_construct_new(m_pd3d12resourceTexture);

      m_pd3d12resourceTexture->m_state.m_resourcestates=stateInitial;

      m_pd3d12resourceTexture->create(&heapproperties, eheap, &textureDesc, pclearvalue);


      //HRESULT hrCreateCommittedResource = pdevice->m_pd3d12device->CreateCommittedResource(
         
         //__interface_of(m_presourceTexture));

      //pdevice->defer_throw_hresult(hrCreateCommittedResource);

      m_pd3d12resourceTexture->set_name(m_strTextureName);

      //m_state = stateInitial;

      if (texturedata.is_pixmap_array())
      {

         //int iCount;


         if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
         {
           // iCount = 6;
            if (texturedata.pixmapa().first()->size() != this->size())
            {

               throw ::exception(error_failed);

            }

         }
         else
         {
            //iCount = 1;
            if (texturedata.pixmapa().first()->size() != this->size())
            {

               throw ::exception(error_failed);

            }

         }

         ::cast<command_buffer> pcommandbuffer = m_pgpucontext->m_pgpurenderer->getLoadAssetsCommandBuffer();

         if (!pcommandbuffer)
         {

            pcommandbuffer = m_pgpucontext->m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());
         
         }

         auto pstaticuploadbuffer = _get_static_upload_buffer();

         pstaticuploadbuffer->update_with_texture_data(pcommandbuffer, texturedata);

      }
      else if (texturedata.is_raw_scoped_pixmap())
      {


         
      ::cast<command_buffer> pcommandbuffer = m_pgpucontext->m_pgpurenderer->getLoadAssetsCommandBuffer();

         if (!pcommandbuffer)
         {

            pcommandbuffer = m_pgpucontext->m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());
         }

         auto pstaticuploadbuffer = _get_static_upload_buffer();
         //::gpu::texture_data texturedata(texturedata.raw_scoped_data());
         pcommandbuffer->m_particleaHold.add(pstaticuploadbuffer);

         pstaticuploadbuffer->update_with_texture_data(pcommandbuffer, texturedata);


      }

      new_texture.set_new_texture();

      if (m_textureflags.m_bRenderTarget || m_pheapRenderTargetView)
      {

         create_render_target();

      }

      if (m_textureflags.m_bShaderResource || m_pheapRenderTargetView)
      {

         create_shader_resource();

      }

      set_ok_flag();

   }


   // void texture::initialize_image_texture(::gpu::renderer* prenderer, const ::i32_rectangle& rectangleTarget, bool bWithDepth, const ::pointer_array < ::image::image >& imagea, enum_type etype)
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
   //       if (m_etype == ::gpu::e_texture_cube_map)
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
   //       HRESULT hrCreateCommittedResource = pdevice->m_pd3d12device->CreateCommittedResource(
   //          &heapproperties,
   //          eheap,
   //          &textureDesc,
   //          stateInitial,
   //          pclearvalue,
   //          __interface_of(m_presourceTexture));
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
   //          if (m_etype == ::gpu::e_texture_cube_map)
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
   //          const UINT64 presourceUploadBufferSize = GetRequiredIntermediateSize(m_presourceTexture, 0, iCount);
   //
   //          CD3DX12_HEAP_PROPERTIES propertiesUpload(D3D12_HEAP_TYPE_UPLOAD);
   //
   //          auto descUpload = CD3DX12_RESOURCE_DESC::Buffer(presourceUploadBufferSize);
   //
   //          pdevice->m_pd3d12device->CreateCommittedResource(
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
   //          if (m_etype == ::gpu::e_texture_cube_map)
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
   //             pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());
   //
   //          }
   //
   //          UpdateSubresources(
   //             pcommandbuffer->m_pcommandlist,
   //             m_presourceTexture,
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




   void texture::create_hdr_texture_on_memory(::gpu::context *pgpucontext, const ::block &block)
   {
      //Dx12Texture out = {};

      m_pgpucontext = pgpucontext;

      auto data = block.data();

      auto size = block.size();

      // ------------------------------------------------------------
      // Load image (float)
      // ------------------------------------------------------------
      int width = 0, height = 0, channels = 0;

      stbi_set_flip_vertically_on_load(0);
      float *image = stbi_loadf_from_memory((const stbi_uc *)data, (int)size, &width, &height, &channels, 0);

      if (!image)
      {
         throw ::exception(error_failed, "stbi_loadf_from_memory failed");
      }

      //m_textureattributes.m_rectangleTarget.left = 0;
      //m_textureattributes.m_rectangleTarget.top = 0;
      m_textureattributes.m_size = { width, height };
      m_textureattributes.m_iLayerCount = 1;
      m_textureattributes.m_iMipCount = 1;
      m_textureattributes.m_iBitsPerChannel = 32;
      m_textureattributes.m_iChannelCount = 4;

      // ------------------------------------------------------------
      // Expand to RGBA if needed
      // ------------------------------------------------------------
      ::memory memory;
      float *src = image;

      if (channels != 4)
      {
         size_t pixels = size_t(width) * height;
         auto sizeInBytes = pixels * 4 * sizeof(float);
         memory.set_size(sizeInBytes);

         auto rgba= (float *)memory.data();

         if (channels == 3)
         {
            for (size_t i = 0; i < pixels; ++i)
            {
               rgba[i * 4 + 0] = src[i * 3 + 0];
               rgba[i * 4 + 1] = src[i * 3 + 1];
               rgba[i * 4 + 2] = src[i * 3 + 2];
               rgba[i * 4 + 3] = 1.0f;
            }
         }
         else if (channels == 1)
         {
            for (size_t i = 0; i < pixels; ++i)
            {
               float v = src[i];
               rgba[i * 4 + 0] = v;
               rgba[i * 4 + 1] = v;
               rgba[i * 4 + 2] = v;
               rgba[i * 4 + 3] = 1.0f;
            }
         }
         else
         {
            stbi_image_free(image);
            throw ::exception(error_failed, "Unsupported channel count");
         }

         src = rgba;
         channels = 4;
      }

      // ------------------------------------------------------------
      // Create GPU texture (DEFAULT heap)
      // ------------------------------------------------------------
      auto &texDesc = m_resourcedesc;
      ::zero(texDesc);
      texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      texDesc.Width = width;
      texDesc.Height = height;
      texDesc.DepthOrArraySize = 1;
      texDesc.MipLevels = 1;
      texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      texDesc.SampleDesc.Count = 1;
      texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;


      ::cast<::gpu_directx12::device> pdevice = m_pgpucontext->m_pgpudevice;

      auto device = pdevice->m_pd3d12device;

      CD3DX12_HEAP_PROPERTIES heapproperties(D3D12_HEAP_TYPE_DEFAULT);

      D3D12_RESOURCE_STATES stateInitial;

      //if (m_textureflags.m_bRenderTarget)
      //{

      //   stateInitial = D3D12_RESOURCE_STATE_RENDER_TARGET;
      //}
      //else if (m_textureflags.m_bShaderResource)
      //{

      //   stateInitial = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
      //}
      //else
      {

         // There's texture data (hdr image).
         // First operation will copying data to texture.

         stateInitial = D3D12_RESOURCE_STATE_COPY_DEST;
      }

      //m_state = stateInitial;

      ::cast<::gpu_directx12::context> pcontext = m_pgpucontext;

      pcontext->_construct_new(m_pd3d12resourceTexture);

      m_pd3d12resourceTexture->m_state.m_resourcestates = stateInitial;

      m_pd3d12resourceTexture->create(&heapproperties, D3D12_HEAP_FLAG_NONE, &texDesc);
      //auto hrCreateCommittedResource = device->CreateCommittedResource(&heapproperties, D3D12_HEAP_FLAG_NONE, &texDesc, stateInitial, nullptr,
        //                              __interface_of(m_presourceTexture));

      //pdevice->defer_throw_hresult(hrCreateCommittedResource);

      m_pd3d12resourceTexture->set_name(m_strTextureName);

      ::cast<command_buffer> pcommandbuffer = m_pgpucontext->m_pgpurenderer->getLoadAssetsCommandBuffer();

      if (!pcommandbuffer)
      {

         pcommandbuffer = m_pgpucontext->m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());

      }

      auto pstaticuploadbuffer = _get_static_upload_buffer();

      pixmap_t pixmap;
      pixmap.m_pimage32 = (image32_t *) src;
      pixmap.m_pimage32Raw = (image32_t *)src;

      ::gpu::texture_data texturedata(pixmap);
      pcommandbuffer->m_particleaHold.add(pstaticuploadbuffer);

      pstaticuploadbuffer->update_with_texture_data(pcommandbuffer, texturedata);

      set_ok_flag();
      // ------------------------------------------------------------
      // Create upload buffer
      // ------------------------------------------------------------
      //UINT64 uploadSize = 0;
      //device->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadSize);

      //device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
      //                                &CD3DX12_RESOURCE_DESC::Buffer(uploadSize), D3D12_RESOURCE_STATE_GENERIC_READ,
      //                                nullptr, IID_PPV_ARGS(&out.upload));

      //// ------------------------------------------------------------
      //// Upload texture data
      //// ------------------------------------------------------------
      //D3D12_SUBRESOURCE_DATA sub = {};
      //sub.pData = src;
      //sub.RowPitch = width * 4 * sizeof(float);
      //sub.SlicePitch = sub.RowPitch * height;

      //UpdateSubresources(cmd, out.resource.Get(), out.upload.Get(), 0, 0, 1, &sub);

      //// ------------------------------------------------------------
      //// Transition to SRV
      //// ------------------------------------------------------------
      //cmd->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(out.resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
      //                                                              D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

      //// ------------------------------------------------------------
      //// Create SRV (CPU descriptor heap)
      //// ------------------------------------------------------------
      //D3D12_SHADER_RESOURCE_VIEW_DESC srv = {};
      //srv.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      //srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      //srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
      //srv.Texture2D.MipLevels = 1;

      //device->CreateShaderResourceView(out.resource.Get(), &srv, cpuSrv);

      //out.cpuSrv = cpuSrv;

      stbi_image_free(image);
      //return out;
   }


   void texture::layer::_create_render_target_view(::gpu_directx12::texture *ptexture, 
                                            int iAttachmentCount)
   {

   ::cast < ::gpu_directx12::device > pdevice = ptexture->m_pgpucontext->m_pgpudevice;

      //dx12_framebuffer fb{};

      if (ptexture->m_iCurrentLayer < 0)
      {
         m_size.cx = ptexture->width();
         m_size.cy = ptexture->height();
         m_iLayerCount = ptexture->m_textureattributes.m_iLayerCount;
      }
      else
      {
         m_size.cx = ptexture->mip_width();
         m_size.cy = ptexture->mip_height();
         m_iLayerCount = 1;
      }

      //fb.rtvHandles.resize(attachmentCount);

      //for (int i = 0; i < iAttachmentCount; ++i)
      //{
         // Allocate RTV descriptor
         m_handleRenderTargetView = ptexture->_allocate_render_target_view_handle();

         D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
         rtvDesc.Format = ptexture->m_resourcedesc.Format;
         rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
         rtvDesc.Texture2DArray.MipSlice = maximum(0,ptexture->m_iCurrentMip);
         rtvDesc.Texture2DArray.FirstArraySlice = (ptexture->m_iCurrentLayer < 0) ? 0 : ptexture->m_iCurrentLayer;
         rtvDesc.Texture2DArray.ArraySize = maximum(1,m_iLayerCount);
         rtvDesc.Texture2DArray.PlaneSlice = 0;

         pdevice->m_pd3d12device->CreateRenderTargetView(ptexture->m_pd3d12resourceTexture->m_presource, &rtvDesc,
                                                    m_handleRenderTargetView);


      //}

      //m_dx12_framebuffer = fb;
   }


   void texture::layer::create_render_target_view(::gpu_directx12::texture *ptexture)
   {

      int iAttachmentCount = 1;

      if (ptexture->m_textureflags.m_bWithDepth)
      {

         iAttachmentCount++;

      }

      _create_render_target_view(ptexture, iAttachmentCount);

   }


   D3D12_CPU_DESCRIPTOR_HANDLE texture::_allocate_render_target_view_handle()
   {

      if (!m_pheapRenderTargetView)
      {

         if (m_iRenderTargetViewHandleCount < 0)
         {

            int iMipCount = m_textureattributes.m_iMipCount;

            if (iMipCount <= 0)
            {

               iMipCount = m_textureattributes.maximum_mip_count();
            }

            m_iRenderTargetViewHandleCount = m_textureattributes.m_iLayerCount * iMipCount + 2;
         }

         if (m_iRenderTargetViewHandleCount <= 0)
         {

            return {};

         }
               ::cast<device> pdevice = m_pgpucontext->m_pgpudevice;
         m_uRenderTargetViewIncrement = pdevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
         // 2. Create RTV descriptor heap
         D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
               rtvHeapDesc.NumDescriptors = m_iRenderTargetViewHandleCount;
         rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
         rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
         HRESULT hrCreateDescriptorHeap =
            pdevice->m_pd3d12device->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_pheapRenderTargetView));

         pdevice->defer_throw_hresult(hrCreateDescriptorHeap);
         m_iRenderTargetViewHandle = 0;
      }
      if (m_iRenderTargetViewHandle < 0)
      {

         return {};

      }
      if (m_iRenderTargetViewHandle > m_iRenderTargetViewHandleCount)
      {

         throw ::exception(error_failed);

      }
      
      //// 3. Create RTV
//      m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
      CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart(),
                                              m_iRenderTargetViewHandle * 
         m_uRenderTargetViewIncrement);

      m_iRenderTargetViewHandle++;

      return rtvHandle;

   }


   IDXGISurface * texture::_dxgi_surface()
   {

      auto & pdxgisurface = d3d11()->m_pdxgisurface;

      if (!pdxgisurface)
      {

         if (!d3d11()->m_pd3d11resourceWrapped)
         {

            throw ::exception(error_failed);

         }

         d3d11()->m_pd3d11resourceWrapped.as(pdxgisurface);

      }

      return pdxgisurface;

   }
      
   
   struct texture::layer &texture::current_layer()
   {

      // ASSERT(m_textureattributes.m_etexture == ::gpu::e_texture_cube_map);

      int iMip = maximum(0, m_iCurrentMip);

      int iLayerPlusOne = maximum(0, m_iCurrentLayer + 1);

      auto &layer = this->m_miplayera.atø(iMip).atø(iLayerPlusOne);

      if (layer.is_empty())
      {

         layer.create_render_target_view(this);

      }

      return layer;
   }



   void texture::create_render_target()
   {

      if (m_textureflags.m_bRenderTarget)
      {

         if (m_pgpucontext->m_bD3D11On12Shared)
         {

            return;

         }


         //pdevice->m_pd3d12device->CreateRenderTargetView(m_pd3d12resourceTexture->m_presource, nullptr, m_handleRenderTargetView);


      }

   }


   void texture::create_shader_resource()
   {

      //if (m_pgpurenderer->m_pgpucontext->m_bD3D11On12Shared)
      //{

      //   return;

      //}

      ::cast < device > pdevice = m_pgpucontext->m_pgpudevice;

      // 4. Create SRV descriptor heap
      D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
      srvHeapDesc.NumDescriptors = 1;
      srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

      HRESULT hrCreateDescriptorHeap = pdevice->m_pd3d12device->CreateDescriptorHeap(
         &srvHeapDesc, __interface_of(m_pheapShaderResourceView));

      pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      // 5. Create SRV
      DXGI_FORMAT format = m_resourcedesc.Format;
      D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Format = format;
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
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

      pdevice->m_pd3d12device->CreateShaderResourceView(m_pd3d12resourceTexture->m_presource, &srvDesc, m_handleShaderResourceView);

      // Descriptor heap for Sampler
      D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
      samplerHeapDesc.NumDescriptors = 1;
      samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
      samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
      pdevice->m_pd3d12device->CreateDescriptorHeap(&samplerHeapDesc, __interface_of(m_pheapSampler));

      D3D12_SAMPLER_DESC samplerDesc = {};
      samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
      samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
      samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
      samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
      if (m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
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

      pdevice->m_pd3d12device->CreateSampler(&samplerDesc, m_handleSampler);

   }


   void texture::create_depth_resources()
   {

      if (!m_handleDepthStencilView.ptr)
      {
         if (m_pgpucontext->m_bD3D11On12Shared)
         {

            return;

         }
         ::cast < device > pdevice = m_pgpucontext->m_pgpudevice;



         // 2. Describe depth stencil resource
         D3D12_RESOURCE_DESC depthDesc = {};
         depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
         depthDesc.Width = this->width();
         depthDesc.Height = this->height();
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
         pdevice->m_pd3d12device->CreateCommittedResource(
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
         HRESULT hrCreateDescriptorHeapDsv = pdevice->m_pd3d12device->CreateDescriptorHeap(
            &dsvHeapDesc, __interface_of(m_pheapDepthStencilView));
         pdevice->defer_throw_hresult(hrCreateDescriptorHeapDsv);

         // 4. Create DSV
         D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
         dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
         dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
         dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

         m_handleDepthStencilView = m_pheapDepthStencilView->GetCPUDescriptorHandleForHeapStart();
         //::cast < device>pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

         pdevice->m_pd3d12device->CreateDepthStencilView(
            m_presourceDepthStencilView, &dsvDesc,
            m_handleDepthStencilView);

      }


   }


   class texture::d3d11* texture::d3d11()
   {

      informationf("DX12 D2D_WRAP_STAGE enter helper=%p", m_pd3d11.m_p);

      if (!m_pd3d11)
      {

         construct_newø(m_pd3d11);

         informationf("DX12 D2D_WRAP_STAGE helper-created");

         if (!m_pd3d11->m_pd3d11resourceWrapped)
         {

            assert(!m_pheapDepthStencilView);
            //assert(!ptexture->m_pheapRenderTargetView);
            //assert(!ptexture->m_pheapShaderResourceView);
            //assert(!ptexture->m_pheapSampler);

            //auto & sharedHandle= ptexture->d3d11()->sharedHandle;

            //::defer_throw_hresult(pdevice->m_pd3d12device->CreateSharedHandle(
            //   ptexture->m_presource, nullptr, GENERIC_ALL, nullptr, 
            //   &sharedHandle));

            D3D11_RESOURCE_FLAGS flags = {};
            //flags.BindFlags = D3D11_BIND_RENDER_TARGET;
            flags.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
            assert(m_pd3d12resourceTexture->m_presource); // Confirm it’s non-null
            ::cast < ::gpu_directx12::device > pdevice = m_pgpucontext->m_pgpudevice;

            informationf("DX12 D2D_WRAP_STAGE device-ready");

            auto pd3d11on12 = pdevice->d3d11on12();

            informationf("DX12 D2D_WRAP_STAGE d3d11on12-ready");

            auto pgpud3d11on12 = pdevice->d3d11on12()->m_pd3d11on12;

            HRESULT hrCreateWrappedResource = pgpud3d11on12->CreateWrappedResource(
               m_pd3d12resourceTexture->m_presource,
               &flags,
               D3D12_RESOURCE_STATE_RENDER_TARGET,
               D3D12_RESOURCE_STATE_RENDER_TARGET,
               __interface_of(m_pd3d11->m_pd3d11resourceWrapped)
            );

            informationf(
               "DX12 D2D_WRAP hr=0x%08x resourceFlags=0x%x initialState=0x%x",
               (unsigned int)hrCreateWrappedResource,
               (unsigned int)m_resourcedesc.Flags,
               (unsigned int)m_pd3d12resourceTexture->m_state.m_resourcestates);

            ::defer_throw_hresult(hrCreateWrappedResource);

         }

         m_pd3d11->m_d3d11resourceaWrapped[0] = { m_pd3d11->m_pd3d11resourceWrapped };

      }

      return m_pd3d11;

   }


   //void texture::blend(::gpu::texture* ptexture)
   //{



   //}


   void texture::_initialize_gpu_texture(::gpu::context * pgpucontext, UINT uCurrentBufferIndex, IDXGISwapChain3* pdxgiswapchain)
   {

      m_pgpucontext = pgpucontext;

      if (!m_pd3d12resourceTexture || !m_pd3d12resourceTexture->m_presource)
      {

         m_textureattributes.m_iMipCount = 1;
         m_textureattributes.m_iLayerCount = 1;
         m_iCurrentMip = 0;
         m_iCurrentLayer = 0;

         ::cast<::gpu_directx12::context> pcontext = pgpucontext;

         pcontext->_construct_new(m_pd3d12resourceTexture);

         auto hrSwapChainGetBuffer = pdxgiswapchain->GetBuffer(
            uCurrentBufferIndex, __interface_of(m_pd3d12resourceTexture->m_presource));

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
      //         auto hrCreateDescriptorHeap = pgpudevice->m_pd3d12device->CreateDescriptorHeap(
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
      //         pgpudevice->m_pd3d12device->CreateRenderTargetView(
      //            m_presourceTexture, nullptr, m_handleRenderTargetView);
      //            //rtvHandle.Offset(1, m_rtvDescriptorSize);
      //         }
      //      }
      //
            //auto pdevice = pgpudevice->m_pd3d12device;

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
            ////   //HRESULT hrCreateDescriptorHeap = pdevice->m_pd3d12device->CreateDescriptorHeap(&rtvHeapDesc, __interface_of(m_pheapRenderTargetView));

            ////   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

            ////   //// 3. Create RTV
            ////   //m_handleRenderTargetView = m_pheapRenderTargetView->GetCPUDescriptorHandleForHeapStart();
            ////   CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

            ////   pdevice->m_pd3d12device->CreateRenderTargetView(m_presourceTexture, nullptr, m_handleRenderTargetView);

            ////}

            ////if (bCreateShaderResourceView)
            ////{

            ////   //// 4. Create SRV descriptor heap
            ////   //D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
            ////   //srvHeapDesc.NumDescriptors = 1;
            ////   //srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            ////   //srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            ////   //
            ////   //HRESULT hrCreateDescriptorHeap = pdevice->m_pd3d12device->CreateDescriptorHeap(&srvHeapDesc, __interface_of(m_pheapShaderResourceView));

            ////   //pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

            ////   //// 5. Create SRV
            ////   //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            ////   //srvDesc.Format = format;
            ////   //srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            ////   //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            ////   //srvDesc.Texture2D.MostDetailedMip = 0;
            ////   //srvDesc.Texture2D.MipLevels = 1;

            ////   //m_handleShaderResourceView = m_pheapShaderResourceView->GetCPUDescriptorHandleForHeapStart();

            ////   //pdevice->m_pd3d12device->CreateShaderResourceView(m_presourceTexture, &srvDesc, m_handleShaderResourceView);

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
//      pdevice->m_pd3d12device->CreateCommittedResource(
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

   //void texture::set_pixels_problem_to_maintain_upload_buffer_active_while_command_list_is_not_yet_executed(const ::i32_rectangle& rectangle, const void* data)
   //{

   //   ::cast < renderer > prenderer = m_pgpurenderer;
   //   
   //   ::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());

   //   ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

   //   _new_state(
   //      pcommandbuffer->m_pcommandlist,
   //      D3D12_RESOURCE_STATE_COPY_DEST
   //      );

   //   // Dimensions of update
   //   UINT updateWidth = rectangle.right - rectangle.left;
   //   UINT updateHeight = rectangle.bottom - rectangle.top;

   //   // Describe destination
   //   D3D12_RESOURCE_DESC texDesc = m_presourceTexture->GetDesc();

   //   // Compute layout
   //   D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
   //   UINT64 totalBytes = 0;
   //   pdevice->m_pd3d12device->GetCopyableFootprints(
   //      &texDesc, 0, 1, 0,
   //      &footprint, nullptr, nullptr, &totalBytes
   //   );

   //   // Create upload buffer

   //   auto  presourceUploadBuffer = _get_upload_buffer(totalBytes);
   //   //CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
   //   //CD3DX12_RESOURCE_DESC bufDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBytes);

   //   //pdevice->m_pd3d12device->CreateCommittedResource(
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
   //   dstLoc.pResource = m_presourceTexture;
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


   //void texture::set_pixels_upload_buffer_is_big_exclamation_exclamation_lets_again_try_to_keep_it_around(const ::i32_rectangle& rectangle, const void* data)
   //{

   //   ::cast < renderer > prenderer = m_pgpurenderer;

   //   ::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());

   //   ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;


   //   // 1. Describe the subresource we want to update
   //   D3D12_SUBRESOURCE_DATA subresource = {};
   //   subresource.pData = data;
   //   subresource.RowPitch = rectangle.width() * 4;          // bytes per row in CPU buffer
   //   subresource.SlicePitch = rectangle.width() * 4 * rectangle.height();  // total size of region

   //   // 2. Create an intermediate UPLOAD buffer big enough for this region
   //   const UINT64 uploadBufferSize =
   //      GetRequiredIntermediateSize(m_presourceTexture, 0, 1);

   //   ::comptr<ID3D12Resource> presourceUpload;
   //   CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
   //   CD3DX12_RESOURCE_DESC   bufferDesc =
   //      CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

   //   pdevice->m_pd3d12device->CreateCommittedResource(
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
   //      m_presourceTexture, 
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

      ::cast < renderer > prenderer = ptexture->m_pgpucontext->m_pgpurenderer;

      ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      m_ptexture = ptexture;

      auto & presource = ptexture->m_pd3d12resourceTexture->m_presource;

      // 2. Create an intermediate UPLOAD buffer big enough for this region
      const UINT64 uploadBufferSize =
         GetRequiredIntermediateSize(presource, 0, 1);

      CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
      CD3DX12_RESOURCE_DESC   bufferDesc =
         CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

      HRESULT hrCreateCommittedResource = 
         pdevice->m_pd3d12device->CreateCommittedResource(
         &heapProps,
         D3D12_HEAP_FLAG_NONE,
         &bufferDesc,
         D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr,
         IID_PPV_ARGS(&m_presourceUpload));

      ::defer_throw_hresult(hrCreateCommittedResource);

      m_presourceUpload->SetName(::wstring(ptexture->m_strTextureName + " Upload"));

      m_descTexture = presource->GetDesc();

      pdevice->m_pd3d12device->GetCopyableFootprints(
         &m_descTexture,      // texture description
         0,             // first subresource
         1,             // num subresources
         0,             // base offset
         &m_footprint,    // out: layout for subresource
         &m_uNumRows,      // out: number of rows
         &m_uRowSizeInBytes, // out: bytes per row (unpadded)
         &m_uUploadBufferSize); // out: required buffer size


   }


      texture::static_upload_buffer::static_upload_buffer() {
         //m_pMap = nullptr; 
          }


   texture::static_upload_buffer::~static_upload_buffer()
         {
            
            
            //unmap();
         }


   void texture::static_upload_buffer::initialize_static_upload_buffer(texture *ptexture)
   {

      ::cast<renderer> prenderer = ptexture->m_pgpucontext->m_pgpurenderer;

      ::cast<::gpu_directx12::context> pcontext = prenderer->m_pgpucontext;

      ::cast<::gpu_directx12::device> pdevice = pcontext->m_pgpudevice;

      m_ptexture = ptexture;

      auto & presource = ptexture->m_pd3d12resourceTexture->m_presource;

      //::comptr<ID3D12Resource> presourceUpload;

      auto iCount = ptexture->m_textureattributes.m_iLayerCount;

      if (m_iResourceCount < 0)
      {
         m_iResourceCount = iCount;

      }

      const UINT64 uUploadBufferSize = GetRequiredIntermediateSize(presource, 0, m_iResourceCount);

      CD3DX12_HEAP_PROPERTIES propertiesUpload(D3D12_HEAP_TYPE_UPLOAD);

      auto descUpload = CD3DX12_RESOURCE_DESC::Buffer(uUploadBufferSize);

      pcontext->_construct_new(m_pd3d12resourceUpload);

      pdevice->m_pd3d12device->CreateCommittedResource(&propertiesUpload, D3D12_HEAP_FLAG_NONE, &descUpload,
                                                  D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                  __interface_of(m_pd3d12resourceUpload->m_presource));



   }


   void texture::static_upload_buffer::update_with_texture_data(::gpu::command_buffer *pgpucommandbuffer,
                                                                const ::gpu::texture_data &texturedata)
   {


            // 3. Prepare subresources
      D3D12_SUBRESOURCE_DATA subresources[6];

      if (texturedata.is_pixmap_array())
      {
         if (m_ptexture->m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
         {
            for (int i = 0; i < 6; ++i)
            {
               auto ppixmap = texturedata.pixmapa()[i];
               subresources[i].pData = ppixmap->data(); // Your CPU data pointer
               subresources[i].RowPitch = ppixmap->m_iScan; // 512 * 4
               subresources[i].SlicePitch = m_ptexture->m_resourcedesc.Width * m_ptexture->m_resourcedesc.Height * 4;
               // subresources[i].SlicePitch = 0;
            }
         }
         else if (texturedata.pixmapa().size() == 1)
         {
            auto ppixmap = texturedata.pixmapa().first();
            subresources[0].pData = ppixmap->data(); // pointer to your bitmap data (RGBA8, etc.)
            subresources[0].RowPitch = ppixmap->m_iScan;
            subresources[0].SlicePitch = subresources[0].RowPitch * ppixmap->height();
         }
      }
      else
      {

         subresources[0].pData = texturedata.raw_scoped_pixmap().m_pimage32Raw;
         subresources[0].RowPitch = m_ptexture->m_resourcedesc.Width * m_ptexture->m_textureattributes.m_iBitsPerChannel
            * m_ptexture->m_textureattributes.m_iChannelCount/8;
         subresources[0].SlicePitch = subresources[0].RowPitch * m_ptexture->m_resourcedesc.Height;

      }

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //::cast<command_buffer> pcommandbuffer = m_pgpurenderer->getLoadAssetsCommandBuffer();

      //if (!pcommandbuffer)
      //{

      //   pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());
      //}

      auto iCount = m_ptexture->m_textureattributes.m_iLayerCount;

      UpdateSubresources(pcommandbuffer->m_pcommandlist, m_ptexture->m_pd3d12resourceTexture->m_presource, m_pd3d12resourceUpload->m_presource, 0, 0, iCount, subresources);

      comptr<IUnknown> punknownResourceUpdate(m_pd3d12resourceUpload->m_presource);

      pcommandbuffer->m_comptraHold.add(punknownResourceUpdate);


      //// 2. Create an intermediate UPLOAD buffer big enough for this region
      // const UINT64 uploadBufferSize = GetRequiredIntermediateSize(presource, 0, 1);

      // CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
      // CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

      // HRESULT hrCreateCommittedResource = pdevice->m_pd3d12device->CreateCommittedResource(
      //    &heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
      //    IID_PPV_ARGS(&m_presourceUpload));

      //::defer_throw_hresult(hrCreateCommittedResource);

      // m_descTexture = presource->GetDesc();

      // pdevice->m_pd3d12device->GetCopyableFootprints(&m_descTexture, // texture description
      //                                           0, // first subresource
      //                                           1, // num subresources
      //                                           0, // base offset
      //                                           &m_footprint, // out: layout for subresource
      //                                           &m_uNumRows, // out: number of rows
      //                                           &m_uRowSizeInBytes, // out: bytes per row (unpadded)
      //                                           &m_uUploadBufferSize); // out: required buffer size

      //::cast<::gpu_directx12::device> pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      //// ------------------------------------------------------------
      //// Create upload buffer
      //// ------------------------------------------------------------
      //UINT64 uploadSize = 0;
      //device->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadSize);

      //device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
      //                                &CD3DX12_RESOURCE_DESC::Buffer(uploadSize), D3D12_RESOURCE_STATE_GENERIC_READ,
      //                                nullptr, IID_PPV_ARGS(&out.upload));

      //// ------------------------------------------------------------
      //// Upload texture data
      //// ------------------------------------------------------------
      //D3D12_SUBRESOURCE_DATA sub = {};
      //sub.pData = src;
      //sub.RowPitch = width * 4 * sizeof(float);
      //sub.SlicePitch = sub.RowPitch * height;

      //UpdateSubresources(cmd, out.resource.Get(), out.upload.Get(), 0, 0, 1, &sub);

      //// ------------------------------------------------------------
      //// Transition to SRV
      //// ------------------------------------------------------------

  //     auto resourcebarrier = CD3DX12_RESOURCE_BARRIER::Transition(
    //     m_ptexture->m_presourceTexture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);


      //m_ptexture->set_state(pcommandbuffer, ::gpu::e_texture_state_shader_read);

//      pcommandbuffer->m_pcommandlist->ResourceBarrier(1, &resourcebarrier);


   }




   texture::upload_buffer* texture::_get_upload_buffer()
   {

      ::cast < renderer > prenderer = m_pgpucontext->m_pgpurenderer;

      ::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      auto pgpuwindowattachment = ::gpu::window_attachment::get(prenderer);

      auto pframestorage = pgpuwindowattachment->current_frame_storage();

      auto& pobject = pframestorage->m_mapObject[this][::gpu::e_resource_upload_buffer];

      if (!pobject)
      {

         auto puploadbuffer = allocateø upload_buffer();

         puploadbuffer->initialize_upload_buffer(this);

         pobject = puploadbuffer;

      }

      ::cast < texture::upload_buffer > puploadbuffer = pobject;

      return puploadbuffer;

   }


   texture::static_upload_buffer *texture::_get_static_upload_buffer(int iResourceCount)
   {

      ::cast<renderer> prenderer = m_pgpucontext->m_pgpurenderer;

      construct_newø(m_pstaticuploadbuffer);

      m_pstaticuploadbuffer->m_iResourceCount = iResourceCount;

      m_pstaticuploadbuffer->initialize_static_upload_buffer(this);

      return m_pstaticuploadbuffer;

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


         ::cast < renderer > prenderer = m_ptexture->m_pgpucontext->m_pgpurenderer;

         ::cast < ::gpu_directx12::context > pcontext = prenderer->m_pgpucontext;

         auto pgpucommandbuffer = pcontext->beginSingleTimeCommands(
            pcontext->m_pgpudevice->graphics_queue(),
            ::gpu::e_command_buffer_graphics);

         ::cast < command_buffer > pcommandbuffer = pgpucommandbuffer;

         {

            resource_state_guard guard(
               pcommandbuffer->m_pcommandlist,
               m_ptexture->m_pd3d12resourceTexture,
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


   void texture::upload_buffer::update_pixels(const ::i32_rectangle& rectangle, const void* data)
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
      damageNew.m_copylocationTarget.pResource = m_ptexture->m_pd3d12resourceTexture->m_presource;
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


   void texture::set_pixels(const ::i32_rectangle& rectangle, const void* data)
   {

      //::cast < renderer > prenderer = m_pgpurenderer;

      //::cast < command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2(::gpu::current_layer());

      //::cast < ::gpu_directx12::device > pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      auto puploadbuffer = _get_upload_buffer();

      //D3D12_RESOURCE_DESC texDesc = m_presourceTexture->GetDesc();


      puploadbuffer->update_pixels(rectangle, data);

      //puploadbuffer->map();


   }


   void texture::read_pixels(::gpu::command_buffer * pgpucommandbuffer, ::pixmap_t * ppixmap, const ::i32_point & pointOutput)
   {

      (void)pgpucommandbuffer;

      if (!ppixmap || ppixmap->size() != size() ||
          ppixmap->m_iScan < width() * (int)sizeof(::image32_t) ||
          !ppixmap->m_pimage32 || !m_pd3d12resourceTexture ||
          !m_pd3d12resourceTexture->m_presource || !m_pgpucontext ||
          pointOutput.x < 0 || pointOutput.y < 0)
      {

         throw ::exception(error_bad_argument);

      }

      if (!m_pgpucontext->m_pgpudevice)
      {

         throw ::exception(error_wrong_state);

      }

      auto psource = m_pd3d12resourceTexture->m_presource;

      auto desc = psource->GetDesc();

      if (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D ||
          desc.SampleDesc.Count != 1 ||
          (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM &&
           desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB &&
           desc.Format != DXGI_FORMAT_B8G8R8A8_TYPELESS))
      {

         throw ::exception(error_failed, "Unsupported DirectX 12 texture format for pixel readback.");

      }

      auto iMip = maximum(0, m_iCurrentMip);

      auto iLayer = maximum(0, m_iCurrentLayer);

      if (iMip >= desc.MipLevels || iLayer >= desc.DepthOrArraySize)
      {

         throw ::exception(error_bad_argument);

      }

      auto uMipWidth = desc.Width >> iMip;

      auto uMipHeight = ((UINT64)desc.Height) >> iMip;

      if (uMipWidth == 0)
      {

         uMipWidth = 1;

      }

      if (uMipHeight == 0)
      {

         uMipHeight = 1;

      }

      auto uWidth = (UINT64)ppixmap->size().width();

      auto uHeight = (UINT64)ppixmap->size().height();

      if (uWidth == 0 || uHeight == 0 ||
          (UINT64)pointOutput.x + uWidth > uMipWidth ||
          (UINT64)pointOutput.y + uHeight > uMipHeight)
      {

         throw ::exception(error_bad_argument);

      }

      ::cast < ::gpu_directx12::context > pcontext = m_pgpucontext;

      ::cast < ::gpu_directx12::device > pdevice = m_pgpucontext->m_pgpudevice;

      auto uSubresource = (UINT)iMip + (UINT)iLayer * (UINT)desc.MipLevels;

      D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};

      UINT64 uReadbackSize = 0;

      pdevice->m_pd3d12device->GetCopyableFootprints(
         &desc,
         uSubresource,
         1,
         0,
         &footprint,
         nullptr,
         nullptr,
         &uReadbackSize);

      auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_READBACK);

      auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uReadbackSize);

      ::comptr < ID3D12Resource > preadback;

      auto hrCreateReadback = pdevice->m_pd3d12device->CreateCommittedResource(
         &heapProperties,
         D3D12_HEAP_FLAG_NONE,
         &bufferDesc,
         D3D12_RESOURCE_STATE_COPY_DEST,
         nullptr,
         __interface_of(preadback));

      pdevice->defer_throw_hresult(hrCreateReadback);

      auto pgpucommandbufferReadback = pcontext->beginSingleTimeCommands(
         pcontext->m_pgpudevice->graphics_queue());

      ::cast < ::gpu_directx12::command_buffer > pcommandbufferReadback =
         pgpucommandbufferReadback;

      auto pd3d12commandlist = pcommandbufferReadback->m_pcommandlist;

      auto locationTarget = CD3DX12_TEXTURE_COPY_LOCATION(preadback, footprint);

      auto locationSource = CD3DX12_TEXTURE_COPY_LOCATION(psource, uSubresource);

      auto boxSource = CD3DX12_BOX(
         pointOutput.x,
         pointOutput.y,
         0,
         (LONG)((UINT64)pointOutput.x + uWidth),
         (LONG)((UINT64)pointOutput.y + uHeight),
         1);

      {

         resource_state_guard state(
            pd3d12commandlist,
            m_pd3d12resourceTexture,
            D3D12_RESOURCE_STATE_COPY_SOURCE);

         pd3d12commandlist->CopyTextureRegion(
            &locationTarget,
            0,
            0,
            0,
            &locationSource,
            &boxSource);

      }

      pcontext->endSingleTimeCommands(pcommandbufferReadback);

      D3D12_RANGE rangeRead{
         (SIZE_T)footprint.Offset,
         (SIZE_T)uReadbackSize
      };

      void * pMapped = nullptr;

      auto hrMap = preadback->Map(0, &rangeRead, &pMapped);

      pdevice->defer_throw_hresult(hrMap);

      try
      {

         auto pimage32Source = (const ::image32_t *)
            ((const unsigned char *)pMapped + footprint.Offset);

         ppixmap->copy(
            ppixmap->size(),
            pimage32Source,
            (int)footprint.Footprint.RowPitch);

      }
      catch (...)
      {

         D3D12_RANGE rangeWritten{0, 0};

         preadback->Unmap(0, &rangeWritten);

         throw;

      }

      D3D12_RANGE rangeWritten{0, 0};

      preadback->Unmap(0, &rangeWritten);

   }



   void texture::set_state(::gpu::command_buffer *pgpucommandbuffer, ::gpu::enum_texture_state etexturestate)
   {

      ::gpu::texture::set_state(pgpucommandbuffer, etexturestate);

      ::cast<::gpu_directx12::command_buffer> pcommandbuffer = pgpucommandbuffer;

      if (etexturestate == ::gpu::e_texture_state_shader_read)
      {
         m_pd3d12resourceTexture->_set_state(pcommandbuffer, {D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE});
      }
      else if (etexturestate == ::gpu::e_texture_state_color_attachment)
      {
         m_pd3d12resourceTexture->_set_state(pcommandbuffer, {D3D12_RESOURCE_STATE_RENDER_TARGET});
      }
      else if (etexturestate == ::gpu::e_texture_state_copy_target)
      {
         m_pd3d12resourceTexture->_set_state(pcommandbuffer, {D3D12_RESOURCE_STATE_COPY_DEST});
      }
      else if (etexturestate == ::gpu::e_texture_state_copy_source)
      {
         m_pd3d12resourceTexture->_set_state(pcommandbuffer, {D3D12_RESOURCE_STATE_COPY_SOURCE});
      }
      else if (etexturestate == ::gpu::e_texture_state_present)
      {
         m_pd3d12resourceTexture->_set_state(pcommandbuffer, {D3D12_RESOURCE_STATE_PRESENT});

      }
   }

   void texture::generate_mipmap(::gpu::command_buffer * pgpucommandbuffer)
   {

      if (!m_pmipmapgenerator)
      {
         construct_newø(m_pmipmapgenerator);

         m_pmipmapgenerator->initialize_mip_map_generator(this);
      }
      m_pmipmapgenerator->generate(pgpucommandbuffer);
   }

   texture::mip_map_generator::mip_map_generator()
   {


   }
   texture::mip_map_generator::~mip_map_generator() {}


   void texture::mip_map_generator::initialize_mip_map_generator(::gpu_directx12::texture *ptexture)
   {

      m_ptexture = ptexture;

      CD3DX12_DESCRIPTOR_RANGE ranges[2];
      ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0
      ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0); // u0

      CD3DX12_ROOT_PARAMETER rootParams[2];
      rootParams[0].InitAsDescriptorTable(1, &ranges[0]);
      rootParams[1].InitAsDescriptorTable(1, &ranges[1]);

      CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
      rootSigDesc.Init(_countof(rootParams), rootParams, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_NONE);

      ::comptr<ID3DBlob> sigBlob, errorBlob;
      D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &sigBlob, &errorBlob);

      ::cast<::gpu_directx12::device> pdevice = m_ptexture->m_pgpucontext->m_pgpudevice;
      auto hrCreateRootSignature =pdevice->m_pd3d12device->CreateRootSignature(
         0, sigBlob->GetBufferPointer(), sigBlob->GetBufferSize(),
                                              __interface_of(m_prootsignatureMipMap));
      pdevice->defer_throw_hresult(hrCreateRootSignature);
      const char compute_shader[] = R"comp(
      Texture2D<float4> SrcMip : register(t0);
      RWTexture2D<float4> DstMip : register(u0);

      [numthreads(8, 8, 1)] void main(uint3 id : SV_DispatchThreadID)
      {
         uint2 dst = id.xy;
         uint2 src = dst * 2;

         float4 sum = SrcMip.Load(int3(src, 0)) + SrcMip.Load(int3(src + uint2(1, 0), 0)) +
                      SrcMip.Load(int3(src + uint2(0, 1), 0)) + SrcMip.Load(int3(src + uint2(1, 1), 0));

         DstMip[dst] = sum * 0.25f;
      }
)comp";

      auto blockComputeShader = ::as_memory_block(compute_shader);

      auto pblob = create_computer_shader_blob(blockComputeShader);

      D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc{};
      psoDesc.pRootSignature = m_prootsignatureMipMap;
      psoDesc.CS = CD3DX12_SHADER_BYTECODE(pblob);

      auto hrCreateComputePipelineState =
         pdevice->m_pd3d12device->CreateComputePipelineState(&psoDesc, __interface_of(m_ppipelinestateMipMap));
      pdevice->defer_throw_hresult(hrCreateComputePipelineState);

      m_iLayerCount = m_ptexture->m_textureattributes.m_iLayerCount;

      m_iMipCount = m_ptexture->mip_count();

      UINT srvCount = m_iMipCount * m_iLayerCount;
      UINT uavCount = m_iMipCount * m_iLayerCount;

      UINT totalDescriptors = srvCount + uavCount;
      D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
      heapDesc.NumDescriptors = totalDescriptors;
      heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
      heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

      
      auto hrCreateDescriptorHeap =
         pdevice->m_pd3d12device->CreateDescriptorHeap(&heapDesc, __interface_of(m_pheapMipMap));
      pdevice->defer_throw_hresult(hrCreateDescriptorHeap);

      UINT descriptorSize =
         pdevice->m_pd3d12device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

      m_cpuBase = m_pheapMipMap->GetCPUDescriptorHandleForHeapStart();
      m_gpuBase = m_pheapMipMap->GetGPUDescriptorHandleForHeapStart();

      for (UINT mip = 0; mip < m_iMipCount; mip++)
      {
         for (UINT face = 0; face < m_iLayerCount; face++)
         {
            UINT i = idx(mip, face);

            D3D12_SHADER_RESOURCE_VIEW_DESC srv{};
            srv.Format = m_ptexture->m_resourcedesc.Format;
            srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

            srv.Texture2DArray.MostDetailedMip = mip;
            srv.Texture2DArray.MipLevels = 1;
            srv.Texture2DArray.FirstArraySlice = face;
            srv.Texture2DArray.ArraySize = 1;
            srv.Texture2DArray.PlaneSlice = 0;

            auto cpu = m_cpuBase;
            cpu.ptr += i * descriptorSize;

            pdevice->m_pd3d12device->CreateShaderResourceView(m_ptexture->m_pd3d12resourceTexture->m_presource, &srv, cpu);

            m_handleaShaderResourceView.atø(i) = m_gpuBase;
            m_handleaShaderResourceView.atø(i).ptr += i * descriptorSize;
         }
      }

      UINT uavBase = srvCount;

      for (UINT mip = 0; mip < m_iMipCount; mip++)
      {
         for (UINT face = 0; face < m_iLayerCount; face++)
         {
            UINT i = idx(mip, face);

            D3D12_UNORDERED_ACCESS_VIEW_DESC uav{};
            uav.Format = m_ptexture->m_resourcedesc.Format;
            uav.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
            uav.Texture2DArray.MipSlice = mip;
            uav.Texture2DArray.FirstArraySlice = face;
            uav.Texture2DArray.ArraySize = 1;
            uav.Texture2DArray.PlaneSlice = 0;

            auto cpu = m_cpuBase;
            cpu.ptr += (uavBase + i) * descriptorSize;

            pdevice->m_pd3d12device->CreateUnorderedAccessView(m_ptexture->m_pd3d12resourceTexture->m_presource, nullptr, &uav, cpu);

            m_handleaUnorderedAccessView.atø(i) = m_gpuBase;
            m_handleaUnorderedAccessView.atø(i).ptr += (uavBase + i) * descriptorSize;
         }
      }

   }

   void texture::mip_map_generator::generate(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<::gpu_directx12::command_buffer> pcommandbuffer = pgpucommandbuffer;

      auto cmd = pcommandbuffer->m_pcommandlist.m_p;
      ID3D12DescriptorHeap *heaps[] = {m_pheapMipMap};
      cmd->SetDescriptorHeaps(1, heaps);
      m_ptexture->m_pd3d12resourceTexture->_set_state(pcommandbuffer, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

      int w = m_ptexture->m_textureattributes.m_size.width();
      int h = m_ptexture->m_textureattributes.m_size.height();
      for (UINT mip = 1; mip < m_iMipCount; mip++)
      {
         for (UINT face = 0; face < m_iLayerCount; face++)
         {
            UINT srcIndex = idx(mip - 1, face);
            UINT dstIndex = idx(mip, face);

            //auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
              // m_ptexture->m_pd3d12resourceTexture->m_presource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
               //D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
               
//            cmd->ResourceBarrier(1, &barrier);

            cmd->SetPipelineState(m_ppipelinestateMipMap);
            cmd->SetComputeRootSignature(m_prootsignatureMipMap);

            cmd->SetComputeRootDescriptorTable(0, m_handleaShaderResourceView[srcIndex]);
            cmd->SetComputeRootDescriptorTable(1, m_handleaUnorderedAccessView[dstIndex]);

            UINT w1 = ::maximum(1u, w >> mip);
            UINT h1 = ::maximum(1u, h >> mip);

            cmd->Dispatch(w1, h1, 1);
         }
      }

   }

} // namespace gpu_directx12




