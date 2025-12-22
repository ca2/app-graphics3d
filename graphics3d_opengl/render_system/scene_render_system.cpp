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
#include "shader/scene.vert.h"
#include "shader/scene.frag.h"
#include "gpu_opengl/ibl/diffuse_irradiance_map.h"
#include "gpu_opengl/ibl/specular_map.h"
#include "app-graphics3d/graphics3d/scene.h"
#include "gpu_opengl/_gpu_opengl.h"
#include "bred/gltf/vertex.h"
//#include "gpu/ibl/cubemap_framebuffer.h"
#include "shader/scene.frag.h"
#include "shader/scene.vert.h"


namespace graphics3d_opengl
{


   scene_render_system::scene_render_system()
   {


   }


   scene_render_system::~scene_render_system() 
   {

   
   }

   ::memory scene_render_system::pbr_with_ibl_vert_memory()
   {

      //      static unsigned int pvertexshader[] = {
      // #include "render_system/shader/gltf.vert.spv.inl"
      //
      //      };

      return ::as_memory_block(g_psz_scene_vert);
   }


   ::memory scene_render_system::pbr_with_ibl_frag_memory()
   { 
      return ::as_memory_block(g_psz_scene_frag); 
   }

   void scene_render_system::on_prepare(::gpu::context *pgpucontext)
   {

      ::graphics3d::scene_render_system::on_prepare(pgpucontext);

      //::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();

      //øconstruct(m_pshaderOpaque);
      //øconstruct(m_pshaderMask);
      //øconstruct(m_pshaderBlend);

      //m_pshaderOpaque->initialize_shader_with_block(pgpucontext->m_pgpurenderer, 
      //   ::as_memory_block(g_psz_scene_vert),
      //   ::as_memory_block(g_psz_scene_frag), 
      //   //{},
      //   //{},
      //   pgpucontext->input_layout <::gpu::gltf::vertex>());

      //m_pshaderMask->m_bEnableBlend = true;
      //m_pshaderMask->initialize_shader_with_block(pgpucontext->m_pgpurenderer, ::as_memory_block(g_psz_scene_vert),
      //                                            ::as_memory_block(g_psz_scene_frag),
      //   //{},
      //     //                                       {},
      //                                            pgpucontext->input_layout<::gpu::gltf::vertex>());

      //m_pshaderBlend->m_bDisableDepthTest = true;
      //m_pshaderBlend->m_bEnableBlend = true;
      //m_pshaderBlend->initialize_shader_with_block(pgpucontext->m_pgpurenderer, ::as_memory_block(g_psz_scene_vert),
      //                                             ::as_memory_block(g_psz_scene_frag), 
      //   //{},
      //     //                                        {
      //   //
      //     //                                        },
      //                                             pgpucontext->input_layout<::gpu::gltf::vertex>());

   }


