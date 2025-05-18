// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 03:54 <3ThomasBorregaardSorensen!!
#pragma once

#include "app-cube/cube/graphics3d/model.h"
#include "app-cube/cube/graphics3d/types.h"

#include "acme/prototype/collection/map.h"
// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>


namespace graphics3d
{


	struct CLASS_DECL_CUBE TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)

		glm::mat4 mat4();
		glm::mat3 normalMatrix();

	};

	struct CLASS_DECL_CUBE PointLightComponent :
		virtual public ::particle {
		float lightIntensity = 1.0f;
	};


	class CLASS_DECL_CUBE scene_object :
		virtual public ::particle
	{
	public:

		::collection::index m_iId;

		TransformComponent m_transform;

		glm::vec3 m_color;

		::pointer<model> m_pmodel;

		using map = index_map < ::pointer < scene_object > >;

		static interlocked_long_long s_interlockedll;


		scene_object() :
			m_iId{ s_interlockedll++ }
		{

		}

		scene_object& translate(const location& locationOffset)
		{

			m_transform.translation += locationOffset;

			return *this;

		}

		scene_object& scale(const pole& poleScaling)
		{

			m_transform.scale *= poleScaling;

			return *this;

		}
	};


	class CLASS_DECL_CUBE point_light :
		virtual public scene_object
	{
	public:


		PointLightComponent	m_pointlightcomponent;


		point_light(float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));
		~point_light() override;

	};


} // namespace graphics3d




