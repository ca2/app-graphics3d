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


		::pointer < ::cube::context >				m_pcontext;

		::string_map < ::pointer < scene > >	m_mapScene;
		//::pointer < ::cube::application >			m_p3dapplication;

		::pointer < scene >							m_pscene;



		engine();
		~engine() override;


		void run() override;



		virtual void on_layout(int cx, int cy);

		virtual void on_mouse_move(int x, int y);

		virtual ::pointer<model> create_model_from_file(const ::file::path& path);

		virtual void add_scene(::cube::scene* pscene);

		virtual void set_current_scene(::cube::scene* pscene);


	};


} // namespace cube


