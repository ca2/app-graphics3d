// opengl_guide.h : Include file for standard system include files,
// or project specific include files.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <span>
#include <array>
#include <functional>
#include <deque>

#include <opengl/opengl.h>
#include <opengl/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>

#include <iostream>




#include <cstdlib> // For abort()

#define VK_CHECK(x)                                                              \
    do {                                                                         \
        VkResult err = x;                                                        \
        if (err) {                                                               \
            std::cerr << "Detected OpenGL error: " << string_VkResult(err)       \
                      << std::endl;                                              \
            abort();                                                             \
        }                                                                        \
    } while (0)


struct AllocatedBuffer {
    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo info;
};

struct Vertex {

    floating_sequence3 position;
    float uv_x;
    floating_sequence3 normal;
    float uv_y;
    floating_sequence4 color;
};

// holds the resources needed for a mesh
struct GPUMeshBuffers {

    AllocatedBuffer indexBuffer;
    AllocatedBuffer vertexBuffer;
    VkDeviceAddress vertexBufferAddress;
};

// push constants for our mesh object draws
struct GPUDrawPushConstants {
    floating_matrix4 worldMatrix;
    VkDeviceAddress vertexBuffer;
};
struct AllocatedImage {
    VkImage image;
    VkImageView imageView;
    VmaAllocation allocation;
    VkExtent3D imageExtent;
    VkFormat imageFormat;
};

struct GPUSceneData {
    floating_matrix4 view;
    floating_matrix4 proj;
    floating_matrix4 viewproj;
    floating_sequence4 ambientColor;
    floating_sequence4 sunlightDirection; // w for sun power
    floating_sequence4 sunlightColor;
};

//> mat_types
enum class MaterialPass :uint8_t {
    MainColor,
    Transparent,
    Other
};
struct MaterialPipeline {
    VkPipeline pipeline;
    VkPipelineLayout layout;
};

struct MaterialInstance {
    MaterialPipeline* pipeline;
    VkDescriptorSet materialSet;
    MaterialPass passType;
};

struct DrawContext;

class IRenderable {

    virtual void Draw(const floating_matrix4& topMatrix, DrawContext& ctx) = 0;
};

// implementation of a drawable scene node.
// the scene node can hold children and will also keep a transform to propagate
// to them
struct Node : public IRenderable {

    // parent pointer must be a weak pointer to avoid circular dependencies
    std::weak_ptr<Node> parent;
    std::vector<::pointer<Node>> children;

    floating_matrix4 localTransform;
    floating_matrix4 worldTransform;

    void refreshTransform(const floating_matrix4& parentMatrix)
    {
        worldTransform = parentMatrix * localTransform;
        for (auto c : children) {
            c->refreshTransform(worldTransform);
        }
    }

    virtual void Draw(const floating_matrix4& topMatrix, DrawContext& ctx)
    {
        // draw children
        for (auto& c : children) {
            c->Draw(topMatrix, ctx);
        }
    }
};