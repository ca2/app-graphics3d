// Created by camilo on 2025-06-11 01:08 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "context.h"
#include "memory_buffer.h"
#include "model_buffer.h"
#include "acme/prototype/geometry2d/matrix.h"
#include "bred/graphics3d/types.h"


namespace gpu_vulkan
{


   model_buffer::model_buffer()
   {

      m_iVertexCount = 0;

      m_iIndexCount = 0;

   }


   model_buffer::~model_buffer()
   {


   }


   void model_buffer::create_rectangle(::gpu_vulkan::context* pcontext, bool bIndexed, bool bYSwap)
   {

      float y0;
      float y1;

      if (bYSwap)
      {

         y0 = 1.0f;
         y1 = -1.0f;


      }
      else
      {

         y0 = -1.0f;
         y1 = 1.0f;

      }


      // Fullscreen quad vertex data
      float quadVertex[] = {
         // pos     // uv
         -1.0f, y0, 0.0f, 0.0f,
          1.0f, y0, 1.0f, 0.0f,
          1.0f, y1, 1.0f, 1.0f,
         -1.0f, y1, 0.0f, 1.0f,
      };

      m_iVertexCount = 6;


      pcontext->defer_construct_new(m_pbufferVertex, ::as_memory_block(quadVertex));

      if (bIndexed)
      {

         uint16_t quadIndex[] = {
            0, 1, 2,
            2, 3, 0
         };

         m_iIndexCount = 6;

         pcontext->defer_construct_new(m_pbufferIndex, ::as_memory_block(quadIndex));

      }

      //auto vertexSize = sizeof(quadVertices);
      //auto indexSize = sizeof(quadIndices);

     /* __defer_construct_new(m_pbufferVertex);
      __defer_construct_new(m_pbufferIndex);

      m_pbufferVertex->initialize_memory_buffer(pcontext, vertexSize);
      m_pbufferIndex->initialize_memory_buffer(pcontext, indexSize);

      m_pbufferVertex->set(quadVertices);
      m_pbufferIndex->set(quadIndices);*/

      //VkBufferCreateInfo bufferInfo = {
      //   .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
      //   .size = vertexSize,
      //   .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      //   .sharingMode = VK_SHARING_MODE_EXCLUSIVE
      //};
      //vkCreateBuffer(device, &bufferInfo, NULL, vertexBuffer);

      //VkMemoryRequirements memReq;
      //vkGetBufferMemoryRequirements(device, *vertexBuffer, &memReq);

      //VkMemoryAllocateInfo allocInfo = {
      //   .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
      //   .allocationSize = memReq.size,
      //   .memoryTypeIndex = 0
      //};

      //VkPhysicalDeviceMemoryProperties memProps;
      //vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
      //for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
      //   if ((memReq.memoryTypeBits & (1 << i)) &&
      //      (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
      //      (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
      //      allocInfo.memoryTypeIndex = i;
      //      break;
      //   }
      //}

      //vkAllocateMemory(device, &allocInfo, NULL, vertexMemory);
      //vkBindBufferMemory(device, *vertexBuffer, *vertexMemory, 0);

      //void* data;
      //vkMapMemory(device, *vertexMemory, 0, vertexSize, 0, &data);
      //memcpy(data, quadVertices, (size_t)vertexSize);
      //vkUnmapMemory(device, *vertexMemory);

      //// Index buffer
      //bufferInfo.size = indexSize;
      //bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
      //vkCreateBuffer(device, &bufferInfo, NULL, indexBuffer);
      //vkGetBufferMemoryRequirements(device, *indexBuffer, &memReq);
      //allocInfo.allocationSize = memReq.size;

   //   for (uint32_t i = 0; i < memProps.memoryTypeCount; i++) {
   //      if ((memReq.memoryTypeBits & (1 << i)) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) &&
   //         (memProps.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
   //         allocInfo.memoryTypeIndex = i;
   //         break;
   //      }
   //   }

   //   vkAllocateMemory(device, &allocInfo, NULL, indexMemory);
   //   vkBindBufferMemory(device, *indexBuffer, *indexMemory, 0);
   //   vkMapMemory(device, *indexMemory, 0, indexSize, 0, &data);
   //   memcpy(data, quadIndices, (size_t)indexSize);
   //   vkUnmapMemory(device, *indexMemory);
   //}
   }


