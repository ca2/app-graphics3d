// Created by camilo on 2026-01-10 01:15 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include <DirectXMath.h>
#include "acme/platform/application.h"
#include "acme_windows_common/dxgi_surface_bindable.h"
#include "approach.h"
#include "command_buffer.h"
#include "aura/graphics/image/image.h"
#include "aura/user/user/interaction.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/graphics.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/types.h"
#include "bred/graphics3d/types.h"
#include "context.h"
#include "device.h"
#include "gpu_directx12/descriptors.h"
#include "initializers.h"
#include "memory_buffer.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
//#include "swap_chain_render_target_view.h"
#include "texture.h"

#include <glad/glad.h>
#include "block.h"
// #include <algorithm>
#include <assert.h>
#include <d3d12.h>
#include <ktx.h>
// #include <vector>
#include <vulkan/vulkan.h> // for VkFormat enum only


//using namespace directx12;


namespace gpu_directx12
{


   //struct Dx12Texture
   //{
   //   ::comptr < ID3D12Resource > m_presourceTexture;
   //   ::comptr<ID3D12Resource> m_presourceTextureUpload; // keep alive until upload completes
   //   //::comptr < ID3D12Resource > m_presourceTextureUpload = nullptr; // keep alive until upload completes
   //   //D3D12_CPU_DESCRIPTOR_HANDLE srv = {};
   //};

    static DXGI_FORMAT VkFormatToDxgi(VkFormat vk)
   {
      switch (vk)
      {
         case VK_FORMAT_R8G8B8A8_UNORM:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
         case VK_FORMAT_R8G8B8A8_SRGB:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

         case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
            return DXGI_FORMAT_BC1_UNORM;
         case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
            return DXGI_FORMAT_BC1_UNORM_SRGB;
         case VK_FORMAT_BC3_UNORM_BLOCK:
            return DXGI_FORMAT_BC3_UNORM;
         case VK_FORMAT_BC3_SRGB_BLOCK:
            return DXGI_FORMAT_BC3_UNORM_SRGB;
         case VK_FORMAT_BC7_UNORM_BLOCK:
            return DXGI_FORMAT_BC7_UNORM;
         case VK_FORMAT_BC7_SRGB_BLOCK:
            return DXGI_FORMAT_BC7_UNORM_SRGB;

         default:
            assert(false && "Unsupported KTX/Vk format");
            return DXGI_FORMAT_UNKNOWN;
      }
   }
#define GL_COMPRESSED_RGBA_BPTC_UNORM 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_R8 33321
   DXGI_FORMAT GlInternalFormatToDxgi(GLenum internalFormat, GLenum format, GLenum type)
   {
      switch (internalFormat)
      {
         case GL_R8:
            return DXGI_FORMAT_R8_UNORM;
         case GL_RGBA8:
            return DXGI_FORMAT_R8G8B8A8_UNORM;
         case GL_RGBA16F:
            return DXGI_FORMAT_R16G16B16A16_FLOAT;
         case GL_SRGB8_ALPHA8:
            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

         case GL_COMPRESSED_RGBA_BPTC_UNORM:
            return DXGI_FORMAT_BC7_UNORM;

         case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:
            return DXGI_FORMAT_BC7_UNORM_SRGB;

         default:
            assert(false && "Unsupported KTX1 GL format");
            return DXGI_FORMAT_UNKNOWN;
      }
   }

