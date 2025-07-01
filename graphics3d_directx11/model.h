// From V0idsEmbrace@Twich continuum project
// base from graphics3d::model by
// camilo on 2025-05-17 02:47 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/model.h"


//#include "context.h"
#include "buffer.h"

// libs
#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>


// std
#include <memory>
#include <vector>

namespace graphics3d_directx11
{


   class CLASS_DECL_GRAPHICS3D_DIRECTX11 model :
      virtual public ::graphics3d::model
   {
   public:


      bool hasIndexBuffer = false;
      //::pointer < ::gpu_directx11::context > m_pgpucontext;


      //::pointer<::gpu_directx11::buffer> m_pbufferVertex;
      uint32_t vertexCount;

      //::pointer<::gpu_directx11::buffer> m_pbufferIndex;
      uint32_t indexCount;


      //// Triangle vertex data
      //struct Vertex { float x, y, z; float r, g, b, a; };
      //Vertex triangle[] = {
      //    {  0.0f,  0.5f, 0.0f, 1, 0, 0, 1 },
      //    {  0.5f, -0.5f, 0.0f, 0, 1, 0, 1 },
      //    { -0.5f, -0.5f, 0.0f, 0, 0, 1, 1 }
      //};

      ::comptr<ID3D11Buffer> m_pbufferVertex;
      ::comptr<ID3D11Buffer> m_pbufferIndice;
      //D3D11_BUFFER_DESC bd = { sizeof(triangle), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER };
      //D3D11_SUBRESOURCE_DATA initData = { triangle };
      //device->CreateBuffer(&bd, &initData, &vbo);


      //static ::array<VkVertexInputBindingDescription> getVertexBindingDescriptions();
      //static ::array<VkVertexInputAttributeDescription> getVertexAttributeDescriptions();


      //static ::pointer<model> createModelFromFile(::gpu::context * pgpucontext, const std::string& filepath);


      model();
      ~model();

      void initialize_model(::gpu::renderer* pgpurenderer, const ::graphics3d::model::Builder& builder) override;

      void draw() override;
      void bind() override;

      void createVertexBuffers(const ::array<::graphics3d::Vertex>& vertices);
      void createIndexBuffers(const ::array<uint32_t>& indices);


   };


}  // namespace graphics3d_directx11



