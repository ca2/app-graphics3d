//
// Created by camilo on 2026-01-28 23:02 <3ThomasBorregaardSørensen!!
//
#include "platform.h"
#include "context.h"
#include "semaphore.h"


namespace gpu_vulkan
{


   semaphore::semaphore()
   {

      m_vksemaphore = VK_NULL_HANDLE;

   }


   semaphore::~semaphore()
   {



   }


   void semaphore::initialize_gpu_semaphore(::gpu::context * pgpucontext)
   {

      initialize_gpu_context_object(pgpucontext);

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpucontext;

      auto vkdevice = pcontext->logicalDevice();

      VkSemaphoreCreateInfo semaphoreInfo{};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      if (vkCreateSemaphore(
              vkdevice,
              &semaphoreInfo,
              nullptr,
              &m_vksemaphore) != VK_SUCCESS)
      {

         throw ::exception(error_failed);

      }

      auto u = (::uptr) m_vksemaphore;

      if ((u & 0xffff) == 0x0232)
      {

         information("vkSemaphore (u & 0xffff) == 0x0232");

      }

   }


} // namespace gpu_vulkan



