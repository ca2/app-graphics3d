//
// Created by camilo on 2025-08-22 09:41 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "bred/gpu/queue.h"


namespace gpu_vulkan
{


   class queue :
      virtual public ::gpu::queue
   {
   public:



      VkQueue m_vkqueue;


      queue();
      ~queue() override;


      virtual void initialize_gpu_queue(::gpu::context * pgpucontext);


   };


} // namespace gpu_vulkan



