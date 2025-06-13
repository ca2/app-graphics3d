// From V0idsEmbrace@Twich continuum project
// base from cube::model by
// camilo on 2025-05-17 02:47 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/model.h"


//#include "context.h"
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
      ::pointer < ::gpu_vulkan::context > m_pgpucontext;


      ::pointer<::gpu_vulkan::buffer> m_pbufferVertex;
      uint32_t vertexCount;

      ::pointer<::gpu_vulkan::buffer> m_pbufferIndex;
      uint32_t indexCount;


      static ::array<VkVertexInputBindingDescription> getVertexBindingDescriptions();
      static ::array<VkVertexInputAttributeDescription> getVertexAttributeDescriptions();


      //static ::pointer<model> createModelFromFile(::gpu::context * pgpucontext, const std::string& filepath);


      model();
      ~model();

      void initialize_model(::gpu::context* pgpucontext, const ::graphics3d::model::Builder& builder) override;

      void draw(::gpu::context* pgpucontext) override;
      void bind(::gpu::context* pgpucontext) override;

      void createVertexBuffers(const ::array<::gpu::Vertex>& vertices);
      void createIndexBuffers(const ::array<uint32_t>& indices);


   };


}  // namespace graphics3d_vulkan