   //void model_buffer::vec3_color_create_rectangle(::gpu_vulkan::context* pcontext, int iIndexCount)
   //{

   //   m_iVertexCount = 6;

   //   auto size = sizeof(vec3_color) * m_iVertexCount;

   //   pcontext->defer_construct_new(m_pbufferVertex, size);

   //   if (iIndexCount > 0)
   //   {

   //      m_iIndexCount = iIndexCount;

   //      pcontext->defer_construct_new(m_pbufferIndex, iIndexCount * sizeof(unsigned short));

   //   }

   //}


   void model_buffer::sequence3_color_set_rectangle(
      ::gpu_vulkan::context* pcontext,
      const ::double_point points1[4],
      const  ::color::color& color,
      const ::double_size& size)
   {


      //void editQuadVertexBuffer(
      //   VkDevice device,
      //   VkDeviceMemory outMemory,
      //   const ::double_point points1[4],
      //   const  ::color::color & color,
      //   const ::double_size & size)
      //{
      //   VkBuffer vertexBuffer;


         //  0 l,t
        //  1 r,t
        //  2 r,b
         //  3 l,b

      ::geometry2d::matrix m;
      m.scale(2.0 / size.cx(), 2.0 / size.cy());
      m.translate(-1.0, -1.0);

      ::double_point points[4];

      points[0] = points1[0]; // top-left
      points[1] = points1[1]; // top-right
      points[2] = points1[2]; // bottom-right
      points[3] = points1[3]; // bottom-left

      m.transform(points[0]);
      m.transform(points[1]);
      m.transform(points[2]);
      m.transform(points[3]);

      float fA = color.f32_opacity();
      //float fR = color.f32_red();
      //float fG = color.f32_green();
      //float fB = color.f32_blue();
      float fR = color.f32_red() * fA;
      float fG = color.f32_green() * fA;
      float fB = color.f32_blue() * fA;

      float g_z = 0.0f; // Assuming z is 0 for 2D rendering, adjust as needed

      graphics3d::sequence3_color quadVertices[] = {
         // Triangle 1
         {{(float)points[0].x(), (float)points[0].y(), g_z}, {fR, fG, fB, fA}}, // Red
         {{(float)points[1].x(), (float)points[1].y(), g_z}, {fR, fG, fB, fA}}, // Green
         {{(float)points[2].x(), (float)points[2].y(), g_z}, {fR, fG, fB, fA}}, // Blue
         // Triangle 2
         {{(float)points[0].x(), (float)points[0].y(), g_z}, {fR, fG, fB, fA}}, // Green
         {{(float)points[2].x(), (float)points[2].y(), g_z}, {fR, fG, fB, fA}}, // Yellow
         {{(float)points[3].x(), (float)points[3].y(), g_z}, {fR, fG, fB, fA}}, // Blue
      };


      m_pbufferVertex->assign(::as_memory_block(quadVertices));

   }




   void model_buffer::bind(::gpu_vulkan::command_buffer* pcommandbuffer)
   {
      if (m_pbufferVertex)
      {
         VkDeviceSize offsets[] = { 0 };

         vkCmdBindVertexBuffers(
            pcommandbuffer->m_vkcommandbuffer,
            0, 1,
            &m_pbufferVertex->m_vkbuffer, offsets);

         if (m_pbufferIndex)
         {

            vkCmdBindIndexBuffer(
               pcommandbuffer->m_vkcommandbuffer,
               m_pbufferIndex->m_vkbuffer, 0, VK_INDEX_TYPE_UINT16);

         }

      }

   }


   void model_buffer::draw(::gpu_vulkan::command_buffer* pcommandbuffer)
   {
      if (m_pbufferVertex)
      {
         if (m_pbufferIndex)
         {
            vkCmdDrawIndexed(
               pcommandbuffer->m_vkcommandbuffer,
               m_iIndexCount, 1, 0, 0, 0);
         }
         else
         {
            vkCmdDraw(
               pcommandbuffer->m_vkcommandbuffer,
               m_iVertexCount, 1, 0, 0);
         }
      }
   }


} // namespace gpu_vulkan



