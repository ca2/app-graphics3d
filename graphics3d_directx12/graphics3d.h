// From engine_instance by camilo on 2026-09-04 22:048 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "bred/graphics3d/graphics3d.h"


#include "scene_object.h"


// libs
//
//
//
#include <D3D11.h>
#include <D2D1.h>
#include <D2D1_1.h>
//// std
//#include <memory>
//#include <unordered_map>


namespace graphics3d_directx12 
{

	//class SimpleRenderSystem;
	//class point_light_system;
	//class buffer;


	class CLASS_DECL_GRAPHICS3D_DIRECTX12 graphics3d :
		virtual public ::graphics3d::graphics3d
	{
	public:


		//::pointer < context >             m_pgpucontext;
		//::pointer < ::gpu_directx12::renderer >		m_prenderer;

		graphics3d();
		~graphics3d() override;


      void on_initialize_particle() override;


      void initialize_graphics3d() override;


		////::file::path _translate_shader_path(const ::file::path& pathShader);

		//void defer_update_engine(const ::i32_rectangle& rectanglePlacement) override;

		//void _engine_on_frame_context_initialization() override;

		//void engine_on_after_load_scene(::graphics3d::scene_base* pscene) override;

		//void _prepare_frame() override;

		////void do_frame_step(::gpu::context * pgpucontextUpper) override;
  //    void do_draw_layer() override;

		//void run() override;

		//void on_begin_frame() override;

		//void on_after_done_frame_step(::draw2d::graphics_pointer& pdraw2dgraphics) override;
		////virtual void on_after_done_frame_step2(::draw2d::graphics_pointer& pdraw2dgraphics);

		////void on_render_frame() override;
  //    void on_render_layer() override;

		////void create_global_ubo(::gpu::context* pgpucontext) override;
		////void update_global_ubo(::gpu::context* pgpucontext) override;
		////void on_render_frame() override;

		////floating_sequence3 camera_pole_up() override;
  //    floating_matrix4 perspective(const f32_angle &angleFovY, float aspect, float zNear,
  //                                         float zFar) override;
	};

} // namespace graphics3d_directx12 



