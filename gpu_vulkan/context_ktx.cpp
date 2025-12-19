// Created by camilo on 2025-12-18 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/application.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"
#include "approach.h"
#include "aura/graphics/image/image.h"
#include "binding.h"
#include "block.h"
#include "bred/gpu/compositor.h"
#include "bred/gpu/frame.h"
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

//
//   // Optional — depends on your conventions
#include <chrono>
//
// #include "gltf_model.h"
#include "initializers.h"
#include "vk_init.h"

#include "pipeline.h"
#include "queue.h"
// #include "gpu_vulkan/ibl/cubemap_framebuffer.h"

using namespace vulkan;


namespace gpu_vulkan
{


   void context::load_ktxTexture(::pointer<::gpu::texture> &pgputexture, void *p_ktxTexture)
   {

      ::cast<::gpu_vulkan::queue> pqueueGraphics = m_pgpudevice->graphics_queue();

      ::cast<::gpu_vulkan::texture> ptexture = pgputexture;

      ptexture->KTXLoadFrom_ktxTexture(p_ktxTexture, ptexture->m_vkformat, pqueueGraphics->m_vkqueue,
                                       VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                                       VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, false);
   }

} //  namespace gpu_vulkan