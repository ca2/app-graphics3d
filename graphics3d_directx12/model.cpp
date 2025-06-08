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
#include "aura/user/user/graphics3d.h"
#include "gpu_directx12/buffer.h"
#include "gpu_directx12/context.h"
#include "gpu_directx12/renderer.h"



#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtx/hash.hpp>


// lib headers
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


// std
//#include <cassert>
//#include <cstring>
//#include <stdexcept>
//#include <unordered_map>
//#include <unordered_set>


//
//namespace std {
//    template <>
//    struct hash<::graphics3d_directx12::model::Vertex> {
//        size_t operator()(::graphics3d_directx12::model::Vertex const& vertex) const {
//            size_t seed = 0;
//            ::graphics3d_directx12::hash_combine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
//            return seed;
//        }
//    };
//}  // namespace std
//

namespace graphics3d_directx12

{


   model::model()
   {

   
   }


   model::~model()
   {


   }


   void model::initialize_model(::gpu::context* pgpucontext, const ::graphics3d::model::Builder& builder)
   {

      m_pgpucontext = pgpucontext;

      initialize(pgpucontext);

      ::cast < ::gpu_directx12::renderer > prenderer= pgpucontext->m_pgpurenderer;

      auto commandList = prenderer->getCurrentCommandList();

      createVertexBuffers(builder.vertices, commandList);

      createIndexBuffers(builder.indices, commandList);

   }


   //::pointer<model> model::createModelFromFile(::gpu::context * pgpucontext, const std::string& filepath) {
   //    Builder builder{};
   //    builder.loadModel(pgpucontext, filepath);

   //    __refdbg_this(pgpucontext);

   //    return __allocate model(pgpucontext, builder);
   //}


   void model::createVertexBuffers(const ::array<::gpu::Vertex>& vertices, ID3D12GraphicsCommandList* commandList)
   {

      //vertexCount = static_cast<uint32_t>(vertices.size());
      //assert(vertexCount >= 3 && "Vertex count must be at least 3");
      //VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
      //uint32_t vertexSize = sizeof(vertices[0]);
      ::cast < ::gpu_directx12::device > pdevice = m_pgpucontext->m_pgpudevice;
      
      ::array<::gpu::Vertex> a;

      a.copy(vertices);

      for (int i = 0; i < a.size(); i+=3)
      {
         auto& item2 = a[i + 1];
         //auto& item3 = a[i + 2];
         //swap(item2, item3);
         // item.uv.y = 1.0f-item.uv.y;
         //item.position.x = -item.position.x;
         //item.position.y = -item.position.y;
         // item.position.z = 1.0 - item.position.z;
         //item.position.z = - item.position.z;
      }

      //    // Triangle vertex data
      //struct Vertex { float x, y, z; float r, g, b, a; };
      //Vertex triangle[] = {
      //    {  0.0f,  0.5f, 0.0f, 1, 0, 0, 1 },
      //    {  0.5f, -0.5f, 0.0f, 0, 1, 0, 1 },
      //    { -0.5f, -0.5f, 0.0f, 0, 0, 1, 1 }
      //};

      ////ID3D11Buffer* vbo = nullptr;
      //int iCount = vertices.size();
      //int iTypeSize = sizeof(::gpu::Vertex);
      //int iMyCalculatedTotalSizeInBytes = iTypeSize * iCount;
      //int iFrameworkCalculatedTotalSizeInBytes = a.get_size_in_bytes();
      //D3D11_BUFFER_DESC bd =
      //{ (UINT)
      //   iFrameworkCalculatedTotalSizeInBytes,
      //   D3D11_USAGE_DEFAULT, 
      //   D3D11_BIND_VERTEX_BUFFER
      //};
      //D3D11_SUBRESOURCE_DATA initData = 
      //{
      //   a.data()
      //};
      //::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;

      //auto hresult = pgpudevice->m_pdevice->CreateBuffer(&bd, &initData, &m_pbufferVertex);

      //if (FAILED(hresult))
      //{

      //   throw ::hresult_exception(hresult);

      //}

      ////auto pbufferStaging = __create_new < ::gpu_directx12::buffer >();

      ////pbufferStaging->initialize_buffer(
      ////   m_pgpucontext,
      ////   vertexSize,
      ////   vertexCount,
      ////   VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      ////   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
      ////);

      ////pbufferStaging->map();
      ////pbufferStaging->writeToBuffer((void*)vertices.data());

      ////m_pbufferVertex = __create_new < ::gpu_directx12::buffer>();

      ////m_pbufferVertex->initialize_buffer(
      ////   m_pgpucontext,
      ////   vertexSize,
      ////   vertexCount,
      ////   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      ////   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      ////);


      ////m_pgpucontext->copyBuffer(pbufferStaging->getBuffer(), m_pbufferVertex->getBuffer(), bufferSize);

      //  // Vertex and Index data
      //Vertex vertices[] = {
      //    {{0.0f, 0.5f, 0.0f}, {1, 0, 0, 1}},
      //    {{0.5f, -0.5f, 0.0f}, {0, 1, 0, 1}},
      //    {{-0.5f, -0.5f, 0.0f}, {0, 0, 1, 1}}
      //};
      //UINT16 indices[] = { 0, 1, 2 };

      UINT vertexBufferSize = vertices.get_size_in_bytes();

      // Create default heap resources
      CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);
      CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

