// From gpu_directx11/memory_buffer.cpp by
// camilo on 2025-07-16 06:23 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "device.h"
#include "frame_storage.h"
#include "gpu_directx12/context.h"
#include "model_buffer.h"
#include "bred/gpu/frame.h"
/*
 * Encapsulates a directx11 memory_buffer
 *
 * Initially based off DirectX11Buffer by Sascha Willems -
 * https://github.com/SaschaWillems/DirectX11/blob/master/base/DirectX11Buffer.h
 */

#include "memory_buffer.h"


namespace gpu_directx12
{


   memory_buffer::memory_buffer()
   {

      ///m_bStatic = false;

   }


   memory_buffer::~memory_buffer()
   {

      unmap();

   }


   void memory_buffer::on_initialize_memory_buffer(const ::block & block)
   {

      auto etype = m_etype;

      m_bDynamic = true;

      ::cast < ::gpu_directx12::device > pdevice = m_pcontext->m_pgpudevice;

      if (etype == ::gpu::memory_buffer::e_type_shared_dynamic_vertex_buffer)
      {

         UINT bufSize = m_size; // number of vertexes the buffer can hold

         // Describe heap (UPLOAD so CPU can write directly)
         D3D12_HEAP_PROPERTIES heapProps = {};
         heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

         D3D12_RESOURCE_DESC desc = {};
         desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
         desc.Width = bufSize;
         desc.Height = 1;
         desc.DepthOrArraySize = 1;
         desc.MipLevels = 1;
         desc.SampleDesc.Count = 1;
         desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

         ::cast<::gpu_directx12::context> pcontext = m_pcontext;

         pcontext->_construct_new(m_pd3d12resourceMemoryBuffer);

         // Create upload buffer
         HRESULT hresultCreateCommittedResource =
            pdevice->m_pdevice->CreateCommittedResource(
               &heapProps,
               D3D12_HEAP_FLAG_NONE,
               &desc,
               D3D12_RESOURCE_STATE_GENERIC_READ,
               nullptr,
               __interface_of(m_pd3d12resourceMemoryBuffer->m_presource));

         ::defer_throw_hresult(hresultCreateCommittedResource);

         m_pd3d12resourceMemoryBuffer->set_name(m_strMemoryBufferName);

         m_pMap = m_pd3d12resourceMemoryBuffer->get_persistent_map();

         m_bDynamic = true;

      }
      else if (etype == ::gpu::memory_buffer::e_type_vertex_buffer)
      {

         if (block.data())
         {

            m_bDynamic = false;

            //auto vertexBufferSize = (UINT)block.size();

            ::cast<::gpu_directx12::context> pcontext = m_pcontext;

            pcontext->_construct_new(m_pd3d12resourceMemoryBuffer);

            m_pd3d12resourceMemoryBuffer->create_named_buffer_with_upload(m_strMemoryBufferName, block, m_pd3d12resourceMemoryBufferUpload);


         }

      }
      else if (etype == ::gpu::memory_buffer::e_type_index_buffer)
      {

         if (block.data())
         {

            m_bDynamic = false;

                        ::cast<::gpu_directx12::context> pcontext = m_pcontext;

            pcontext->_construct_new(m_pd3d12resourceMemoryBuffer);

            m_pd3d12resourceMemoryBuffer->create_named_buffer_with_upload(m_strMemoryBufferName, block,
                                                                          m_pd3d12resourceMemoryBufferUpload);



            //auto indexBufferSize = (UINT)block.size();

            
            //CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
            //::cast<::gpu_directx12::context> pcontext = m_pcontext;

            //pcontext->_construct_new(m_pd3d12resourceMemoryBuffer);
            //m_pd3d12resourceMemoryBuffer->create(&ibDesc);
            ////auto hresultCreateCommittedResource = pdevice->m_pdevice->CreateCommittedResource(
            ////   &defaultHeap, D3D12_HEAP_FLAG_NONE,
            ////   &ibDesc, D3D12_RESOURCE_STATE_COMMON,
            ////   nullptr, __interface_of(m_presourceMemoryBuffer));

            ////pdevice->defer_throw_hresult(hresultCreateCommittedResource);

            ////CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);

            ////auto hresultCreateCommittedResourceUpload = pdevice->m_pdevice->CreateCommittedResource(
            ////   &uploadHeap, D3D12_HEAP_FLAG_NONE,
            ////   &ibDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
            ////   nullptr, __interface_of(m_presourceMemoryBufferUpload));

            ////pdevice->defer_throw_hresult(hresultCreateCommittedResourceUpload);

            ////void* data = nullptr;
            ////D3D12_RANGE range = { 0, 0 }; // We don’t intend to read from it
            ////m_presourceMemoryBufferUpload->Map(0, &range, &data);
            ////memcpy(data, block.data(), indexBufferSize);
            ////m_presourceMemoryBufferUpload->Unmap(0, nullptr);

            //            // Create upload heap resources
            //// CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);
            //pcontext->_construct_new(m_pd3d12resourceMemoryBufferUpload);
            //m_pd3d12resourceMemoryBufferUpload->create_upload(D3D12_HEAP_FLAG_NONE, &vbDesc);

            //// pdevice->defer_throw_hresult(hresultCreateCommittedResourceUpload);

            //m_pd3d12resourceMemoryBufferUpload->set_name(m_strMemoryBufferName + " Upload");


            //m_pd3d12resourceMemoryBufferUpload->map_assign(block);

            //::cast < ::gpu_directx12::command_buffer> pcommandbufferLoading = m_pmodelbuffer->_defer_get_loading_command_buffer();

            //auto pcommandlistLoading = pcommandbufferLoading->m_pcommandlist;

            //pcommandlistLoading->CopyBufferRegion(m_presourceMemoryBuffer, 0, m_presourceMemoryBufferUpload, 0, indexBufferSize);

         }

      }
      else if (etype == ::gpu::memory_buffer::e_type_constant_buffer)
      {

         //bufferdesc.ByteWidth = (UINT)(total_size_in_bytes() + 15) & ~15;
         //bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
         //bufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         //bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_constant_buffer)
      {

         //bufferdesc.ByteWidth = (UINT)(total_size_in_bytes() + 15) & ~15;
         //bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
         //bufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         //bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_none)
      {

         //bufferdesc.ByteWidth = (UINT)(total_size_in_bytes() + 15) & ~15;
         //bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
         //bufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         //bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else
      {

         throw ::not_implemented();

      }

      //::cast < ::gpu_directx11::context > pgpucontext = m_pcontext;

      //::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      //auto pd3d11device = pgpudevice->m_pdevice;

      //D3D11_SUBRESOURCE_DATA data{};

      //data.pSysMem = dataStatic;

      //auto hresult = pd3d11device->CreateBuffer(
      //   &bufferdesc, dataStatic ? &data : nullptr, &m_pbuffer);

      //if (FAILED(hresult))
      //{

      //   throw ::hresult_exception(hresult);

      //}

   }


   bool memory_buffer::is_initialized() const
   {

      return m_pd3d12resourceMemoryBuffer && m_pd3d12resourceMemoryBuffer->m_presource;

   }


   void* memory_buffer::_map(memsize start, memsize count)
   {

      if (!m_bDynamic)
      {

         throw ::exception(error_wrong_state);

      }

      ::cast < device > pdevice = m_pcontext->m_pgpudevice;

      ::cast < frame_storage > pframestorage = pdevice->current_frame_storage();

      pframestorage->map_allocate(this, count);

      if (m_pmodelbuffer)
      {

         m_pmodelbuffer->m_bNew = false;

      }

      return m_pMap;

      //::cast < ::gpu_directx11::context > pcontext = m_pcontext;

      //D3D11_MAPPED_SUBRESOURCE mapped;

      //pcontext->m_pcontext->Map(
      //   m_pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
      ////      memcpy(mapped.pData, &myData, sizeof(MyConstants));

      //m_pMap = mapped.pData;

      //return m_pMap;

   }


   void memory_buffer::_unmap()
   {

      //::cast < ::gpu_directx11::context > pcontext = m_pcontext;

      //pcontext->m_pcontext->Unmap(m_pbuffer, 0);

   }


   void memory_buffer::bind()
   {

      //if (m_etype == e_type_vertex_buffer)
      //{

      //   UINT stride = type_size();

      //   UINT offset = 0;

      //   ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

      //   pcontext->m_pcontext->IASetVertexBuffers(0, 1, m_pbuffer.pp(), &stride, &offset);

      //}
      //else if (m_etype == e_type_index_buffer)
      //{

      //   ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

      //   pcontext->m_pcontext->IASetIndexBuffer(m_pbuffer, DXGI_FORMAT_R32_UINT, 0);

      //}

   }


   void memory_buffer::unbind()
   {


   }


   void memory_buffer::_complete_map_allocate(::gpu::memory_buffer* pmemorybufferSource, ::gpu::frame_storage * pgpuframestorage, int size)
   {

      ::gpu::memory_buffer::_complete_map_allocate(pmemorybufferSource, pgpuframestorage, size);

      ::cast < memory_buffer > pbufferSource = pmemorybufferSource;

      auto p = (unsigned char*)pmemorybufferSource->m_pMap;

      m_pMap = p + pgpuframestorage->m_iBufferOffset;

      m_pd3d12resourceMemoryBuffer = pbufferSource->m_pd3d12resourceMemoryBuffer;

      if (m_bDynamic)
      {

         if (m_etype == e_type_vertex_buffer)
         {

            ::cast < model_buffer > pmodelbuffer = m_pmodelbuffer;

            if (pmodelbuffer)
            {

               auto& vertexbufferview = pmodelbuffer->m_vertexbufferview;

               vertexbufferview.BufferLocation = this->m_pd3d12resourceMemoryBuffer->gpu_address() + pgpuframestorage->m_iBufferOffset;
               vertexbufferview.StrideInBytes = pmodelbuffer->m_pmodeldatabase2->vertex_type_size();
               vertexbufferview.SizeInBytes = pmodelbuffer->m_pmodeldatabase2->vertex_bytes();

               //pmodelbuffer->m_bNew = false;

            }

         }


      }

   }


   void memory_buffer::set_state(::gpu::command_buffer *pgpucommandbuffer, ::gpu::enum_buffer_state ebufferstate)
   {

      ::gpu::memory_buffer::set_state(pgpucommandbuffer, ebufferstate);

      ::cast<::gpu_directx12::command_buffer> pcommandbuffer = pgpucommandbuffer;

      if (ebufferstate == ::gpu::e_buffer_state_vertex)
      {
         m_pd3d12resourceMemoryBuffer->_set_state(pcommandbuffer, {D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER});
      }
      else if (ebufferstate == ::gpu::e_buffer_state_index)
      {
         m_pd3d12resourceMemoryBuffer->_set_state(pcommandbuffer, {D3D12_RESOURCE_STATE_INDEX_BUFFER});
      }

   }


}  // namespace graphics3d_directx12

