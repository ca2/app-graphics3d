// Created by camilo on 2025-06-16 04:42 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "window_attachment.h"
#include "frame.h"
#include "layer.h"
#include "model_buffer.h"
#include "renderer.h"
#include "render_target.h"
#include "queue.h"
#include "texture.h"
#include "fence.h"
#include "semaphore.h"
#include "bred/gpu/texture_site.h"


namespace gpu_vulkan
{


   command_buffer::command_buffer()
   {

      //m_bPresentQueue = false;
      m_vkcommandbuffer = VK_NULL_HANDLE;
      m_vkcommandbufferlevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      m_vkcommandpool = VK_NULL_HANDLE;
      m_vkviewport = {};
      m_vkrect2dScissor = {};
      m_bViewportSet = false;
      m_bScissorSet = false;


   }


   command_buffer::~command_buffer()
   {

      ::cast <context > pcontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      ::cast <device > pdevice = pcontext->m_pgpudevice;

      vkFreeCommandBuffers(
         pcontext->logicalDevice(),
         m_vkcommandpool,
         1,
         &m_vkcommandbuffer);

   }


   void command_buffer::initialize_command_buffer(::gpu::render_target* pgpurendertarget, ::gpu::queue * pqueue, ::gpu::enum_command_buffer ecommandbuffer)
   {

      ::gpu::command_buffer::initialize_command_buffer(pgpurendertarget, pqueue, ecommandbuffer);

      ::cast <context > pcontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      ::cast <device > pdevice = pcontext->m_pgpudevice;

      VkCommandBufferAllocateInfo allocInfo{};

      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

      if (m_ecommandbuffer == ::gpu::e_command_buffer_present)
      {
         
         m_vkcommandpool = pcontext->getPresentCommandPool();

      }
      else if (m_ecommandbuffer == ::gpu::e_command_buffer_graphics
         || m_ecommandbuffer == ::gpu::e_command_buffer_graphics_no_layer)
      {
         
         m_vkcommandpool = pcontext->getGraphicsCommandPool();

      }
      else if (m_ecommandbuffer == ::gpu::e_command_buffer_transfer
         || m_ecommandbuffer == ::gpu::e_command_buffer_copy)
      {

         m_vkcommandpool = pcontext->getTransferCommandPool();
         //m_vkcommandbufferlevel = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
      }
      else
      {

         throw ::exception(error_wrong_state, "currently unsupported command buffer type");

      }

      ASSERT(m_vkcommandpool != VK_NULL_HANDLE);

      allocInfo.level = m_vkcommandbufferlevel;
      allocInfo.commandPool = m_vkcommandpool;
      allocInfo.commandBufferCount = 1;

      //VkCommandBuffer pcommandbuffer;
      vkAllocateCommandBuffers(pcontext->logicalDevice(), &allocInfo, &m_vkcommandbuffer);

   }


   void command_buffer::begin_command_buffer(bool bOneTime)
   {

      VkCommandBufferBeginInfo beginInfo{};

      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      beginInfo.flags = bOneTime ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : 0;

      auto result = vkBeginCommandBuffer(m_vkcommandbuffer, &beginInfo);

      if (result != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to begin recording command buffer!");

      }

      m_estate = ::gpu::command_buffer::e_state_recording;
      m_bViewportSet = false;
      m_bScissorSet = false;

   }


