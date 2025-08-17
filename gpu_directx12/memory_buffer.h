// From gpu_directx11/memory_buffer.h by
// camilo on 2025-07-16 06:18 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/memory_buffer.h"
#include "context.h"


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 memory_buffer :
      virtual public ::gpu::memory_buffer
   {
   public:

      //bool m_bStatic;

      ::comptr<ID3D12Resource> m_presource;
      ::comptr<ID3D12Resource> m_presourceUpload;


      //context* m_pgpucontext;
      //void* m_mapped = nullptr;
      //VkBuffer m_buffer = VK_NULL_HANDLE;
      //VkDeviceMemory m_memory = VK_NULL_HANDLE;

      //VkDeviceSize m_bufferSize;
      // uint32_t m_instanceCount;
      //VkDeviceSize m_instanceSize;
      //VkDeviceSize m_alignmentSize;
      //VkBufferUsageFlags m_usageFlags;
      //VkMemoryPropertyFlags m_memoryPropertyFlags;

      //::comptr<ID3D11Buffer> m_pbuffer;

      memory_buffer();
      ~memory_buffer();



      void on_initialize_memory_buffer(const void* dataStatic = nullptr, memsize sizeStatic = 0) override;


      bool is_initialized() const override;




      //uint32_t getInstanceCount() const { return m_instanceCount; }

      void* _map(memsize start, memsize count) override;
      void _unmap() override;

      void bind() override;
      void unbind()override;


      void _complete_map_allocate(::gpu::memory_buffer* pmemorybufferSource, ::gpu::frame_storage* pgpuframestorage, int size) override;


   };


}  // namespace gpu_directx11



