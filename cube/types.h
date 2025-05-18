// From landen by camilo on 2025-05-18 03:39
#pragma once

// libs
#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
namespace cube
{

	namespace geometry = glm;

	using pole = glm::vec3;
	using location = glm::vec3;
	using quad_matrix = glm::mat4;



} // namespace cube




namespace user
{


	enum enum_key_state
	{

		e_key_state_none,
		e_key_state_pressed = 1,

	};


} // namespace user


namespace cube
{

	/** @brief State of mouse/touch input */
	struct mouse_state
	{


		struct
		{


			bool left = false;
			bool right = false;
			bool middle = false;


		} m_buttons;


		::glm::vec2		m_position;


	};


} // namespace cube

