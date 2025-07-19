// From gpu_directx12/buffer.cpp by camilo on 2025-07-17 00:05 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "gpu_vulkan/context.h"
/*
 * Encapsulates a directx12 buffer
 *
 * Initially based off DirectX12Buffer by Sascha Willems -
 * https://github.com/SaschaWillems/DirectX12/blob/master/base/DirectX12Buffer.h
 */

#include "buffer.h"


namespace gpu_vulkan
{

   /**
    * Returns the minimum m_vkinstance size required to be compatible with devices minOffsetAlignment
    *
    * @param instanceSize The size of an m_vkinstance
    * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
    * minUniformBufferOffsetAlignment)
    *
    * @return HRESULT of the buffer mapping call
    */
    //VkDeviceSize buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
    //   if (minOffsetAlignment > 0) {
    //      return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
    //   }
    //   return instanceSize;
    //}

   buffer::buffer()
   {
      m_vkbuffer = VK_NULL_HANDLE;
      m_vkdevicememory = VK_NULL_HANDLE;

   }


   //void buffer::initialize_buffer(
   //   ::gpu::context* pgpucontext,
   //   VkDeviceSize instanceSize,
   //   uint32_t instanceCount,
   //   VkBufferUsageFlags usageFlags,
   //   VkMemoryPropertyFlags memoryPropertyFlags,
   //   VkDeviceSize minOffsetAlignment)
   //{
   //   m_pgpucontext = dynamic_cast <::gpu_directx12::context*>(pgpucontext);
   //   m_instanceSize = instanceSize;
   //   m_instanceCount = instanceCount;
   //   m_usageFlags = usageFlags;
   //   m_memoryPropertyFlags = memoryPropertyFlags;

   //   m_alignmentSize = getAlignment(instanceSize, minOffsetAlignment);
   //   m_bufferSize = m_alignmentSize * instanceCount;
   //   m_pgpucontext->createBuffer(m_bufferSize, m_usageFlags, m_memoryPropertyFlags, m_buffer, m_memory);
   //}

   buffer::~buffer() {
      //unmap();

      ::cast < context > pcontext = m_pgpucontext;
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



   /**
    * Copies the specified data to the mapped buffer. Default value writes whole buffer range
    *
    * @param data Pointer to the data to copy
    * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
    * range.
    * @param offset (Optional) Byte offset from beginning of mapped region
    *
    */
    //void buffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset) {
    //   assert(m_mapped && "Cannot copy to unmapped buffer");

    //   if (size == VK_WHOLE_SIZE) {
    //      memcpy(m_mapped, data, m_bufferSize);
    //   }
    //   else {
    //      char* memOffset = (char*)m_mapped;
    //      memOffset += offset;
    //      memcpy(memOffset, data, size);
    //   }
    //}

    ///**
    // * Flush a memory range of the buffer to make it visible to the pgpucontext
    // *
    // * @note Only required for non-coherent memory
    // *
    // * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
    // * complete buffer range.
    // * @param offset (Optional) Byte offset from beginning
    // *
    // * @return HRESULT of the flush call
    // */
    //HRESULT buffer::flush(VkDeviceSize size, VkDeviceSize offset)
    //{
    //   VkMappedMemoryRange mappedRange = {};
    //   mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    //   mappedRange.memory = m_memory;
    //   mappedRange.offset = offset;
    //   mappedRange.size = size;
    //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
    //   return vkFlushMappedMemoryRanges(m_pgpucontext->logicalDevice(), 1, &mappedRange);
    //}

    ///**
    // * Invalidate a memory range of the buffer to make it visible to the host
    // *
    // * @note Only required for non-coherent memory
    // *
    // * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
    // * the complete buffer range.
    // * @param offset (Optional) Byte offset from beginning
    // *
    // * @return HRESULT of the invalidate call
    // */
    //HRESULT buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) 
    //{
    //   VkMappedMemoryRange mappedRange = {};
    //   mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    //   mappedRange.memory = m_memory;
    //   mappedRange.offset = offset;
    //   mappedRange.size = size;
    //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
    //   return vkInvalidateMappedMemoryRanges(m_pgpucontext->logicalDevice(), 1, &mappedRange);
    //}

    ///**
    // * Create a buffer info descriptor
    // *
    // * @param size (Optional) Size of the memory range of the descriptor
    // * @param offset (Optional) Byte offset from beginning
    // *
    // * @return VkDescriptorBufferInfo of specified offset and range
    // */
    //VkDescriptorBufferInfo buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
    //   return VkDescriptorBufferInfo{
    //       m_buffer,
    //       offset,
    //       size,
    //   };
    //}

    /**
     * Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
     *
     * @param data Pointer to the data to copy
     * @param index Used in offset calculation
     *
     */


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



   void buffer::assign_cube_map(const void* dataSrc, int w, int h, int scan)
   {

      defer_stage([&](buffer* pbuffer)
         {
            
            pbuffer->_assign_cube_map(dataSrc, w, h, scan);

         });

   }



   void buffer::_assign_cube_map(const void* dataSrc, int wFull, int h, int scan)
   {
      ::cast < context > pcontext = m_pgpucontext;
      // copy all 6 faces into the staging buffer, one after another
      void* data;
      vkMapMemory(pcontext->logicalDevice(), m_vkdevicememory, 0, m_size, 0, &data);
      auto w = wFull / 6;
      for (int i = 0; i < 6; i++)
      {
         auto pimage32 = (image32_t*)data + w * h  * i;
         pimage32->vertical_swap_copy(w, h, w * 4,
            (image32_t*)(((unsigned char*)dataSrc) + w * 4 * i), scan);
      }
      vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);

   }

}  // namespace gpu_vulkan

