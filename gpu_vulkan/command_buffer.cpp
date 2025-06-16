// Created by camilo on 2025-06-16 04:42 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "renderer.h"


namespace gpu_vulkan
{


   command_buffer::command_buffer()
   {

      m_vkcommandbuffer = nullptr;

   }

   
   command_buffer::~command_buffer()
   {

      ::cast <context > pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device > pdevice = pcontext->m_pgpudevice;

      vkFreeCommandBuffers(
         pcontext->logicalDevice(),
         pdevice->getCommandPool(),
         1,
         &m_vkcommandbuffer);


   }


   void command_buffer::initialize_command_buffer(::gpu::renderer* pgpurenderer)
   {

      ::cast <context > pcontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device > pdevice = pcontext->m_pgpudevice;

      m_pgpurenderer = pgpurenderer;

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = pdevice->getCommandPool();
      allocInfo.commandBufferCount = 1;

      VkCommandBuffer pcommandbuffer;
      vkAllocateCommandBuffers(pcontext->logicalDevice(), &allocInfo, &m_vkcommandbuffer);


   }


   void command_buffer::begin_command_buffer()
   {

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      auto result = vkBeginCommandBuffer(m_vkcommandbuffer, &beginInfo);

      if (result != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to begin recording command buffer!");

      }


   }


} // namespace gpu_vulkan



