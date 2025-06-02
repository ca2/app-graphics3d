#include "framework.h"
#include "model.h"
#include <stb/stb_image.h>
#include "context.h"
#include "aura/graphics/gpu/context.h"
//#include <iostream>


namespace graphics3d_opengl
{


	// Constructor
	//model::model(std::string const& path, bool gamma) : gammaCorrection(gamma)
	model::model() 
	{
		//loadModel(path);
		m_gluVBO = 0;
		m_gluVAO = 0;
		m_gluEBO = 0;
		m_cIndexes = 0;
	}

	model::~model()
	{


	}


	void model::initialize_model(::gpu::context* pgpucontext, const ::graphics3d::model::Builder& builder)
	{

		m_pgpucontext = pgpucontext;

		initialize(pgpucontext);

		createVertexBuffers(builder.vertices);

		createIndexBuffers(builder.indices);

	}


	//// Draws the model and all its meshes
	//void model::Draw(glc::shader* pshader)
	//{
	//	for (unsigned int i = 0; i < meshes.size(); i++)
	//		meshes[i]->Draw(pshader);
	//}


	void model::createVertexBuffers(const ::array<::gpu::Vertex>& vertices) 
	{
		/*vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		uint32_t vertexSize = sizeof(vertices[0]);

		buffer stagingBuffer;

		stagingBuffer.initialize_buffer(
			m_pgpucontext,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)vertices.data());

		vertexBuffer = __allocate buffer;

		vertexBuffer->initialize_buffer(
			m_pgpucontext,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		);


		m_pgpucontext->copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);*/


		glGenVertexArrays(1, &m_gluVAO);
		glGenBuffers(1, &m_gluVBO);

		glBindVertexArray(m_gluVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_gluVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(::gpu::Vertex), vertices.data(), GL_STATIC_DRAW);

	}


