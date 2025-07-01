// From gpu_vulkan/memory_buffer.cpp by
// camilo on 2025-06-29 06:58 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-11 00:50 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "context.h"
#include "device.h"
#include "d3d12_resource.h"
#include "physical_device.h"


namespace gpu_directx12
{


   d3d12_resource::d3d12_resource()
   {
      m_pcontext = nullptr;
      //m_vkbuffer = VK_NULL_HANDLE;
      //m_vkdevicememory = VK_NULL_HANDLE;
   }


   d3d12_resource::~d3d12_resource()
   {
      //if (m_vkbuffer != VK_NULL_HANDLE)
      //{
      //   vkDestroyBuffer(m_pcontext->logicalDevice(), m_vkbuffer, nullptr);
      //   m_vkbuffer = VK_NULL_HANDLE;
      //}
      //if (m_vkdevicememory != VK_NULL_HANDLE)
      //{
      //   vkFreeMemory(m_pcontext->logicalDevice(), m_vkdevicememory, nullptr);
      //   m_vkdevicememory = VK_NULL_HANDLE;
      //}
   }


   void d3d12_resource::initialize_resource(::gpu_directx12::context* pcontext, memsize size)
   {

      m_pcontext = pcontext;

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

      //const UINT bufferSize = sizeof(ImageBlendVertex) * vertexCount;
      const UINT bufferSize = (const UINT) size;

      CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
      CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

      ::cast < device > pdevice = m_pcontext->m_pgpudevice;

      pdevice->m_pdevice->CreateCommittedResource(
         &heapProps,
         D3D12_HEAP_FLAG_NONE,
         &bufferDesc,
         D3D12_RESOURCE_STATE_COMMON,
         nullptr,
         __interface_of(m_presource));



   }


   void d3d12_resource::assign(const void* data, memsize size)
   {

      if (!m_presource)
      {

         throw ::exception(error_wrong_state, "Buffer not initialized");

      }

      void* mappedData = nullptr;
      CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
      m_presource->Map(0, &readRange, &mappedData);
      memcpy(mappedData, data, size);
      m_presource->Unmap(0, nullptr);

   }


   void d3d12_resource::assign(const ::block& block)
   {

      assign(block.data(), block.size());

   }


   //void vertex_buffer::initialize_vertex_buffer(::gpu_vulkan::context* pcontext, memsize size)
   //{

   //   

   //}


} // namespace gpu_vulkan



