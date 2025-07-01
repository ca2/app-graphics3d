// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 02:40 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "model.h"
#include "utilities.h"
//#include "context.h"
#include "engine.h"
#include "frame.h"
//#include "renderer.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/path_system.h"
#include "aura/platform/application.h"
#include "bred/user/user/graphics3d.h"
#include "gpu_directx12/buffer.h"
#include "gpu_directx12/context.h"
#include "gpu_directx12/renderer.h"




namespace graphics3d_directx12
{


   model::model()
   {


   }


   model::~model()
   {


   }


   void model::initialize_model(::gpu::renderer * pgpurenderer, const ::graphics3d::model::Builder& builder)
   {

      m_pgpurenderer = pgpurenderer;

      initialize(pgpurenderer);

      ::cast < ::gpu_directx12::renderer > prenderer = pgpurenderer;

      m_pcommandbufferLoading = prenderer->getLoadAssetsCommandBuffer();

      createVertexBuffers(builder.vertices);

      createIndexBuffers(builder.indices);

   }


   void model::createVertexBuffers(const ::array<::graphics3d::Vertex>& vertices)
   {

      ::cast < ::gpu_directx12::device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      //::array<::gpu::Vertex> a;

      //a.copy(vertices);

      //for (int i = 0; i < a.size(); i += 3)
      //{
      //   auto& item2 = a[i + 1];
      //   //auto& item3 = a[i + 2];
      //   //swap(item2, item3);
      //   // item.uv.y = 1.0f-item.uv.y;
      //   //item.position.x = -item.position.x;
      //   //item.position.y = -item.position.y;
      //   // item.position.z = 1.0 - item.position.z;
      //   //item.position.z = - item.position.z;
      //}

      auto vertexBufferSize = (UINT) vertices.get_size_in_bytes();

      // Create default heap resources
      CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);

      CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

      pdevice->m_pdevice->CreateCommittedResource(
         &defaultHeap, D3D12_HEAP_FLAG_NONE,
         &vbDesc, D3D12_RESOURCE_STATE_COMMON,
         nullptr, __interface_of(m_presourceVertexBufferGPU));

