#pragma once


#include "bred/gpu/aaa_cpu_buffer.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN aaa_cpu_buffer :
      virtual public ::gpu::aaa_cpu_buffer
   {
   public:


      //__creatable_from_base(buffer, ::gpu::buffer);


      aaa_cpu_buffer();
      ~aaa_cpu_buffer() override;


      void gpu_read() override;
      void gpu_write() override;
      


   };


} // namespace gpu_vulkan



