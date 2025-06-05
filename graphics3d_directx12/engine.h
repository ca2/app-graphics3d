// From application_object by camilo on 2025-05-17 01:09 <3ThomasBorregaardSorensen!!
#pragma once


#include "aura/graphics/graphics3d/engine.h"


#include "scene_object.h"


// libs
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/matrix_transform.hpp>

//// std
//#include <memory>
//#include <unordered_map>


namespace graphics3d_directx12 
{

	//class SimpleRenderSystem;
	//class point_light_system;
	class buffer;


	class CLASS_DECL_GRAPHICS3D_DIRECTX12 engine :
		virtual public ::graphics3d::engine
	{
	public:


		//::pointer < context >             m_pgpucontext;
		//::pointer < ::gpu_directx12::renderer >		m_prenderer;





		engine();
		~engine() override;


		//::file::path _translate_shader_path(const ::file::path& pathShader);

		void defer_update_engine(const ::int_rectangle& rectanglePlacement) override;

		void _engine_on_frame_context_initialization() override;

		void _prepare_frame() override;

		void do_frame_step(::gpu::context * pgpucontextUpper) override;

		void run() override;

		void on_begin_frame() override;

		void on_after_done_frame_step(::draw2d::graphics_pointer& pgraphics);


		void on_render_frame() override;

		//void create_global_ubo(::gpu::context* pgpucontext) override;
		//void update_global_ubo(::gpu::context* pgpucontext) override;
		//void on_render_frame() override;

		//glm::vec3 camera_pole_up() override;

		
	};

}