      // Create upload heap resources
      CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);

      pdevice->m_pdevice->CreateCommittedResource(
         &uploadHeap, D3D12_HEAP_FLAG_NONE,
         &vbDesc, D3D12_RESOURCE_STATE_COMMON,
         nullptr, __interface_of(m_presourceVertexBufferUpload));

      // Copy vertex data
      void* data = nullptr;
      D3D12_RANGE range = { 0, 0 }; // We don’t intend to read from it
      m_presourceVertexBufferUpload->Map(0, &range, &data);
      memcpy(data, vertices.data(), vertexBufferSize);
      m_presourceVertexBufferUpload->Unmap(0, nullptr);

      auto pcommandlist = m_pcommandbufferLoading->m_pcommandlist;

      pcommandlist->CopyBufferRegion(m_presourceVertexBufferGPU, 0, m_presourceVertexBufferUpload, 0, vertexBufferSize);

      m_vertexbufferview.BufferLocation = m_presourceVertexBufferGPU->GetGPUVirtualAddress();
      m_vertexbufferview.StrideInBytes = sizeof(::graphics3d::Vertex);
      m_vertexbufferview.SizeInBytes = vertexBufferSize;

   }


   void model::createIndexBuffers(const ::array<uint32_t>& indices)
   {

      indexCount = static_cast<uint32_t>(indices.size());

      hasIndexBuffer = indexCount > 0;

      if (hasIndexBuffer)
      {

         auto indexBufferSize = (UINT) indices.get_size_in_bytes();

         ::cast < ::gpu_directx12::device > pdevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

         CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);
         CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

         pdevice->m_pdevice->CreateCommittedResource(
            &defaultHeap, D3D12_HEAP_FLAG_NONE,
            &ibDesc, D3D12_RESOURCE_STATE_COMMON,
            nullptr, __interface_of(m_presourceIndexBufferGPU));

         CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);

         pdevice->m_pdevice->CreateCommittedResource(
            &uploadHeap, D3D12_HEAP_FLAG_NONE,
            &ibDesc, D3D12_RESOURCE_STATE_COMMON,
            nullptr, __interface_of(m_presourceIndexBufferUpload));

         void* data = nullptr;
         D3D12_RANGE range = { 0, 0 }; // We don’t intend to read from it
         m_presourceIndexBufferUpload->Map(0, &range, &data);
         memcpy(data, indices.data(), indexBufferSize);
         m_presourceIndexBufferUpload->Unmap(0, nullptr);

         auto pcommandlist = m_pcommandbufferLoading->m_pcommandlist;

         pcommandlist->CopyBufferRegion(m_presourceIndexBufferGPU, 0, m_presourceIndexBufferUpload, 0, indexBufferSize);

         m_indexbufferview.BufferLocation = m_presourceIndexBufferGPU->GetGPUVirtualAddress();
         m_indexbufferview.Format = DXGI_FORMAT_R32_UINT;
         m_indexbufferview.SizeInBytes = indexBufferSize;

      }

   }


   void model::bind()
   {

      auto pcommandbufferLoading = m_pcommandbufferLoading;

      if (pcommandbufferLoading)
      {

         if (!pcommandbufferLoading->has_finished())
         {

            return;

         }

         m_pcommandbufferLoading.release();

      }

      ::cast <::gpu_directx12::renderer> prenderer = m_pgpurenderer;

      ::cast <::gpu_directx12::context> pgpucontext = prenderer->m_pgpucontext;

      ::cast <::gpu_directx12::device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast < ::gpu_directx12::command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      if (m_bNew)
      {

         m_bNew = false;

         if (hasIndexBuffer)
         {

            D3D12_RESOURCE_BARRIER barrier = {};
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Transition.pResource = m_presourceIndexBufferGPU;
            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;

            pcommandlist->ResourceBarrier(1, &barrier);

         }

         {

            D3D12_RESOURCE_BARRIER barrier = {};

            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Transition.pResource = m_presourceVertexBufferGPU;
            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

            pcommandlist->ResourceBarrier(1, &barrier);

         }

      }

      pcommandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

      pcommandlist->IASetVertexBuffers(0, 1, &m_vertexbufferview);

      if (hasIndexBuffer)
      {

         pcommandlist->IASetIndexBuffer(&m_indexbufferview);

      }

   }


   void model::draw()
   {

      if (m_pcommandbufferLoading)
      {

         if (!m_pcommandbufferLoading->has_finished())
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
      ::cast <::gpu_directx12::renderer> pgpurenderer = m_pgpurenderer;
      ::cast <::gpu_directx12::context> pgpucontext = pgpurenderer->m_pgpucontext;
      ::cast <::gpu_directx12::device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast < ::gpu_directx12::command_buffer > pcommandbuffer = pgpurenderer->getCurrentCommandBuffer2();

      auto pcommandlist = pcommandbuffer->m_pcommandlist;

      if (hasIndexBuffer)
      {

         //   vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
         pcommandlist->DrawIndexedInstanced(
            indexCount,        // Number of indices to draw
            1,
            0,                 // Start index location in the index buffer
            0,                  // Base vertex location (added to each index)
            0
         );

      }
      else 
      {
      
         //   vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
         pcommandlist->DrawInstanced(
            vertexCount,       // Number of vertices to draw
            1,
            0,                  // Start vertex location
            0
         );

      }

   }



   //void model::Builder::loadModel(::graphics3d::gpu::context * pgpucontext, const std::string& filepath) {
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

   //    vertices.clear();
   //    indices.clear();


   //    ::map<Vertex, uint32_t> uniqueVertices{};
   //    for (const auto& shape : shapes) {
   //        for (const auto& index : shape.mesh.indices) {
   //            Vertex vertex{};

   //            if (index.vertex_index >= 0) {
   //                vertex.position = {
   //                    attrib.vertices[3 * index.vertex_index + 0],
   //                    attrib.vertices[3 * index.vertex_index + 1],
   //                    attrib.vertices[3 * index.vertex_index + 2],
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
   //                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
   //                vertices.add(vertex);
   //            }
   //            indices.add(uniqueVertices[vertex]);

   //        }
   //    }
   //}
}  // namespace graphics3d_directx12