#pragma once
#include "vk_device.h"
#include "Renderer/vk_buffer.h"

// libs
	




// std
#include <memory>
#include <vector>

namespace vkc {

    class VkcModel :
    virtual public ::particle{
    public:
        struct Vertex {
            floating_sequence3 position{};
            floating_sequence3 color{};
            floating_sequence3 normal{};
            floating_sequence2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const { return position == other.position && normal == other.normal && uv == other.uv; }
        };

        struct Builder {
            std::vector<Vertex> vertexes{};
            std::vector<uint32_t> indexes{};

            void loadModel(::particle * pparticle, const std::string& filepath);
        };




        static ::pointer<VkcModel> createModelFromFile(VkcDevice * pvkcdevice, const std::string& filepath);


        VkcModel(VkcDevice * pvkcdevice, const VkcModel::Builder& builder);
        ~VkcModel();

        VkcModel(const VkcModel&) = delete;
        void operator=(const VkcModel&) = delete;

        void draw(VkCommandBuffer commandBuffer);
        void bind(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex>& vertexes);
        void createIndexBuffers(const std::vector<uint32_t>& indexes);

        bool hasIndexBuffer = false;
        ::pointer < VkcDevice > m_pvkcdevice;


        ::pointer<VkcBuffer> vertexBuffer;
        uint32_t vertexCount;

        ::pointer<VkcBuffer> indexBuffer;
        uint32_t indexCount;
    };
}  // namespace vkc