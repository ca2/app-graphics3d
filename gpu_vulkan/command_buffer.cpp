// Created by camilo on 2025-06-16 04:42 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "frame.h"
#include "layer.h"
#include "model_buffer.h"
#include "renderer.h"
#include "render_target.h"
//#include "swa"
#include "texture.h"


namespace gpu_vulkan
{
   command_buffer::command_buffer()
   {

      //m_bPresentQueue = false;
      m_vkcommandbuffer = VK_NULL_HANDLE;
      m_vkfence = VK_NULL_HANDLE;
      m_vkcommandpool = VK_NULL_HANDLE;

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


   void command_buffer::initialize_command_buffer(::gpu::render_target* pgpurendertarget, ::gpu::enum_command_buffer ecommandbuffer)
   {

      ::gpu::command_buffer::initialize_command_buffer(pgpurendertarget, ecommandbuffer);

      ::cast <context > pcontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      ::cast <device > pdevice = pcontext->m_pgpudevice;

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      
      if (m_ecommandbuffer == ::gpu::e_command_buffer_present)
      {
         m_vkcommandpool = pdevice->getPresentCommandPool();
      }
      else if(m_ecommandbuffer == ::gpu::e_command_buffer_graphics)
      {
         m_vkcommandpool = pdevice->getCommandPool();
      }
      allocInfo.commandPool = m_vkcommandpool;
      allocInfo.commandBufferCount = 1;

      //VkCommandBuffer pcommandbuffer;
      vkAllocateCommandBuffers(pcontext->logicalDevice(), &allocInfo, &m_vkcommandbuffer);

      VkFenceCreateInfo fenceInfo = {};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      if (vkCreateFence(
         pcontext->logicalDevice(),
         &fenceInfo,
         nullptr,
         &m_vkfence) != VK_SUCCESS
         )
      {

         throw ::exception(error_failed, "failed to create fence!");

      }


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

   }


   void command_buffer::submit_command_buffer(::gpu::layer* pgpulayer)
   {

      ::cast < layer > player = pgpulayer;

      ::cast < render_target > prendertarget = m_pgpurendertarget;

      //::cast < render_pass > prenderpass = prendertarget->render_pass();

      //if (prenderpass)
      if(prendertarget)
      {

         auto vkcommandbuffer = m_vkcommandbuffer;

         if (vkEndCommandBuffer(vkcommandbuffer) != VK_SUCCESS)
         {

            throw ::exception(error_failed, "failed to record command buffer!");

         }

         ::cast < layer > playerPrevious = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->get_previous_layer(player);


         ::array < VkSemaphore > semaphoreaWait;
         ::array < VkPipelineStageFlags > stageaWait;

         if (playerPrevious)
         {

            semaphoreaWait.add(playerPrevious->m_vksemaphoreRenderFinished);
            stageaWait.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

         }

         auto vksemaphoreRenderFinished = player->m_vksemaphoreRenderFinished;

         auto ptexture = prendertarget->current_texture(::gpu::current_frame());

         submitCommandBuffers(
            ptexture,
            {},
            semaphoreaWait,
            stageaWait,
            { vksemaphoreRenderFinished });


      }

   }


   VkResult command_buffer::submitCommandBuffers(
      ::gpu::texture* pgputextureTarget,
      const ::pointer_array < ::gpu::texture > & gputextureaSource,
      const ::array < VkSemaphore >& semaphoreaWait,
      const ::array < VkPipelineStageFlags >& stageaWait,
      const ::array < VkSemaphore >& semaphoreaSignal,
      VkFence * pvkfence)
   {

      auto& ecommandbufferstate = m_estate;

      ASSERT(ecommandbufferstate == ::gpu::command_buffer::e_state_recording);

      ::pointer < ::gpu_vulkan::texture > ptextureDst;
      
      if (pgputextureTarget)
      {

         ptextureDst = pgputextureTarget;

      }

      ::pointer_array < ::gpu_vulkan::texture > textureaSrc;

      for(auto & pgputextureSource : gputextureaSource)
      {

         textureaSrc.add(pgputextureSource);

      }

      ::cast < ::gpu_vulkan::render_target > prendertarget = m_pgpurendertarget;

      ::cast < ::gpu_vulkan::renderer > prenderer = prendertarget->m_pgpurenderer;

      ::cast < ::gpu_vulkan::context > pcontext = prenderer->m_pgpucontext;

      //::cast < ::gpu_vulkan::texture > ptexture = prenderer->m_pgputexture;

      bool bWithDepth = pcontext->m_escene == ::gpu::e_scene_3d;

      ///auto prenderpass = prendertarget->render_pass2(bWithDepth);

      ::gpu_vulkan::texture_synchronization* psynchronizationDst = nullptr;

      if (ptextureDst)
      {

         psynchronizationDst = ptextureDst->synchronization();

      }

      ::array< ::gpu_vulkan::texture_synchronization *> synchronizationaSrc;

      for(auto & ptextureSrc : textureaSrc)
      {

         synchronizationaSrc.add(ptextureSrc->synchronization());

      }


      //if (imagesInFlight[*imageIndex] != VK_NULL_HANDLE)
      //{

      //   vkWaitForFences(m_pgpucontext->logicalDevice(), 1, &imagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);

      //}

      //imagesInFlight[*imageIndex] = inFlightFences[m_pgpurenderer->get_frame_index()];

      VkSubmitInfo submitInfo = {};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

      ::array<VkSemaphore> waitSemaphores(semaphoreaWait);
      ::array<VkPipelineStageFlags> waitStages(stageaWait);
      for(auto psynchronizationSrc : synchronizationaSrc)
      {
         if (psynchronizationSrc && psynchronizationSrc->m_iImageAvailable > 0)
         {
            waitSemaphores.add(psynchronizationSrc->m_vksemaphoreAvailable);
            waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

         }
      }
      for (auto psynchronizationSrc : synchronizationaSrc)
      {
         if (psynchronizationSrc && psynchronizationSrc->m_iRendering > 0)
         {
            waitSemaphores.add(psynchronizationSrc->m_vksemaphoreRenderFinished);
            waitStages.add(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
            psynchronizationSrc->m_iRendering = 0;
         }
      }
      waitStages.append(::transfer(m_stageaWaitToSubmit));
      waitSemaphores.append(::transfer(m_semaphoreaWaitToSubmit));
      submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores.size();
      submitInfo.pWaitSemaphores = waitSemaphores.data();
      submitInfo.pWaitDstStageMask = waitStages.data();

      VkCommandBuffer vkcommandbuffera[] = { m_vkcommandbuffer };
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = vkcommandbuffera;

      ::comparable_array<VkSemaphore> signalSemaphores(semaphoreaSignal);

        // && texture.m_vksemaphoreRenderFinished)
      if(psynchronizationDst && psynchronizationDst->m_vksemaphoreRenderFinished)
      {

         signalSemaphores.add_unique(psynchronizationDst->m_vksemaphoreRenderFinished);
         psynchronizationDst->m_iRendering = 1;

      }

      if (signalSemaphores.has_elements())
      {

         signalSemaphores.append_unique(signalSemaphores);

      }

      //if (::gpu::current_frame()->m_pgpulayer)
      //{

      //   //if (m_iSentLayerCount > 0)
      //   {

      //      ::cast < ::gpu_vulkan::context > pcontextMain = pcontext->m_pgpudevice->m_pgpucontextMain;

      //      //::cast < ::gpu_vulkan::swap_chain > pswapchain = pcontextMain->get_swap_chain();

      //      ::cast < layer > playerLast = ::gpu::current_frame()->m_pgpulayer;

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

      signalSemaphores.append(::transfer(m_semaphoreaSignalOnSubmit));

      submitInfo.signalSemaphoreCount = (uint32_t)signalSemaphores.count();

      submitInfo.pSignalSemaphores = signalSemaphores.data();

      //vkResetFences(m_pgpucontext->logicalDevice(), 1, &inFlightFences[m_pgpurenderer->get_frame_index()]);

      auto queueGraphics = pcontext->graphicsQueue();

      VkFence fence = VK_NULL_HANDLE;

      if (psynchronizationDst)
      {

         fence = psynchronizationDst->in_flight_fence();

      }

      bool bCreatedFence = false;

      if (fence)
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
         vkWaitForFences(pcontext->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

         vkResetFences(pcontext->logicalDevice(), 1, &fence);

      }

      //if (vkQueueSubmit(queueGraphics, 1, &submitInfo, inFlightFences[m_pgpurenderer->get_frame_index()]) != VK_SUCCESS)
      if (vkQueueSubmit(queueGraphics, 1, &submitInfo, fence) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to submit draw command buffer!");

      }

      if (pvkfence)
      {

         *pvkfence = fence;

      }

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

      return VK_SUCCESS;

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


   void command_buffer::set_viewport(const ::int_rectangle& rectangle)
   {

      VkViewport viewport =
      {
         (float)rectangle.left(),
         (float)rectangle.top(),
         (float)rectangle.width(),
         (float)rectangle.height(),
         0.0f, 1.0f 
      };

      vkCmdSetViewport(m_vkcommandbuffer, 0, 1, &viewport);

   }


   void command_buffer::set_scissor(const ::int_rectangle& rectangle)
   {
      
      VkRect2D rect2d = 
      {

         {
            rectangle.left(),
            rectangle.top(),
         },
         {
            (uint32_t) rectangle.width(),
            (uint32_t)rectangle.height(),
         }

      };

      vkCmdSetScissor(m_vkcommandbuffer, 0, 1, &rect2d);

   }


   void command_buffer::draw(::gpu_vulkan::model_buffer* pmodelbuffer)
   {

      pmodelbuffer->draw(this);

   }


} // namespace gpu_vulkan



