// From gpu_vulkan/memory_buffer.cpp by
// camilo on 2025-06-29 06:58 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-11 00:50 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "command_buffer.h"
#include "context.h"
#include "device.h"
#include "d3d12_resource.h"
#include "physical_device.h"
#include "renderer.h"
#include "bred/gpu/frame.h"


namespace gpu_directx12
{


   d3d12_resource::d3d12_resource()
   {
      m_state.m_resourcestates = D3D12_RESOURCE_STATE_COMMON;
      m_pPersistentMap = nullptr;
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


   void d3d12_resource::initialize_d3d12_context(::gpu::context * pgpucontext)
   {

      m_pcontext = pgpucontext;

   }

   void d3d12_resource::create_named_buffer_with_upload(const ::scoped_string &scopedstrName, const ::block &block,
                                                        ::pointer<d3d12_resource> &pd3d12resourceUpload)
   {

                  // Create default heap resources
      CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);

      CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(block.size());

      //m_pd3d12resourceMemoryBuffer->create(&defaultHeap, D3D12_HEAP_FLAG_NONE, &vbDesc);

      create(&defaultHeap, D3D12_HEAP_FLAG_NONE, &vbDesc);
      set_name(scopedstrName);
      // pdevice->defer_throw_hresult(hresultCreateCommittedResource);

      // Create upload heap resources
      // CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);
      m_pcontext->_construct_new(pd3d12resourceUpload);
      pd3d12resourceUpload->create_upload(D3D12_HEAP_FLAG_NONE, &vbDesc);

      // pdevice->defer_throw_hresult(hresultCreateCommittedResourceUpload);

      pd3d12resourceUpload->set_name(scopedstrName + " Upload");


      pd3d12resourceUpload->map_assign(block);

      ::cast<::gpu_directx12::command_buffer> pcommandbufferLoading;
      ::cast<::gpu_directx12::renderer> prenderer = m_pcontext->m_pgpurenderer;

      if (::is_set(prenderer))
      {

         pcommandbufferLoading =  prenderer->getLoadAssetsCommandBuffer();
      }
      if (!pcommandbufferLoading)
      {

         pcommandbufferLoading = m_pcontext->m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_frame());

      }

