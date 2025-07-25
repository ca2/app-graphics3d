
// Created by camilo on 2025-06-12 21:08 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "layer.h"
#include "render_target.h"
#include "renderer.h"
#include <assert.h>


namespace gpu_vulkan
{


   layer::layer()
   {


   }


   layer::~layer()
   {


   }


   void layer::initialize_gpu_layer(::gpu::renderer* pgpurenderer, int iFrameIndex, int iLayerIndex)
   {

      ::gpu::layer::initialize_gpu_layer(pgpurenderer, iFrameIndex, iLayerIndex);

      ::cast <context> pcontext = pgpurenderer->m_pgpucontext;

      VkSemaphoreCreateInfo semaphoreInfo = {};

      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      if (vkCreateSemaphore(pcontext->logicalDevice(),
         &semaphoreInfo, nullptr,
         &m_vksemaphoreRenderFinished) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create synchronization objects for a layer!");

      }

   }


   void layer::create_command_buffers()
   {

      ::gpu::layer::create_command_buffers();

      //m_commandbufferaLayer.set_size(m_pgpurenderer->m_iDefaultFrameCount);

      //for (auto& pcommandbufferLayer : m_commandbufferaLayer)
      //{

      //   __defer_construct(pcommandbufferLayer);

      //   pcommandbufferLayer->initialize_command_buffer(m_pgpurenderer->m_pgpurendertarget);

      //}

      ////commandBuffers.resize(render_target_view::MAX_FRAMES_IN_FLIGHT);

      ////VkCommandBufferAllocateInfo allocInfo{};
      ////allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      ////allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      ////allocInfo.commandPool = m_pgpucontext->m_pgpudevice->getCommandPool();
      ////allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      ////if (vkAllocateCommandBuffers(m_pgpucontext->logicalDevice(), &allocInfo, commandBuffers.data()) !=
      ////   VK_SUCCESS) {
      ////   throw ::exception(error_failed, "failed to allocate command buffers!");
      ////}


   }

   
   ::gpu::command_buffer* layer::getCurrentCommandBuffer4()
   {

      if (m_commandbufferaLayer.is_empty())
      {

         create_command_buffers();

      }

      auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      auto pcommandbufferLayer = m_commandbufferaLayer[iFrameIndex];

      return pcommandbufferLayer;

   }


} // namespace gpu_vulkan


