#pragma once


#include "bred/graphics3d/mesh.h"
#include <glm/glm.hpp>


namespace graphics3d_opengl
{



   class mesh:
      virtual public ::graphics3d::mesh
   {
   public:


      //::pointer_array<texture>      m_texturea;
      //::array<float> m_Vertices;
      //::array<unsigned int> m_Indices;
      //unsigned int m_VAO, m_VBO, m_EBO;
      //unsigned int m_IndexOffset;
      //unsigned int m_IndexCount;
      //unsigned int m_VertexOffset;
      //::array<glm::mat4> m_ModelMatrices;
      // New instance buffer
      //unsigned int m_InstanceVBO;

      //mesh(const ::array<float>& vertices, const ::array<unsigned int>& indices,
      //   unsigned int vertexOffset, unsigned int indexOffset, unsigned int indexCount);
      //~mesh() override;


      mesh();

      ~mesh() override;


      void bind() override;
      void unbind() override;
      unsigned int GetIndexCount() const override;
      unsigned int GetIndexOffset() const override;
      void SetInstanceModelMatrices(const ::array<glm::mat4>& modelmatricies) override;
      void UpdateInstanceModelMatrices(const ::array<glm::mat4>& modelMatrices) override;
      void draw_instanced() override;


      void draw(::gpu::shader * pshader) override;
   //private:
      void SetupMesh() override;




   };


} // namespace graphics3d_opengl


