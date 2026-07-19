//
// Created by camilo on 2025-08-22 09:41 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "bred/gpu/queue.h"
#include "queue_host_call_diagnostics.h"


namespace gpu_vulkan
{


   class queue :
      virtual public ::gpu::queue
   {
   public:



      VkQueue m_vkqueue;
      std::shared_ptr<queue_host_call_diagnostic_state> m_pqueuehostcalldiagnosticstate;


      queue();
      ~queue() override;


      void initialize_gpu_queue(::gpu::device * pgpudevice) override;

      VkResult submit(
         std::uint32_t uSubmitCount,
         const VkSubmitInfo *psubmitinfo,
         VkFence vkfence,
         const ::scoped_string &scopedstrName = {},
         const ::scoped_string &scopedstrAnnotation = {});

      VkResult wait_idle(
         const ::scoped_string &scopedstrName = {},
         const ::scoped_string &scopedstrAnnotation = {});

      VkResult present(const VkPresentInfoKHR *ppresentinfo);


   };


   class queue_host_call_scope
   {
   public:


      queue_host_call_scope(
         queue *pqueue,
         const ::scoped_string &scopedstrOperation,
         const ::scoped_string &scopedstrName = {},
         const ::scoped_string &scopedstrAnnotation = {});


   private:


      scoped_queue_host_call m_scopedqueuehostcall;


   };


} // namespace gpu_vulkan



