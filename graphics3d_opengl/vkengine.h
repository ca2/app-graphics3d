// From application_object by camilo on 2025-05-17 01:09 <3ThomasBorregaardSorensen!!
#pragma once


#include "cube/engine.h"


#include "scene_object.h"


// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>


namespace graphics3d_opengl 
{



	class CLASS_DECL_GRAPHICS3D_OPENGL engine :
		virtual public ::cube::engine
	{
	public:


		//::pointer < context >             m_pgpucontext;
		::pointer < renderer >				m_prenderer;


		//::pointer <descriptor_pool>		m_pglobalpool;

		//::graphics3d::application_object::map				m_mapObjects;



		engine();
		~engine() override;


		void run() override;


		
	};

}