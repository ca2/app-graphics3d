// Created by camilo on 2025-12-11 16:02 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/block.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN block :
      virtual public ::gpu::block
   {
   public:


      ::pointer_array<::gpu_vulkan::memory_buffer> m_uboBuffers;

      VkDescriptorSet m_vkdescriptorset;

      block();
      ~block() override;


      //void initialize_gpu_block(::gpu::context *pgpucontext) override;


      void create_gpu_block(::gpu::context *pgpucontext) override;


      virtual VkDescriptorSet descriptor_set(::gpu::binding_slot_set * pgpubindingset, ::gpu::command_buffer *pgpucommandbuffer);


      void update_frame(gpu::renderer* pgpurenderer) override;


      virtual VkDescriptorBufferInfo descriptor_info(::gpu::command_buffer *pgpucommandbuffer);
     


   };


} // namespace gpu_vulkan


