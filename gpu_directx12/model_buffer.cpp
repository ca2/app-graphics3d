// from graphics3d_direct2d/model.cpp by 
// camilo on 2025-07-16 06:10 <3ThomasBorregaardSørensen!!
// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 02:40 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "memory_buffer.h"
#include "model_buffer.h"
//#include "utilities.h"
//#include "context.h"
//#include "engine.h"
#include "frame.h"
//#include "renderer.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/path_system.h"
#include "aura/platform/application.h"
//#include "bred/user/user/graphics3d.h"
#include "bred/gpu/model_buffer.h"
//#include "gpu_directx12/buffer.h"
#include "gpu_directx12/context.h"
#include "gpu_directx12/renderer.h"




namespace gpu_directx12
{


   model_buffer::model_buffer()
   {


   }


   model_buffer::~model_buffer()
   {


   }


   void model_buffer::on_initialize_gpu_context_object()
   {

      ::gpu::model_buffer::on_initialize_gpu_context_object();

   }


   //void model_buffer::initialize_model(::gpu::renderer * pgpurenderer, const ::graphics3d::model_buffer::Builder& builder)
   //{

   //   m_pgpurenderer = pgpurenderer;

   //   initialize(pgpurenderer);

   //   ::cast < ::gpu_directx12::renderer > prenderer = pgpurenderer;

   //   m_pcommandbufferLoading = prenderer->getLoadAssetsCommandBuffer();

   //   if (builder.vertexes.has_element())
   //   {

   //      createVertexBuffers(builder.vertexes);

   //   }

   //   if (builder.indexes.has_element())
   //   {

   //      createIndexBuffers(builder.indexes);

   //   }

   //}


   //void model_buffer::createVertexBuffers(const ::array<::graphics3d::Vertex>& vertexes)
   //{

   //   ::cast < ::gpu_directx12::device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

   //   //::array<::gpu::Vertex> a;

   //   //a.copy(vertexes);

   //   //for (int i = 0; i < a.size(); i += 3)
   //   //{
   //   //   auto& item2 = a[i + 1];
   //   //   //auto& item3 = a[i + 2];
   //   //   //swap(item2, item3);
   //   //   // item.uv.y = 1.0f-item.uv.y;
   //   //   //item.position.x = -item.position.x;
   //   //   //item.position.y = -item.position.y;
   //   //   // item.position.z = 1.0 - item.position.z;
   //   //   //item.position.z = - item.position.z;
   //   //}

   //   auto vertexBufferSize = (UINT) vertexes.get_size_in_bytes();

   //   // Create default heap resources
   //   CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);

   //   CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

   //   pdevice->m_pdevice->CreateCommittedResource(
   //      &defaultHeap, D3D12_HEAP_FLAG_NONE,
   //      &vbDesc, D3D12_RESOURCE_STATE_COMMON,
   //      nullptr, __interface_of(m_presourceVertexBufferGPU));

   //   // Create upload heap resources
   //   CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);

   //   pdevice->m_pdevice->CreateCommittedResource(
   //      &uploadHeap, D3D12_HEAP_FLAG_NONE,
   //      &vbDesc, D3D12_RESOURCE_STATE_COMMON,
   //      nullptr, __interface_of(m_presourceVertexBufferUpload));

   //   // Copy vertex data
   //   void* data = nullptr;
   //   D3D12_RANGE range = { 0, 0 }; // We don’t intend to read from it
   //   m_presourceVertexBufferUpload->Map(0, &range, &data);
   //   memcpy(data, vertexes.data(), vertexBufferSize);
   //   m_presourceVertexBufferUpload->Unmap(0, nullptr);

   //   ::cast < ::gpu_directx12::command_buffer > pcommandbufferLoading = m_pcommandbufferLoading;

   //   auto pcommandlist = pcommandbufferLoading->m_pcommandlist;

   //   pcommandlist->CopyBufferRegion(m_presourceVertexBufferGPU, 0, m_presourceVertexBufferUpload, 0, vertexBufferSize);

   //   m_vertexbufferview.BufferLocation = m_presourceVertexBufferGPU->GetGPUVirtualAddress();
   //   m_vertexbufferview.StrideInBytes = sizeof(::graphics3d::Vertex);
   //   m_vertexbufferview.SizeInBytes = vertexBufferSize;

   //}


   //void model_buffer::createIndexBuffers(const ::array<uint32_t>& indexes)
   //{

   //   m_iIndexCount = static_cast<uint32_t>(indexes.size());

   //   // hasIndexBuffer = indexCount > 0;

   //   if (m_iIndexCount > 0)
   //   {

   //      auto indexBufferSize = (UINT) indexes.get_size_in_bytes();

   //      ::cast < ::gpu_directx12::device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

   //      CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);
   //      CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

