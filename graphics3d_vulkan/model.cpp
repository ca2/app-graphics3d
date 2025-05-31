// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 02:40 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "model.h"
#include "utilities.h"
//#include "context.h"
#include "engine.h"
#include "frame.h"
//#include "renderer.h"
#include "app-cube/cube/impact.h"
#include "app-cube/gpu_vulkan/buffer.h"
#include "app-cube/gpu_vulkan/context.h"
#include "app-cube/gpu_vulkan/renderer.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/path_system.h"


// lib headers
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

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


   void model::initialize_model(::gpu::context* pgpucontext, const ::graphics3d::model::Builder& builder)
   {

      m_pgpucontext = pgpucontext;

      initialize(pgpucontext);

      createVertexBuffers(builder.vertices);

      createIndexBuffers(builder.indices);

   }


   //::pointer<model> model::createModelFromFile(::gpu::context * pgpucontext, const std::string& filepath) {
   //    Builder builder{};
   //    builder.loadModel(pgpucontext, filepath);

   //    __refdbg_this(pgpucontext);

   //    return __allocate model(pgpucontext, builder);
   //}


   void model::createVertexBuffers(const ::array<::gpu::Vertex>& vertices)
   {

      vertexCount = static_cast<uint32_t>(vertices.size());
      assert(vertexCount >= 3 && "Vertex count must be at least 3");
      VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
      uint32_t vertexSize = sizeof(vertices[0]);

      auto pbufferStaging = __create_new < ::gpu_vulkan::buffer >();

      pbufferStaging->initialize_buffer(
         m_pgpucontext,
         vertexSize,
         vertexCount,
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
      );

      pbufferStaging->map();
      pbufferStaging->writeToBuffer((void*)vertices.data());

      m_pbufferVertex = __create_new < ::gpu_vulkan::buffer>();

      m_pbufferVertex->initialize_buffer(
         m_pgpucontext,
         vertexSize,
         vertexCount,
         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      );


      m_pgpucontext->copyBuffer(pbufferStaging->getBuffer(), m_pbufferVertex->getBuffer(), bufferSize);


   }


   void model::createIndexBuffers(const ::array<uint32_t>& indices) 
   {

      indexCount = static_cast<uint32_t>(indices.size());
      hasIndexBuffer = indexCount > 0;

      if (!hasIndexBuffer) {
         return;
      }

      VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
      uint32_t indexSize = sizeof(indices[0]);

      auto pbufferStaging = __create_new < ::gpu_vulkan::buffer>();

      pbufferStaging->initialize_buffer(
         m_pgpucontext,
         indexSize,
         indexCount,
         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
         ;

      pbufferStaging->map();
      pbufferStaging->writeToBuffer((void*)indices.data());

      m_pbufferIndex = __create_new < ::gpu_vulkan::buffer>();

      m_pbufferIndex->initialize_buffer(
         m_pgpucontext,
         indexSize,
         indexCount,
         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      m_pgpucontext->copyBuffer(pbufferStaging->getBuffer(), m_pbufferIndex->getBuffer(), bufferSize);

   }


   void model::draw(::gpu::context* pgpucontext)
   {

      ::cast <::gpu_vulkan::renderer> pgpurenderer = pgpucontext->m_pgpurenderer;

      auto commandBuffer = pgpurenderer->getCurrentCommandBuffer();

      if (hasIndexBuffer) {
         vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
      }
      else {
         vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
      }
   }

   
   void model::bind(::gpu::context* pgpucontext)
   {

      ::cast <::gpu_vulkan::renderer> prenderer = pgpucontext->m_pgpurenderer;

      auto commandBuffer = prenderer->getCurrentCommandBuffer();


      VkBuffer buffers[] = { m_pbufferVertex->getBuffer() };
      VkDeviceSize offsets[] = { 0 };
      vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

      if (hasIndexBuffer) 
      {

         vkCmdBindIndexBuffer(commandBuffer, m_pbufferIndex->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

      }

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
}  // namespace graphics3d_vulkan