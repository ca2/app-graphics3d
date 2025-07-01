// From gpu_vulkan/memory_buffer.h by
// camilo on 2025-06-29 06:52 <3ThomasBorregaardSørensen!!
// Created by camilo on 2025-06-11 00:50 <3ThomasBorregaardSørensen!!
#pragma once


namespace gpu_directx12
{


   class CLASS_DECL_GPU_DIRECTX12 d3d12_resource :
      virtual public ::particle
   {
   public:


      ::comptr<ID3D12Resource> m_presource;
      //D3D12_VERTEX_BUFFER_VIEW& vbView,


      ::pointer < ::gpu_directx12::context > m_pcontext;
      //VkDeviceMemory m_vkdevicememory;
      //VkBuffer m_vkbuffer;


      d3d12_resource();
      ~d3d12_resource();


      virtual void initialize_resource(::gpu_directx12::context* pcontext, memsize size);


      virtual void assign(const void* pData, memsize size);
      virtual void assign(const ::block& block);


   };
   
   
   template < typename VERTEX >
   class vertex_buffer :
      virtual public d3d12_resource
   {
   public:


      D3D12_VERTEX_BUFFER_VIEW m_vertextbufferview;


      vertex_buffer()
      {

      }

      ~vertex_buffer() override
      {


      }


      virtual void initialize_vertex_buffer(::gpu_directx12::context* pcontext, ::collection::count c)
      {

         UINT size = (UINT) (c * sizeof(VERTEX));

         initialize_resource(pcontext, size);

         m_vertextbufferview.BufferLocation = m_presource->GetGPUVirtualAddress();
         m_vertextbufferview.SizeInBytes = (UINT)size;
         m_vertextbufferview.StrideInBytes = sizeof(VERTEX);


      }


   };


} // namespace gpu_directx12