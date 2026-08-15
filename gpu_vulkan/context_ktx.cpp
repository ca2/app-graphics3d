// Created by camilo on 2025-12-18 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/application.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"
#include "approach.h"
#include "aura/graphics/image/image.h"
#include "binding.h"
#include "block.h"
#include "bred/gpu/compositor.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/pixmap.h"
#include "bred/gpu/types.h"
#include "bred/graphics3d/engine.h"
#include "buffer.h"
#include "command_buffer.h"
#include "context.h"
#include "debug.h"
#include "device.h"
#include "gltf/model.h"
#include "memory_buffer.h"
#include "physical_device.h"
#include "program.h"
#include "render_target.h"
#include "renderer.h"
#include "shader.h"
#include "swap_chain.h"
#include "texture.h"
#include <glad/glad.h>
#include <vulkan/vulkan.h>

//
//   // Optional — depends on your conventions
#include <chrono>
//
// #include "gltf_model.h"
#include "initializers.h"
#include "vk_init.h"

#include "pipeline.h"
#include "queue.h"
#include <ktx.h>
//#include <ktx_vulkan.h>
// #include "gpu_vulkan/ibl/cubemap_framebuffer.h"

using namespace vulkan;


CLASS_DECL_GPU_VULKAN bool is_ktx2(ktxTexture *pktxtexture);
CLASS_DECL_GPU_VULKAN VkFormat get_ktx_vk_format(ktxTexture *pktxtexture, int &iChannelCount, int &iBitsPerChannel, int & iSrgb, int & iFloat);


namespace gpu_vulkan
{


   void context::load_ktxTexture(::gpu::texture *pgputexture, void *p_ktxTexture)
   {

      ::cast<::gpu_vulkan::queue> pqueueGraphics = m_pgpudevice->graphics_queue();

      ::cast<::gpu_vulkan::texture> ptexture = pgputexture;

      ptexture->KTXLoadFrom_ktxTexture(p_ktxTexture, pqueueGraphics->m_vkqueue,
                                       VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                                       VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, false);
   }

} //  namespace gpu_vulkan


