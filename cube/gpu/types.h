// Created by camilo on 2025-05-22 09:30 PM <3ThomasBorregaardSoerensen!!
#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <functional>


namespace gpu
{


	// from: https://stackoverflow.com/a/57595105
	template <typename T, typename... Rest>
	void hash_combine(::hash32& seed, const T& v, const Rest&... rest)
	{
		seed.m_u ^= ::hash32((::uptr)(size_t)::std::hash<T>{}(v)).m_u + 0x9e3779b9 + (seed.m_u << 6) + (seed.m_u >> 2);
		(hash_combine(seed, rest), ...);
	};


	struct Vertex
	{

		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};

		//static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		//static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& other) const { return position == other.position && normal == other.normal && uv == other.uv; }

	};


} // namespace gpu


template < >
inline ::hash32 as_hash32<gpu::Vertex>(const ::gpu::Vertex& vertex)
{

	hash32 seed{};

	::gpu::hash_combine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);

	return seed;

}



