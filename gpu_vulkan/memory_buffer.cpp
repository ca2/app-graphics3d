#include "framework.h"
#include "buffer.h"
#include "context.h"
#include "physical_device.h"
#include "app-graphics3d/gpu_vulkan/context.h"
/*
 * Encapsulates a vulkan memory_buffer
 *
 * Initially based off VulkanBuffer by Sascha Willems -
 * https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanBuffer.h
 */

#include "memory_buffer.h"


namespace gpu_vulkan
{


   memory_buffer::memory_buffer()
   {
      //m_pcontext = nullptr;
      //m_vkbuffer = VK_NULL_HANDLE;
      //m_vkdevicememory = VK_NULL_HANDLE;
      //m_pMap = nullptr;
   }


   memory_buffer::~memory_buffer()
   {

   }



   void memory_buffer::on_initialize_memory_buffer(const void* dataStatic, memsize sizeStatic)
   {

      if (sizeStatic > 0)
      {

         if (m_etype == e_type_vertex_buffer)
         {

            ::cast < context > pcontext = m_pcontext;

            m_pbuffer = pcontext->create_buffer(
               sizeStatic,
               VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
               m_bDynamic ?
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 
               | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
               : 
               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );



            ////m_iVertexCount = static_cast<uint32_t>(vertexes.size());
            ////assert(m_iVertexCount >= 3 && "Vertex count must be at least 3");
            ////VkDeviceSize bufferSize = sizeof(vertexes[0]) * m_iVertexCount;
            //VkDeviceSize bufferSize = sizeStatic;
            ////uint32_t vertexSize = sizeof(vertexes[0]);

            //auto pbufferStaging = øcreate_new < ::gpu_vulkan::memory_buffer >();

            //pbufferStaging->_initialize_buffer(
            //   m_pcontext,
            //   bufferSize,
            //   VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            //   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            //);

            //pbufferStaging->map();
            //pbufferStaging->writeToBuffer((void*)dataStatic);


            //::cast < gpu_vulkan::context > pcontext = m_pcontext;

            //pcontext->copyBuffer(pbufferStaging->m_vk, this->getBuffer(), bufferSize);

         }
         else if (m_etype == e_type_index_buffer)
         {

            ::cast < context > pcontext = m_pcontext;

            m_pbuffer = pcontext->create_buffer(
               sizeStatic,
               VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
               m_bDynamic ?
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
               | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
               :
               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            );

         }

      }

      if (m_pbuffer && ::is_set(dataStatic))
      {

         m_pbuffer->assign(dataStatic, sizeStatic);


      }


      //      //VkDeviceSize bufferSize = sizeStatic;

      //      ////uint32_t indexSize = sizeof(indexes[0]);

      //      //auto pbufferStaging = øcreate_new < ::gpu_vulkan::memory_buffer>();

      //      //pbufferStaging->_initialize_buffer(
      //      //   m_pcontext,
      //      //   bufferSize,
      //      //   VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      //      //   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
      //      //   ;

      //      //pbufferStaging->map();
      //      //pbufferStaging->writeToBuffer((void*)dataStatic);

      //      ////to pbufferIndex = øcreate_new < ::gpu_vulkan::memory_buffer>();

      //      //_initialize_buffer(
      //      //   m_pcontext,
      //      //   bufferSize,
      //      //   VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      //      //   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      //      ////m_pbufferIndex = pbufferIndex;

      //      //::cast < ::gpu_vulkan::context > pcontext = m_pcontext;

      //      //pcontext->copyBuffer(pbufferStaging->getBuffer(), this->getBuffer(), bufferSize);

      //   }


      //}
      //else
      //{

      //   if (m_etype == e_type_vertex_buffer
      //      || m_etype == e_type_index_buffer)
      //   {

      //      ::cast < context > pcontext = m_pcontext;

      //      VkBufferUsageFlags usage;

      //      if (m_etype == e_type_vertex_buffer)
      //      {

      //         usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

      //      }
      //      else
      //      {

      //         usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

      //      }

      //      VkBufferCreateInfo bufferInfo = {
      //         .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      //         .size = (uint64_t)this->total_size_in_bytes(),
      //         .usage = usage,
      //         .sharingMode = VK_SHARING_MODE_EXCLUSIVE
      //      };
      //      vkCreateBuffer(pcontext->logicalDevice(), &bufferInfo, NULL, &m_vkbuffer);

      //      VkMemoryRequirements memReq{};
      //      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), m_vkbuffer, &memReq);

      //      ::cast < device > pdevice = pcontext->m_pgpudevice;

      //      auto pphysicaldevice = pdevice->m_pphysicaldevice;

      //      uint32_t memTypeIndex = 0;
      //      VkPhysicalDeviceMemoryProperties memProps;
      //      vkGetPhysicalDeviceMemoryProperties(pphysicaldevice->m_physicaldevice, &memProps);

      //      for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
      //      {
      //         if ((memReq.memoryTypeBits & (1 << i)) &&
      //            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
      //            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
      //            memTypeIndex = i;
      //            break;
      //         }
      //      }

      //      //VkBufferCreateInfo bufferInfo = {
      //      //    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      //      //    .size = sizeof(quadVertices),
      //      //    .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      //      //    .sharingMode = VK_SHARING_MODE_EXCLUSIVE
      //      //};
      //      //vkCreateBuffer(device, &bufferInfo, NULL, &vertexBuffer);


      //      VkMemoryAllocateInfo allocInfo = {
      //          .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      //          .allocationSize = memReq.size,
      //          .memoryTypeIndex = memTypeIndex
      //      };
      //      vkAllocateMemory(pcontext->logicalDevice(), &allocInfo, NULL, &m_vkdevicememory);
      //      vkBindBufferMemory(pcontext->logicalDevice(), m_vkbuffer, m_vkdevicememory, 0);

      //      if (dataStatic && sizeStatic > 0)
      //      {

      //         void* data;
      //         vkMapMemory(pcontext->logicalDevice(), m_vkdevicememory, 0, bufferInfo.size, 0, &data);
      //         memcpy(data, dataStatic, sizeStatic);
      //         vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);

      //      }

   //}

//}

   }


