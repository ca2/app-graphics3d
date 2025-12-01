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

      øconstruct(m_ppipelineOpaque);
      øconstruct(m_ppipelineMask);
      øconstruct(m_ppipelineBlend);

      m_ppipelineOpaque->initialize_shader(pgpucontext->m_pgpurenderer,
         "matter://shaders/scene_vert.vert",
         "matter://shaders/scene_frag.frag");

      m_ppipelineMask->initialize_shader(pgpucontext->m_pgpurenderer,
         "matter://shaders/scene_vert.vert",
         "matter://shaders/scene_frag.frag");

      m_ppipelineBlend->initialize_shader(pgpucontext->m_pgpurenderer,
         "matter://shaders/scene_vert.vert",
         "matter://shaders/scene_frag.frag");

   }


   void scene_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene)
   {

      static bool warnedThisFrame = false;

      auto pframe = ::gpu::current_frame();

      ::cast<::gpu_opengl::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

      pgpucontext->defer_bind(m_ppipelineOpaque);

      auto &scenerenderables = pscene->scene_renderables();

      ::cast < ::gpu_opengl::context > pcontext = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      auto pshader =pcontext->m_pshaderBound;

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
            
            continue; // not mine, skip

         }

         ::cast<::gpu_opengl::gltf::model> pgltfmodel = prenderable;

         if (!pgltfmodel)
         {

            continue;

         }

         pgltfmodel->bind(pframe->m_pgpucommandbuffer);

         for (auto pmesh: pgltfmodel->m_mesha)
         {
            //if (!node->mesh)
              // continue;

            floating_matrix4 world = pscenerenderable->model_matrix();
            floating_matrix4 normalMat = world.inversed().transposed();

            m_ppipelineOpaque->set_matrix4("modelMatrix", world);
            m_ppipelineOpaque->set_matrix4("normalMatrix", normalMat);
            //m_ppipelineOpaque->set_matrix4("view", pscene->global_ubo().mat4("view"));
            //m_ppipelineOpaque->set_matrix4("projection", pscene->global_ubo().mat4("projection"));

            //memcpy(pmesh->uniformBuffer.mapped, &world, sizeof(world));
            //memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
            m_erendersystem = ::graphics3d::e_render_system_gltf_scene;
            pcommandbuffer->m_prendersystem = this;
            pmesh->draw(pcommandbuffer);
            // for (auto *primitive: node->mesh->primitives)
            // {
            //
            //
            //    std::array<VkDescriptorSet, 2> sets = {vkdescriptorsetGlobal, // set 0
            //                                           node->mesh->uniformBuffer.descriptorSet};
            //
            //
            //    vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0,
            //                            static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
            //
            //    switch (primitive->m_pmaterial->alphaMode)
            //    {
            //       case ::gpu_opengl::gltf::Material::ALPHAMODE_OPAQUE:
            //          m_ppipelineOpaque->bind(pcommandbuffer);
            //          break;
            //       case ::gpu_opengl::gltf::Material::ALPHAMODE_MASK:
            //          m_ppipelineMask->bind(pcommandbuffer);
            //          break;
            //       case ::gpu_opengl::gltf::Material::ALPHAMODE_BLEND:
            //       default:
            //          m_ppipelineBlend->bind(pcommandbuffer);
            //          break;
            //    }
            //
            //    pgltfmodel->drawNode(node, pcommandbuffer->m_vkcommandbuffer,
            //                         ::gpu_opengl::gltf::RenderFlags::BindImages,
            //                    m_pipelineLayout, 2);
            //    warnedThisFrame = false;
            // }
         }
      }

      pgpucontext->defer_unbind(m_ppipelineOpaque);

   }


} // namespace graphics3d_opengl
