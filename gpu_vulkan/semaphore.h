//
// Created by camilo on 2026-01-28 23:0s0 <3ThomasBorregaardSørensen!!
//
#pragma once


#include "bred/gpu/semaphore.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN semaphore :
      virtual public ::gpu::semaphore
   {
   public:


      VkSemaphore m_vksemaphore;


      semaphore();
      ~semaphore() override;


      void initialize_gpu_semaphore(::gpu::context * pgpucontext) override;


   };


} // namespace gpu_vulkan



