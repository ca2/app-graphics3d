// From bred/gpu/binding.h by camilo on 2025-12-11 01:08 <3ThomasBorregaardSørensen!!
#pragma once


#include "descriptors.h"
#include "acme/prototype/prototype/pointer.h"
#include "bred/gpu/binding.h"


namespace gpu_vulkan
{

   class binding :
      virtual public ::gpu::binding
   {
   public:

   };

   class binding_set : virtual public ::gpu::binding_set
   {
   public:

      
      ::pointer < ::gpu_vulkan::descriptor_set_layout > m_pdescriptorsetlayout1;
      ::pointer <::gpu_vulkan::descriptor_pool> m_pdescriptorpool;


      virtual ::gpu_vulkan::descriptor_set_layout *descriptor_set_layout(::gpu::command_buffer * pgpucommandbuffer);

      virtual void create_descriptor_set_layout(::gpu::command_buffer *pgpucommandbuffer);


   };


   //class CLASS_DECL_BRED binding_set_array : virtual public ::pointer_array<binding_set>
   //{
   //public:
   //   bool has_global_ubo() const;
   //};


} // namespace gpu_vulkan




