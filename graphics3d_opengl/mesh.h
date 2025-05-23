#pragma once
//#include <vector>
#include <glm/glm.hpp>

namespace graphics3d_opengl
{



   class mesh:
      virtual public ::particle
   {
   public:


      mesh(const ::array<float>& vertices, const ::array<unsigned int>& indices,
         unsigned int vertexOffset, unsigned int indexOffset, unsigned int indexCount);
      ~mesh() override;


      void Bind() const;
      void Unbind() const;
      unsigned int GetIndexCount() const;
      unsigned int GetIndexOffset() const;
      void SetInstanceModelMatrices(const ::array<glm::mat4>& modelmatricies);
      void UpdateInstanceModelMatrices(const ::array<glm::mat4>& modelMatrices);



      void Draw(::gpu::shader * pshader);
   //private:
      void SetupMesh();

      ::pointer_array<texture>      m_texturea;
      ::array<float> m_Vertices;
      ::array<unsigned int> m_Indices;
      unsigned int m_VAO, m_VBO, m_EBO;
      unsigned int m_IndexOffset;
      unsigned int m_IndexCount;
      unsigned int m_VertexOffset;
      ::array<glm::mat4> m_ModelMatrices;
      // New instance buffer
      unsigned int m_InstanceVBO;


   };


} // namespace graphics3d_opengl


