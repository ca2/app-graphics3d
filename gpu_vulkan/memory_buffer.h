// Created by camilo on 2025-06-11 00:50 <3ThomasBorregaardSørensen!!
#pragma once


#include "context.h"
#include "bred/gpu/memory_buffer.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN memory_buffer :
      virtual public ::gpu::memory_buffer
   {
   public:


      //context* m_pgpucontext;
      //void* m_mapped = nullptr;
      VkBuffer                   m_vkbuffer = VK_NULL_HANDLE;
      VkDeviceMemory             m_vkdevicememory = VK_NULL_HANDLE;

      //VkDeviceSize m_bufferSize;
      uint32_t                   m_instanceCount;
      VkDeviceSize               m_vkdevicesizeInstance;
      VkDeviceSize               m_vkdevicesizeAlignment;
      VkBufferUsageFlags         m_vkbufferusageflags;
      VkMemoryPropertyFlags      m_vkmemorypropertyflags;

      memory_buffer();
      ~memory_buffer();


      void _initialize_buffer(::gpu::context* pgpudevice,
         VkDeviceSize instanceSize,
         uint32_t instanceCount,
         VkBufferUsageFlags usageFlags,
         VkMemoryPropertyFlags memoryPropertyFlags,
         VkDeviceSize minOffsetAlignment = 1);

      void initialize_memory_buffer(::gpu::context* pgpucontext, memsize size, bool bIndices) override;

      bool is_initialized() const override;
      //VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
      //void unmap();

      virtual void* __map(memsize start, memsize count);
      virtual void __unmap();

      void writeToBuffer(void* data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
      VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
      VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
      VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

      void writeToIndex(void* data, int index);
      VkResult flushIndex(int index);
      VkDescriptorBufferInfo descriptorInfoForIndex(int index);
      VkResult invalidateIndex(int index);

      VkBuffer getBuffer() const { return m_vkbuffer; }
      void* getMappedMemory() const { return m_pMap; }
      uint32_t getInstanceCount() const { return m_instanceCount; }
      VkDeviceSize getInstanceSize() const { return m_vkdevicesizeInstance; }
      VkDeviceSize getAlignmentSize() const { return m_vkdevicesizeAlignment; }
      VkBufferUsageFlags getUsageFlags() const { return m_vkbufferusageflags; }
      VkMemoryPropertyFlags getMemoryPropertyFlags() const { return m_vkmemorypropertyflags; }
      VkDeviceSize getBufferSize() const { return m_size; }

      static VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment);


   };


}  // namespace gpu_vulkan



