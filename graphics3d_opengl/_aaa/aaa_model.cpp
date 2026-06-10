#include "../framework.h"
//#include "model.h"
//#include <stb/stb_image.h>
//#include "context.h"
//#include "bred/gpu/context.h"
//#include "bred/gpu/renderer.h"
//
//
//namespace graphics3d_opengl
//{
//
//
//   
//   model::model()
//   {
//
//      //m_gluVBO = 0;
//      //m_gluVAO = 0;
//      //m_gluEBO = 0;
//      //m_cIndexes = 0;
//
//   }
//
//   
//   model::~model()
//   {
//
//
//   }
//
//   
//   void model::initialize_model(::gpu::renderer* pgpurenderer, const ::block& blockVertices, const ::block& blockIndices)
//   {
//
//      this->m_pgpurenderer = pgpurenderer;
//
//      initialize_gpu_context_object(pgpurenderer->m_pgpucontext);
//
//      initialize(pgpurenderer);
//
//      glBindVertexArray(m_gluVao);
//      ::opengl::check_error("");
//
//      createVertexBuffers(blockVertices);
//
//      createIndexBuffers(blockIndices);
//
//      glBindVertexArray(0);
//      ::opengl::check_error("");
//
//      //defer_set_input_layout(pgpurenderer->m_pgpucontext->input_layout<::graphics3d::Vertex>());
//
//   }
//
//   
//   void model::initialize_dummy_model(::gpu::renderer* pgpurenderer, int iVertices)
//   {
//
//      initialize_gpu_context_object(pgpurenderer->m_pgpucontext);
//
//      this->m_pgpurenderer = pgpurenderer;
//
//      this->m_iVertexCount = iVertices;
//
//      //glGenVertexArrays(1, &m_gluVAO);
//      // ::opengl::check_error("");
//
//   }
//
//   
//   void model::createVertexBuffers(const ::block& blockVertices)
//   {
//
//      if (blockVertices.is_empty())
//      {
//
//         return;
//
//      }
//
//      static_initialize_vertices_block(blockVertices);
//
//      //set_vertex_array<::graphics3d::Vertex>(vertices.data(), (int) vertices.size());
//      //{
//
//      //	auto data = map < ::graphics3d::Vertex>();
//
//      //	memcpy(data, vertices.data(), m_pbufferVertex->m_size);
//
//      //}
//      //glGenVertexArrays(1, &m_gluVAO);
//      //::opengl::check_error("");
//      //glGenBuffers(1, &m_gluVBO);
//      //::opengl::check_error("");
//
//      //glBindVertexArray(m_gluVAO);
//      //::opengl::check_error("");
//
//      //glBindBuffer(GL_ARRAY_BUFFER, m_gluVBO);
//      //::opengl::check_error("");
//      //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(::graphics3d::Vertex), vertices.data(), GL_STATIC_DRAW);
//      //::opengl::check_error("");
//
//      //glBindVertexArray(0);
//      //::opengl::check_error("");
//
//   }
//
//   
//   void model::createIndexBuffers(const ::block& blockIndices)
//   {
//
//      static_initialize_indices_block(blockIndices);
//
//      //set_index_array<uint32_t>(indices.data(), indices.size());
//
//      //glBindVertexArray(m_gluVao);
//      //::opengl::check_error("");
//
//      //::cast < ::gpu_opengl::memory_buffer > pbuffer = m_pbufferIndex;
//
//      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbuffer->m_gluVbo);
//      //::opengl::check_error("");
//
//      ////glGenBuffers(1, &m_gluEBO);
//      ////::opengl::check_error("");
//
//      ////// Upload indices (from f lines)
//      ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gluEBO);
//      ////::opengl::check_error("");
//
//      ////m_cIndexes = indices.size();
//      ////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_cIndexes, indices.data(), GL_STATIC_DRAW);
//      ////::opengl::check_error("");
//
//      //////// Vertex layout
//      //////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(floating_sequence3), (void*)0);
//      //////glEnableVertexAttribArray(0);
//
//      //glBindVertexArray(0);
//      //::opengl::check_error("");
//
//   }
//
//   
//   void model::bind()
//   {
//
//      auto pcommandbuffer = this->m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());
//
//      gpu_opengl::model_buffer::bind(pcommandbuffer);
//      // glBindVertexArray(m_gluVAO);
//      // ::opengl::check_error("");
//
//   }
//
//   
//   void model::draw()
//   {
//
//      auto pcommandbuffer = this->m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());
//
//      ::gpu_opengl::model_buffer::draw(pcommandbuffer);
//
//      //if (m_iVertices > 0 && m_cIndexes <= 0)
//      //{
//
//      //	glDrawArrays(GL_TRIANGLES, 0, m_iVertices);
//      //	::opengl::check_error("");
//
//      //}
//      //else
//      //{
//
//      //	glDrawElements(GL_TRIANGLES, m_cIndexes, GL_UNSIGNED_INT, 0);
//      //	::opengl::check_error("");
//
//      //}
//
//   }
//
//   
//   void model::unbind()
//   {
//
//      auto pcommandbuffer = this->m_pgpurenderer->getCurrentCommandBuffer2(::gpu::current_layer());
//
//      ::gpu_opengl::model_buffer::unbind(pcommandbuffer);
//
//
//      //glBindVertexArray(0);
//      //::opengl::check_error("");
//
//   }
//
//
//} // namespace graphics3d_opengl
//
//
//
