#include "platform.h"
#include "vk_model.h"
#include "Utils/vkc_utils.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/path_system.h"





//  // Optional — depends on your conventions





// lib headers
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


// std
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>



namespace std {
    template <>
    struct hash<vkc::VkcModel::Vertex> {
        size_t operator()(vkc::VkcModel::Vertex const& vertex) const {
            size_t seed = 0;
            vkc::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}  // namespace std


namespace vkc {
    VkcModel::VkcModel(VkcDevice * pvkcdevice, const VkcModel::Builder& builder) : m_pvkcdevice{ pvkcdevice } {
       initialize(pvkcdevice);
        createVertexBuffers(builder.vertexes);
        createIndexBuffers(builder.indexes);
    }
    VkcModel::~VkcModel() {
    }

    ::pointer<VkcModel> VkcModel::createModelFromFile(VkcDevice * pvkcdevice, const std::string& filepath) {
        Builder builder{};
        builder.loadModel(pvkcdevice, filepath);

        __refdbg_this(pvkcdevice);

        return allocateø VkcModel(pvkcdevice, builder);
    }


    void VkcModel::createVertexBuffers(const std::vector<Vertex>& vertexes) {
        vertexCount = static_cast<uint32_t>(vertexes.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertexes[0]) * vertexCount;
        uint32_t vertexSize = sizeof(vertexes[0]);

        VkcBuffer stagingBuffer;
        
        stagingBuffer.initialize_buffer(
            m_pvkcdevice,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)vertexes.data());

        vertexBuffer = allocateø VkcBuffer;
        
           vertexBuffer->initialize_buffer(
            m_pvkcdevice,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );


        m_pvkcdevice->copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);


    }

    void VkcModel::createIndexBuffers(const std::vector<uint32_t>& indexes) {
        indexCount = static_cast<uint32_t>(indexes.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) {
            return;
        }

        VkDeviceSize bufferSize = sizeof(indexes[0]) * indexCount;
        uint32_t indexSize = sizeof(indexes[0]);

        VkcBuffer stagingBuffer;
        stagingBuffer.initialize_buffer(
            m_pvkcdevice,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        ;

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void*)indexes.data());

        indexBuffer = allocateø VkcBuffer();
        
        indexBuffer->initialize_buffer(
            m_pvkcdevice,
            indexSize,
            indexCount,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        m_pvkcdevice->copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
    }


    void VkcModel::draw(VkCommandBuffer commandBuffer) {
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        }
        else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void VkcModel::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = { vertexBuffer->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer) {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }
    }
    std::vector<VkVertexInputBindingDescription> VkcModel::Vertex::getBindingDescriptions() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1, VkVertexInputBindingDescription{});

        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }
    std::vector<VkVertexInputAttributeDescription> VkcModel::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position) });
        attributeDescriptions.push_back({ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color) });
        attributeDescriptions.push_back({ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal) });
        attributeDescriptions.push_back({ 3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv) });

        return attributeDescriptions;
    }


    void VkcModel::Builder::loadModel(::particle * pparticle, const std::string& filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        

        auto path = pparticle->directory()->defer_get_file_system_file(filepath.c_str(), true);

        ::string str(::system()->path_system()->shell_path(path));

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, str.c_str())) {
            throw std::runtime_error(warn + err);
        }

        vertexes.clear();
        indexes.clear();


        std::unordered_map<Vertex, uint32_t> uniqueVertices{};
        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indexes) {
                Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.position = {
                        attrib.vertexes[3 * index.vertex_index + 0],
                        attrib.vertexes[3 * index.vertex_index + 1],
                        attrib.vertexes[3 * index.vertex_index + 2],
                    };

                    vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                    };

                }

                if (index.normal_index >= 0) {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertexes.size());
                    vertexes.push_back(vertex);
                }
                indexes.push_back(uniqueVertices[vertex]);

            }
        }
    }
}  // namespace vkc