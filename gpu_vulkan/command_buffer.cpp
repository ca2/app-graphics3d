// Created by camilo on 2025-06-16 04:42 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "layer.h"
#include "model_buffer.h"
#include "renderer.h"


namespace gpu_vulkan
{


   command_buffer::command_buffer()
   {

      m_vkcommandbuffer = nullptr;

   }


   command_buffer::~command_buffer()
   {

      ::cast <context > pcontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      ::cast <device > pdevice = pcontext->m_pgpudevice;

      vkFreeCommandBuffers(
         pcontext->logicalDevice(),
         pdevice->getCommandPool(),
         1,
         &m_vkcommandbuffer);

   }


   void command_buffer::initialize_command_buffer(::gpu::render_target* pgpurendertarget)
   {

      ::gpu::command_buffer::initialize_command_buffer(pgpurendertarget);

      ::cast <context > pcontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      ::cast <device > pdevice = pcontext->m_pgpudevice;

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = pdevice->getCommandPool();
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

   }


   void command_buffer::submit_command_buffer(::gpu::layer* pgpulayer)
   {

      ::cast < layer > player = pgpulayer;

      ::cast < render_pass > prenderpass = m_pgpurendertarget;

      if (prenderpass)
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

         prenderpass->submitCommandBuffers(
            this,
            semaphoreaWait,
            stageaWait,
            { vksemaphoreRenderFinished });


      }

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



