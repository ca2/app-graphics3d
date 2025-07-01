// From bred/gpu_directx11/context_object.h by 
// camilo on 2025-06-29 16:12 <3ThomasBorregaardSørensen!!
// From bred/gpu/context_object.h by 
// camilo on 2025-06-29 15:57 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/context_object.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN context_object :
      virtual public ::gpu::context_object
   {
   public:


      ::gpu_vulkan::context* m_pcontext;


      context_object();
      ~context_object() override;


      void on_initialize_gpu_context_object() override;


      ::gpu_vulkan::context* context() { return m_pcontext; }


   };



} // namespace gpu_vulkan