   void command_buffer::submit_command_buffer(::gpu::layer* pgpulayer)
   {

      ::cast < layer > player = pgpulayer;

      ::cast < render_target > prendertarget = m_pgpurendertarget;

      //::cast < render_pass > prenderpass = prendertarget->render_pass();

      //if (prenderpass)
      if (prendertarget)
      {
         auto vkcommandbuffer = m_vkcommandbuffer;

         auto vkresultEndCommandBuffer = vkEndCommandBuffer(vkcommandbuffer);

         if (vkresultEndCommandBuffer != VK_SUCCESS)
         {

            information(
               "gpu_vulkan command buffer end failed: result={} task={} command_buffer={} name={} annotation={}",
               (int)vkresultEndCommandBuffer,
               ::current_task_name(),
               (::uptr)vkcommandbuffer,
               m_strName,
               m_strAnnotation);

            throw ::exception(error_failed, "failed to record command buffer!");

         }

         auto pgpuwindowattachment = ::gpu::window_attachment::get(m_pgpurendertarget);

         ::cast < layer > playerPrevious = pgpuwindowattachment->get_previous_layer(player);


         ::comparable_array< VkSemaphore > vksemaphoreaWait;
         ::array_base < VkPipelineStageFlags > vkpipelinestageaWait;
         ::comparable_array < VkSemaphore > vksemaphoreaSignal;

         if (playerPrevious)
         {

            if (vksemaphoreaWait.add_unique(playerPrevious->m_vksemaphoreRenderFinished))
            {

               vkpipelinestageaWait.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

            }

         }

         //for (auto & pgpusemaphore : m_semaphoreaWait)
         //{

         //   ::cast < ::gpu_vulkan::semaphore > psemaphore = pgpusemaphore;

         //   if (psemaphore)
         //   {


         //      if (psemaphore->m_vksemaphore != VK_NULL_HANDLE)
         //      {

         //         if (vksemaphoreaWait.add_unique(psemaphore->m_vksemaphore))
         //         {

         //            vkpipelinestageaWait.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

         //         }

         //      }

         //   }

         //}

         //if (::is_set(player))
         //{

         //   auto vksemaphoreRenderFinished = player->m_vksemaphoreRenderFinished;

         //   if (vksemaphoreRenderFinished != VK_NULL_HANDLE)
         //   {

         //      vksemaphoreaSignal.add_unique(vksemaphoreRenderFinished);

         //   }

         //}

         //for (auto & pgpusemaphore : m_semaphoreaSignal)
         //{

         //   ::cast < ::gpu_vulkan::semaphore > psemaphore = pgpusemaphore;

         //   if (psemaphore)
         //   {

         //      if (psemaphore->m_vksemaphore != VK_NULL_HANDLE)
         //      {

         //         vksemaphoreaSignal.add_unique(psemaphore->m_vksemaphore);

         //      }

         //   }

         //}

         //       auto ptexture = prendertarget->current_texture(::gpu::current_layer());
         //
         //       submitCommandBuffers(
         //          ptexture,
         //          {},
         //          vksemaphoreaWait,
         //          vkpipelinestageaWait,
         //          vksemaphoreaSignal);
         //
         //
         //    }
         //
         // }
         //
         //
         // VkResult command_buffer::submitCommandBuffers(
         //    ::gpu::texture* pgputextureTarget,
         //    const ::pointer_array < ::gpu::texture >& gputextureaSource,
         //    const ::array < VkSemaphore >& semaphoreaWait,
         //    const ::array < VkPipelineStageFlags >& stageaWait,
         //    const ::array < VkSemaphore >& semaphoreaSignal)
         // {
      }

      auto& ecommandbufferstate = m_estate;

      ASSERT(ecommandbufferstate == ::gpu::command_buffer::e_state_recording);

      // ::pointer < ::gpu_vulkan::texture > ptextureDst;
      //
      // if (pgputextureTarget)
      // {
      //
      //    ptextureDst = pgputextureTarget;
      //
      // }
      //
      // ::pointer_array < ::gpu_vulkan::texture > textureaSrc;
      //
      // for (auto& pgputextureSource : gputextureaSource)
      // {
      //
      //    textureaSrc.add(pgputextureSource);
      //
      // }

      //::cast < ::gpu_vulkan::render_target > prendertarget = m_pgpurendertarget;

      ::cast < ::gpu_vulkan::renderer > prenderer = prendertarget->m_pgpurenderer;

      ::cast < ::gpu_vulkan::context > pcontext = prenderer->m_pgpucontext;

      //::cast < ::gpu_vulkan::texture > ptexture = prenderer->m_pgputexture;

      bool bWithDepth = pcontext->m_escene == ::gpu::e_scene_3d;

      ///auto prenderpass = prendertarget->render_pass2(bWithDepth);

      // ::gpu_vulkan::texture_synchronization* psynchronizationDst = nullptr;
      //
      // if (ptextureDst)
      // {
      //
      //    psynchronizationDst = ptextureDst->synchronization();
      //
      // }

      // ::array< ::gpu_vulkan::texture_synchronization*> synchronizationaSrc;
      //
      // for (auto& ptextureSrc : textureaSrc)
      // {
      //
      //    synchronizationaSrc.add(ptextureSrc->synchronization());
      //
      // }


      //if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE)
      //{

      //   vkWaitForFences(m_pgpucontext->logicalDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);

      //}

      //imagesInFlight[*imageIndex] = inFlightFences[m_pgpurenderer->get_frame_index()];

      VkSubmitInfo submitInfo = {};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      ::comparable_array<VkSemaphore> vksemaphoreaWait;
      ::array_base<VkPipelineStageFlags> vkpipelinestageflagsaWait;
      ::comparable_array<VkSemaphore> vksemaphoreaSignal;
      for(::collection::index i = 0; i < m_semaphoreaWait.get_count(); i++)
      {
         ::cast < ::gpu_vulkan::semaphore > psemaphore = m_semaphoreaWait[i];
         auto epipelinestage = m_epipelinestageaWait.atø(i);
         if (psemaphore)
         {
            auto vksemaphore = psemaphore->m_vksemaphore;
            if (vksemaphore != VK_NULL_HANDLE)
            {
               vksemaphoreaWait.add(vksemaphore);
               vkpipelinestageflagsaWait.add(::vulkan::to_vk_pipeline_stage(epipelinestage));
            }
         }
      }
      //m_semaphoreaWait.clear();
      // for (auto psynchronizationSrc : synchronizationaSrc)
      // {
      //    if (psynchronizationSrc && psynchronizationSrc->m_iRendering > 0)
      //    {
      //       waitSemaphores.add(psynchronizationSrc->m_vksemaphoreRenderFinished);
      //       waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
      //       psynchronizationSrc->m_iRendering = 0;
      //    }
      // }
      //waitStages.append(::transfer(m_stageaWaitToSubmit));
      //waitSemaphores.append(::transfer(m_semaphoreaWaitToSubmit));
      submitInfo.waitSemaphoreCount = (uint32_t)vksemaphoreaWait.size();
      submitInfo.pWaitSemaphores = vksemaphoreaWait.data();
      submitInfo.pWaitDstStageMask = vkpipelinestageflagsaWait.data();

      VkCommandBuffer vkcommandbuffera[] = { m_vkcommandbuffer };
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = vkcommandbuffera;

      //::comparable_array<VkSemaphore> signalSemaphores(semaphoreaSignal);

      for (auto & pgpusemaphore : m_semaphoreaSignal)
      {
         ::cast < ::gpu_vulkan::semaphore > psemaphore = pgpusemaphore;
         if (psemaphore)
         {
            auto vksemaphore = psemaphore->m_vksemaphore;
            if (vksemaphore != VK_NULL_HANDLE)
            {
               vksemaphoreaSignal.add(vksemaphore);
               //waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
            }
         }
      }
      //m_semaphoreaSignal.clear();

      // // && texture.m_vksemaphoreRenderFinished)
      // if (psynchronizationDst && psynchronizationDst->m_vksemaphoreRenderFinished)
      // {
      //
      //    signalSemaphores.add_unique(psynchronizationDst->m_vksemaphoreRenderFinished);
      //    psynchronizationDst->m_iRendering = 1;
      //
      // }
      //
      // if (signalSemaphores.has_elements())
      // {
      //
      //    signalSemaphores.append_unique(signalSemaphores);
      //
      // }

      //if (::gpu::current_layer()->m_pgpulayer)
      //{

      //   //if (m_iSentLayerCount > 0)
      //   {

      //      ::cast < ::gpu_vulkan::context > pcontextMain = pcontext->m_pgpudevice->m_pgpucontextMain;

      //      //::cast < ::gpu_vulkan::swap_chain > pswapchain = pcontextMain->get_swap_chain();

      //      ::cast < layer > playerLast = ::gpu::current_layer()->m_pgpulayer;

      //      auto vksemaphoreRenderFinished = playerLast->m_vksemaphoreRenderFinished;
      //      if (vksemaphoreRenderFinished)
      //      {
      //         //pswapchain->m_stageaWaitToSubmit.add(VK_PIPELINE_STAGE_TRANSFER_BIT);
      //         //pswapchain->m_semaphoreaWaitToSubmit.add(vksemaphoreRenderFinished);
      //         m_stageaWaitToSubmit.add(VK_PIPELINE_STAGE_TRANSFER_BIT);
      //         m_semaphoreaWaitToSubmit.add(vksemaphoreRenderFinished);
      //         signalSemaphores.add(vksemaphoreRenderFinished);
      //      }
      //   }


      //}

      //signalSemaphores.append(::transfer(m_semaphoreaSignalOnSubmit));

      submitInfo.signalSemaphoreCount = (uint32_t)vksemaphoreaSignal.count();

      submitInfo.pSignalSemaphores = vksemaphoreaSignal.data();

      //vkResetFences(m_pgpucontext->logicalDevice(), 1, &inFlightFences[m_pgpurenderer->get_frame_index()]);

      // VkFence fence = VK_NULL_HANDLE;
      //
      // if (psynchronizationDst)
      // {
      //
      //    fence = psynchronizationDst->in_flight_fence();
      //
      // }

      bool bCreatedFence = false;

      ::cast < ::gpu_vulkan::fence > pfence = m_pgpufence;

      VkFence vkfence = VK_NULL_HANDLE;

      bool bCreatedFenceHere = false;

      if (pfence)
         //{

         //   VkFenceCreateInfo fenceInfo = {
         //       .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
         //       .pNext = NULL,
         //       .flags = 0  // 0 = fence starts in unsignaled state
         //   };

         //   VkResult result = vkCreateFence(pcontext->logicalDevice(), &fenceInfo, NULL, &fence);
         //   if (result != VK_SUCCESS) {
         //      fprintf(stderr, "Failed to create fence\n");
         //      // handle error
         //   }

         //   bCreatedFence = true;

         //}
         //else
      {

         vkfence = pfence->m_vkfence;

         if (vkfence == VK_NULL_HANDLE)
         {

            throw ::exception(error_wrong_state);

         }

         auto vkresultFenceWait = vkWaitForFences(pcontext->logicalDevice(), 1, &vkfence, VK_TRUE, UINT64_MAX);

         if (vkresultFenceWait != VK_SUCCESS)
         {

            information(
               "gpu_vulkan pre-submit fence wait failed: result={} task={} fence={} command_buffer={} name={} annotation={}",
               (int)vkresultFenceWait,
               ::current_task_name(),
               (::uptr)vkfence,
               (::uptr)m_vkcommandbuffer,
               m_strName,
               m_strAnnotation);

            throw ::exception(error_failed, "failed waiting for command buffer fence before submit!");

         }

         auto vkresultFenceReset = vkResetFences(pcontext->logicalDevice(), 1, &vkfence);

         if (vkresultFenceReset != VK_SUCCESS)
         {

            information(
               "gpu_vulkan pre-submit fence reset failed: result={} task={} fence={} command_buffer={} name={} annotation={}",
               (int)vkresultFenceReset,
               ::current_task_name(),
               (::uptr)vkfence,
               (::uptr)m_vkcommandbuffer,
               m_strName,
               m_strAnnotation);

            throw ::exception(error_failed, "failed resetting command buffer fence before submit!");

         }

      }
      else if (m_bFenceWaitIfNoPreexistingFence)
      {

         //      ::cast<::gpu_vulkan::fence> pfence = pcommandbuffer->m_pgpufence;

         // bool bCreatedFenceHere = false;

         // if (::is_set(pfence))
         //{

         //   vkfence = pfence->m_vkfence;
         //}
         // else
         //{

         pfence = this->insert_gpu_fence(false);

         vkfence = pfence->m_vkfence;

         bCreatedFenceHere = true;
         //}
      }

               ::cast<::gpu_vulkan::queue> pqueue = this->m_pgpuqueue;

      VkQueue vkqueue = pqueue->m_vkqueue;



         // if (pcommandbuffer->m_ecommandbuffer == ::gpu::e_command_buffer_present)
         // {
         //
         //    vkqueue = m_vkqueuePresent;
         //
         // }
         // else
         // {
         //
         //    vkqueue = m_vkqueueGraphics;
         //
         // }

         ::string strCommandListName = this->m_strName;

         auto timeStart = ::time::now();


      //}

      //::cast < ::gpu_vulkan::queue > pqueue = m_pgpuqueue;
         if (submitInfo.pWaitSemaphores && submitInfo.pWaitDstStageMask)
         {
            informationf("submit wait semaphore=%p commandBuffer=%p waitStage=0x%08x",
                         (void *)submitInfo.pWaitSemaphores[0], (void *)m_vkcommandbuffer,
                         submitInfo.pWaitDstStageMask[0]);
         }

      //if (vkQueueSubmit(queueGraphics, 1, &submitInfo, inFlightFences[m_pgpurenderer->get_frame_index()]) != VK_SUCCESS)
      auto vkresultQueueSubmit = pqueue->submit(1, &submitInfo, vkfence, m_strName, m_strAnnotation);

      if (vkresultQueueSubmit != VK_SUCCESS)
      {

         information(
            "gpu_vulkan queue submit failed: result={} task={} queue={} command_buffer={} fence={} "
            "wait_count={} signal_count={} name={} annotation={}",
            (int)vkresultQueueSubmit,
            ::current_task_name(),
            (::uptr)vkqueue,
            (::uptr)m_vkcommandbuffer,
            (::uptr)vkfence,
            submitInfo.waitSemaphoreCount,
            submitInfo.signalSemaphoreCount,
            m_strName,
            m_strAnnotation);

         throw ::exception(error_failed, "failed to submit draw command buffer!");

      }

            if (bCreatedFenceHere)
      {

         pfence->wait_gpu_fence();

         auto vkresultQueueIdle = pqueue->wait_idle(m_strName, m_strAnnotation);

         if (vkresultQueueIdle != VK_SUCCESS)
         {

            information(
               "gpu_vulkan post-submit queue wait failed: result={} task={} queue={} command_buffer={} name={} annotation={}",
               (int)vkresultQueueIdle,
               ::current_task_name(),
               (::uptr)vkqueue,
               (::uptr)m_vkcommandbuffer,
               m_strName,
               m_strAnnotation);

            throw ::exception(error_failed, "failed waiting for queue after command buffer submit!");

         }
         
         auto timeElapsed = timeStart.elapsed();

         information("submit_command_buffer with Fence and queue wait took {} ms. (thread={},cmdlst_name={})", timeElapsed.floating_millisecond(),
                     ::current_task_name(), strCommandListName);

      }

      // vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
      //
      // vkQueueWaitIdle(vkqueue);
      //
      // vkDestroyFence(this->logicalDevice(), fence, NULL);


      // if (pvkfence)
      // {
      //
      //    *pvkfence = fence;
      //
      // }

      ecommandbufferstate = ::gpu::command_buffer::e_state_submitted;

      //vkWaitForFences(pcontext->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

      //vkQueueWaitIdle(queueGraphics);


      //vkDestroyFence(pcontext->logicalDevice(), fence, NULL);

      //VK_CHECK(vkWaitForFences(m_pgpucontext->logicalDevice(), 1, &inFlightFences[m_pgpurenderer->get_frame_index()], VK_TRUE, UINT64_MAX));


      //VkPresentInfoKHR presentInfo = {};
      //presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

      //presentInfo.waitSemaphoreCount = 1;
      //presentInfo.pWaitSemaphores = signalSemaphores;

      //VkSwapchainKHR swapChains[] = { swapChain };
      //presentInfo.swapchainCount = 1;
      //presentInfo.pSwapchains = swapChains;

      //presentInfo.pImageIndices = imageIndex;

      //auto result = vkQueuePresentKHR(m_pgpucontext->presentQueue(), &presentInfo);

      //return VK_SUCCESS;

   }


