// From V0idsEmbrace.gltf_render_system by
// camilo on 2025-04-24 21:28 <3ThomasBorregaardSorensen!!
#include "framework.h"
// #include "bred/gpu/gltf_model.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "app/gpu_opengl/command_buffer.h"
// #include "app/gpu_opengl/descriptors.h"
#include "app/gpu_opengl/gltf/model.h"
// #include "app/gpu_opengl/pipeline.h"
// #include "app/gpu_opengl/render_pass.h"
#include "app/gpu_opengl/render_target.h"
#include "app/gpu_opengl/renderer.h"
#include "app/gpu_opengl/texture.h"
// #include "app/gpu_opengl/vk_init.h"
#include "app-graphics3d/graphics3d/scene.h"
#include "bred/gpu/frame.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/scene_renderable.h"
#include "gltf_render_system.h"
#include "gpu_opengl/ibl/diffuse_irradiance_map.h"
#include "gpu_opengl/ibl/specular_map.h"
// #include <stdexcept>
#include "app-graphics3d/graphics3d_opengl//render_system/shader/gltf.frag.h"
#include "app-graphics3d/graphics3d_opengl//render_system/shader/gltf.vert.h"
//#include "gpu/ibl/cubemap_framebuffer.h"


namespace graphics3d_opengl
{


   gltf_render_system::gltf_render_system() {}


   gltf_render_system::~gltf_render_system()
   {

      //   vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
   }


   
   ::memory gltf_render_system::pbr_with_ibl_vert_memory()
   {

//      static unsigned int pvertexshader[] = {
//#include "render_system/shader/gltf.vert.spv.inl"
//
//      };

      return ::as_memory_block(g_psz_gltf_vert);
   }


   ::memory gltf_render_system::pbr_with_ibl_frag_memory()
   {

      
      return ::as_memory_block(g_psz_gltf_frag);
   }


   // void gltf_render_system::init(::gpu_opengl::descriptor_set_layout *psetdescriptorlayoutGlobal,
   //::pointer<::gpu_opengl::descriptor_set_layout> &descriptorPool, size_t frameCount)
   void gltf_render_system::on_prepare(::gpu::context *pgpucontext)
   {

      ::graphics3d::gltf_render_system::on_prepare(pgpucontext);
      //// m_pdescriptorsetlayoutUbo = globalSetLayout;

      //::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();

      //øconstruct(m_pshaderOpaque);
      //øconstruct(m_pshaderMask);
      //øconstruct(m_pshaderBlend);

      //// m_ppipelineOpaque->initialize_shader(pgpucontext->m_pgpurenderer, "matter://shaders/gltf_vert.vert",
      ////                                      "matter://shaders/gltf_frag.frag");

      //// m_ppipelineMask->initialize_shader(pgpucontext->m_pgpurenderer, "matter://shaders/gltf_vert.vert",
      ////                                    "matter://shaders/gltf_frag.frag");

      //// m_ppipelineBlend->initialize_shader(pgpucontext->m_pgpurenderer, "matter://shaders/gltf_vert.vert",
      ////                                     "matter://shaders/gltf_frag.frag");
      //m_pshaderOpaque->initialize_shader_with_block(pgpucontext->m_pgpurenderer, as_memory_block(g_psz_gltf_vert),
      //                                               as_memory_block(g_psz_gltf_frag));

      //m_pshaderMask->initialize_shader_with_block(pgpucontext->m_pgpurenderer, as_memory_block(g_psz_gltf_vert),
      //                                            as_memory_block(g_psz_gltf_frag));


      //m_pshaderBlend->initialize_shader_with_block(pgpucontext->m_pgpurenderer, as_memory_block(g_psz_gltf_vert),
      //                                             as_memory_block(g_psz_gltf_frag));
   }


