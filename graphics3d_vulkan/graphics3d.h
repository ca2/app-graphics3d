// From engine_instance by camilo on 2026-09-04 22:37 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "bred/graphics3d/graphics3d.h"


//#include "scene_object.h"


// libs

//  // Optional — depends on your conventions//// std
//#include <memory>
//#include <unordered_map>


namespace graphics3d_vulkan 
{

	//class SimpleRenderSystem;
	//class point_light_system;
	//class buffer;


	class CLASS_DECL_GRAPHICS3D_VULKAN graphics3d :
		virtual public ::graphics3d::graphics3d
	{
	public:


		//::pointer < context >             m_pgpucontext;
		//::pointer < ::gpu_vulkan::renderer >		m_prenderer;





		graphics3d();
		~graphics3d() override;


      void on_initialize_particle() override;


      void initialize_graphics3d() override;

		////::file::path _translate_shader_path(const ::file::path& pathShader);

		//void defer_update_engine(const ::i32_rectangle& rectanglePlacement) override;

		//void _engine_on_frame_context_initialization() override;

		//void _prepare_frame() override;

		////void do_frame_step(::gpu::context * pgpucontextUpper) override;
  //    void do_draw_layer() override;

		//void run() override;

		//void on_begin_frame() override;


		////void on_render_frame() override;
  //    void on_render_layer() override;

		////void create_global_ubo(::gpu::context* pgpucontext) override;
		////void update_global_ubo(::gpu::context* pgpucontext) override;
		////void on_render_frame() override;

  //    void calculate_impact(::floating_matrix4 &matrixImpact, const ::graphics3d::camera &camera) override;
  //    floating_matrix4 perspective(const f32_angle &angleFovY, float aspect, float zNear, float zFar) override;
		
	};

} // namespace graphics3d_vulkan 