   void command_buffer::wait_commands_to_execute()
   {

      ::cast <context > pcontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      ::cast <render_pass > prenderpass = m_pgpurendertarget;

      //vkWaitForFences(pcontext->logicalDevice(), 1, 
      //   &prenderpass->inFlightFences[prenderpass->get_frame_index()],
      //   VK_TRUE, UINT64_MAX);


   }


   void command_buffer::set_line_width(float fLineWidth)
   {

      vkCmdSetLineWidth(m_vkcommandbuffer, fLineWidth);

   }


   void command_buffer::clear(::gpu::texture * pgputexture, const ::color::color & color)
   {


      VkClearColorValue clearColor = { .float32 = { 
         color.f32_red() * color.f32_opacity(),
         color.f32_green() * color.f32_opacity(),
         color.f32_blue() * color.f32_opacity(),
         color.f32_opacity()} };

      VkImageSubresourceRange range = {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
          .baseMipLevel = 0,
          .levelCount = 1,
          .baseArrayLayer = 0,
          .layerCount = 1,
      };

      ::cast < ::gpu_vulkan::texture > ptexture = pgputexture;

      vkCmdClearColorImage(
         m_vkcommandbuffer,
         ptexture->m_vkimage,
         ptexture->m_state2a.mip_layer_state(0, 0).m_vkimagelayout,
         &clearColor,
         1, // rangeCount
         &range
      );

   }


