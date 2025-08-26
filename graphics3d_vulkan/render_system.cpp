// Created by camilo on 2025-04-24 21:26 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "render_system.h"
#include "app-graphics3d/gpu_vulkan/command_buffer.h"
#include "app-graphics3d/gpu_vulkan/context.h"
#include "app-graphics3d/gpu_vulkan/frame.h"
#include "app-graphics3d/gpu_vulkan/renderer.h"
#include "app-graphics3d/gpu_vulkan/render_target.h"
#include "app-graphics3d/gpu_vulkan/texture.h"
#include "app-graphics3d/gpu_vulkan/vk_init.h"
#include "bred/graphics3d/engine.h"


namespace graphics3d_vulkan
{


   render_system::render_system() {}


   render_system ::~render_system() {}


   ::gpu_vulkan::render_pass *render_system::render_pass2()
   {

      if (!m_pgpurenderpass)
      {

               øconstruct(m_pgpurenderpass);

               m_pgpurenderpass->initialize_gpu_context_object(m_pengine->gpu_context());

         m_pgpurenderpass->m_bLoadClearOp = false;
               m_pgpurenderpass->m_bWithDepth = true;

         m_pgpurenderpass->createRenderPass();

      }

      return m_pgpurenderpass;

   }

   
   
   void render_system::render(::gpu::context *pgpucontext, ::graphics3d::scene *pscene)
   {
      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();
      ::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;
      auto pframe = ::gpu::current_frame();

          // Render
      VkClearValue clearValues[1];

      clearValues[0].color = {{0.0f, 0.0f, 0.0f, 0.0f}};

      auto prenderpass = prenderer->render_pass2();

      auto prendertarget = prenderer->m_pgpurendertarget;

      auto vkrenderpass = prenderpass->m_vkrenderpass;

      ::cast<::gpu_vulkan::texture> ptexture = prendertarget->current_texture(::gpu::current_frame());

      if (ptexture->m_state.m_vkimagelayout == VK_IMAGE_LAYOUT_UNDEFINED)
      {

         warning() << "what?";

      }

      VkRenderPassBeginInfo renderPassBeginInfo = vkinit::renderPassBeginInfo();
      renderPassBeginInfo.renderPass = vkrenderpass;
      renderPassBeginInfo.renderArea.extent.width = pgpucontext->m_rectangle.width();
      renderPassBeginInfo.renderArea.extent.height = pgpucontext->m_rectangle.height();
      renderPassBeginInfo.clearValueCount = 1;
      renderPassBeginInfo.pClearValues = clearValues;
      renderPassBeginInfo.framebuffer = ptexture->framebuffer(prenderpass);

      ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

      vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
      on_render(pgpucontext, pscene);
      vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
   }



} // namespace graphics3d_vulkan
