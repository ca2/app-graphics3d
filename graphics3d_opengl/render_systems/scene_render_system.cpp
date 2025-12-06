#include "framework.h"
#include "scene_render_system.h"
#include "bred/gpu/render_target.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/scene_renderable.h"
#include "app/gpu_opengl/command_buffer.h"
#include "app/gpu_opengl/context.h"
//#include "app/gpu_opengl/descriptors.h"
#include "app/gpu_opengl/gltf/model.h"
//#include "app/gpu_opengl/pipeline.h"
//#include "app/gpu_opengl/render_pass.h"
#include "app/gpu_opengl/renderer.h"
#include "bred/gpu/frame.h"
#include "gpu_opengl/ibl/diffuse_irradiance_map.h"
#include "gpu_opengl/ibl/specular_map.h"
//#include "app/gpu_opengl/vk_init.h"
#include "shaders/scene.vert.h"
#include "shaders/scene.frag.h"


namespace graphics3d_opengl
{


   scene_render_system::scene_render_system()
   {


   }


   scene_render_system::~scene_render_system() 
   {

   
   }


   void scene_render_system::on_prepare(::gpu::context *pgpucontext)
   {

      ::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();

      øconstruct(m_pshaderOpaque);
      øconstruct(m_pshaderMask);
      øconstruct(m_pshaderBlend);

      m_pshaderOpaque->initialize_shader_with_block(pgpucontext->m_pgpurenderer, 
         ::as_memory_block(g_psz_scene_vert),
         ::as_memory_block(g_psz_scene_frag));

      m_pshaderMask->initialize_shader_with_block(pgpucontext->m_pgpurenderer, ::as_memory_block(g_psz_scene_vert),
                                         ::as_memory_block(g_psz_scene_frag));

      m_pshaderBlend->initialize_shader_with_block(pgpucontext->m_pgpurenderer, ::as_memory_block(g_psz_scene_vert),
                                          ::as_memory_block(g_psz_scene_frag));

   }


   void scene_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene)
   {

      static bool warnedThisFrame = false;

      auto pframe = ::gpu::current_frame();

      ::cast<::gpu_opengl::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

      pgpucontext->defer_bind(m_pshaderOpaque);

      auto &scenerenderables = pscene->scene_renderables();

      ::cast < ::gpu_opengl::context > pcontext = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      auto pshader = pcontext->m_pshaderBound;

      ::cast<::gpu_opengl::renderer> prenderer = pcontext->m_pgpurenderer;

      for (auto &[id, pscenerenderable]: scenerenderables)
      {

         if (!pscenerenderable)
         {

            debug("Hey, there is a null object named '{}' in scene renderables map.", id);

            continue;

         }

         if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_scene)
         {

            continue;

         }

         auto prenderable = pscenerenderable->renderable();
         
         if (!prenderable)
         {

            continue;

         }

         auto erenderabletype = prenderable->m_erenderabletype;

         if (erenderabletype != ::gpu::e_renderable_type_gltf)
         {
            
            continue;

         }

         ::cast<::gpu_opengl::gltf::model> pgltfmodel = prenderable;

         if (!pgltfmodel)
         {

            continue;

         }

         pgltfmodel->bind(pframe->m_pgpucommandbuffer);

         for (auto pmesh: pgltfmodel->m_mesha)
         {

            floating_matrix4 world = pscenerenderable->model_matrix();
            floating_matrix3 matrix3World(world);
            auto matrix3Normal = matrix3World.inversed().transposed();
            floating_matrix4 normalMat = matrix3Normal;

            m_pshaderOpaque->set_matrix4("modelMatrix", world);
            m_pshaderOpaque->set_matrix4("normalMatrix", normalMat);

            m_erendersystem = ::graphics3d::e_render_system_gltf_scene;
            pcommandbuffer->m_prendersystem = this;
            pmesh->draw(pcommandbuffer);

         }

      }

      pgpucontext->defer_unbind(m_pshaderOpaque);

   }


} // namespace graphics3d_opengl