   //      pdevice->m_pdevice->CreateCommittedResource(
   //         &defaultHeap, D3D12_HEAP_FLAG_NONE,
   //         &ibDesc, D3D12_RESOURCE_STATE_COMMON,
   //         nullptr, __interface_of(m_presourceIndexBufferGPU));

   //      CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);

   //      pdevice->m_pdevice->CreateCommittedResource(
   //         &uploadHeap, D3D12_HEAP_FLAG_NONE,
   //         &ibDesc, D3D12_RESOURCE_STATE_COMMON,
   //         nullptr, __interface_of(m_presourceIndexBufferUpload));

   //      void* data = nullptr;
   //      D3D12_RANGE range = { 0, 0 }; // We don’t intend to read from it
   //      m_presourceIndexBufferUpload->Map(0, &range, &data);
   //      memcpy(data, indexes.data(), indexBufferSize);
   //      m_presourceIndexBufferUpload->Unmap(0, nullptr);

   //      ::cast < ::gpu_directx12::command_buffer> pcommandbufferLoading = m_pcommandbufferLoading;

   //      auto pcommandlistLoading = pcommandbufferLoading->m_pcommandlist;

   //      pcommandlistLoading->CopyBufferRegion(m_presourceIndexBufferGPU, 0, m_presourceIndexBufferUpload, 0, indexBufferSize);

   //      m_indexbufferview.BufferLocation = m_presourceIndexBufferGPU->GetGPUVirtualAddress();
   //      m_indexbufferview.Format = DXGI_FORMAT_R32_UINT;
   //      m_indexbufferview.SizeInBytes = indexBufferSize;

   //   }

   //}


   void model_buffer::_static_initialize_vertex_buffer(const ::block &block)
   {

      ::gpu::model_buffer::_static_initialize_vertex_buffer(block);

      ::cast < memory_buffer > pbufferVertex = m_pbufferVertex;

      m_vertexbufferview.BufferLocation = pbufferVertex->m_pd3d12resourceMemoryBuffer->gpu_address();
      m_vertexbufferview.StrideInBytes = m_pmodeldatabase2->vertex_type_size();
      m_vertexbufferview.SizeInBytes = (UINT) block.size();


   }



   void model_buffer::_static_initialize_index_buffer(const ::block &block)
   {

      ::gpu::model_buffer::_static_initialize_index_buffer(block);

      ::cast < memory_buffer > pbufferIndex = m_pbufferIndex;

      m_indexbufferview.BufferLocation = pbufferIndex->m_pd3d12resourceMemoryBufferUpload->gpu_address();
      m_indexbufferview.Format = DXGI_FORMAT_R32_UINT;
      m_indexbufferview.SizeInBytes = (UINT) (block.size());

   }


   ::gpu::command_buffer* model_buffer::_defer_get_loading_command_buffer()
   {

      ::cast < ::gpu_directx12::renderer > prenderer = m_pgpucontext->m_pgpurenderer;

      if (::is_null(prenderer))
      {

         return nullptr;

      }

      return prenderer->getLoadAssetsCommandBuffer();

   }


   void model_buffer::bind2(::gpu::command_buffer* pgpucommandbuffer)
   {

      if (!m_pbufferVertex && !m_pbufferIndex)
      {

         return;

      }

      if ((m_pbufferVertex && !m_pbufferVertex->m_bDynamic)
         || (m_pbufferIndex && !m_pbufferIndex->m_bDynamic))
      {


         auto pcommandbufferLoading = m_pcommandbufferLoading;

         if (pcommandbufferLoading)
         {

            ::cast < ::gpu_directx12::command_buffer> pcommandbufferLoading = m_pcommandbufferLoading;

            if (!pcommandbufferLoading->has_finished())
            {

               return;

            }

            m_pcommandbufferLoading.release();

         }

      }

      ::cast <::gpu_directx12::renderer> prenderer = m_pgpucontext->m_pgpurenderer;

      ::cast <::gpu_directx12::context> pgpucontext = prenderer->m_pgpucontext;

      ::cast <::gpu_directx12::device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast < ::gpu_directx12::command_buffer > pcommandbuffer = pgpucommandbuffer;

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      if (m_bNew)
      {

         m_bNew = false;

         if (m_pbufferIndex)
         {

            m_pbufferIndex->set_state(pgpucommandbuffer, ::gpu::e_buffer_state_index);

            //::cast < memory_buffer > pbufferIndex = m_pbufferIndex;

            //D3D12_RESOURCE_BARRIER barrier = {};
            //barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            //barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            //barrier.Transition.pResource = pbufferIndex->m_presourceMemoryBufferUpload;
            //barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            //barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
            //barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;

            //pcommandlist->ResourceBarrier(1, &barrier);

         }

         //::cast < memory_buffer > pbufferVertex = m_pbufferVertex;

         //if(pbufferVertex && pbufferVertex->m_presourceMemoryBufferUpload)
         //{

         //   D3D12_RESOURCE_BARRIER barrier = {};

         //   barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
         //   barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
         //   barrier.Transition.pResource = pbufferVertex->m_presourceMemoryBufferUpload;
         //   barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
         //   barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
         //   barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

         //   pcommandlist->ResourceBarrier(1, &barrier);

         //}

         if (m_pbufferVertex)
         {

            m_pbufferVertex->set_state(pgpucommandbuffer, ::gpu::e_buffer_state_vertex);

         }

      }

      if (m_pmodeldatabase2->is_dummy())
      {

         pcommandlist->IASetVertexBuffers(0, 0, nullptr);
         pcommandlist->IASetIndexBuffer(nullptr);

      }
      else
      {
         pcommandlist->IASetVertexBuffers(0, 1, &m_vertexbufferview);
      }






      if (m_pmodeldatabase2->index_count() > 0)
      {

         pcommandlist->IASetIndexBuffer(&m_indexbufferview);

      }

   }


