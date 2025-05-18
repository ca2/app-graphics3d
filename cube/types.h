// From landen by camilo on 2025-05-18 03:39
#pragma once


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



