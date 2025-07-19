// From gpu_directx12/buffer.h by camilo on 2025-07-16 23:55 <3ThomasBorregaardSørensen!!
#pragma once


#include "context.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN buffer :
      virtual public ::particle
   {
   public:


      context *         m_pgpucontext;
      VkBuffer          m_vkbuffer;
      VkDeviceMemory    m_vkdevicememory;
      VkDeviceSize      m_size;
      uint8_t *         m_pMapped;

      //uint32_t m_instanceCount;
      //VkDeviceSize m_instanceSize;
      //VkDeviceSize m_alignmentSize;
      //VkBufferUsageFlags m_usageFlags;
      //VkMemoryPropertyFlags m_memoryPropertyFlags;

      buffer();
      ~buffer();

      virtual bool is_initialized() const;

      virtual void* map();
      virtual void unmap();

      VkResult flush(VkDeviceSize size, VkDeviceSize offset);
      VkResult invalidate(VkDeviceSize size, VkDeviceSize offset);
      VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size, VkDeviceSize offset);
 

      virtual void defer_stage(const ::function<void(buffer * pbuffer)> & functionAssign);


      virtual void assign(const void* data, int size);
      virtual void _assign(const void* data, int size);
      virtual void assign_cube_map(const void* data, int w, int h, int scan);
      virtual void _assign_cube_map(const void* data, int w, int h, int scan);


   };


}  // namespace gpu_directx12



