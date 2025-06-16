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
		
		m_gluVBO = 0;
		m_gluVAO = 0;
		m_gluEBO = 0;
		m_cIndexes = 0;

	}

	model::~model()
	{


	}


	void model::initialize_model(::gpu::renderer* pgpurenderer, const ::graphics3d::model::Builder& builder)
	{

		m_pgpurenderer = pgpurenderer;

		initialize(pgpurenderer);

		createVertexBuffers(builder.vertices);

		createIndexBuffers(builder.indices);

	}


	void model::createVertexBuffers(const ::array<::gpu::Vertex>& vertices) 
	{

		glGenVertexArrays(1, &m_gluVAO);
		glGenBuffers(1, &m_gluVBO);

		glBindVertexArray(m_gluVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_gluVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(::gpu::Vertex), vertices.data(), GL_STATIC_DRAW);

	}


	void model::createIndexBuffers(const ::array<uint32_t>& indices)
	{

		glGenBuffers(1, &m_gluEBO);

		// Upload indices (from f lines)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gluEBO);

		m_cIndexes = indices.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_cIndexes, indices.data(), GL_STATIC_DRAW);

		//// Vertex layout
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		//glEnableVertexAttribArray(0);

			// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::Vertex), (void*)offsetof(::gpu::Vertex, position));
		// vertex color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::Vertex), (void*)offsetof(::gpu::Vertex, color));
		// vertex normals
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::Vertex), (void*)offsetof(::gpu::Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(::gpu::Vertex), (void*)offsetof(::gpu::Vertex, uv));

		glBindVertexArray(0);

	}

	
	void model::bind()
	{

		glBindVertexArray(m_gluVAO);
		
	}


	void model::draw()
	{

		glDrawElements(GL_TRIANGLES, m_cIndexes, GL_UNSIGNED_INT, 0);

	}


	void model::unbind()
	{

		glBindVertexArray(0);

	}


} // namespace graphics3d_opengl



