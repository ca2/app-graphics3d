// From gpu_directx12/buffer.h by camilo on 2025-07-16 23:55 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/gpu/buffer.h"
//#include "context.h"


namespace gpu_vulkan
{


   class CLASS_DECL_GPU_VULKAN buffer :
      virtual public ::gpu::buffer
   {
   public:


      //context *         m_pgpucontext;
      VkBuffer          m_vkbuffer;
      VkDeviceMemory    m_vkdevicememory;
      VkDeviceSize      m_size;
      VkDeviceSize      m_vkdevicesizeAllocation;
      VkDeviceSize      m_vkdevicesizeAlignment;
      uint32_t          m_uMemoryTypeBits;
      uint32_t          m_uMemoryTypeIndex;
      VkMemoryPropertyFlags m_vkmemorypropertyflagsRequested;
      VkMemoryPropertyFlags m_vkmemorypropertyflagsSelected;
      uint8_t *         m_pMapped;

      //uint32_t m_instanceCount;
      //VkDeviceSize m_instanceSize;
      //VkDeviceSize m_alignmentSize;
      //VkBufferUsageFlags m_usageFlags;
      //VkMemoryPropertyFlags m_memoryPropertyFlags;

      buffer();
      ~buffer();


      virtual void _create_buffer(::gpu_vulkan::context *pcontext, 
         VkDeviceSize size, VkBufferUsageFlags usage,
                                      VkMemoryPropertyFlags properties);


      virtual bool is_initialized() const;

      virtual void* map();
      virtual void unmap();

      VkResult flush(VkDeviceSize size, VkDeviceSize offset);
      VkResult invalidate(VkDeviceSize size, VkDeviceSize offset);
      VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size, VkDeviceSize offset);
 

      virtual void defer_stage(const ::function<void(buffer * pbuffer)> & functionAssign);


      virtual void assign(const void* data, memsize size);
      virtual void _assign(const void* data, memsize size);
      virtual void assign_cube_map(const ::pointer_array < ::pixmap >& pixmapa);
      virtual void _assign_cube_map(const ::pointer_array < ::pixmap >& pixmapa);

      void gpu_read() override;
      void gpu_write() override;


   };


}  // namespace gpu_vulkan