   bool memory_buffer::is_initialized() const
   {

      return m_pbuffer && m_pbuffer->is_initialized();

   }





   /**
    * Returns the minimum m_vkinstance size required to be compatible with devices minOffsetAlignment
    *
    * @param instanceSize The size of an m_vkinstance
    * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
    * minUniformBufferOffsetAlignment)
    *
    * @return VkResult of the memory_buffer mapping call
    */
   VkDeviceSize memory_buffer::getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
      if (minOffsetAlignment > 0) {
         return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
      }
      return instanceSize;
   }


   void memory_buffer::_initialize_buffer(
      ::gpu::context* pgpucontext,
      VkDeviceSize size,
      VkBufferUsageFlags usageFlags,
      VkMemoryPropertyFlags memoryPropertyFlags,
      VkDeviceSize minOffsetAlignment)
   {

      m_pcontext = pgpucontext;
      m_vkdevicesizeInstance = 0;
      m_instanceCount = 0;
      m_vkbufferusageflags = usageFlags;
      m_vkmemorypropertyflags = memoryPropertyFlags;

      m_vkdevicesizeAlignment = getAlignment(size, minOffsetAlignment);
      m_size = size;

      ::cast < context > pcontext = m_pcontext;

      auto pbuffer = pcontext->create_buffer(
         m_size,
         m_vkbufferusageflags,
         m_vkmemorypropertyflags);

      m_pbuffer = pbuffer;

   }




   /**
    * Map a memory range of this memory_buffer. If successful, mapped points to the specified memory_buffer range.
    *
    * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
    * memory_buffer range.
    * @param offset (Optional) Byte offset from beginning
    *
    * @return VkResult of the memory_buffer mapping call
    */
   void* memory_buffer::_map(memsize start, memsize count)
   {

      if (m_pMap)
      {

         return m_pMap;

      }

      assert(is_initialized() && "Called map on memory_buffer before create");

      ::cast < context > pcontext = m_pcontext;

      m_pMap = m_pbuffer->map();

      return m_pMap;

   }

   /**
    * Unmap a mapped memory range
    *
    * @note Does not return a result as vkUnmapMemory can't fail
    */
   void memory_buffer::_unmap()
   {

      if (m_pMap)
      {

         m_pbuffer->unmap();

         m_pMap = nullptr;

      }

   }


   /**
    * Copies the specified data to the mapped memory_buffer. Default value writes whole memory_buffer range
    *
    * @param data Pointer to the data to copy
    * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete memory_buffer
    * range.
    * @param offset (Optional) Byte offset from beginning of mapped region
    *
    */
   void memory_buffer::writeToBuffer(void* data, VkDeviceSize size, VkDeviceSize offset)
   {

      assert(m_pMap && "Cannot copy to unmapped memory_buffer");

      if (size == VK_WHOLE_SIZE)
      {

         memcpy(m_pMap, data, m_size);

      }
      else
      {

         char* memOffset = (char*)m_pMap;

         memOffset += offset;

         memcpy(memOffset, data, size);

      }

   }


   void memory_buffer::on_set_memory_buffer(const void* dataStatic, memsize sizeStatic)
   {

      auto pmap = _map(0, sizeStatic);
      
      writeToBuffer((void*)dataStatic, sizeStatic);
      
      _unmap();

   }


   void memory_buffer::_on_set_memory_buffer(const void* dataStatic, memsize sizeStatic)
   {

      ::cast < context > pcontext = m_pcontext;

      vkBindBufferMemory(
         pcontext->logicalDevice(), 
         m_pbuffer->m_vkbuffer, 
         m_pbuffer->m_vkdevicememory, 0);

      auto pmap = _map(0, sizeStatic);

      writeToBuffer((void*)dataStatic, sizeStatic);

      _unmap();

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
   VkResult memory_buffer::flush(VkDeviceSize size, VkDeviceSize offset)
   {
      //VkMappedMemoryRange mappedRange = {};
      //mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      //mappedRange.memory = m_vkdevicememory;
      //mappedRange.offset = offset;
      //mappedRange.size = size;

      //::cast < context > pcontext = m_pcontext;

      return m_pbuffer->flush(size, offset);

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
   VkResult memory_buffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
   {
      //VkMappedMemoryRange mappedRange = {};
      //mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      //mappedRange.memory = m_vkdevicememory;
      //mappedRange.offset = offset;
      //mappedRange.size = size;

      //::cast < context > pcontext = m_pcontext;

      //return vkInvalidateMappedMemoryRanges(pcontext->logicalDevice(), 1, &mappedRange);

      return m_pbuffer->invalidate(size, offset);
   }

   /**
    * Create a memory_buffer info descriptor
    *
    * @param size (Optional) Size of the memory range of the descriptor
    * @param offset (Optional) Byte offset from beginning
    *
    * @return VkDescriptorBufferInfo of specified offset and range
    */
   VkDescriptorBufferInfo memory_buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset)
   {
      return m_pbuffer->descriptorInfo(size, offset);
   }

   /**
    * Copies "instanceSize" bytes of data to the mapped memory_buffer at an offset of index * alignmentSize
    *
    * @param data Pointer to the data to copy
    * @param index Used in offset calculation
    *
    */
   void memory_buffer::writeToIndex(void* data, int index) {
      writeToBuffer(data, m_vkdevicesizeInstance, index * m_vkdevicesizeAlignment);
   }

   /**
    *  Flush the memory range at index * alignmentSize of the memory_buffer to make it visible to the pgpucontext
    *
    * @param index Used in offset calculation
    *
    */
   VkResult memory_buffer::flushIndex(int index)
   {

      return flush(m_vkdevicesizeAlignment, index * m_vkdevicesizeAlignment);

   }

   /**
    * Create a memory_buffer info descriptor
    *
    * @param index Specifies the region given by index * alignmentSize
    *
    * @return VkDescriptorBufferInfo for m_vkinstance at index
    */
   VkDescriptorBufferInfo memory_buffer::descriptorInfoForIndex(int index)
   {

      return descriptorInfo(m_vkdevicesizeAlignment, index * m_vkdevicesizeAlignment);

   }

   /**
    * Invalidate a memory range of the memory_buffer to make it visible to the host
    *
    * @note Only required for non-coherent memory
    *
    * @param index Specifies the region to invalidate: index * alignmentSize
    *
    * @return VkResult of the invalidate call
    */
   VkResult memory_buffer::invalidateIndex(int index)
   {

      return invalidate(m_vkdevicesizeAlignment, index * m_vkdevicesizeAlignment);

   }



   //void memory_buffer::assign(const void* pData, memsize size)
   //{

   //   if (m_vkdevicememory == VK_NULL_HANDLE || m_vkbuffer == VK_NULL_HANDLE)
   //   {

   //      throw ::exception(error_wrong_state, "Buffer not initialized");

   //   }

   //   void* data;

   //   vkMapMemory(m_pcontext->logicalDevice(), m_vkdevicememory, 0, size, 0, &data);

   //   memcpy(data, pData, size);

   //   vkUnmapMemory(m_pcontext->logicalDevice(), m_vkdevicememory);

   //}


   //void memory_buffer::assign(const ::block& block)
   //{

   //   assign(block.data(), block.size()); 

   //}


   //void* memory_buffer::__map(memsize start, memsize count)
   //{

   //   void* data;

   //   ::cast < context > pcontext = m_pcontext;


   //   vkMapMemory(
   //      pcontext->logicalDevice(),
   //      m_vkdevicememory,
   //      start,
   //      count > 0 ? count : (m_size + count + 1),
   //      0,
   //      &data);

   //   m_pMap = data;

   //   return data;


   //}


   //void memory_buffer::__unmap()
   //{

   //   //if (!m_pMap)
   //   //{

   //   //   return;

   //   //}

   //   ::cast < context > pcontext = m_pcontext;

   //   vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);



   //   //m_pMap = nullptr;

   //}



}  // namespace graphics3d_vulkan

