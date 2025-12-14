// From graphics3d_opengl/render_systems/wavefront_obj_render_system.h
// by camilo on 2025-12-03 03:16 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/render_system/wavefront_obj_render_system.h"


namespace graphics3d_vulkan
{


	class CLASS_DECL_GRAPHICS3D_VULKAN wavefront_obj_render_system
       :
		virtual public ::graphics3d::wavefront_obj_render_system
	{
	public:


		wavefront_obj_render_system();
		~wavefront_obj_render_system();

      void model_and_normal_matrices(::floating_matrix4 &matrixModel, ::floating_matrix4 &matrixNormal,
                                                   ::graphics3d::scene_renderable *pscenerenderable) override;

      void on_before_draw_renderable(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene,
                                             ::graphics3d::scene_renderable *pscenerenderable) override;

      
      ::memory vert_shader_memory() override;
      ::memory frag_shader_memory() override;


	};


} // namespace graphics3d_vulkan


