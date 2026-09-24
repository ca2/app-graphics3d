//
// Created by camilo on 2025-08-22 09:41 <3ThomasBorregaardSorensen!!
//
#include "platform.h"
#include "context.h"
#include "queue.h"


namespace gpu_vulkan
{


   queue::queue()
   {

      m_vkqueue = VK_NULL_HANDLE;
      m_pqueuehostcalldiagnosticstate = std::make_shared<queue_host_call_diagnostic_state>();

   }


   queue::~queue()
   {


   }


   void queue::initialize_gpu_queue(::gpu::device * pgpudevice)
   {

      ::gpu::queue::initialize_gpu_queue(pgpudevice);

   }


   VkResult queue::submit(
      std::uint32_t uSubmitCount,
      const VkSubmitInfo *psubmitinfo,
      VkFence vkfence,
      const ::scoped_string &scopedstrName,
      const ::scoped_string &scopedstrAnnotation)
   {

      queue_host_call_scope scope(this, "vkQueueSubmit", scopedstrName, scopedstrAnnotation);

      return vkQueueSubmit(m_vkqueue, uSubmitCount, psubmitinfo, vkfence);

   }


   VkResult queue::wait_idle(
      const ::scoped_string &scopedstrName,
      const ::scoped_string &scopedstrAnnotation)
   {

      queue_host_call_scope scope(this, "vkQueueWaitIdle", scopedstrName, scopedstrAnnotation);

      return vkQueueWaitIdle(m_vkqueue);

   }


   VkResult queue::present(const VkPresentInfoKHR *ppresentinfo)
   {

      queue_host_call_scope scope(this, "vkQueuePresentKHR");

      return vkQueuePresentKHR(m_vkqueue, ppresentinfo);

   }


   queue_host_call_scope::queue_host_call_scope(
      queue *pqueue,
      const ::scoped_string &scopedstrOperation,
      const ::scoped_string &scopedstrName,
      const ::scoped_string &scopedstrAnnotation) :
      m_scopedqueuehostcall(pqueue->m_pqueuehostcalldiagnosticstate)
   {

      const auto &entry = m_scopedqueuehostcall.entry();

      if (entry.m_bOverlap)
      {

         information(
            "gpu_vulkan queue host overlap: queue={} operation={} serial={} active_count={} task={} name={} annotation={}",
            (::uptr)pqueue->m_vkqueue,
            scopedstrOperation,
            entry.m_uSerial,
            entry.m_uActiveCallCount,
            ::current_task_name(),
            scopedstrName,
            scopedstrAnnotation);

      }

   }


} // namespace gpu_gpu_vulkan_EXPORTS



