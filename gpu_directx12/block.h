// From gpu_directx11/block.h by camilo on 2025-12-11 18:57 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/block.h"


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 block :
      virtual public ::gpu::block
   {
   public:

      //comptr < ID3D11Buffer>              m_pbufferGlobalUbo;
      //comptr < ID3D11Buffer>              m_pbuffer;

      ::comptr<ID3D12Resource> m_presource;
      void* m_pData;


      //::pointer_array<::gpu_vulkan::memory_buffer> m_uboBuffers;

      //VkDescriptorSet m_vkdescriptorset;

      block();
      ~block() override;


      void initialize_gpu_block(::gpu::context *pgpucontext);


      ID3D12Resource * resource();
      //virtual VkDescriptorSet descriptor_set(::gpu::binding_set_pointer pgpubindingset, ::gpu::command_buffer *pgpucommandbuffer);

      void update_frame(::gpu::renderer * pgpurenderer) override;


   };


} // namespace gpu_directx11


