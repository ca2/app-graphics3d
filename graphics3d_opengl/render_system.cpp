// Created by camilo on 2025-04-24 21:26 <3ThomasBorregaardSorensen!!
#include "platform.h"
#include "render_system.h"
#include "app/gpu_opengl/command_buffer.h"
#include "app/gpu_opengl/context.h"
#include "app/gpu_opengl/frame.h"
#include "app/gpu_opengl/renderer.h"
#include "app/gpu_opengl/render_target.h"
#include "app/gpu_opengl/texture.h"
//#include "app/gpu_opengl/vk_init.h"
#include "bred/graphics3d/engine.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/texture_site.h"


namespace graphics3d_opengl
{


   render_system::render_system() {}


   render_system ::~render_system() {}


   // ::gpu_opengl::render_pass *render_system::render_pass2()
   // {
   //
   //  ::cast<::gpu_opengl::renderer> prenderer = m_pengine->gpu_context()->m_pgpurenderer;
   //
   //  return prenderer->render_pass2();
   //
   //    //if (!m_pgpurenderpass)
   //    //{
   //
   //    //         constructø(m_pgpurenderpass);
   //
   //    //         m_pgpurenderpass->initialize_gpu_context_object(m_pengine->gpu_context());
   //
   //    //   m_pgpurenderpass->m_bLoadClearOp = false;
   //    //         m_pgpurenderpass->m_flags.m_bWithDepth = true;
   //
   //    //   m_pgpurenderpass->createRenderPass();
   //
   //    //}
   //
   //    //return m_pgpurenderpass;
   //
   // }

   
   
   void render_system::render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene)
   {

      if (!m_bPrepared)
      {

         return;

      }
      
      ::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();
      
      ::cast<::gpu_opengl::renderer> prenderer = pcontext->m_pgpurenderer;
      
      auto pgpulayer = ::gpu::current_layer();

      auto prendertarget = prenderer->render_target();

      //::cast<::gpu_opengl::texture> ptexture = prendertarget->current_texture(::gpu::current_layer());

      auto ptexturesite = prendertarget->current_texture(::gpu::current_layer(), true);

      auto ptexture = ptexturesite->gpu_texture();

      ::cast<::gpu_opengl::command_buffer> pcommandbuffer = pgpulayer->getCurrentCommandBuffer4();

      pcommandbuffer->m_prendersystem = this;

      on_render(pgpucontext, pscene);
      
   }



} // namespace graphics3d_opengl