   //void CreateDx12TextureFromKtx(::gpu_directx12::texture * ptexture, ID3D12Device *device, ID3D12GraphicsCommandList *cmd, const ktxTexture *ktxTex, D3D12_RESOURCE_STATES & states)
    void CreateDx12TextureFromKtx(::gpu_directx12::texture * ptexture, ID3D12Device *device, ID3D12GraphicsCommandList *cmd, const ktxTexture *ktxTex)
      //,
      /*     ID3D12DescriptorHeap *srvHeap, UINT srvIndex)*/
   {
      assert(device);
      assert(cmd);
      assert(ktxTex);
      assert(ktxTex->pData);

      //Dx12Texture out = {};

      const bool isKtx2 = ktxTex->classId == ktxTexture2_c;
      ktxTexture1 *ktxTex1 = nullptr;
      ktxTexture2 *ktxTex2 = nullptr;

      if (isKtx2)
         ktxTex2 = (ktxTexture2 *)ktxTex;
      else
         ktxTex1 = (ktxTexture1 *)ktxTex;

      const bool isCubemap = ktxTex->isCubemap;
      const uint32_t faceCount = isCubemap ? 6 : 1;

      DXGI_FORMAT format;
      if (ktxTex2)
         format = VkFormatToDxgi((VkFormat)ktxTex2->vkFormat);
      else
         format = GlInternalFormatToDxgi(ktxTex1->glInternalformat, ktxTex1->glFormat, ktxTex1->glType);

      const uint32_t mipCount = ktxTex->numLevels;
      const uint32_t layerCount = ktxTex->numLayers * faceCount;

      // ---------------------------------------------------------------------
      // Create default heap texture
      // ---------------------------------------------------------------------
      D3D12_RESOURCE_DESC desc = {};
      desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
      desc.Width = ktxTex->baseWidth;
      desc.Height = ktxTex->baseHeight;
      desc.DepthOrArraySize = (UINT16)layerCount;
      desc.MipLevels = (UINT16)mipCount;
      desc.Format = format;
      desc.SampleDesc.Count = 1;
      desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
      desc.Flags = D3D12_RESOURCE_FLAG_NONE;
      CD3DX12_HEAP_PROPERTIES props(D3D12_HEAP_TYPE_DEFAULT);
      HRESULT hr =
         device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
                                         D3D12_RESOURCE_STATE_COPY_DEST, nullptr, __interface_of(ptexture->m_pd3d12resourceTexture->m_presource));
      assert(SUCCEEDED(hr));
      ptexture->m_pd3d12resourceTexture->m_state.m_resourcestates = D3D12_RESOURCE_STATE_COPY_DEST;
      // ---------------------------------------------------------------------
      // Prepare subresource data
      // ---------------------------------------------------------------------
      const UINT subresourceCount = mipCount * layerCount;
      ::array_base<D3D12_SUBRESOURCE_DATA> subresources;
      
      subresources.set_size(subresourceCount);

      for (uint32_t layer = 0; layer < ktxTex->numLayers; ++layer)
      {
         for (uint32_t face = 0; face < faceCount; ++face)
         {
            for (uint32_t mip = 0; mip < mipCount; ++mip)
            {
               ktx_size_t offset = 0;
               KTX_error_code err = ktxTexture_GetImageOffset((ktxTexture *)ktxTex, mip, layer, face, &offset);
               assert(err == KTX_SUCCESS);

               uint32_t arraySlice = layer * faceCount + face;
               uint32_t sub = mip + arraySlice * mipCount;

               subresources[sub].pData = (const uint8_t *)ktxTex->pData + offset;

               subresources[sub].RowPitch = ktxTexture_GetRowPitch((ktxTexture *)ktxTex, mip);

               subresources[sub].SlicePitch = 0;
            }
         }
      }


      auto pstaticuploadbuffer = ptexture->_get_static_upload_buffer(subresourceCount);

      //// ---------------------------------------------------------------------
      //// Create upload heap
      //// ---------------------------------------------------------------------
      //UINT64 uploadSize = GetRequiredIntermediateSize(out.m_presourceTexture, 0, subresourceCount);

      //auto props2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

      //auto desc2 = CD3DX12_RESOURCE_DESC::Buffer(uploadSize);
      //hr = device->CreateCommittedResource(&props2, D3D12_HEAP_FLAG_NONE,
      //                                     &desc2,
      //                                     D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __interface_of(out.m_presourceTextureUpload));
      //assert(SUCCEEDED(hr));

      // ---------------------------------------------------------------------
      // Upload
      // ---------------------------------------------------------------------
      UpdateSubresources(cmd, ptexture->m_pd3d12resourceTexture->m_presource, pstaticuploadbuffer->m_pd3d12resourceUpload->m_presource, 0, 0,
                         subresourceCount, subresources.data());

