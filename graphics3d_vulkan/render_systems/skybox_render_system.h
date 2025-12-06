// From graphics3d_opengl/render_systems/skybox_render_system.h
// by camilo on 2025-12-04 16:16 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/render_systems/skybox_render_system.h"


namespace graphics3d_vulkan
{


   class CLASS_DECL_GRAPHICS3D_VULKAN skybox_render_system : virtual public ::graphics3d::skybox_render_system
   {
   public:


      skybox_render_system();
      ~skybox_render_system();


      ::memory vertex_shader_memory() override;
      ::memory fragment_shader_memory() override;
      ::memory hdr_fragment_shader_memory() override;


      void on_prepare(gpu::context *pgpucontext) override;
      void on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;

     
   };


} // namespace graphics3d_vulkan



