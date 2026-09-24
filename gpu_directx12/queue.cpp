//
// Created by camilo on 2026-08-14 08:38 <3ThomasBorregaardSorensen!!
//
#include "platform.h"
#include "command_buffer.h"
#include "context.h"
#include "queue.h"


namespace gpu_directx12
{


   queue::queue()
   {

      //m_vkqueue = VK_NULL_HANDLE;
      //m_pqueuehostcalldiagnosticstate = std::make_shared<queue_host_call_diagnostic_state>();

   }


   queue::~queue()
   {


   }


   void queue::initialize_gpu_queue(::gpu::device * pgpudevice)
   {

      ::gpu::queue::initialize_gpu_queue(pgpudevice);

   }


   void queue::execute_command_buffer(::gpu::command_buffer * pgpucommandbuffer)
   {

      ::cast < ::gpu_directx12::command_buffer > pcommandbuffer = pgpucommandbuffer;

      ID3D12CommandList * ppCommandLists[] = { pcommandbuffer->m_pcommandlist };

      m_pd3d12commandqueue->ExecuteCommandLists(1, ppCommandLists);



   }

   //VkResult queue::submit(
   //   std::uint32_t uSubmitCount,
   //   const VkSubmitInfo * psubmitinfo,
   //   VkFence vkfence,
   //   const ::scoped_string & scopedstrName,
   //   const ::scoped_string & scopedstrAnnotation)
   //{

   //   queue_host_call_scope scope(this, "vkQueueSubmit", scopedstrName, scopedstrAnnotation);

   //   return vkQueueSubmit(m_vkqueue, uSubmitCount, psubmitinfo, vkfence);

   //}


   //VkResult queue::wait_idle(
   //   const ::scoped_string & scopedstrName,
   //   const ::scoped_string & scopedstrAnnotation)
   //{

   //   queue_host_call_scope scope(this, "vkQueueWaitIdle", scopedstrName, scopedstrAnnotation);

   //   return vkQueueWaitIdle(m_vkqueue);

   //}


   //VkResult queue::present(const VkPresentInfoKHR * ppresentinfo)
   //{

   //   queue_host_call_scope scope(this, "vkQueuePresentKHR");

   //   return vkQueuePresentKHR(m_vkqueue, ppresentinfo);

   //}


   //queue_host_call_scope::queue_host_call_scope(
   //   queue * pqueue,
   //   const ::scoped_string & scopedstrOperation,
   //   const ::scoped_string & scopedstrName,
   //   const ::scoped_string & scopedstrAnnotation) :
   //   m_scopedqueuehostcall(pqueue->m_pqueuehostcalldiagnosticstate)
   //{

   //   const auto & entry = m_scopedqueuehostcall.entry();

   //   if (entry.m_bOverlap)
   //   {

   //      information(
   //         "gpu_vulkan queue host overlap: queue={} operation={} serial={} active_count={} task={} name={} annotation={}",
   //         (::uptr)pqueue->m_vkqueue,
   //         scopedstrOperation,
   //         entry.m_uSerial,
   //         entry.m_uActiveCallCount,
   //         ::current_task_name(),
   //         scopedstrName,
   //         scopedstrAnnotation);

   //   }

   //}


} // namespace gpu_directx12



