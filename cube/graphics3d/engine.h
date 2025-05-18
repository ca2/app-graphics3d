// Created by camilo on 2025-05-17 03:59 <3ThomasBorregaardSorensen!!
#pragma once



#include "apex/platform/app_consumer.h"
#include "app-cube/cube/graphics3d/scene_object.h"


// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>


namespace graphics3d
{



	class CLASS_DECL_CUBE engine :
		virtual public ::app_consumer < ::cube::application, ::object >
	{
	public:


		::pointer < ::graphics3d::context >		m_pcontext;
		::pointer < ::cube::impact >				m_pimpact;
		::string_map < ::pointer < scene > >	m_mapScene;
		//::pointer < ::cube::application >			m_p3dapplication;

		::pointer < scene >							m_pscene;

		::pointer < ::graphics3d::input >		m_pinput;
		::pointer < ::graphics3d::camera >		m_pcamera;

		engine();
		~engine() override;


		void run() override;


		virtual void on_start_engine();

		virtual void on_layout(int cx, int cy);

		virtual void on_mouse_move(int x, int y);

		virtual void on_render_frame();

		virtual void on_update_frame(TransformComponent& transform, float frameTime);

		virtual ::pointer<model> create_model_from_file(const ::file::path& path);

		virtual void add_scene(::graphics3d::scene* pscene);

		virtual void set_current_scene(::graphics3d::scene* pscene);


	};


} // namespace cube


