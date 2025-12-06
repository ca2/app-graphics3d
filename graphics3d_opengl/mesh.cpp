#include "framework.h"
//#include "GLFW/glfw3.h"  
#include "mesh.h"
#include "gpu_opengl/shader.h"
#include "gpu_opengl/texture.h"
#include <glad/glad.h>
// #include <vector>


namespace graphics3d_opengl
{

   mesh::mesh()
   {


   }




   mesh::~mesh() {
      //glDeleteVertexArrays(1, &m_VAO);
      //glDeleteBuffers(1, &m_VBO);
      //glDeleteBuffers(1, &m_EBO);
      //glDeleteBuffers(1, &m_InstanceVBO);  // Delete the instance VBO
   }




   //const ::array<float>& vertexes, const ::array<unsigned int>& indexes,
   //   unsigned int vertexOffset, unsigned int indexOffset, unsigned int indexCount)
   //   : m_Vertices(vertexes), m_Indices(indexes), m_VertexOffset(vertexOffset),
   //   m_IndexOffset(indexOffset), m_IndexCount(indexCount)
   //void mesh::initialize_mesh(::gpu::renderer * prenderer, )
   //   {


   //      glGenBuffers(1, &m_InstanceVBO);
   //      SetupMesh();

   //}
   // render the mesh
   void mesh::draw(::gpu::shader *pshader)
   {

      // bind appropriate textures
      unsigned int diffuseNr = 1;
      unsigned int specularNr = 1;
      unsigned int normalNr = 1;
      unsigned int heightNr = 1;
      unsigned int number;
      for (unsigned int i = 0; i < m_texturea.size(); i++)
      {

         ::cast < ::gpu_opengl::texture > ptexture = m_texturea[i];
         glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
         // retrieve texture number (the N in diffuse_textureN)
         auto strTextureType = ptexture->texture_type();
         if (strTextureType == "texture_diffuse")
            number = diffuseNr++;
         else if (strTextureType == "texture_specular")
            number = specularNr++; // transfer unsigned int to string
         else if (strTextureType == "texture_normal")
            number = normalNr++; // transfer unsigned int to string
         else if (strTextureType == "texture_height")
            number = heightNr++; // transfer unsigned int to string

         ::string strUniform;

         strUniform.formatf("%s%d",strTextureType.c_str(), i);
         // now set the sampler to the correct texture unit
         //glUniform1i(glGetUniformLocation(pshader->m_ProgramID, (name + number).c_str()), i);
         pshader->set_int(strUniform, i);
         // and finally bind the texture
         glBindTexture(GL_TEXTURE_2D, ptexture->m_gluTextureID);
      }

      // draw mesh
      //glBindVertexArray(m_VAO);
      //glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, 0);
      //glBindVertexArray(0);

      // always good practice to set everything back to defaults once configured.
      glActiveTexture(GL_TEXTURE0);

   }


   void mesh::SetupMesh() 
   {
      //glGenVertexArrays(1, &m_VAO);
      //glGenBuffers(1, &m_VBO);
      //glGenBuffers(1, &m_EBO);

      //glBindVertexArray(m_VAO);

      //// Setup the vertex buffer (VBO)
      //glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      //glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

      //// Setup the index buffer (EBO)
      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
      //glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

      //// Position attribute
      //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(m_VertexOffset * 8 * sizeof(float)));
      //glEnableVertexAttribArray(0);

      //// Color attribute
      //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(m_VertexOffset * 8 * sizeof(float) + 3 * sizeof(float)));
      //glEnableVertexAttribArray(1);

      //// texture coord attribute
      //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(m_VertexOffset * 8 * sizeof(float) + 6 * sizeof(float)));
      //glEnableVertexAttribArray(2);


      //glBindVertexArray(0);
   }

   void mesh::SetInstanceModelMatrices(const ::array<floating_matrix4>& modelMatrices) {
      //// Bind VAO
      //glBindVertexArray(m_VAO);

      //// Create and bind instance VBO for model matrices
      //glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
      //glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(floating_matrix4), modelMatrices.data(), GL_STATIC_DRAW);

      //// Set instance attributes for the model matrix (layout locations 3, 4, 5, and 6)
      //for (unsigned int i = 0; i < 4; i++) {
      //   glEnableVertexAttribArray(3 + i);
      //   glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(floating_matrix4), (void*)(i * sizeof(floating_sequence4)));
      //   glVertexAttribDivisor(3 + i, 1);  // Set attribute divisor to 1 for instanced rendering
      //}

      //glBindVertexArray(0);  // Unbind VAO
   }
   void mesh::UpdateInstanceModelMatrices(const ::array<floating_matrix4>& modelMatrices) {
      //glBindVertexArray(m_VAO);
      //glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO);
      //glBufferSubData(GL_ARRAY_BUFFER, 0, modelMatrices.size() * sizeof(floating_matrix4), modelMatrices.data());
      //glBindVertexArray(0); // Unbind VAO
   }



   void mesh::bind() 
   {
      //glBindVertexArray(m_VAO);
   }

   void mesh::unbind() 
   {
      //glBindVertexArray(0);
   }

   unsigned int mesh::GetIndexCount() const {
      return m_IndexCount;
   }

   unsigned int mesh::GetIndexOffset() const {
      return m_IndexOffset;
   }



   void mesh::draw_instanced()
   {


      //for (auto pmesh : *this)
      //{
      //   pmesh->bind();
      //   glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, (void*)(mesh->GetIndexOffset() * sizeof(unsigned int)), instanceCount);
      //   pmesh->unbind();
      //}

   }


} // namespace graphics3d_opengl




