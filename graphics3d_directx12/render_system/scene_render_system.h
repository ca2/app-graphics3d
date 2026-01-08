#pragma once

#include "app-graphics3d/graphics3d_directx12/render_system.h"
#include "app-graphics3d/graphics3d/render_system/scene_render_system.h"


namespace graphics3d_directx12
{


   class scene_render_system : virtual public ::graphics3d::scene_render_system,
                               virtual public ::graphics3d_directx12::render_system
   {
   public:

      
   
      scene_render_system();
      ~scene_render_system();

      void on_prepare(::gpu::context *pgpucontext) override;

      ::memory pbr_with_ibl_vert_memory() override;
      ::memory pbr_with_ibl_frag_memory() override;
      
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_directx12
