// Created by camilo on 2025-04-24 21:26 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/render_system.h"


namespace graphics3d_opengl
{


   class render_system : 
         virtual public ::graphics3d::render_system
   {
   public:


      //::pointer<::gpu_opengl::render_pass> m_pgpurenderpass;

      render_system();
      ~render_system();


      //virtual ::gpu_opengl::render_pass *render_pass2();

      void render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene) override;


   };


} // namespace graphics3d_vulkan
 




