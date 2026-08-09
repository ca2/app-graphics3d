// From gpu_directx12/buffer.cpp by camilo on 2025-07-17 00:05 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "context.h"
#include "physical_device.h"
#include "gpu_vulkan/context.h"
#include "aura/graphics/image/image.h"
/*
 * Encapsulates a directx12 buffer
 *
 * Initially based off DirectX12Buffer by Sascha Willems -
 * https://github.com/SaschaWillems/DirectX12/blob/master/base/DirectX12Buffer.h
 */

#include "buffer.h"
#include "cube_map_upload.h"
#include <assert.h>


namespace gpu_vulkan
{


   buffer::buffer()
   {

      m_pgpucontext = nullptr;
      m_vkbuffer = VK_NULL_HANDLE;
      m_vkdevicememory = VK_NULL_HANDLE;
      m_size = 0;
      m_vkdevicesizeAllocation = 0;
      m_vkdevicesizeAlignment = 0;
      m_uMemoryTypeBits = 0;
      m_uMemoryTypeIndex = 0;
      m_vkmemorypropertyflagsRequested = 0;
      m_vkmemorypropertyflagsSelected = 0;
      m_pMapped = nullptr;

   }


   buffer::~buffer()
   {

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
   
   void buffer::_create_buffer(
      ::gpu_vulkan::context * pcontext, 
      VkDeviceSize size, VkBufferUsageFlags usage,
                                         VkMemoryPropertyFlags properties)
   {


      if (size <= 0)
      {

         throw ::exception(error_failed, "size must be greater than zero!");

      }

      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      //auto pbuffer = create_newø<buffer>();

      m_pgpucontext = pcontext;

      m_size = size;

      if (vkCreateBuffer(pcontext->logicalDevice(), &bufferInfo, nullptr, &m_vkbuffer) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create vertex buffer!");
      }

      if (((::uptr)m_vkbuffer & 0xffff) == 0x019b)
      {

         information("~buffer (m_vkbuffer & 0xffff) == 0x019b");
      }



      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), m_vkbuffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex =
         pcontext->m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

      const auto & memoryProperties =
         pcontext->m_pgpudevice->m_pphysicaldevice->m_vkphysicaldevicememoryproperties;

      m_vkdevicesizeAllocation = memRequirements.size;
      m_vkdevicesizeAlignment = memRequirements.alignment;
      m_uMemoryTypeBits = memRequirements.memoryTypeBits;
      m_uMemoryTypeIndex = allocInfo.memoryTypeIndex;
      m_vkmemorypropertyflagsRequested = properties;
      m_vkmemorypropertyflagsSelected =
         memoryProperties.memoryTypes[allocInfo.memoryTypeIndex].propertyFlags;

      if (vkAllocateMemory(pcontext->logicalDevice(), &allocInfo, nullptr, &m_vkdevicememory) != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to allocate vertex buffer memory!");

      }

      auto vkbuffer = m_vkbuffer;

      auto vkdevicememory = m_vkdevicememory;

      auto vkresultBind = vkBindBufferMemory(pcontext->logicalDevice(), vkbuffer, vkdevicememory, 0);

      if (vkresultBind != VK_SUCCESS)
      {

         information(
            "gpu_vulkan buffer memory bind failed: result={} buffer={} memory={} requested_bytes={} "
            "allocation_bytes={} alignment={} memory_type_bits={} memory_type_index={} "
            "memory_flags_requested={} memory_flags_selected={}",
            (int)vkresultBind,
            (::uptr)vkbuffer,
            (::uptr)vkdevicememory,
            (::u64)size,
            (::u64)m_vkdevicesizeAllocation,
            (::u64)m_vkdevicesizeAlignment,
            (::u64)m_uMemoryTypeBits,
            m_uMemoryTypeIndex,
            (::u64)m_vkmemorypropertyflagsRequested,
            (::u64)m_vkmemorypropertyflagsSelected);

         throw ::exception(error_failed, "failed to bind buffer memory!");

      }

      //return pbuffer;
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

         auto pbufferStaging = pcontext->_create_buffer(
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



   void buffer::assign_cube_map(const ::pointer_array < ::pixmap >& pixmapa)
   {

      defer_stage([&](buffer* pbuffer)
         {
            
            pbuffer->_assign_cube_map(pixmapa);

         });

   }


   void buffer::_assign_cube_map(const ::pointer_array < ::pixmap >& pixmapaSource)
   {
      
      ::cast < context > pcontext = m_pgpucontext;
      
      // copy all 6 faces into the staging buffer, one after another
      
      void* data;
      
      vkMapMemory(pcontext->logicalDevice(), m_vkdevicememory, 0, m_size, 0, &data);
      
      auto pimageFirst = pixmapaSource.first();

      auto w = pimageFirst->width();

      auto h = pimageFirst->height();

      const image32_t * psourcefaces[6];

      int sourcescana[6];
      
      for (int face = 0; face < 6; face++)
      {

         auto pimage = pixmapaSource[face];

         psourcefaces[face] = pimage->image32();

         sourcescana[face] = pimage->m_iScan;
         
      }

      pack_cube_map_for_vulkan(
         static_cast < image32_t * >(data), w, h, psourcefaces, sourcescana);

      vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);

   }



   void buffer::gpu_read()
   {

      _synchronous_lock synchronouslock(this->synchronization());

      if (m_ppixmap.nok())
      {

         return;

      }

      //m_pixmap.map();

      auto cx = m_ppixmap->width();

      auto cy = m_ppixmap->height();

      //auto sizeNeeded = cx * cy * 4;

      //m_pixmap.create(m_memory, sizeNeeded);

      auto data = m_ppixmap->data();

      {

         auto dst = (unsigned char *)data;
         auto size = cx * cy;

         while (size > 0)
         {
            dst[0] = byte_clip(((int)dst[0] * (int)dst[3]) / 255);
            dst[1] = byte_clip(((int)dst[1] * (int)dst[3]) / 255);
            dst[2] = byte_clip(((int)dst[2] * (int)dst[3]) / 255);
            dst += 4;
            size--;
         }

      }

      //::copy_image32(m_pixmap.m_pimage32,
      //   cx, cy,
      //   m_pixmap.m_iScan,
      //   (const ::image32_t*) data, cx * 4);

   }


   void buffer::gpu_write()
   {

      synchronous_lock synchronouslock(this->synchronization());

      if (m_ppixmap.nok())
      {

         return;

      }

      // //     m_pixmap.map();
      //      //
      ////      glDrawPixels(
      ////         m_pixmap.m_size.cx, m_pixmap.m_size.cy,
      ////         GL_BGRA,
      ////         GL_UNSIGNED_BYTE,
      ////         m_pixmap.m_pimage32Raw);
      //      
      //      glTexImage2D(GL_TEXTURE_2D, 0, 0, 0,
      //                   m_pixmap.m_size.cx, m_pixmap.m_size.cy,
      //                   GL_RGBA, GL_UNSIGNED_BYTE,
      //                   m_pixmap.m_pimage32Raw);

   }


}  // namespace gpu_vulkan



