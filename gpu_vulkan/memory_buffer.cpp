#include "framework.h"
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
      m_vkbuffer = VK_NULL_HANDLE;
      m_vkdevicememory = VK_NULL_HANDLE;
      //m_pMap = nullptr;
   }


   memory_buffer::~memory_buffer()
   {

      ::cast < context > pcontext = m_pcontext;
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
   }


   void memory_buffer::initialize_memory_buffer(::gpu::context* pgpucontext, memsize size, bool bIndices)
   {

      ::gpu::memory_buffer::initialize_memory_buffer(pgpucontext, size, bIndices);

      //m_pcontext = pcontext;

      //m_size = size;

      //VkBuffer vertexBuffer;


      // //  0 l,t
      ////  1 r,t
      ////  2 r,b
      // //  3 l,b

      // ::geometry2d::matrix m;
      // m.scale(2.0 / size.cx(), 2.0 / size.cy());
      // m.translate(-1.0, -1.0);

      // ::double_point points[4];

      // points[0] = points1[0]; // top-left
      // points[1] = points1[1]; // top-right
      // points[2] = points1[2]; // bottom-right
      // points[3] = points1[3]; // bottom-left

      // m.transform(points[0]);
      // m.transform(points[1]);
      // m.transform(points[2]);
      // m.transform(points[3]);

      // float fA = color.f32_opacity();
      // //float fR = color.f32_red();
      // //float fG = color.f32_green();
      // //float fB = color.f32_blue();
      // float fR = color.f32_red() * fA;
      // float fG = color.f32_green() * fA;
      // float fB = color.f32_blue() * fA;


      // graphics::RectangleVertex quadVertices[] = {
      //    // Triangle 1
      //    {{(float)points[0].x(), (float)points[0].y(), g_z}, {fR, fG, fB, fA}}, // Red
      //    {{(float)points[1].x(), (float)points[1].y(), g_z}, {fR, fG, fB, fA}}, // Green
      //    {{(float)points[2].x(), (float)points[2].y(), g_z}, {fR, fG, fB, fA}}, // Blue
      //    // Triangle 2
      //    {{(float)points[0].x(), (float)points[0].y(), g_z}, {fR, fG, fB, fA}}, // Green
      //    {{(float)points[2].x(), (float)points[2].y(), g_z}, {fR, fG, fB, fA}}, // Yellow
      //    {{(float)points[3].x(), (float)points[3].y(), g_z}, {fR, fG, fB, fA}}, // Blue
      // };

      ::cast < context > pcontext = m_pcontext;

      VkBufferCreateInfo bufferInfo = {
         .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
         .size = (uint64_t)size,
         .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
         .sharingMode = VK_SHARING_MODE_EXCLUSIVE
      };
      vkCreateBuffer(pcontext->logicalDevice(), &bufferInfo, NULL, &m_vkbuffer);

      VkMemoryRequirements memReq{};
      vkGetBufferMemoryRequirements(pcontext->logicalDevice(), m_vkbuffer, &memReq);

      ::cast < device > pdevice = pcontext->m_pgpudevice;

      auto pphysicaldevice = pdevice->m_pphysicaldevice;

      uint32_t memTypeIndex = 0;
      VkPhysicalDeviceMemoryProperties memProps;
      vkGetPhysicalDeviceMemoryProperties(pphysicaldevice->m_physicaldevice, &memProps);

      for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
      {
         if ((memReq.memoryTypeBits & (1 << i)) &&
            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
            (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            memTypeIndex = i;
            break;
         }
      }

      //VkBufferCreateInfo bufferInfo = {
      //    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      //    .size = sizeof(quadVertices),
      //    .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      //    .sharingMode = VK_SHARING_MODE_EXCLUSIVE
      //};
      //vkCreateBuffer(device, &bufferInfo, NULL, &vertexBuffer);


      VkMemoryAllocateInfo allocInfo = {
          .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
          .allocationSize = memReq.size,
          .memoryTypeIndex = memTypeIndex
      };
      vkAllocateMemory(pcontext->logicalDevice(), &allocInfo, NULL, &m_vkdevicememory);
      vkBindBufferMemory(pcontext->logicalDevice(), m_vkbuffer, m_vkdevicememory, 0);

      //void* data;
      //vkMapMemory(device, *outMemory, 0, bufferInfo.size, 0, &data);
      //memcpy(data, quadVertices, sizeof(quadVertices));
      //vkUnmapMemory(device, *outMemory);

   }


   bool memory_buffer::is_initialized() const
   {

      return m_vkdevicememory != VK_NULL_HANDLE && m_vkbuffer != VK_NULL_HANDLE;

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
      VkDeviceSize instanceSize,
      uint32_t instanceCount,
      VkBufferUsageFlags usageFlags,
      VkMemoryPropertyFlags memoryPropertyFlags,
      VkDeviceSize minOffsetAlignment)
   {

      m_pcontext = pgpucontext;
      m_vkdevicesizeInstance = instanceSize;
      m_instanceCount = instanceCount;
      m_vkbufferusageflags = usageFlags;
      m_vkmemorypropertyflags = memoryPropertyFlags;

      m_vkdevicesizeAlignment = getAlignment(instanceSize, minOffsetAlignment);
      m_size = m_vkdevicesizeAlignment * instanceCount;
      
      ::cast < context > pcontext = m_pcontext;
      
      pcontext->createBuffer(
         m_size,
         m_vkbufferusageflags, 
         m_vkmemorypropertyflags, 
         m_vkbuffer, 
         m_vkdevicememory);

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
   void * memory_buffer::__map(memsize start, memsize count)
   {

      if (m_pMap)
      {

         return m_pMap;

      }

      assert(m_vkbuffer && m_vkdevicememory && "Called map on memory_buffer before create");

      ::cast < context > pcontext = m_pcontext;

      auto vkresult = vkMapMemory(pcontext->logicalDevice(),
         m_vkdevicememory, 
         (VkDeviceSize) start, 
         (VkDeviceSize)count,
         0,
         &m_pMap);

      if(vkresult != VK_SUCCESS)
      {
         throw ::exception(error_failed, "Failed to map memory_buffer");
      }  

      return m_pMap;

   }

   /**
    * Unmap a mapped memory range
    *
    * @note Does not return a result as vkUnmapMemory can't fail
    */
   void memory_buffer::__unmap() 
   {
      
      if (m_pMap) 
      {
         
         ::cast < context > pcontext = m_pcontext;
         
         vkUnmapMemory(pcontext->logicalDevice(), m_vkdevicememory);

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
      VkMappedMemoryRange mappedRange = {};
      mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      mappedRange.memory = m_vkdevicememory;
      mappedRange.offset = offset;
      mappedRange.size = size;
      
      ::cast < context > pcontext = m_pcontext;

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
   VkResult memory_buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) 
   {
      VkMappedMemoryRange mappedRange = {};
      mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
      mappedRange.memory = m_vkdevicememory;
      mappedRange.offset = offset;
      mappedRange.size = size;
      
      ::cast < context > pcontext = m_pcontext;

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
   VkDescriptorBufferInfo memory_buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
      return VkDescriptorBufferInfo{
          m_vkbuffer,
          offset,
          size,
      };
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