   void model_buffer::draw2(::gpu::command_buffer* pgpucommandbuffer)
   {

      if (m_pcommandbufferLoading)
      {

         ::cast <::gpu_directx12::command_buffer> pcommandbufferLoading = m_pcommandbufferLoading;

         if (!pcommandbufferLoading->has_finished())
         {

            return;

         }

         m_pcommandbufferLoading.release();

      }

      if (m_bNew)
      {

         return;

      }

      //cast <::gpu_directx12::renderer> pgpurenderer = pgpucontext->m_pgpurenderer;
      ::cast <::gpu_directx12::renderer> pgpurenderer = m_pgpucontext->m_pgpurenderer;
      ::cast <::gpu_directx12::context> pgpucontext = pgpurenderer->m_pgpucontext;
      ::cast <::gpu_directx12::device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast < ::gpu_directx12::command_buffer > pcommandbuffer = pgpucommandbuffer;

      auto pcommandlist = pcommandbuffer->m_pcommandlist;


      int iIndexCount = m_pmodeldatabase2->index_count();

      if (iIndexCount > 0)
      {

         //   vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
         
         pcommandlist->DrawIndexedInstanced(iIndexCount, // Number of indexes to draw
            1,
            0,                 // Start index location in the index buffer
            0,                  // Base vertex location (added to each index)
            0
         );

      }
      else 
      {
      
         auto iVertexCount = m_pmodeldatabase2->vertex_count();
         //   vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
         pcommandlist->DrawInstanced(iVertexCount, // Number of vertexes to draw
            1,
            0,                  // Start vertex location
            0
         );

      }

   }



   //void model_buffer::Builder::loadModel(::graphics3d::gpu::context * pgpucontext, const std::string& filepath) {
   //    tinyobj::attrib_t attrib;
   //    ::array<tinyobj::shape_t> shapes;
   //    ::array<tinyobj::material_t> materials;
   //    std::string warn, err;

   //    

   //    auto path = pparticle->directory()->defer_get_file_system_file(filepath.c_str(), true);

   //    ::string str(::system()->path_system()->shell_path(path));

   //    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, str.c_str())) {
   //        throw std::runtime_error(warn + err);
   //    }

   //    vertexes.clear();
   //    indexes.clear();


   //    ::map<Vertex, uint32_t> uniqueVertices{};
   //    for (const auto& shape : shapes) {
   //        for (const auto& index : shape.mesh.indexes) {
   //            Vertex vertex{};

   //            if (index.vertex_index >= 0) {
   //                vertex.position = {
   //                    attrib.vertexes[3 * index.vertex_index + 0],
   //                    attrib.vertexes[3 * index.vertex_index + 1],
   //                    attrib.vertexes[3 * index.vertex_index + 2],
   //                };

   //                vertex.color = {
   //                attrib.colors[3 * index.vertex_index + 0],
   //                attrib.colors[3 * index.vertex_index + 1],
   //                attrib.colors[3 * index.vertex_index + 2],
   //                };

   //            }

   //            if (index.normal_index >= 0) {
   //                vertex.normal = {
   //                    attrib.normals[3 * index.normal_index + 0],
   //                    attrib.normals[3 * index.normal_index + 1],
   //                    attrib.normals[3 * index.normal_index + 2],
   //                };
   //            }

   //            if (index.texcoord_index >= 0) {
   //                vertex.uv = {
   //                    attrib.texcoords[2 * index.texcoord_index + 0],
   //                    attrib.texcoords[2 * index.texcoord_index + 1],
   //                };
   //            }

   //            if (uniqueVertices.count(vertex) == 0) {
   //                uniqueVertices[vertex] = static_cast<uint32_t>(vertexes.size());
   //                vertexes.add(vertex);
   //            }
   //            indexes.add(uniqueVertices[vertex]);

   //        }
   //    }
   //}
}  // namespace gpu_directx12


