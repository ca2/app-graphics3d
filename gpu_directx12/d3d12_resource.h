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


      

      struct state_t
      {
         D3D12_RESOURCE_STATES m_resourcestates;

         state_t() {}

         state_t(D3D12_RESOURCE_STATES resourcestates) : m_resourcestates(resourcestates) {}
      };


      ::comptr<ID3D12Resource> m_presource;
      //D3D12_VERTEX_BUFFER_VIEW& vbView,
      state_t m_state;
      bool m_bUpload = false;
      void *m_pPersistentMap;
      ::pointer < ::gpu_directx12::context > m_pcontext;
      //VkDeviceMemory m_vkdevicememory;
      //VkBuffer m_vkbuffer;


      d3d12_resource();
      ~d3d12_resource();


      virtual void initialize_d3d12_resource(::gpu::context *pgpucontext);


      virtual void create_named_buffer_with_upload(const ::scoped_string & scopedstrName, const ::block &block, ::pointer < d3d12_resource > & pd3d12resourceUpload);
      //virtual void initialize_resource(::gpu_directx12::context* pcontext, memsize size);
      virtual void create(const D3D12_RESOURCE_DESC *pDesc, const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr);
      virtual void create(const D3D12_HEAP_PROPERTIES *pHeapProperties, D3D12_HEAP_FLAGS HeapFlags,
                                           const D3D12_RESOURCE_DESC *pDesc,
                                           const D3D12_CLEAR_VALUE *pOptimizedClearValue = nullptr);
      virtual void create_upload(D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC *pDesc);


      virtual void set_name(const ::scoped_string &scopedstr);
      virtual void *get_persistent_map();

      virtual void map_assign(const void* pData, memsize size);
      virtual void map_assign(const ::block& block);

            void _new_state(ID3D12GraphicsCommandList *pcommandlist, D3D12_RESOURCE_STATES estate);

      virtual void _set_state(::gpu_directx12::command_buffer *pcommandbuffer, state_t state);
            virtual D3D12_GPU_VIRTUAL_ADDRESS gpu_address();
   };
   
   
   template < typename VERTEX >
   class vertex_buffer :
      virtual public d3d12_resource
   {
   public:


      D3D12_VERTEX_BUFFER_VIEW m_vertextbufferview;
      ::pointer<d3d12_resource> m_pd3d12resourceUpload;


      vertex_buffer()
      {

      }

      ~vertex_buffer() override
      {


      }


      virtual void initialize_vertex_buffer(::gpu_directx12::context* pcontext, ::collection::count c)
      {

         UINT size = (UINT) (c * sizeof(VERTEX));

         //initialize_d3de12_resource(pcontext, size);

         initialize_d3d12_resource(pcontext);

         create_named_buffer_with_upload("vertex_buffer", size, m_pd3d12resourceUpload);

         m_vertextbufferview.BufferLocation = this->gpu_address();
         m_vertextbufferview.SizeInBytes = (UINT)size;
         m_vertextbufferview.StrideInBytes = sizeof(VERTEX);


      }


   };

   
    class resource_state_guard
   {
    public:

      d3d12_resource* m_presource;
      ID3D12GraphicsCommandList* m_pcommandlist;
      d3d12_resource::state_t m_stateOld;

      resource_state_guard(ID3D12GraphicsCommandList *pcommandlist, d3d12_resource *presource, D3D12_RESOURCE_STATES estate)
      {

         m_presource = presource;
         m_pcommandlist = pcommandlist;
         m_stateOld = m_presource->m_state;

         m_presource->_new_state(m_pcommandlist, estate);

      }

      ~resource_state_guard()
      {

         m_presource->_new_state(m_pcommandlist, m_stateOld.m_resourcestates);

      }


   };



} // namespace gpu_directx12