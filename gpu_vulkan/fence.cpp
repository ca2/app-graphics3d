//
// Created by camilo on 2026-01-06 23:50 <3ThomasBorregaardSørensen!!
//
#include "platform.h"
#include "context.h"
#include "fence.h"

#include "device.h"
#include "bred/gpu/context.h"


namespace gpu_vulkan
{


   fence::fence()
   {

      m_vkfence = VK_NULL_HANDLE;

   }


   fence::~fence()
   {

      if (m_vkfence != VK_NULL_HANDLE)
      {

         ::cast < ::gpu_vulkan::device > pdevice = m_pgpucontextGpuFence->m_pgpudevice;

         vkDestroyFence(pdevice->m_vkdevice, m_vkfence, nullptr);

      }

   }


   void fence::initialize_gpu_fence(::gpu::context *pgpucontext, bool bCreateSignaled)
   {

      ::gpu::fence::initialize_gpu_fence(pgpucontext, bCreateSignaled);

      ::cast<::gpu_vulkan::device> pdevice = m_pgpucontextGpuFence->m_pgpudevice;

      VkFenceCreateInfo fenceCreateInfo = {};

      fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      if (bCreateSignaled)
      {
      
         fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Initial state is signaled, meaning we don't need to
         // wait
         //  for it when it's created
      }  

      VkResult result = vkCreateFence(pdevice->m_vkdevice, &fenceCreateInfo, nullptr, &m_vkfence);

      if (result != VK_SUCCESS)
      {

         throw ::exception(error_failed);

      }

   }


   bool fence::has_finished()
   {

      ::cast < ::gpu_vulkan::device > pdevice = m_pgpucontextGpuFence->m_pgpudevice;

      auto vkdevice = pdevice->logicalDevice();

      auto vkfence = m_vkfence;

      auto timeStart = ::time::now();

      // Wait for the fence to be signaled
      auto result = vkWaitForFences(vkdevice, 1, &vkfence, VK_TRUE, 0); // VK_TRUE: wait for all fences, UINT64_MAX: wait indefinitely

      auto timeElapsed = timeStart.elapsed();

      if (result != VK_SUCCESS)
      {

         ////information(
         //   "gpu_vulkan fence wait: result={} elapsed_ms={} task={} fence={}",
         //   (int)result,
         //   timeElapsed.floating_millisecond(),
         //   ::current_task_name(),
         //   (::uptr)vkfence);

         return false;

      }

      //if (result != VK_SUCCESS)
      //{

      //   throw ::exception(error_failed);

      //}

      return true;

   }



   void fence::wait_gpu_fence()
   {

      ::cast < ::gpu_vulkan::device > pdevice = m_pgpucontextGpuFence->m_pgpudevice;

      auto vkdevice = pdevice->logicalDevice();

      auto vkfence = m_vkfence;

      auto timeStart = ::time::now();

      // Wait for the fence to be signaled
      auto result = vkWaitForFences(vkdevice, 1, &vkfence, VK_TRUE, UINT64_MAX); // VK_TRUE: wait for all fences, UINT64_MAX: wait indefinitely

      auto timeElapsed = timeStart.elapsed();

      if (result != VK_SUCCESS || timeElapsed.floating_millisecond() >= 100.0)
      {

         information(
            "gpu_vulkan fence wait: result={} elapsed_ms={} task={} fence={}",
            (int)result,
            timeElapsed.floating_millisecond(),
            ::current_task_name(),
            (::uptr)vkfence);

      }

      if (result != VK_SUCCESS)
      {

         throw ::exception(error_failed);

      }

   }


} // gpu_vulkan
