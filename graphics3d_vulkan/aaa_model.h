// From V0idsEmbrace@Twich continuum project
// base from graphics3d::model by
// camilo on 2025-05-17 02:47 <3ThomasBorregaardSorensen!!
#pragma once
//
//
//#include "bred/graphics3d/model.h"
//#include "gpu_vulkan/model_buffer.h"
//
//
////#include "context.h"
//#include "buffer.h"
//
//// libs
//	
////// std
////#include <memory>
////#include <vector>
//
//
//namespace graphics3d_vulkan
//{
//
//
//   class model :
//      virtual public ::graphics3d::model_base,
//      virtual public ::gpu_vulkan::model_buffer
//   {
//   public:
//
//
//      //bool hasIndexBuffer = false;
//      //s::pointer < ::gpu_vulkan::renderer > m_pgpurenderer;
//
//
//      //::pointer<::gpu_vulkan::buffer> m_pbufferVertex;
//      //uint32_t vertexCount;
//
//      //::pointer<::gpu_vulkan::buffer> m_pbufferIndex;
//      //uint32_t indexCount;
//
//
//      static ::array<VkVertexInputBindingDescription> getVertexBindingDescriptions();
//      static ::array<VkVertexInputAttributeDescription> getVertexAttributeDescriptions();
//
//
//      //static ::pointer<model> createModelFromFile(::gpu::context * pgpucontext, const std::string& filepath);
//
//
//      model();
//      ~model();
//
//
//      void initialize_model(::gpu::renderer* pgpurenderer, const ::block& blockVertices, const ::block& blockIndices) override;
//
//
//      void draw() override;
//      void bind() override;
//
//      void createVertexBuffers(const ::block& blockVertices);
//      void createIndexBuffers(const ::block& blockIndices);
//
//
//   };
//
//
//}  // namespace graphics3d_vulkan
//
//
//
