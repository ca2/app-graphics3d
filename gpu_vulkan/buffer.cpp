// From gpu_directx12/buffer.cpp by camilo on 2025-07-17 00:05 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "gpu_vulkan/context.h"
#include "aura/graphics/image/image.h"
/*
 * Encapsulates a directx12 buffer
 *
 * Initially based off DirectX12Buffer by Sascha Willems -
 * https://github.com/SaschaWillems/DirectX12/blob/master/base/DirectX12Buffer.h
 */

#include "buffer.h"
#include <assert.h>

namespace gpu_vulkan
{

   buffer::buffer()
   {
      m_vkbuffer = VK_NULL_HANDLE;
      m_vkdevicememory = VK_NULL_HANDLE;

   }


   buffer::~buffer() {
      //unmap();

      ::cast < context > pcontext = m_pgpucontext;

      if (((::uptr)m_vkbuffer & 0xffff) == 0x019b)
      {

         information("~buffer (m_vkbuffer & 0xffff) == 0x019b");
      }
      if (m_vkbuffer != VK_NULL_HANDLE)
      {
         vkDestroyBuffer(pcontext->logicalDevice(), m_vkbuffer, nullptr);
         m_vkbuffer = VK_NULL_HANDLE;
      }
      if (m_vkdevicememory != VK_NULL_HANDLE)
      {
         vkFreeMemory(pcontext->logicalDevice(), m_vkdevicememory, nullptr);
         m_vkdevicememory = VK_NULL_HANDLE;
      }


      /*     vkDestroyBuffer(m_pgpucontext->logicalDevice(), m_buffer, nullptr);
           vkFreeMemory(m_pgpucontext->logicalDevice(), m_memory, nullptr);*/

   }


   bool buffer::is_initialized() const
   {

      return m_vkdevicememory != VK_NULL_HANDLE && m_vkbuffer != VK_NULL_HANDLE;

   }


   void* buffer::map()
   {

      if (m_pMapped)
      {

         return m_pMapped;

      }

      assert(is_initialized() && "Called map on memory_buffer before create");

      ::cast < context > pcontext = m_pgpucontext;

      auto vkresult = vkMapMemory(pcontext->logicalDevice(),
         m_vkdevicememory,
         (VkDeviceSize)0,
         (VkDeviceSize)m_size,
         0,
         (void **) & m_pMapped);

      if (vkresult != VK_SUCCESS)
      {
         throw ::exception(error_failed, "Failed to map memory_buffer");
      }

      return m_pMapped;

   }

   /**
    * Unmap a mapped memory range
    *
    * @note Does not return a result as vkUnmapMemory can't fail
    */
   void buffer::unmap() 
   {

         if (m_pMapped)
         {

            ::cast < context > pcontext = m_pgpucontext;

            vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);

            m_pMapped = nullptr;

         }


   }



   /**
 * Flush a memory range of the memory_buffer to make it visible to the pgpucontext
 *
 * @note Only required for non-coherent memory
 *
 * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
 * complete memory_buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the flush call
 */
   VkResult buffer::flush(VkDeviceSize size, VkDeviceSize offset)
   {
      VkMappedMemoryRange mappedRange = {};
      mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      mappedRange.memory = m_vkdevicememory;
      mappedRange.offset = offset;
      mappedRange.size = size;

      ::cast < context > pcontext = m_pgpucontext;

      return vkFlushMappedMemoryRanges(pcontext->logicalDevice(), 1, &mappedRange);

   }


   /**
 * Invalidate a memory range of the memory_buffer to make it visible to the host
 *
 * @note Only required for non-coherent memory
 *
 * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
 * the complete memory_buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the invalidate call
 */
   VkResult buffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
   {
      VkMappedMemoryRange mappedRange = {};
      mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      mappedRange.memory = m_vkdevicememory;
      mappedRange.offset = offset;
      mappedRange.size = size;

      ::cast < context > pcontext = m_pgpucontext;

      return vkInvalidateMappedMemoryRanges(pcontext->logicalDevice(), 1, &mappedRange);

   }


   /**
 * Create a memory_buffer info descriptor
 *
 * @param size (Optional) Size of the memory range of the descriptor
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkDescriptorBufferInfo of specified offset and range
 */
   VkDescriptorBufferInfo buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
   {
      return VkDescriptorBufferInfo{
          m_vkbuffer,
          offset,
          size,
      };
   }


   void buffer::defer_stage(const ::function<void(buffer* pbuffer)>& functionAssign)
   {
      ::cast < context > pcontext = m_pgpucontext;

      if (m_size > 2_KiB)
      {

         auto pbufferStaging = pcontext->create_buffer(
            m_size,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
         );

         functionAssign(pbufferStaging);
         //pbufferStaging->_assign_cube_map(dataSrc, w, h, scan);

         //_initialize_buffer(
         //   m_pcontext,
         //   bufferSize,
         //   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
         //   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
         //);

         //::cast < gpu_vulkan::context > pcontext = m_pcontext;

         pcontext->copyBuffer(pbufferStaging->m_vkbuffer, this->m_vkbuffer, m_size);

         return;

      }

      functionAssign(this);
      //_assign_cube_map(dataSrc, w, h, scan);


   }


   void buffer::assign(const void* dataSrc, int size)
   {

      defer_stage([&](buffer* pbuffer)
         {

            pbuffer->_assign(dataSrc, size);

         });


   }



   void buffer::_assign(const void* dataSrc, int size)
   {
      ::cast < context > pcontext = m_pgpucontext;
      // copy all 6 faces into the staging buffer, one after another
      void* data;
      vkMapMemory(pcontext->logicalDevice(), m_vkdevicememory, 0, m_size, 0, &data);
      memcpy(data, dataSrc, size);
      //for (int i = 0; i < 6; i++) 
      //{
      //   memcpy((char*)data + layerSize * i, images[i], layerSize);
      //   stbi_image_free(images[i]);
      //}
      vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);

   }



   void buffer::assign_cube_map(const ::pointer_array < ::image::image >& imagea)
   {

      defer_stage([&](buffer* pbuffer)
         {
            
            pbuffer->_assign_cube_map(imagea);

         });

   }


   void buffer::_assign_cube_map(const ::pointer_array < ::image::image >& imageaSource)
   {
      
      ::cast < context > pcontext = m_pgpucontext;
      
      // copy all 6 faces into the staging buffer, one after another
      
      void* data;
      
      vkMapMemory(pcontext->logicalDevice(), m_vkdevicememory, 0, m_size, 0, &data);
      
      auto imagea = imageaSource;

      imagea[0]->rotate(180_degrees);
      imagea[1]->rotate(180_degrees);
      imagea[4]->rotate(180_degrees);
      imagea[5]->rotate(180_degrees);
      swap(imagea[2], imagea[3]);
      swap(imagea[4], imagea[5]);

      auto pimageFirst = imagea.first();  

      auto w = pimageFirst->width();

      auto h = pimageFirst->height();

      auto layerarea = w * h;

      int iScanDst = w * 4;
      
      for (int i = 0; i < 6; i++)
      {

         auto pimage = imagea[i];

         int iScanSrc = pimage->m_iScan;

         auto pimage32Dst = (image32_t*)data + layerarea * i;

         auto pimage32Src = pimage->image32();  
         
         pimage32Dst->copy(w, h, iScanDst, pimage32Src, iScanSrc);

      }

      vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);

   }


}  // namespace gpu_vulkan



