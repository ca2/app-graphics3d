// Created by camilo on 2025-04-24 21:26 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "render_system.h"
//#include "gpu_directx11/command_buffer.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/frame.h"
#include "gpu_directx11/renderer.h"
//#include "gpu_directx11/render_target.h"
#include "gpu_directx11/texture.h"
//#include "gpu_directx11/vk_init.h"
#include "bred/graphics3d/engine.h"
#include "bred/gpu/layer.h"


namespace graphics3d_directx11
{


   render_system::render_system() {}


   render_system ::~render_system() {}


   //::gpu_directx11::render_pass *render_system::render_pass2()
   //{

   // ::cast<::gpu_directx11::renderer> prenderer = m_pengine->gpu_context()->m_pgpurenderer;

   // return prenderer->render_pass2();

   //   //if (!m_pgpurenderpass)
   //   //{

   //   //         øconstruct(m_pgpurenderpass);

   //   //         m_pgpurenderpass->initialize_gpu_context_object(m_pengine->gpu_context());

   //   //   m_pgpurenderpass->m_bLoadClearOp = false;
   //   //         m_pgpurenderpass->m_flags.m_bWithDepth = true;

   //   //   m_pgpurenderpass->createRenderPass();

   //   //}

   //   //return m_pgpurenderpass;

   //}

   //
   
   void render_system::render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene)
   {
      
      ::cast<::gpu_directx11::context> pcontext = m_pengine->gpu_context();
      
      ::cast<::gpu_directx11::renderer> prenderer = pcontext->m_pgpurenderer;
      
      auto pframe = ::gpu::current_frame();

      //auto prenderpass = render_pass2();

      auto prendertarget = prenderer->render_target();

      //auto vkrenderpass = prenderpass->m_vkrenderpass;

      ::cast<::gpu_directx11::texture> ptexture = prendertarget->current_texture(::gpu::current_frame());

      //if (ptexture->m_state.m_vkimagelayout == VK_IMAGE_LAYOUT_UNDEFINED)
      //{

      //   warning() << "what?";

      //}

      //VkRenderPassBeginInfo renderPassBeginInfo = vkinit::renderPassBeginInfo();
      //renderPassBeginInfo.renderPass = vkrenderpass;
      //renderPassBeginInfo.renderArea.extent.width = pgpucontext->m_rectangle.width();
      //renderPassBeginInfo.renderArea.extent.height = pgpucontext->m_rectangle.height();
      //renderPassBeginInfo.clearValueCount = 0;
      ////renderPassBeginInfo.pClearValues = clearValues;
      //renderPassBeginInfo.pClearValues = nullptr;
      //renderPassBeginInfo.framebuffer = ptexture->framebuffer(prenderpass);

      //::cast<::gpu_directx11::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

      //if (!pframe->m_pgpulayer->m_bRenderTargetFramebufferInitialized)
      //{

      //   pframe->m_pgpulayer->m_bRenderTargetFramebufferInitialized = true;

      //   auto width = pcontext->m_rectangle.width();
      //   auto height = pcontext->m_rectangle.height();

      //   VkViewport vp = {0, 0, (float)width, (float)height, 0.0f, 1.0f};
      //   VkRect2D sc = {{0, 0}, {width, height}};
      //   vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);
      //   vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);
      //   vkCmdSetDepthBounds(pcommandbuffer->m_vkcommandbuffer, 0.0f, 1.0f);

      //   VkClearValue clearValues[1];

      //   clearValues[0].color = {{0.0f, 0.0f, 0.0f, 0.0f}};

      //   ptexture->_set_state(pcommandbuffer, {

      //      VK_ACCESS_TRANSFER_WRITE_BIT, 
      //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //      VK_PIPELINE_STAGE_TRANSFER_BIT

      //                                        });

      //   VkClearColorValue clearColor = {.float32 = {0.0f, 0.0f, 0.0f, 0.0f}};
      //   VkImageSubresourceRange range = {
      //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //      .baseMipLevel = 0,
      //      .levelCount = 1,
      //      .baseArrayLayer = 0,
      //      .layerCount = 1,
      //   };

      //   vkCmdClearColorImage(
      //      pcommandbuffer->m_vkcommandbuffer,
      //      ptexture->m_vkimage, 
      //      ptexture->m_state.m_vkimagelayout,
      //      &clearColor, 1, &range);

      //    ptexture->_set_state(pcommandbuffer, {

      //      VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
      //      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      //      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT

      //                                        });



      //   ::cast<::gpu_directx11::texture> ptextureDepth = ptexture->m_ptextureDepth;

      //   if (ptextureDepth)
      //   {

      //             ptextureDepth->_set_state(pcommandbuffer, {

      //                                              VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
      //                                              VK_PIPELINE_STAGE_TRANSFER_BIT

      //                                           });

      //      VkClearDepthStencilValue clearValueDepth = {.depth = 1.0f, .stencil = 0};

      //      VkImageSubresourceRange rangeDepth = {.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
      //                                       .baseMipLevel = 0,
      //                                       .levelCount = 1,
      //                                       .baseArrayLayer = 0,
      //                                       .layerCount = 1};

      //         vkCmdClearDepthStencilImage(
      //            pcommandbuffer->m_vkcommandbuffer, 
      //            ptextureDepth->m_vkimage,
      //            ptextureDepth->m_state.m_vkimagelayout,
      //            &clearValueDepth,
      //            1,
      //            &rangeDepth);

      //                            ptextureDepth->_set_state(
      //            pcommandbuffer,
      //            {

      //         VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
      //                                                      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      //                                                      VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT

      //            });


      //   }

      //}

      //vkCmdBeginRenderPass(pcommandbuffer->m_vkcommandbuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

      on_render(pgpucontext, pscene);
      
      //vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);

   }



} // namespace graphics3d_directx11