// Returns VK_FORMAT_UNDEFINED if unsupported
VkFormat ktxGLInternalFormatToVkFormat(int internalFormat, int &iChannelCount, int &iBitsPerChannel, int &iSrgb, int &iFloat)
{
   switch (internalFormat)
   {
      // 8-bit normalized
      case GL_R8:
      case GL_RED:
         iChannelCount = 1;
         iBitsPerChannel = 8;
         iSrgb = false;
         iFloat = false;
         return VK_FORMAT_R8_UNORM;
      case GL_RG8:
      case GL_RG:
         iChannelCount = 2;
         iBitsPerChannel = 8;
         iSrgb = false;
         iFloat = false;
         return VK_FORMAT_R8G8_UNORM;
      case GL_RGB8:
      case GL_RGB:
         iChannelCount = 3;
         iBitsPerChannel = 8;
         iSrgb = false;
         iFloat = false;
         return VK_FORMAT_R8G8B8_UNORM; // ⚠ rarely supported
      case GL_RGBA8:
      case GL_RGBA:
         iChannelCount = 4;
         iBitsPerChannel = 8;
         iSrgb = false;
         iFloat = false;
         return VK_FORMAT_R8G8B8A8_UNORM;

      // sRGB
      case GL_SRGB8:
         iChannelCount = 3;
         iBitsPerChannel = 8;
         iSrgb = true;
         iFloat = false;
         return VK_FORMAT_R8G8B8_SRGB; // ⚠ rarely supported
      case GL_SRGB8_ALPHA8:
         iChannelCount = 4;
         iBitsPerChannel = 8;
         iSrgb = true;
         iFloat = false;
         return VK_FORMAT_R8G8B8A8_SRGB;

      // 16-bit
      case GL_R16:
         iChannelCount = 1;
         iBitsPerChannel = 16;
         iSrgb = false;
         iFloat = false;
         return VK_FORMAT_R16_UNORM;
      case GL_RG16:
         iChannelCount = 2;
         iBitsPerChannel = 16;
         iSrgb = false;
         iFloat = false;
         return VK_FORMAT_R16G16_UNORM;
      case GL_RGBA16:
         iChannelCount = 4;
         iBitsPerChannel = 16;
         iSrgb = false;
         iFloat = false;
         return VK_FORMAT_R16G16B16A16_UNORM;

      // Floating point
      case GL_R16F:
         iChannelCount = 1;
         iBitsPerChannel = 16;
         iSrgb = false;
         iFloat = true;
         return VK_FORMAT_R16_SFLOAT;
      case GL_RG16F:
         iChannelCount = 2;
         iBitsPerChannel = 16;
         iSrgb = false;
         iFloat = true;
         return VK_FORMAT_R16G16_SFLOAT;
      case GL_RGBA16F:
         iChannelCount = 4;
         iBitsPerChannel = 16;
         iSrgb = false;
         iFloat = true;
         return VK_FORMAT_R16G16B16A16_SFLOAT;

      case GL_R32F:
         iChannelCount = 1;
         iBitsPerChannel = 32;
         iSrgb = false;
         iFloat = true;
         return VK_FORMAT_R32_SFLOAT;
      case GL_RG32F:
         iChannelCount = 2;
         iBitsPerChannel = 32;
         iSrgb = false;
         iFloat = true;
         return VK_FORMAT_R32G32_SFLOAT;
      case GL_RGBA32F:
         iChannelCount = 4;
         iBitsPerChannel = 32;
         iSrgb = false;
         iFloat = true;
         return VK_FORMAT_R32G32B32A32_SFLOAT;

      // Depth
      case GL_DEPTH_COMPONENT16:
         return VK_FORMAT_D16_UNORM;
      case GL_DEPTH_COMPONENT24:
         return VK_FORMAT_X8_D24_UNORM_PACK32;
      case GL_DEPTH_COMPONENT32F:
         return VK_FORMAT_D32_SFLOAT;
      case GL_DEPTH24_STENCIL8:
         return VK_FORMAT_D24_UNORM_S8_UINT;
      case GL_DEPTH32F_STENCIL8:
         return VK_FORMAT_D32_SFLOAT_S8_UINT;

      // BC / S3TC compression
      case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
         return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
      case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT:
         return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
      case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
         return VK_FORMAT_BC2_UNORM_BLOCK;
      case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT:
         return VK_FORMAT_BC2_SRGB_BLOCK;
      case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
         return VK_FORMAT_BC3_UNORM_BLOCK;
      case GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT:
         return VK_FORMAT_BC3_SRGB_BLOCK;

      // ETC2
      case GL_COMPRESSED_RGB8_ETC2:
         return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
      case GL_COMPRESSED_SRGB8_ETC2:
         return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
      case GL_COMPRESSED_RGBA8_ETC2_EAC:
         return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
      case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
         return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;

      // ASTC (example 4x4)
      case GL_COMPRESSED_RGBA_ASTC_4x4_KHR:
         return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
      case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR:
         return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;

      default:
         return VK_FORMAT_UNDEFINED;
   }
}


CLASS_DECL_GPU_VULKAN bool is_ktx2(ktxTexture *pktxtexture)
{ 
   
   bool bIsKtx2 = pktxtexture->classId == ktxTexture2_c; 

   return bIsKtx2;

}



CLASS_DECL_GPU_VULKAN VkFormat get_ktx_vk_format(ktxTexture *pktxtexture, int &iChannelCount, int &iBitsPerChannel, int & iSrgb, int & iFloat)
{

   if (is_ktx2(pktxtexture))
   {

      auto pktxtexture2 = (ktxTexture2 *)pktxtexture;

      return (VkFormat) pktxtexture2->vkFormat;

   }
   else
   {

      auto pktxtexture1 = (ktxTexture1 *)pktxtexture;

      auto iInternalFormatOpenGL = pktxtexture1->glFormat;

      auto vkformat = ktxGLInternalFormatToVkFormat(iInternalFormatOpenGL, iChannelCount, iBitsPerChannel, iSrgb, iFloat);

      return vkformat;

   }

}