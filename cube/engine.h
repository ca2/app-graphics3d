// Created by camilo on 2025-05-17 03:59 <3ThomasBorregaardSorensen!!
#pragma once



#include "apex/platform/app_consumer.h"
#include "scene_object.h"


// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>


namespace cube
{



	class CLASS_DECL_CUBE engine :
		virtual public ::app_consumer < application, ::object >
	{
	public:


		::pointer < ::cube::context > m_pcontext;


		//::pointer < ::cube::application >			m_p3dapplication;

		



		engine();
		~engine() override;


		void run() override;


		::pointer<model> create_model_from_file(const ::file::path& path)


	};


} // namespace cube


