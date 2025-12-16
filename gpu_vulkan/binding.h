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


      virtual ::gpu_vulkan::descriptor_set_layout *descriptor_set_layout(::gpu::command_buffer * pgpucommandbuffer);

      virtual void defer_update_binding_set(::gpu::command_buffer *pgpucommandbuffer);

      virtual void create_descriptor_set_layout(::gpu::command_buffer *pgpucommandbuffer);


      //virtual void defer_create_update_descriptor_set(::gpu::command_buffer *pgpucommandbuffer,
        //                                              ::array<VkDescriptorSet> &descriptorseta,
          //                                            ::array_base<VkDescriptorBufferInfo> &bufferinfoa);

      //virtual void defer_create_update_descriptor_set(::gpu::command_buffer *pgpucommandbuffer,
        //                                              ::array<VkDescriptorSet> &descriptorseta);
      

   };


   //class CLASS_DECL_BRED binding_set_array : virtual public ::pointer_array<binding_set>
   //{
   //public:
   //   bool has_global_ubo() const;
   //};

   class CLASS_DECL_GPU_VULKAN binding_slot_set : virtual public ::gpu::binding_slot_set
   {
   public:

      ::pointer<::gpu_vulkan::descriptor_pool> m_pdescriptorpool;

      ::pointer < ::array<VkDescriptorSet > > m_pvkdescriptorseta;

      ::array<VkDescriptorSet> & descriptor_set(::gpu::command_buffer * pgpucommandbuffer);
      

      virtual ::gpu_vulkan::descriptor_pool *descriptor_pool(::gpu::command_buffer *pgpucommandbuffer);
      
      
      virtual void defer_update_binding_slot_set(::gpu::command_buffer *pgpucommandbuffer);


   };


} // namespace gpu_vulkan




