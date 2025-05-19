// opengl_guide.h : Include file for standard system include files,
// or project specific include files.
#pragma once


#include "app-cube/cube/graphics3d/types.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#pragma once
#pragma warning(push, 0)
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glad.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/hash.hpp"
//#include "Math.h"
#pragma warning(pop)
//#include "Defines.h"
#include "_constant.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <span>
#include <array>
#include <functional>
#include <deque>

//#include <opengl/opengl.h>
///#include <opengl/vk_enum_string_helper.h>
//#include <vma/vk_mem_alloc.h>

#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <cstdlib> // For abort()
//
//#define VK_CHECK(x)                                                              \
//    do {                                                                         \
//        VkResult err = x;                                                        \
//        if (err) {                                                               \
//            std::cerr << "Detected OpenGL error: " << string_VkResult(err)       \
//                      << std::endl;                                              \
//            abort();                                                             \
//        }                                                                        \
//    } while (0)
//

// struct AllocatedBuffer {
//     VkBuffer buffer;
//     VmaAllocation allocation;
//     VmaAllocationInfo info;
// };

struct Vertex {

    glm::vec3 position;
    float uv_x;
    glm::vec3 normal;
    float uv_y;
    glm::vec4 color;
};

// // holds the resources needed for a mesh
// struct GPUMeshBuffers {
//
//     AllocatedBuffer indexBuffer;
//     AllocatedBuffer vertexBuffer;
//     VkDeviceAddress vertexBufferAddress;
// };

//// push constants for our mesh object draws
//struct GPUDrawPushConstants {
//    glm::mat4 worldMatrix;
//    VkDeviceAddress vertexBuffer;
//};
// struct AllocatedImage {
//     VkImage image;
//     VkImageView imageView;
//     VmaAllocation allocation;
//     VkExtent3D imageExtent;
//     VkFormat imageFormat;
// };

struct GPUSceneData {
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 viewproj;
    glm::vec4 ambientColor;
    glm::vec4 sunlightDirection; // w for sun power
    glm::vec4 sunlightColor;
};

//> mat_types
enum class MaterialPass :uint8_t {
    MainColor,
    Transparent,
    Other
};
//struct MaterialPipeline {
//    VkPipeline pipeline;
//    VkPipelineLayout layout;
//};
//
//struct MaterialInstance {
//    MaterialPipeline* pipeline;
//    VkDescriptorSet materialSet;
//    MaterialPass passType;
//};

struct DrawContext;

class IRenderable {

    virtual void Draw(const glm::mat4& topMatrix, DrawContext& ctx) = 0;
};


namespace graphics3d_opengl
{


// implementation of a drawable scene node.
// the scene node can hold children and will also keep a transform to propagate
// to them
struct Node : 
   public IRenderable {

    // parent pointer must be a weak pointer to avoid circular dependencies
    std::weak_ptr<Node> parent;
    std::vector<::pointer<Node>> children;

    glm::mat4 localTransform;
    glm::mat4 worldTransform;

    void refreshTransform(const glm::mat4& parentMatrix)
    {
        worldTransform = parentMatrix * localTransform;
        for (auto c : children) {
            c->refreshTransform(worldTransform);
        }
    }

    virtual void Draw(const glm::mat4& topMatrix, DrawContext& ctx)
    {
        // draw children
        for (auto& c : children) {
            c->Draw(topMatrix, ctx);
        }
    }
};

///** @brief State of mouse/touch input */
//struct mouse_state
//{
//   struct {
//      bool left = false;
//      bool right = false;
//      bool middle = false;
//   } m_buttons;
//   glm::vec2 position;
//};
//
//




#define MAX_BONE_INFLUENCE 4

namespace graphics3d_opengl
{
   struct Vertex {
      //float Position[3];
      //float Color[4];
      //float TexCoords[2];
      //float TexID;

            // position
      glm::vec3 Position;
      // normal
      glm::vec3 Normal;
      // texCoords
      glm::vec2 TexCoords;
      // tangent
      glm::vec3 Tangent;
      // bitangent
      glm::vec3 Bitangent;
      //bone indexes which will influence this vertex
      int m_BoneIDs[MAX_BONE_INFLUENCE];
      //weights from each bone
      float m_Weights[MAX_BONE_INFLUENCE];
   };
   //struct Vertex {
   //   // position
   //   glm::vec3 Position;
   //   // normal
   //   glm::vec3 Normal;
   //   // texCoords
   //   glm::vec2 TexCoords;
   //   // tangent
   //   glm::vec3 Tangent;
   //   // bitangent
   //   glm::vec3 Bitangent;
   //   //bone indexes which will influence this vertex
   //   int m_BoneIDs[MAX_BONE_INFLUENCE];
   //   //weights from each bone
   //   float m_Weights[MAX_BONE_INFLUENCE];
   //};

} // namespace graphics3d_opengl
//
//
///** @brief State of mouse/touch input */
//struct mouseState
//{
//   struct {
//      bool left = false;
//      bool right = false;
//      bool middle = false;
//   } buttons;
//   glm::vec2 position;
//};
//
//
