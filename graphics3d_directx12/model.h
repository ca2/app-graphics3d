// From V0idsEmbrace@Twich continuum project
// base from cube::model by
// camilo on 2025-05-17 02:47 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/model.h"
#include "app-graphics3d/gpu_directx12/command_buffer.h"
#include "app-graphics3d/gpu_directx12/renderer.h"


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

namespace graphics3d_directx12
{


   class CLASS_DECL_GRAPHICS3D_DIRECTX12 model :
      virtual public ::graphics3d::model
   {
   public:



      bool m_bNew = true;
      bool hasIndexBuffer = false;


      //::pointer < ::gpu_directx12::context > m_pgpucontext;


      //::pointer<::gpu_directx12::buffer> m_pbufferVertex;
      uint32_t vertexCount;

      //::pointer<::gpu_directx12::buffer> m_pbufferIndex;
      uint32_t indexCount;

      ::pointer < ::gpu_directx12::command_buffer > m_pcommandbufferLoading;
      //// Triangle vertex data
      //struct Vertex { float x, y, z; float r, g, b, a; };
      //Vertex triangle[] = {
      //    {  0.0f,  0.5f, 0.0f, 1, 0, 0, 1 },
      //    {  0.5f, -0.5f, 0.0f, 0, 1, 0, 1 },
      //    { -0.5f, -0.5f, 0.0f, 0, 0, 1, 1 }
      //};

      //::comptr<ID3D11Buffer> m_pbufferVertex;
      //::comptr<ID3D11Buffer> m_pbufferIndice;

      // GPU Resources
      ::comptr<ID3D12Resource> m_presourceVertexBufferGPU;
      ::comptr<ID3D12Resource> m_presourceIndexBufferGPU;
      ::comptr<ID3D12Resource> m_presourceVertexBufferUpload;
      ::comptr<ID3D12Resource> m_presourceIndexBufferUpload;
      D3D12_VERTEX_BUFFER_VIEW m_vertexbufferview = {};
      D3D12_INDEX_BUFFER_VIEW       m_indexbufferview = {};
      //D3D12_RANGE                   m_rangeVertex;
      //D3D12_RANGE                   m_rangeIndex;
      //D3D11_BUFFER_DESC bd = { sizeof(triangle), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER };
      //D3D11_SUBRESOURCE_DATA initData = { triangle };
      //device->CreateBuffer(&bd, &initData, &vbo);


      //static ::array<VkVertexInputBindingDescription> getVertexBindingDescriptions();
      //static ::array<VkVertexInputAttributeDescription> getVertexAttributeDescriptions();


      //static ::pointer<model> createModelFromFile(::gpu::context * pgpucontext, const std::string& filepath);


      model();
      ~model();

      void initialize_model(::gpu::renderer* pgpurenderer, const ::graphics3d::model::Builder& builder) override;

      void bind() override;
      void draw() override;

      void createVertexBuffers(const ::array<::gpu::Vertex>& vertices);
      void createIndexBuffers(const ::array<uint32_t>& indices);


   };


}  // namespace graphics3d_directx12



