#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace graphics3d_opengl
{



   class mesh:
      virtual public ::particle
   {
   public:


      mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
         unsigned int vertexOffset, unsigned int indexOffset, unsigned int indexCount);
      ~mesh() override;


      void Bind() const;
      void Unbind() const;
      unsigned int GetIndexCount() const;
      unsigned int GetIndexOffset() const;
      void SetInstanceModelMatrices(const std::vector<glm::mat4>& modelmatricies);
      void UpdateInstanceModelMatrices(const std::vector<glm::mat4>& modelMatrices);



      void Draw(shader * pshader);
   //private:
      void SetupMesh();

      ::pointer_array<texture>      m_texturea;
      std::vector<float> m_Vertices;
      std::vector<unsigned int> m_Indices;
      unsigned int m_VAO, m_VBO, m_EBO;
      unsigned int m_IndexOffset;
      unsigned int m_IndexCount;
      unsigned int m_VertexOffset;
      std::vector<glm::mat4> m_ModelMatrices;
      // New instance buffer
      unsigned int m_InstanceVBO;


   };


} // namespace graphics3d_opengl


