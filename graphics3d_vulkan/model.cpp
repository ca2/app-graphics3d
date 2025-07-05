// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 02:40 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "model.h"
#include "utilities.h"
//#include "context.h"
#include "engine.h"
//#include "frame.h"
//#include "renderer.h"
#include "aura/platform/application.h"
#include "bred/user/user/graphics3d.h"
#include "app-graphics3d/gpu_vulkan/memory_buffer.h"
#include "app-graphics3d/gpu_vulkan/command_buffer.h"
#include "app-graphics3d/gpu_vulkan/context.h"
#include "app-graphics3d/gpu_vulkan/renderer.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/path_system.h"



#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#define GLM_FORCE_LEFT_HANDED  // Optional — depends on your conventions
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
//    struct hash<::graphics3d_vulkan::model::Vertex> {
//        size_t operator()(::graphics3d_vulkan::model::Vertex const& vertex) const {
//            size_t seed = 0;
//            ::graphics3d_vulkan::hash_combine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
//            return seed;
//        }
//    };
//}  // namespace std
//

namespace graphics3d_vulkan

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

      createVertexBuffers(builder.vertices);

      createIndexBuffers(builder.indices);

   }


   //::pointer<model> model::createModelFromFile(::gpu::context * pgpucontext, const std::string& filepath) {
   //    Builder builder{};
   //    builder.loadModel(pgpucontext, filepath);

   //    __refdbg_this(pgpucontext);

   //    return __allocate model(pgpucontext, builder);
   //}


   void model::createVertexBuffers(const ::array<::graphics3d::Vertex>& vertices)
   {

      m_iVertexCount = static_cast<uint32_t>(vertices.size());
      assert(m_iVertexCount >= 3 && "Vertex count must be at least 3");
      VkDeviceSize bufferSize = sizeof(vertices[0]) * m_iVertexCount;
      uint32_t vertexSize = sizeof(vertices[0]);

      auto pbufferStaging = __create_new < ::gpu_vulkan::memory_buffer >();

      pbufferStaging->_initialize_buffer(
         m_pgpurenderer->m_pgpucontext,
         vertexSize,
         m_iVertexCount,
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
      );

      pbufferStaging->map();
      pbufferStaging->writeToBuffer((void*)vertices.data());

      auto pvulkanbufferVertex = __create_new < ::gpu_vulkan::memory_buffer>();

      pvulkanbufferVertex->_initialize_buffer(
         m_pgpurenderer->m_pgpucontext,
         vertexSize,
         m_iVertexCount,
         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      );
      
      m_pbufferVertex = pvulkanbufferVertex;

      ::cast < gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext;   

      pcontext->copyBuffer(pbufferStaging->getBuffer(), pvulkanbufferVertex->getBuffer(), bufferSize);


   }


   void model::createIndexBuffers(const ::array<uint32_t>& indices) 
   {

      m_iIndexCount = static_cast<uint32_t>(indices.size());

      if (m_iIndexCount <= 0)
      {

         return;

      }

      VkDeviceSize bufferSize = sizeof(indices[0]) * m_iIndexCount;

      uint32_t indexSize = sizeof(indices[0]);

      auto pbufferStaging = __create_new < ::gpu_vulkan::memory_buffer>();

      pbufferStaging->_initialize_buffer(
         m_pgpurenderer->m_pgpucontext,
         indexSize,
         m_iIndexCount,
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
         ;

      pbufferStaging->map();
      pbufferStaging->writeToBuffer((void*)indices.data());

      auto pbufferIndex = __create_new < ::gpu_vulkan::memory_buffer>();

      pbufferIndex->_initialize_buffer(
         m_pgpurenderer->m_pgpucontext,
         indexSize,
         m_iIndexCount,
         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      m_pbufferIndex = pbufferIndex;

      ::cast < ::gpu_vulkan::context > pcontext = m_pgpurenderer->m_pgpucontext; 

      pcontext->copyBuffer(pbufferStaging->getBuffer(), pbufferIndex->getBuffer(), bufferSize);

   }


   void model::draw()
   {

      ::cast <::gpu_vulkan::renderer> pgpurenderer = m_pgpurenderer;

      ::cast <::gpu_vulkan::command_buffer > pcommandbuffer = pgpurenderer->getCurrentCommandBuffer2();

      if (m_iIndexCount > 0) 
      {

         vkCmdDrawIndexed(pcommandbuffer->m_vkcommandbuffer, m_iIndexCount, 1, 0, 0, 0);

      }
      else 
      {

         vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, m_iVertexCount, 1, 0, 0);

      }

   }

   
   void model::bind()
   {

      if (m_pbufferVertex)
      {

         ::cast <::gpu_vulkan::renderer> prenderer = m_pgpurenderer;

         ::cast <::gpu_vulkan::command_buffer > pcommandbuffer = prenderer->getCurrentCommandBuffer2();

         ::cast < ::gpu_vulkan::memory_buffer> pbufferVertex = m_pbufferVertex;

         VkBuffer buffers[] = { pbufferVertex->getBuffer() };
         VkDeviceSize offsets[] = { 0 };
         vkCmdBindVertexBuffers(pcommandbuffer->m_vkcommandbuffer, 0, 1, buffers, offsets);

         if (m_iIndexCount)
         {

            ::cast < ::gpu_vulkan::memory_buffer> pbufferIndex = m_pbufferIndex;

            vkCmdBindIndexBuffer(
               pcommandbuffer->m_vkcommandbuffer,
               pbufferIndex->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

         }

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
}  // namespace graphics3d_vulkan