      pdevice->m_pdevice->CreateCommittedResource(
         &defaultHeap, D3D12_HEAP_FLAG_NONE,
         &vbDesc, D3D12_RESOURCE_STATE_COPY_DEST,
         nullptr, __interface_of(m_presourceVertexBufferGPU));

      // Create upload heap resources
      CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);

      pdevice->m_pdevice->CreateCommittedResource(
         &uploadHeap, D3D12_HEAP_FLAG_NONE,
         &vbDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
         nullptr, __interface_of(m_presourceVertexBufferUpload));


      // Copy vertex data
      void* pData = nullptr;
      CD3DX12_RANGE range(0, 0);
      m_presourceVertexBufferUpload->Map(0, &range, &pData);
      memcpy(pData, vertices.data(), vertexBufferSize);
      m_presourceVertexBufferUpload->Unmap(0, nullptr);


      // Upload to GPU
      commandList->CopyBufferRegion(m_presourceVertexBufferGPU, 0, m_presourceVertexBufferUpload, 0, vertexBufferSize);

      //// Transition to usable state
      //CD3DX12_RESOURCE_BARRIER vbBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
      //   g_vertexBufferGPU.Get(),
      //   D3D12_RESOURCE_STATE_COPY_DEST,
      //   D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

      //CD3DX12_RESOURCE_BARRIER ibBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
      //   g_indexBufferGPU.Get(),
      //   D3D12_RESOURCE_STATE_COPY_DEST,
      //   D3D12_RESOURCE_STATE_INDEX_BUFFER);

      //commandList->ResourceBarrier(1, &vbBarrier);
      //commandList->ResourceBarrier(1, &ibBarrier);

      // Create buffer views
      m_vertexbufferview.BufferLocation = m_presourceVertexBufferGPU->GetGPUVirtualAddress();
      m_vertexbufferview.StrideInBytes = sizeof(::gpu::Vertex);
      m_vertexbufferview.SizeInBytes = vertexBufferSize;

   

   }


   void model::createIndexBuffers(const ::array<uint32_t>& indices, ID3D12GraphicsCommandList* commandList)
   {

      indexCount = static_cast<uint32_t>(indices.size());
      hasIndexBuffer = indexCount > 0;

      UINT indexBufferSize = indices.get_size_in_bytes();


      if (hasIndexBuffer)
      {


         ::cast < ::gpu_directx12::device > pdevice = m_pgpucontext->m_pgpudevice;
         //if (hasIndexBuffer)
         //{
         //   //ID3D11Buffer* vbo = nullptr;
         //   D3D11_BUFFER_DESC bd =
         //   { 
         //      (UINT)indices.get_size_in_bytes(), 
         //      D3D11_USAGE_DEFAULT, 
         //      D3D11_BIND_INDEX_BUFFER 
         //   };

         //   D3D11_SUBRESOURCE_DATA initData = 
         //   { 
         //      indices.data() 
         //   };
         //   ::cast < ::gpu_directx12::device > pgpudevice = m_pgpucontext->m_pgpudevice;

         //   auto hresult = pgpudevice->m_pdevice->CreateBuffer(&bd, &initData, &m_pbufferIndice);

         //   if (FAILED(hresult))
         //   {

         //      throw ::hresult_exception(hresult);

         //   }

         //}
         CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);
         CD3DX12_RESOURCE_DESC ibDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

         pdevice->m_pdevice->CreateCommittedResource(
            &defaultHeap, D3D12_HEAP_FLAG_NONE,
            &ibDesc, D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr, __interface_of(m_presourceIndexBufferGPU));

         CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);


         pdevice->m_pdevice->CreateCommittedResource(
            &uploadHeap, D3D12_HEAP_FLAG_NONE,
            &ibDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr, __interface_of(m_presourceIndexBufferUpload));

         void* pData = nullptr;
         m_presourceIndexBufferUpload->Map(0, &m_rangeIndex, &pData);
         memcpy(pData, indices.data(), indexBufferSize);
         m_presourceIndexBufferUpload->Unmap(0, nullptr);



         commandList->CopyBufferRegion(m_presourceIndexBufferGPU, 0, m_presourceIndexBufferUpload, 0, indexBufferSize);


         m_indexbufferview.BufferLocation = m_presourceIndexBufferGPU->GetGPUVirtualAddress();
         m_indexbufferview.Format = DXGI_FORMAT_R16_UINT;
         m_indexbufferview.SizeInBytes = indexBufferSize;


      }



      //if (!hasIndexBuffer) {
      //   return;
      //}

      //VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
      //uint32_t indexSize = sizeof(indices[0]);

      //auto pbufferStaging = __create_new < ::gpu_directx12::buffer>();

      //pbufferStaging->initialize_buffer(
      //   m_pgpucontext,
      //   indexSize,
      //   indexCount,
      //   VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      //   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
      //   ;

      //pbufferStaging->map();
      //pbufferStaging->writeToBuffer((void*)indices.data());

      //m_pbufferIndex = __create_new < ::gpu_directx12::buffer>();

      //m_pbufferIndex->initialize_buffer(
      //   m_pgpucontext,
      //   indexSize,
      //   indexCount,
      //   VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      //   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      //m_pgpucontext->copyBuffer(pbufferStaging->getBuffer(), m_pbufferIndex->getBuffer(), bufferSize);

   }


   void model::draw(::gpu::context* pgpucontext)
   {

      //cast <::gpu_directx12::renderer> pgpurenderer = pgpucontext->m_pgpurenderer;
      ::cast <::gpu_directx12::context> pcontext = pgpucontext;
      ::cast <::gpu_directx12::device> pdevice = pgpucontext->m_pgpudevice;
      ::cast <::gpu_directx12::renderer> prenderer = pcontext->m_pgpurenderer;
     auto pcommandlist = prenderer->getCurrentCommandList();

      if (hasIndexBuffer) {
      //   vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
         pcommandlist->DrawIndexedInstanced(
            indexCount,        // Number of indices to draw
            1,
            0,                 // Start index location in the index buffer
            0,                  // Base vertex location (added to each index)
            0
         );
         
      }
      else {
      //   vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
         pcommandlist->DrawInstanced(
            vertexCount,       // Number of vertices to draw
            1,
            0,                  // Start vertex location
            0
         );
      }
   }

   
   void model::bind(::gpu::context* pgpucontext)
   {

      ::cast <::gpu_directx12::context> pcontext = pgpucontext;
      ::cast <::gpu_directx12::device> pdevice = pgpucontext->m_pgpudevice;
      ::cast <::gpu_directx12::renderer> prenderer = pcontext->m_pgpurenderer;
      auto pcommandlist = prenderer->getCurrentCommandList();

      //auto commandBuffer = prenderer->getCurrentCommandBuffer();

      UINT stride = sizeof(gpu::Vertex), offset = 0;
      //auto p = m_presourceBufferVertex.m_p;
      pcommandlist->IASetVertexBuffers(0, 1, &m_vertexbufferview);
      pcommandlist->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      if (hasIndexBuffer)
      {
         //auto p = m_pbufferIndice.m_p;
         pcommandlist->IASetIndexBuffer(&m_indexbufferview); // Assuming 32-bit indices

      }
      //VkBuffer buffers[] = { m_pbufferVertex->getBuffer() };
      //VkDeviceSize offsets[] = { 0 };
      //vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

      //if (hasIndexBuffer) 
      //{

      //   vkCmdBindIndexBuffer(commandBuffer, m_pbufferIndex->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

      //}

   }



   //void model::Builder::loadModel(::cube::gpu::context * pgpucontext, const std::string& filepath) {
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