      //auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(out.texture, D3D12_RESOURCE_STATE_COPY_DEST,
      //
     //                                                  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
       //auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(ptexture->m_pd3d12resourceTexture->m_presource, D3D12_RESOURCE_STATE_COPY_DEST,
      //
         //                                               D3D12_RESOURCE_STATE_COMMON);
      //ptexture->m_pd3d12resourceTexture->_new_state(cmd, D3D12_RESOURCE_STATE_COMMON);
      //cmd->ResourceBarrier(1, &barrier);

      //// ---------------------------------------------------------------------
      //// Create SRV
      //// ---------------------------------------------------------------------
      //D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      //srvDesc.Format = format;
      //srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

      //if (isCubemap)
      //{
      //   srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
      //   srvDesc.TextureCube.MipLevels = mipCount;
      //}
      //else if (layerCount > 1)
      //{
      //   srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
      //   srvDesc.Texture2DArray.MipLevels = mipCount;
      //   srvDesc.Texture2DArray.ArraySize = layerCount;
      //}
      //else
      //{
      //   srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
      //   srvDesc.Texture2D.MipLevels = mipCount;
      //}

      //UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

      //out.srv = srvHeap->GetCPUDescriptorHandleForHeapStart();
      //out.srv.ptr += srvIndex * descriptorSize;

      //device->CreateShaderResourceView(out.texture, &srvDesc, out.srv);

      //return out;
   }


      void context::load_ktxTexture(::gpu::texture *pgputexture, void *p_ktxTexture)
   {

      auto pktxtexture = (ktxTexture *)p_ktxTexture;

      ::cast<::gpu_directx12::texture> ptexture = pgputexture;

      ::cast<device> pdevice = m_pgpudevice;
      ::cast<::gpu_directx12::command_buffer> pcommandbufferLoading;

      ::cast<::gpu_directx12::renderer> prenderer = m_pgpurenderer;

      if (::is_set(prenderer))
      {

         pcommandbufferLoading = prenderer->getLoadAssetsCommandBuffer();
      }
      if (!pcommandbufferLoading)
      {

         pcommandbufferLoading = m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());
      }
      _construct_new(ptexture->m_pd3d12resourceTexture);
      CreateDx12TextureFromKtx(ptexture,
         pdevice->m_pd3d12device, pcommandbufferLoading->m_pcommandlist, pktxtexture);

      //ptexture->m_pd3d12resourceTexture->m_presource = tex.m_presourceTexture;
      //ptexture->m_pd3d12resourceTextureUpload = tex.m_presourceTextureUpload;
      ptexture->m_resourcedesc = ptexture->m_pd3d12resourceTexture->m_presource->GetDesc();
      //ptexture->set_c = tex.srv;
      ptexture->m_textureattributes.m_etexture = ::gpu::e_texture_image;
      ptexture->set_ok_flag();
   }


         void context::load_ktxTexture_cube_map(::gpu::texture *pgputexture, void *p_ktxTexture)
   {
      auto pktxtexture = (ktxTexture *)p_ktxTexture;

       ::cast<::gpu_directx12::texture> ptexture = pgputexture;

      ::cast<device> pdevice = m_pgpudevice;
      ::cast<::gpu_directx12::command_buffer> pcommandbufferLoading;

      ::cast<::gpu_directx12::renderer> prenderer = m_pgpurenderer;

      if (::is_set(prenderer))
      {

         pcommandbufferLoading = prenderer->getLoadAssetsCommandBuffer();
      }
      if (!pcommandbufferLoading)
      {

         pcommandbufferLoading = m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());
      }

            _construct_new(ptexture->m_pd3d12resourceTexture);

      CreateDx12TextureFromKtx(ptexture, pdevice->m_pd3d12device, pcommandbufferLoading->m_pcommandlist, pktxtexture
                                                );

            //_construct_new(ptexture->m_pd3d12resourceTexture);
      //ptexture->m_pd3d12resourceTexture->m_presource = tex.texture;
            ptexture->m_resourcedesc = ptexture->m_pd3d12resourceTexture->m_presource->GetDesc();
      //ptexture->m_pshaderresourceview = tex.srv;
      ptexture->m_textureattributes.m_etexture = ::gpu::e_texture_cube_map;
      ptexture->set_ok_flag();
   }


} // namespace gpu_directx12
 


