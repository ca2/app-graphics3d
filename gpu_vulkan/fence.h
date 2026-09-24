//
// Created by camilo on 2026-01-06 23:50 <3ThomasBorregaardSørensen!!
//
#pragma once


#include "bred/gpu/fence.h"


namespace gpu_vulkan
{


   class fence :
   virtual public ::gpu::fence
   {
   public:


      VkFence m_vkfence;


      fence();
      ~fence() override;


      void initialize_gpu_fence(::gpu::context * pgpucontext, bool bCreateSignaled) override;

      void wait_gpu_fence() override;

      bool has_finished();


   };


} // namespace gpu_vulkan



