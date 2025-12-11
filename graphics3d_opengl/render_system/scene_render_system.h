#pragma once


#include "app-graphics3d/graphics3d_opengl/render_system.h"
#include "bred/graphics3d/render_systems/scene_render_system.h"


namespace graphics3d_opengl
{


   class CLASS_DECL_GRAPHICS3D_OPENGL scene_render_system : virtual public ::graphics3d::scene_render_system,
                               virtual public ::graphics3d_opengl::render_system
   {
   public:
      const int TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP = 10;
      const int TEXTURE_UNIT_PREFILTERED_ENV_MAP = 11;
      const int TEXTURE_UNIT_BRDF_CONVOLUTION_MAP = 12;

      enum BloomDirection {
         BOTH = 0,
         HORIZONTAL = 1,
         VERTICAL = 2
     };


      ::pointer<::gpu::shader> m_pshaderOpaque;
      ::pointer<::gpu::shader> m_pshaderMask;
      ::pointer<::gpu::shader> m_pshaderBlend;

      scene_render_system();
      ~scene_render_system();

      void on_prepare(::gpu::context *pgpucontext) override;
 
      
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_opengl
