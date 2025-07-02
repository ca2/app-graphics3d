#include "framework.h"
#include "model.h"
#include <stb/stb_image.h>
#include "context.h"
#include "bred/gpu/context.h"
#include "bred/gpu/renderer.h"


namespace graphics3d_opengl
{


	model::model() 
	{
		
		//m_gluVBO = 0;
		//m_gluVAO = 0;
		//m_gluEBO = 0;
		//m_cIndexes = 0;

	}


	model::~model()
	{


	}


	void model::initialize_model(::gpu::renderer* pgpurenderer, const ::graphics3d::model::Builder& builder)
	{

		m_pgpurenderer = pgpurenderer;

		initialize_gpu_context_object(pgpurenderer->m_pgpucontext);

		initialize(pgpurenderer);

		createVertexBuffers(builder.vertices);

		createIndexBuffers(builder.indices);

		defer_set_input_layout(pgpurenderer->m_pgpucontext->input_layout(::graphics3d::Vertex_properties()));

	}


	void model::initialize_dummy_model(::gpu::renderer* pgpurenderer, int iVertices)
	{

		initialize_gpu_context_object(pgpurenderer->m_pgpucontext);

		initialize_dummy_model(pgpurenderer, iVertices);

		//glGenVertexArrays(1, &m_gluVAO);
		// GLCheckError("");

	}


	void model::createVertexBuffers(const ::array<::graphics3d::Vertex>& vertices) 
	{

		if (vertices.is_empty())
		{

			return;

		}

		create_vertex_array<::graphics3d::Vertex>(m_pgpurenderer->m_pgpucontext,
			vertices.size());

		m_pbufferVertex->assign(vertices.data(), vertices.length_in_bytes());
		//{

		//	auto data = map < ::graphics3d::Vertex>();

		//	memcpy(data, vertices.data(), m_pbufferVertex->m_size);

		//}
		//glGenVertexArrays(1, &m_gluVAO);
		//GLCheckError("");
		//glGenBuffers(1, &m_gluVBO);
		//GLCheckError("");

		//glBindVertexArray(m_gluVAO);
		//GLCheckError("");

		//glBindBuffer(GL_ARRAY_BUFFER, m_gluVBO);
		//GLCheckError("");
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(::graphics3d::Vertex), vertices.data(), GL_STATIC_DRAW);
		//GLCheckError("");

		//glBindVertexArray(0);
		//GLCheckError("");

	}


	void model::createIndexBuffers(const ::array<uint32_t>& indices)
	{

		if (indices.is_empty())
		{

			return;

		}

		create_index_array<uint32_t>(m_pgpurenderer->m_pgpucontext,
			indices.size());

		m_pbufferIndex->assign(indices.data(), indices.length_in_bytes());

		//glBindVertexArray(m_gluVao);
		//GLCheckError("");

		//::cast < ::gpu_opengl::memory_buffer > pbuffer = m_pbufferIndex;

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbuffer->m_gluVbo);
		//GLCheckError("");

		////glGenBuffers(1, &m_gluEBO);
		////GLCheckError("");

		////// Upload indices (from f lines)
		////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gluEBO);
		////GLCheckError("");

		////m_cIndexes = indices.size();
		////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_cIndexes, indices.data(), GL_STATIC_DRAW);
		////GLCheckError("");

		//////// Vertex layout
		//////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		//////glEnableVertexAttribArray(0);

		//glBindVertexArray(0);
		//GLCheckError("");

	}

	
	void model::bind()
	{

		auto pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

		gpu_opengl::model_buffer::bind(pcommandbuffer);
		// glBindVertexArray(m_gluVAO);
		// GLCheckError("");
		
	}


	void model::draw()
	{

		auto pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

		::gpu_opengl::model_buffer::draw(pcommandbuffer);

		//if (m_iVertices > 0 && m_cIndexes <= 0)
		//{

		//	glDrawArrays(GL_TRIANGLES, 0, m_iVertices);
		//	GLCheckError("");

		//}
		//else
		//{

		//	glDrawElements(GL_TRIANGLES, m_cIndexes, GL_UNSIGNED_INT, 0);
		//	GLCheckError("");

		//}

	}


	void model::unbind()
	{

		auto pcommandbuffer = m_pgpurenderer->getCurrentCommandBuffer2();

		::gpu_opengl::model_buffer::unbind(pcommandbuffer);


		//glBindVertexArray(0);
		//GLCheckError("");

	}


} // namespace graphics3d_opengl



