// From V0idsEmbrace@Twich continuum project
// base from cube::model by
// camilo on 2025-05-17 02:47 <3ThomasBorregaardSorensen!!
#pragma once


#include "app-cube/cube/graphics3d/model.h"


#include "context.h"
#include "buffer.h"

// libs
#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


// std
#include <memory>
#include <vector>

namespace graphics3d_vulkan
{


   class CLASS_DECL_GRAPHICS3D_VULKAN model :
      virtual public ::graphics3d::model
   {
   public:
      bool hasIndexBuffer = false;
      ::pointer < context > m_pcontext;


      ::pointer<buffer> vertexBuffer;
      uint32_t vertexCount;

      ::pointer<buffer> indexBuffer;
      uint32_t indexCount;

   
         static std::vector<VkVertexInputBindingDescription> getVertexBindingDescriptions();
         static std::vector<VkVertexInputAttributeDescription> getVertexAttributeDescriptions();


      //static ::pointer<model> createModelFromFile(context* pvkcdevice, const std::string& filepath);


      model();
      ~model();

      void initialize_model(::graphics3d::context* pcontext, const ::graphics3d::model::Builder& builder) override;

      void draw(::graphics3d::context* pcontext) override;
      void bind(::graphics3d::context* pcontext) override;

      void createVertexBuffers(const std::vector<::graphics3d::Vertex>& vertices);
      void createIndexBuffers(const std::vector<uint32_t>& indices);


   };


}  // namespace graphics3d_vulkan