	void model::createIndexBuffers(const ::array<uint32_t>& indices) {
		//indexCount = static_cast<uint32_t>(indices.size());
		//hasIndexBuffer = indexCount > 0;

		//if (!hasIndexBuffer) {
		//	return;
		//}

		//VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		//uint32_t indexSize = sizeof(indices[0]);

		//buffer stagingBuffer;
		//stagingBuffer.initialize_buffer(
		//	m_pgpucontext,
		//	indexSize,
		//	indexCount,
		//	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
		//	;

		//stagingBuffer.map();
		//stagingBuffer.writeToBuffer((void*)indices.data());

		//indexBuffer = __allocate buffer();

		//indexBuffer->initialize_buffer(
		//	m_pgpucontext,
		//	indexSize,
		//	indexCount,
		//	VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		//	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		//m_pgpucontext->copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
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

	
	void model::bind(::gpu::context* pgpucontext)
	{
		glBindVertexArray(m_gluVAO);
		//auto pframeinfo = (FrameInfo*)p;
		//VkBuffer buffers[] = { vertexBuffer->getBuffer() };
		//VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(pframeinfo->commandBuffer, 0, 1, buffers, offsets);

		//if (hasIndexBuffer) {
		//	vkCmdBindIndexBuffer(pframeinfo->commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		//}
	}


	void model::draw(::gpu::context* pgpucontext)
	{

		glDrawElements(GL_TRIANGLES, m_cIndexes, GL_UNSIGNED_INT, 0);


		//glBindVertexArray(m_gluVao);
		//auto pframeinfo = (FrameInfo*)p;
		//VkBuffer buffers[] = { vertexBuffer->getBuffer() };
		//VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(pframeinfo->commandBuffer, 0, 1, buffers, offsets);

		//if (hasIndexBuffer) {
		//	vkCmdBindIndexBuffer(pframeinfo->commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		//}
	}


	void model::unbind(::gpu::context* pgpucontext)
	{

		glBindVertexArray(0);

	}

	//// Loads a model from file and stores the resulting meshes
	//void model::loadModel(std::string const& path)
	//{
	//	Assimp::Importer importer;
	//	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//	{
	//		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
	//		return;
	//	}
	//	directory = path.substr(0, path.find_last_of('/'));
	//	processNode(scene->mRootNode, scene);
	//}

	//// Processes a node recursively, handling each mesh and child node
	//void model::processNode(aiNode* node, const aiScene* scene)
	//{
	//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	//	{
	//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	//		meshes.push_back(processMesh(mesh, scene));
	//	}
	//	for (unsigned int i = 0; i < node->mNumChildren; i++)
	//	{
	//		processNode(node->mChildren[i], scene);
	//	}
	//}

	//// Processes an individual mesh and extracts vertex, index, and texture data
	//ModelMesh model::processMesh(aiMesh* mesh, const aiScene* scene)
	//{
	//	::array<Vertex> vertices;
	//	::array<unsigned int> indices;
	//	::array<ModelTexture> textures;

	//	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	//	{
	//		Vertex vertex;
	//		glm::vec3 vector;

	//		vector.x = mesh->mVertices[i].x;
	//		vector.y = mesh->mVertices[i].y;
	//		vector.z = mesh->mVertices[i].z;
	//		vertex.Position = vector;

	//		if (mesh->HasNormals())
	//		{
	//			vector.x = mesh->mNormals[i].x;
	//			vector.y = mesh->mNormals[i].y;
	//			vector.z = mesh->mNormals[i].z;
	//			vertex.Normal = vector;
	//		}

	//		if (mesh->mTextureCoords[0])
	//		{
	//			glm::vec2 vec;
	//			vec.x = mesh->mTextureCoords[0][i].x;
	//			vec.y = mesh->mTextureCoords[0][i].y;
	//			vertex.TexCoords = vec;

	//			vector.x = mesh->mTangents[i].x;
	//			vector.y = mesh->mTangents[i].y;
	//			vector.z = mesh->mTangents[i].z;
	//			vertex.Tangent = vector;

	//			vector.x = mesh->mBitangents[i].x;
	//			vector.y = mesh->mBitangents[i].y;
	//			vector.z = mesh->mBitangents[i].z;
	//			vertex.Bitangent = vector;
	//		}
	//		else
	//			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

	//		vertices.push_back(vertex);
	//	}

	//	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	//	{
	//		aiFace face = mesh->mFaces[i];
	//		for (unsigned int j = 0; j < face.mNumIndices; j++)
	//			indices.push_back(face.mIndices[j]);
	//	}

	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//	::array<ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//	::array<ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//	::array<ModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//	::array<ModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	//	return ModelMesh(vertices, indices, textures);
	//}

	//// Loads material textures, ensuring each is only loaded once
	//::array<ModelTexture> model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	//{
	//	::array<ModelTexture> textures;
	//	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	//	{
	//		aiString str;
	//		mat->GetTexture(type, i, &str);
	//		bool skip = false;
	//		for (unsigned int j = 0; j < textures_loaded.size(); j++)
	//		{
	//			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
	//			{
	//				textures.push_back(textures_loaded[j]);
	//				skip = true;
	//				break;
	//			}
	//		}
	//		if (!skip)
	//		{
	//			ModelTexture texture;
	//			texture.id = TextureFromFile(str.C_Str(), this->directory);
	//			texture.type = typeName;
	//			texture.path = str.C_Str();
	//			textures.push_back(texture);
	//			textures_loaded.push_back(texture);
	//		}
	//	}
	//	return textures;
	//}

	//// Loads a texture from file
	//unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
	//{
	//	std::string filename = directory + '/' + std::string(path);
	//	unsigned int textureID;
	//	glGenTextures(1, &textureID);

	//	int width, height, nrComponents;
	//	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	//	if (data)
	//	{
	//		GLenum format = nrComponents == 1 ? GL_RED : (nrComponents == 3 ? GL_RGB : GL_RGBA);
	//		glBindTexture(GL_TEXTURE_2D, textureID);
	//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//		glGenerateMipmap(GL_TEXTURE_2D);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//		stbi_image_free(data);
	//	}
	//	else
	//	{
	//		std::cout << "texture failed to load at path: " << path << std::endl;
	//		stbi_image_free(data);
	//	}

	//	return textureID;
	//}


} // namespace graphics3d_opengl



