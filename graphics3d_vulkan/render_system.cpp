// Created by camilo on 2025-04-24 21:26 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "render_system.h"
#include "app-graphics3d/gpu_vulkan/context.h"
#include "app-graphics3d/gpu_vulkan/renderer.h"
#include "app-graphics3d/gpu_vulkan/render_target.h"
#include "bred/graphics3d/engine.h"


namespace graphics3d_vulkan
{


   render_system::render_system() {}


   render_system ::~render_system() {}


   ::gpu_vulkan::render_pass *render_system::render_pass2()
   {

      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();

      ::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;

      ::cast<::gpu_vulkan::render_target> prendertarget = prenderer->m_pgpurendertarget;

      if (prenderer->m_pgpucontext->m_escene == ::gpu::e_scene_3d)
      {

         return prendertarget->render_pass_with_depth();
      }
      else
      {

         return prendertarget->render_pass_no_depth();
      }
   }


} // namespace graphics3d_vulkan
