// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 02:40 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "model.h"
#include "utilities.h"
#include "context.h"
#include "frame_info.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/path_system.h"



#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// lib headers
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// std
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>


//
//namespace std {
//    template <>
//    struct hash<::graphics3d_opengl::model::Vertex> {
//        size_t operator()(::graphics3d_opengl::model::Vertex const& vertex) const {
//            size_t seed = 0;
//            ::graphics3d_opengl::hash_combine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
//            return seed;
//        }
//    };
//}  // namespace std
//

namespace graphics3d_opengl 

{
   model::model()
   {

    }
    model::~model()
    {
    }


    void model::initialize_model(::cube::context * pgpucontext, const ::cube::model::Builder& builder)
    {

       m_pgpucontext = pgpucontext;
       
       initialize(pgpucontext);
       
       createVertexBuffers(builder.vertices);

       createIndexBuffers(builder.indices);

    }


    //::pointer<model> model::createModelFromFile(context * pvkcdevice, const std::string& filepath) {
    //    Builder builder{};
    //    builder.loadModel(pvkcdevice, filepath);

    //    __refdbg_this(pvkcdevice);

    //    return __allocate model(pvkcdevice, builder);
    //}


    void model::createVertexBuffers(const ::array<Vertex>& vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);

        buffer stagingBuffer;
        
        stagingBuffer.initialize_buffer(
            m_pgpucontext,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)vertices.data());

        vertexBuffer = __allocate buffer;
        
           vertexBuffer->initialize_buffer(
            m_pgpucontext,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );


        m_pgpucontext->copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);


    }

    void model::createIndexBuffers(const ::array<uint32_t>& indices) {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) {
            return;
        }

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
        uint32_t indexSize = sizeof(indices[0]);

        buffer stagingBuffer;
        stagingBuffer.initialize_buffer(
            m_pgpucontext,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        ;

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)indices.data());

        indexBuffer = __allocate buffer();
        
        indexBuffer->initialize_buffer(
            m_pgpucontext,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_pgpucontext->copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
    }


    void model::draw(void * p) {
        auto pframeinfo = (FrameInfo* )p;
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(pframeinfo->commandBuffer, indexCount, 1, 0, 0, 0);
        }
        else {
            vkCmdDraw(pframeinfo->commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void model::bind(void *p) {
       auto pframeinfo = (FrameInfo*)p;
        VkBuffer buffers[] = { vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(pframeinfo->commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(pframeinfo->commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }
    ::array<VkVertexInputBindingDescription> model::getVertexBindingDescriptions() {
        ::array<VkVertexInputBindingDescription> bindingDescriptions(1, VkVertexInputBindingDescription{});

        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }
    ::array<VkVertexInputAttributeDescription> model::getVertexAttributeDescriptions() {
        ::array<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
        attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
        attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
        attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) });

        return attributeDescriptions;
    }


    //void model::Builder::loadModel(::graphics3d::context * pgpucontext, const std::string& filepath) {
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
    //                vertices.push_back(vertex);
    //            }
    //            indices.push_back(uniqueVertices[vertex]);

    //        }
    //    }
    //}
}  // namespace graphics3d_opengl