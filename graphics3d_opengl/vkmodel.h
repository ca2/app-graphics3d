// From V0idsEmbrace@Twich continuum project
// base from graphics3d::model by
// camilo on 2025-05-17 02:47 <3ThomasBorregaardSorensen!!
#pragma once


#include "cube/model.h"


#include "context.h"
#include "buffer.h"

// libs
#define GLM_FORCE_RADIANS	
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


// std
#include <memory>
#include <vector>

namespace graphics3d_opengl
{


   class CLASS_DECL_GRAPHICS3D_OPENGL model :
      virtual public ::cube::model
   {
   public:

   
         static ::array<VkVertexInputBindingDescription> getVertexBindingDescriptions();
         static ::array<VkVertexInputAttributeDescription> getVertexAttributeDescriptions();


      //static ::pointer<model> createModelFromFile(context* pvkcdevice, const std::string& filepath);


      model();
      ~model();

      void initialize_model(::cube::context* pgpucontext, const ::cube::model::Builder& builder) override;

      void draw(void * pframeinfo) override;
      void bind(void* pframeinfo) override;

      void createVertexBuffers(const ::array<Vertex>& vertices);
      void createIndexBuffers(const ::array<uint32_t>& indices);

      bool hasIndexBuffer = false;
      ::pointer < context > m_pgpucontext;


      ::pointer<buffer> vertexBuffer;
      uint32_t vertexCount;

      ::pointer<buffer> indexBuffer;
      uint32_t indexCount;

   };


}  // namespace graphics3d_opengl