   void gltf_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscenebase)
   {

      ::graphics3d::gltf_render_system::on_render(pgpucontext, pscenebase);
      

      //static bool warnedThisFrame = false;

      //auto pframe = ::gpu::current_frame();

      //auto pskybox = pscenebase->current_skybox();

      //if (::is_null(pskybox))
      //{

      //   return;
      //}

      //::cast<::gpu_opengl::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

      //pgpucontext->defer_bind(m_pshaderOpaque);
      //auto &scenerenderables = pscenebase->scene_renderables();

      ////   //// xxxxxxxxxxxxxxxxx
      //::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();
      //::cast<::gpu_opengl::renderer> prenderer = pcontext->m_pgpurenderer;
      //auto pshader = pcontext->m_pshaderBound;
      //::cast<::graphics3d::scene> pscene = pscenebase;
      ////// xxxxxxxxxxxxxxxxx
      //// auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      //// auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);
      ////  IBL stuff
      //// glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
      //// GLCheckError("");
      //// pshader->set_int("diffuseIrradianceMap", TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
      //// ::cast<::gpu_opengl::ibl::diffuse_irradiance_map> pirradiancemap = pscene->m_pibldiffuseirradiancemap;
      //// int iCubemapId = pirradiancemap->getCubemapId();
      //// glBindTexture(GL_TEXTURE_CUBE_MAP, iCubemapId);
      //// GLCheckError("");
      ////
      //// glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_PREFILTERED_ENV_MAP);
      //// GLCheckError("");
      //// pshader->set_int("prefilteredEnvMap", TEXTURE_UNIT_PREFILTERED_ENV_MAP);
      //// ::cast<::gpu_opengl::ibl::specular_map> pspecularmap = pscene->m_piblspecularmap;
      //// int iPrefilteredEnvMapId = pspecularmap->getPrefilteredEnvMapId();
      //// glBindTexture(GL_TEXTURE_CUBE_MAP, iPrefilteredEnvMapId);
      //// GLCheckError("");
      ////
      //// glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
      //// GLCheckError("");
      //// pshader->set_int("brdfConvolutionMap", TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
      //// int iBrdfConvolutionMapId = pspecularmap->getBrdfConvolutionMapId();
      //// glBindTexture(GL_TEXTURE_2D, iBrdfConvolutionMapId);
      //// GLCheckError("");
      //::cast<::gpu_opengl::ibl::diffuse_irradiance_map> pirradiancemap = pscene->m_pibldiffuseirradiancemap;
      //         pshader->bind_source2(pcommandbuffer,
      //            TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP,
      //            "diffuseIrradianceMap",
      //            pirradiancemap->m_ptextureDiffuseIrradianceCubemap);
      //    ::cast<::gpu_opengl::ibl::specular_map> pspecularmap = pscene->m_piblspecularmap;

      //         pshader->bind_source2(pcommandbuffer,
      //         TEXTURE_UNIT_PREFILTERED_ENV_MAP,
      //         "prefilteredEnvMap",
      //         pspecularmap->m_ptexturePrefilteredEnvMapCubemap);


      //         pshader->bind_source2(pcommandbuffer,
      //         TEXTURE_UNIT_BRDF_CONVOLUTION_MAP,
      //         "brdfConvolutionMap",
      //         pspecularmap->m_ptextureBrdfConvolutionMap);



      //for (auto &[id, pscenerenderable]: scenerenderables)
      //{

      //   if (!pscenerenderable)
      //   {

      //      continue;
      //   }

      //   if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_ibl)
      //   {

      //      continue;
      //   }

      //   auto prenderable = pscenerenderable->renderable();

      //   if (!prenderable)
      //   {

      //      continue;
      //   }

      //   glDisable(GL_CULL_FACE);
      //   auto erenderabletype = prenderable->m_erenderabletype;

      //   if (erenderabletype != ::gpu::e_renderable_type_gltf)
      //   {
      //      
      //      continue; // not mine, skip

      //   }

      //   ::cast<::gpu_opengl::gltf::model> pgltfmodel = prenderable;

      //   if (!pgltfmodel)
      //      continue;

      //   pgltfmodel->bind(pframe->m_pgpucommandbuffer);

      //   for (auto pmesh: pgltfmodel->m_mesha)
      //   {
      //      // if (!node->mesh)
      //      //  continue;

      //      // floating_matrix4 world = psceneobject->transform().getMatrix() * pmesh->uniformBlock.matrix;
      //      // floating_matrix4 normalMat = glm::transpose(glm::inverse(world));

      //      auto matrixTransform = pscenerenderable->model_matrix();

      //      auto matrix = pmesh->uniformBlock.matrix;

      //      float det = floating_matrix3(pmesh->uniformBlock.matrix).determinant();
      //      information("deteterminant of model matrix is %f\n", det);

      //      floating_matrix4 world = matrixTransform * matrix;

      //      // Extract upper-left 3×3 from world
      //      floating_matrix3 world3x3 = floating_matrix3(world);

      //      float det2 = floating_matrix3(world3x3).determinant();
      //      information("deteterminant of model matrix is %f\n", det2);


      //      floating_matrix4 normalMat(world3x3);

      //      // Compute normal matrix correctly
      //      normalMat.inverse().transpose();

      //      pshader->set_matrix4("modelMatrix", world);
      //      pshader->set_matrix4("normalMatrix", normalMat);

      //      float x_multiplier = 1.0f;
      //      float y_multiplier = 1.0f;
      //      float z_multiplier = 1.0f;

      //      if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
      //      {

      //         if (pgpucontext->m_eapi == ::gpu::e_api_opengl)
      //         {

      //            y_multiplier = -1.f;
      //            z_multiplier = -1.f;
      //            //x_multiplier = -1.f;
      //         }
      //      }
      //      else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_z_minus)
      //      {

      //         if (pgpucontext->m_eapi == ::gpu::e_api_opengl)
      //         {

      //            //y_multiplier = -1.f;
      //            z_multiplier = -1.f;
      //         }
      //      }
      //      else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_y_up)
      //      {

      //         if (pgpucontext->m_eapi == ::gpu::e_api_opengl)
      //         {

      //            y_multiplier = -1.f;
      //            z_multiplier = -1.f;

      //         }

      //      }

      //      pshader->set_float("x_multiplier", x_multiplier);
      //      pshader->set_float("y_multiplier", y_multiplier);
      //      pshader->set_float("z_multiplier", z_multiplier);

      //      // memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
      //      // memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
      //      m_erendersystem = ::graphics3d::e_render_system_gltf_ibl;
      //      pcommandbuffer->m_prendersystem = this;
      //      pmesh->draw(pcommandbuffer);

      //   }

      //}

      //pgpucontext->defer_unbind(m_pshaderOpaque);

   }


} // namespace graphics3d_opengl



