// Created by camilo on 2025-05-17 03:59 <3ThomasBorregaardSorensen!!
#pragma once



#include "apex/platform/app_consumer.h"
#include "app-cube/cube/graphics3d/scene_object.h"
#include "app-cube/cube/gpu/properties.h"
#include "app-cube/cube/gpu/shader.h"

// libs
#include <glm/gtc/matrix_transform.hpp>
//
//// std
//#include <memory>
//#include <unordered_map>


namespace graphics3d
{



	class CLASS_DECL_CUBE engine :
		virtual public ::app_consumer < ::cube::application, ::object >
	{
	public:


		::pointer < ::gpu::approach >				m_papproach;
		::pointer < ::gpu::context >				m_pgpucontext;
		::pointer < ::cube::impact >				m_pimpact;
		::string_map < ::pointer < scene > >	m_mapScene;
		//::pointer < ::cube::application >			m_p3dapplication;
		//::pointer < ::graphics3d::renderer >				m_prenderer;
		::pointer < scene >							m_pscene;

		::pointer < ::graphics3d::input >		m_pinput;
		::pointer < ::graphics3d::camera >		m_pcamera;

		TransformComponent							m_transform;
		float												m_fFrameTime;

		bool												m_bWireframeMode = false;


		engine();
		~engine() override;


		virtual void initialize_engine(::cube::impact* pimpact);


		virtual void create_global_ubo(::gpu::context * pgpucontext);

		virtual void defer_start(const ::int_rectangle& rectangle);

		virtual void on_begin_frame();

		virtual void update_global_ubo(::gpu::context* pgpucontext);

		float dt() { return m_fFrameTime; }

		//void on_begin_frame() override;
		void run() override;


		virtual void start_engine(const ::int_rectangle & rectangle);

		virtual void on_layout(int cx, int cy);

		virtual void on_mouse_move(int x, int y);

		virtual void on_render_frame();

		virtual void on_update_frame();

		virtual ::pointer<model> create_tinyobjloader_model(const ::file::path& path);

		virtual void add_scene(::graphics3d::scene* pscene);

		virtual void set_current_scene(::graphics3d::scene* pscene);


	};


} // namespace cube