   void scene_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscenebase)
   {

      ::graphics3d::scene_render_system::on_render(pgpucontext, pscenebase);
      //
//      static bool warnedThisFrame = false;
//
//      auto pframe = ::gpu::current_frame();
//
//      ::cast<::gpu_opengl::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;
//
//      //pgpucontext->defer_bind(m_pshaderOpaque);
//
//      auto &scenerenderables = pscenebase->scene_renderables();
//
//      ::cast < ::gpu_opengl::context > pcontext = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;
//
//      ::gpu::shader * pshader = nullptr;
//
//      ::cast<::gpu_opengl::renderer> prenderer = pcontext->m_pgpurenderer;
//
//      for (auto &[id, pscenerenderable]: scenerenderables)
//      {
//
//         if (!pscenerenderable)
//         {
//
//            debug("Hey, there is a null object named '{}' in scene renderables map.", id);
//
//            continue;
//
//         }
//
//         if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_scene)
//         {
//
//            continue;
//
//         }
//
//         auto prenderable = pscenerenderable->renderable();
//         
//         if (!prenderable)
//         {
//
//            continue;
//
//         }
//
//         auto erenderabletype = prenderable->m_erenderabletype;
//
//         if (erenderabletype != ::gpu::e_renderable_type_gltf)
//         {
//            
//            continue;
//
//         }
//
//         ::cast<::gpu_opengl::gltf::model> pgltfmodel = prenderable;
//
//         if (!pgltfmodel)
//         {
//
//            continue;
//
//         }
//
//         pgltfmodel->bind(pframe->m_pgpucommandbuffer);
//
//         for (auto pmesh: pgltfmodel->m_mesha)
//         {
//
//            floating_matrix4 matrixObject = pscenerenderable->model_matrix();
//
//            auto matrixNode = pmesh->uniformBlock.matrix;
//
//            floating_matrix4 world = matrixObject * matrixNode;
//
//            floating_matrix3 matrix3World(world);
//            auto matrix3Normal = matrix3World.inversed().transposed();
//            floating_matrix4 normalMat = matrix3Normal;
//            bool bChangedShader = false;
//                           // Pick pipeline by alpha mode
//            switch (pmesh->m_pmaterial->alphaMode)
//            {
//               case ::gpu::model::material::ALPHAMODE_OPAQUE:
//                  bChangedShader = pgpucontext->defer_bind(m_pshaderOpaque);
//                  m_pshaderOpaque->set_int("useAlphaMask", 0);
//                  break;
//               case ::gpu::model::material::ALPHAMODE_MASK:
//                  bChangedShader = pgpucontext->defer_bind(m_pshaderMask);
//                  m_pshaderBlend->set_int("useAlphaMask", 1);
//                  break;
//               case ::gpu::model::material::ALPHAMODE_BLEND:
//               default:
//                  bChangedShader = pgpucontext->defer_bind(m_pshaderBlend);
//                  m_pshaderBlend->set_int("useAlphaMask", 0);
//                  break;
//            }
//
//            auto pshader = pgpucontext->m_pshaderBound;
//
//            if (bChangedShader)
//            {
//               ::cast<::graphics3d::scene> pscene = pscenebase;
//                     //// xxxxxxxxxxxxxxxxx
//               // auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
//               // auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);
//
//               //pshader->bind_source2(TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP, "diffuseIrradianceMap",
////                  pscene->m_pibldiffuseirradiancemap->m_pframebufferDiffuseIrradiance->m_ptexture);
//               ::cast<::gpu_opengl::ibl::diffuse_irradiance_map> pirradiancemap = pscene->m_pibldiffuseirradiancemap;
////               int iCubemapId = pirradiancemap->getCubemapId();
//
//               pshader->bind_source2(pcommandbuffer,
//                  TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP,
//                  "diffuseIrradianceMap",
//                  pirradiancemap->m_ptextureDiffuseIrradianceCubemap);
//               // //  IBL stuff
//               // glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
//               // GLCheckError("");
//               // pshader->set_int("diffuseIrradianceMap", TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
//               // ::cast<::gpu_opengl::ibl::diffuse_irradiance_map> pirradiancemap = pscene->m_pibldiffuseirradiancemap;
//               // int iCubemapId = pirradiancemap->getCubemapId();
//               // glBindTexture(GL_TEXTURE_CUBE_MAP, iCubemapId);
//               // GLCheckError("");
//
//               // glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_PREFILTERED_ENV_MAP);
//               // GLCheckError("");
//               // pshader->set_int("prefilteredEnvMap", TEXTURE_UNIT_PREFILTERED_ENV_MAP);
//               // ::cast<::gpu_opengl::ibl::specular_map> pspecularmap = pscene->m_piblspecularmap;
//               // ::cast<::gpu_opengl::texture> pspecularmap = pscene->m_piblspecularmap;
//               // int iPrefilteredEnvMapId = pspecularmap->m_pframebufferPrefilteredEnvMap->m_ptexture();
//               // glBindTexture(GL_TEXTURE_CUBE_MAP, iPrefilteredEnvMapId);
//               // GLCheckError("");
//
//               //::cast<::gpu_opengl::texture> pspecularmap = pscene->m_piblspecularmap;
//               ::cast<::gpu_opengl::ibl::specular_map> pspecularmap = pscene->m_piblspecularmap;
//               //int iPrefilteredEnvMapId = pspecularmap->m_pframebufferPrefilteredEnvMap->m_ptexture();
//
//               pshader->bind_source2(pcommandbuffer,
//               TEXTURE_UNIT_PREFILTERED_ENV_MAP,
//               "prefilteredEnvMap",
//               pspecularmap->m_ptexturePrefilteredEnvMapCubemap);
//
//
//               // glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
//               // GLCheckError("");
//               // pshader->set_int("brdfConvolutionMap", TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
//               // int iBrdfConvolutionMapId = pspecularmap->getBrdfConvolutionMapId();
//               // glBindTexture(GL_TEXTURE_2D, iBrdfConvolutionMapId);
//               // GLCheckError("");
//
//               pshader->bind_source2(pcommandbuffer,
//               TEXTURE_UNIT_BRDF_CONVOLUTION_MAP,
//               "brdfConvolutionMap",
//               pspecularmap->m_ptextureBrdfConvolutionMap);
//
//            }
//
//            pshader->set_matrix4("modelMatrix", world);
//            pshader->set_matrix4("normalMatrix", normalMat);
//
//            m_erendersystem = ::graphics3d::e_render_system_gltf_scene;
//            pcommandbuffer->m_prendersystem = this;
//            pmesh->draw(pcommandbuffer);
//
//         }
//
//      }
//
//      if (pshader)
//      {
//         pgpucontext->defer_unbind(pshader);
//      }
//
   }


} // namespace graphics3d_opengl