   void command_buffer::set_viewport(const ::i32_rectangle& rectangle, const ::i32_size & sizeRaw)
   {

      m_vkviewport =
      {
         (float)rectangle.left,
         (float)rectangle.top,
         (float)rectangle.width(),
         (float)rectangle.height(),
         0.0f, 1.0f
      };

      vkCmdSetViewport(m_vkcommandbuffer, 0, 1, &m_vkviewport);

      m_bViewportSet = true;

   }


   void command_buffer::set_scissor(const ::i32_rectangle& rectangle, const ::i32_size & sizeRaw)
   {

      m_vkrect2dScissor =
      {

         {
            rectangle.left,
            rectangle.top,
         },
         {
            (uint32_t)rectangle.width(),
            (uint32_t)rectangle.height(),
         }

      };

      vkCmdSetScissor(m_vkcommandbuffer, 0, 1, &m_vkrect2dScissor);

      m_bScissorSet = true;

   }


   //void command_buffer::set_primitive_topology_triangle_strip()
   //{

   //   //vkCmdSetPrimitiveTopology(m_vkcommandbuffer, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);

   //}


   void command_buffer::draw(::gpu_vulkan::model_buffer* pmodelbuffer)
   {

      pmodelbuffer->draw2(this);

   }


   void command_buffer::draw_vertexes(int iVertexCount)
   {

      if (::gpu::trace_flags().m_bVulkanPipelineTrace)
      {

         information(
            "VULKAN_PIPELINE_TRACE vkCmdDraw command_buffer_object={} vk_command_buffer={} vertex_count={}",
            (::uptr)this,
            (::uptr)m_vkcommandbuffer,
            iVertexCount);

      }

      vkCmdDraw(m_vkcommandbuffer, iVertexCount, 1, 0, 0);

   }


   void command_buffer::draw_indexes(int iIndexCount)
   {

      vkCmdDrawIndexed(m_vkcommandbuffer, iIndexCount, 1, 0, 0, 0);

   }


   void command_buffer::begin_render(::gpu::shader *pgpushader, ::gpu::texture_site *pgputexturesiteTarget)
   {

      auto ptextureTarget = pgputexturesiteTarget->gpu_texture();

      //ptextureTarget->_set_state(this,

         //                              {VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
            //                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
              //                          VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT});

      ptextureTarget->set_state(this, ::gpu::e_texture_state_color_attachment);

      ::gpu::command_buffer::begin_render(pgpushader, pgputexturesiteTarget);

   }


   void command_buffer::end_render()
   {

      ::gpu::command_buffer::end_render();

      

   }


} // namespace gpu_vulkan



