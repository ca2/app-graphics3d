//
// Created by camilo on 2025-08-22 09:41 <3ThomasBorregaardSorensen!!
//
#include "framework.h"
#include "context.h"
#include "queue.h"


namespace gpu_vulkan
{


   queue::queue()
   {


   }


   queue::~queue()
   {


   }


   void queue::initialize_gpu_queue(::gpu::context * pgpucontext)
   {

      ::gpu::queue::initialize_gpu_queue(pgpucontext);

   }


} // namespace gpu_gpu_vulkan_EXPORTS