      pcommandbufferLoading->_copy_buffer(this, pd3d12resourceUpload,
                                          block.size());


   }

      void d3d12_resource::create(const D3D12_RESOURCE_DESC *pDesc, const D3D12_CLEAR_VALUE *pOptimizedClearValue) 
      {

         CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);
         
         create(&defaultHeap, D3D12_HEAP_FLAG_NONE, pDesc, pOptimizedClearValue);

      }


   void d3d12_resource::create(const D3D12_HEAP_PROPERTIES *pHeapProperties,
                                                D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC *pDesc,
                                                const D3D12_CLEAR_VALUE *pOptimizedClearValue)
   {

      //if (m_state.m_resourcestates == )
      ::cast<::gpu_directx12::device> pdevice = m_pcontext->m_pgpudevice;
      
      auto hrCreateCommittedResource =pdevice->m_pdevice->CreateCommittedResource(
         pHeapProperties, HeapFlags,
         pDesc, m_state.m_resourcestates,
         pOptimizedClearValue, __interface_of(m_presource));

      pdevice->defer_throw_hresult(hrCreateCommittedResource);

   }


   void d3d12_resource::create_upload(D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC *pDesc)
   {
      m_state.m_resourcestates = D3D12_RESOURCE_STATE_GENERIC_READ;
      CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);
      create(&uploadHeap, HeapFlags, pDesc);
   }
   

   //void d3d12_resource::initialize_resource(::gpu_directx12::context* pcontext, memsize size)
   //{

   //   m_pcontext = pcontext;

   //   //VkBuffer vertexBuffer;


   //   // //  0 l,t
   //   ////  1 r,t
   //   ////  2 r,b
   //   // //  3 l,b

   //   // ::geometry2d::matrix m;
   //   // m.scale(2.0 / size.cx, 2.0 / size.cy);
   //   // m.translate(-1.0, -1.0);

   //   // ::double_point points[4];

   //   // points[0] = points1[0]; // top-left
   //   // points[1] = points1[1]; // top-right
   //   // points[2] = points1[2]; // bottom-right
   //   // points[3] = points1[3]; // bottom-left

   //   // m.transform(points[0]);
   //   // m.transform(points[1]);
   //   // m.transform(points[2]);
   //   // m.transform(points[3]);

   //   // float fA = color.f32_opacity();
   //   // //float fR = color.f32_red();
   //   // //float fG = color.f32_green();
   //   // //float fB = color.f32_blue();
   //   // float fR = color.f32_red() * fA;
   //   // float fG = color.f32_green() * fA;
   //   // float fB = color.f32_blue() * fA;


   //   // graphics::RectangleVertex quadVertices[] = {
   //   //    // Triangle 1
   //   //    {{(float)points[0].x, (float)points[0].y, g_z}, {fR, fG, fB, fA}}, // Red
   //   //    {{(float)points[1].x, (float)points[1].y, g_z}, {fR, fG, fB, fA}}, // Green
   //   //    {{(float)points[2].x, (float)points[2].y, g_z}, {fR, fG, fB, fA}}, // Blue
   //   //    // Triangle 2
   //   //    {{(float)points[0].x, (float)points[0].y, g_z}, {fR, fG, fB, fA}}, // Green
   //   //    {{(float)points[2].x, (float)points[2].y, g_z}, {fR, fG, fB, fA}}, // Yellow
   //   //    {{(float)points[3].x, (float)points[3].y, g_z}, {fR, fG, fB, fA}}, // Blue
   //   // };

   //   //const UINT bufferSize = sizeof(ImageBlendVertex) * vertexCount;
   //   const UINT bufferSize = (const UINT) size;

   //   CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
   //   CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

   //   ::cast < device > pdevice = m_pcontext->m_pgpudevice;

   //   pdevice->m_pdevice->CreateCommittedResource(
   //      &heapProps,
   //      D3D12_HEAP_FLAG_NONE,
   //      &bufferDesc,
   //      D3D12_RESOURCE_STATE_COMMON,
   //      nullptr,
   //      __interface_of(m_presource));



   //}

   void d3d12_resource::set_name(const ::scoped_string & scopedstr)
   {

      ::wstring wstr;

      wstr = scopedstr;

      m_presource->SetName(wstr);

   }

   void * d3d12_resource::get_persistent_map()
   {

      if (m_pPersistentMap)
      {

         return m_pPersistentMap;
      }

      //;
//      ;
  //    ;
    //  ;
      //void *pMap = nullptr;
               // Persistent map
      HRESULT hresultResourceMap = m_presource->Map(0, nullptr, reinterpret_cast<void **>(&m_pPersistentMap));

      ::defer_throw_hresult(hresultResourceMap);
      // m_currentOffset = 0;
      return m_pPersistentMap;
   }

   void d3d12_resource::map_assign(const void* data, memsize size)
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


   void d3d12_resource::map_assign(const ::block& block)
   {

      map_assign(block.data(), block.size());

   }


   //void vertex_buffer::initialize_vertex_buffer(::gpu_vulkan::context* pcontext, memsize size)
   //{

   //   

   //}
   D3D12_GPU_VIRTUAL_ADDRESS d3d12_resource::gpu_address()
   {

      return m_presource->GetGPUVirtualAddress();

   }


   void d3d12_resource::_new_state(ID3D12GraphicsCommandList *pcommandlist, D3D12_RESOURCE_STATES estateNew)
   {

      if (m_state.m_resourcestates != estateNew)
      {

         // Transition to copy source
         D3D12_RESOURCE_BARRIER barrier =
            CD3DX12_RESOURCE_BARRIER::Transition(m_presource,
                                                 m_state.m_resourcestates, // Adjust if needed
                                                 estateNew);

         pcommandlist->ResourceBarrier(1, &barrier);

         m_state.m_resourcestates = estateNew;

         // pcommandBuffer->submit_command_buffer();
      }
   }


   void d3d12_resource::_set_state(::gpu_directx12::command_buffer *pcommandbuffer, state_t state)
   {

      _new_state(pcommandbuffer->m_pcommandlist, state.m_resourcestates);

   }




} // namespace gpu_vulkan



