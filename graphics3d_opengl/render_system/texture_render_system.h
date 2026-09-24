// From wavefront_obj_render_system.h by camilo on 2026-07-25 01:22 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
// From bred/graphics3d/render_systems/wavefront_obj_render_system.h
// by camilo on 2025-11-30 21:59 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/render_system/texture_render_system.h"


namespace graphics3d_opengl
{


   class CLASS_DECL_GRAPHICS3D_OPENGL texture_render_system
       : virtual public ::graphics3d::texture_render_system
   {
   public:


      texture_render_system();
      ~texture_render_system();

      void model_and_normal_matrices(::floating_matrix4 &matrixModel, ::floating_matrix4 &matrixNormal,
                                     ::graphics3d::scene_renderable *pscenerenderable) override;

      void on_before_draw_renderable(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene,
                                     ::graphics3d::scene_renderable *pscenerenderable) override;


      ::memory vert_shader_memory() override;
      ::memory frag_shader_memory() override;
   };


} // namespace graphics3d_opengl
