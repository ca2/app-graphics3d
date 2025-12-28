#include "framework.h"
#include "pbr_with_ibl_render_system.h"
#include "bred/gpu/block.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/texture.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/scene_renderable.h"
#include "bred/graphics3d/types.h"
#include "gpu/model/model.h"
#include "bred/graphics3d/global_ubo1.h"
#include "app-graphics3d/graphics3d/scene.h"


namespace graphics3d
{



   pbr_with_ibl_render_system::pbr_with_ibl_render_system()
   {


   }


   pbr_with_ibl_render_system::~pbr_with_ibl_render_system() 
   {


   }



      ::graphics3d::scene_renderable *pbr_with_ibl_render_system::current_scene_renderable() 
      {
         
         return m_pscenerenderableCurrent; 
      
      }

      
      ::memory pbr_with_ibl_render_system::pbr_with_ibl_vert_memory()
      {

         return {};

      }
      
      
      ::memory pbr_with_ibl_render_system::pbr_with_ibl_frag_memory()
      {

         return {};

      }



      ::gpu::binding_set * pbr_with_ibl_render_system::pbr_binding_set()
      {

         throw ::interface_only();

         return nullptr;

      }


      ::gpu::property * pbr_with_ibl_render_system::push_constants_properties()
      {

         return nullptr;

      }



   void pbr_with_ibl_render_system::on_prepare(::gpu::context *pgpucontext)
   {

      
         //{

      auto pcontext = m_pengine->gpu_context();

      øconstruct(m_pshaderOpaque);
      øconstruct(m_pshaderMask);
      øconstruct(m_pshaderBlend);

      auto memoryVert = pbr_with_ibl_vert_memory();
      auto memoryFrag = pbr_with_ibl_frag_memory();
      //         static unsigned int pvertshader[] = {
      // #include "shader/scene.vert.spv.inl"
      //         };
      //
      //
      //         static unsigned int pfragshader[] = {
      // #include "shader/scene.frag.spv.inl"
      //         };
      //         static unsigned int pfragshaderAlphaMask05[] = {
      // #include "shader/scene_alpha_mask_0_5.frag.spv.inl"
      //         };
      // auto blockVert = ::as_memory_block(pvertshader);
      // auto blockFrag = ::as_memory_block(pfragshader);
      // auto blockFragAlphaMask05 = ::as_memory_block(pfragshaderAlphaMask05);

      auto ppropertiesUbo = ::gpu_properties<::graphics3d::global_ubo1>();
      auto ppropertiesPush = push_constants_properties();
      auto pinputlayout = pgpucontext->input_layout<::gpu::gltf::vertex>();

      // øconstruct(m_pbindingsetIbl);

      // m_pbindingsetIbl->binding(0)->m_ebinding = ::gpu::e

      // m_pdescriptorsetlayoutIbl =
      //::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
      //   .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //   .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //   .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //   .build();

      // m_pdescriptorsetlayoutPbr =
      //    ::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
      //       .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // albedo
      //       .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // normal
      //       .build();

      auto pshaderOpaque = m_pshaderOpaque;
      pshaderOpaque->m_bEnableBlend = false;
      pshaderOpaque->binding_set(0, pgpucontext->global_ubo1_binding_set());
      pshaderOpaque->binding_set(1, pgpucontext->ibl1_binding_set());
      pshaderOpaque->binding_set(2, pbr_binding_set());
      m_pshaderOpaque->m_propertiesPushShared.set_properties(ppropertiesPush);
      pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushShared, false);
      // m_pshaderOpaque->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      // m_pshaderOpaque->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      // pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushVertex);
      // pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushFragment);
      m_pshaderOpaque->set_global_ubo();
      m_pshaderOpaque->initialize_shader_with_block(pgpucontext->m_pgpurenderer, memoryVert, memoryFrag,// {}, {},
                                                    pinputlayout);

      auto pshaderMask = m_pshaderMask;
      pshaderMask->m_bEnableBlend = true;
      pshaderMask->binding_set(0, pgpucontext->global_ubo1_binding_set());
      pshaderMask->binding_set(1, pgpucontext->ibl1_binding_set());
      pshaderMask->binding_set(2, pbr_binding_set());
      m_pshaderMask->m_propertiesPushShared.set_properties(ppropertiesPush);
      m_pshaderMask->set_global_ubo();
      pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushShared, false);
      // m_pshaderMask->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      // m_pshaderMask->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      // pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushVertex);
      // pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushFragment);
      m_pshaderMask->initialize_shader_with_block(pgpucontext->m_pgpurenderer, memoryVert, memoryFrag, //{}, {},
                                                  pinputlayout);

      auto pshaderBlend = m_pshaderBlend;

      pshaderBlend->binding_set(0, pgpucontext->global_ubo1_binding_set());
      pshaderBlend->binding_set(1, pgpucontext->ibl1_binding_set());
      pshaderBlend->binding_set(2, pbr_binding_set());
      pshaderBlend->m_bEnableBlend = true;
      m_pshaderBlend->m_propertiesPushShared.set_properties(ppropertiesPush);
      m_pshaderBlend->set_global_ubo();
      pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushShared, false);
      // m_pshaderBlend->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      // m_pshaderBlend->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      // pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushVertex);
      // pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushFragment);
      m_pshaderBlend->initialize_shader_with_block(pgpucontext->m_pgpurenderer, memoryVert, memoryFrag,// {}, {},
                                                   pinputlayout);

   }


   void pbr_with_ibl_render_system::on_render(::gpu::context* pgpucontext, ::graphics3d::scene_base* pscenebase)
   {

            
    //   static bool warnedThisFrame = false;
      
             auto pframe = ::gpu::current_frame();
      
          auto pcommandbuffer = pframe->m_pgpucommandbuffer;
             ::cast<::graphics3d::scene> pscene = pscenebase;
          auto &scenerenderables = pscenebase->scene_renderables();
       //   //// xxxxxxxxxxxxxxxxx
       auto pcontext = m_pengine->gpu_context();
          auto prenderer = pcontext->m_pgpurenderer;
      
       ////// xxxxxxxxxxxxxxxxx
       //auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
          //auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);

          auto pblockGlobalUbo1 = pscene->global_ubo1(pgpucontext);
      
       int iRenderable = -1;

       ::gpu::model::material::AlphaMode alphamodeLast = ::gpu::model::material::ALPHAMODE_NONE;
       ::cast<::gpu::shader> pshader;
      
       for (auto &[id, pscenerenderableItem]: scenerenderables)
       {
      
          iRenderable++;
      
          if (!pscenerenderableItem)
          {
      
             continue;
      
          }

         ::cast<::graphics3d::scene_renderable> pscenerenderable = pscenerenderableItem;
          if (!pscenerenderable)
             continue;


          auto erenderableRenderSystem = pscenerenderable->m_erendersystem;

                if (m_erendersystem != erenderableRenderSystem)
          {
             continue; // not mine, skip
          }

      
      
          auto prenderable = pscenerenderable->renderable();
          if (!prenderable)
             continue;
      
          auto egpumodel = prenderable->m_egpumodel;

          if (egpumodel != ::gpu::e_model_gltf)
          {
             continue; // not mine, skip
          }

          


          ::cast<::gpu::model::model> pgltfmodel = prenderable;
      
          if (!pgltfmodel)
             continue;




          ::string strName = pscenerenderable->m_strName;
      
          for (auto & pmesh: pgltfmodel->m_mesha)
          {
             if (!pmesh)
                continue;
      
             auto matrixObject = pscenerenderable->model_matrix();
      
             auto matrixNode = pmesh->uniformBlock.matrix;
      
             floating_matrix4 world =  matrixObject * matrixNode;
      
             // Extract upper-left 3×3 from world
             floating_matrix3 world3x3 = floating_matrix3(world);
      
             float det2 = floating_matrix3(world3x3).determinant();
             information("deteterminant of model matrix is %f\n", det2);
      
      
             floating_matrix4 normalMat(world3x3);
      
             // Compute normal matrix correctly
             normalMat = normalMat.inversed().transposed();
      
      
             //memcpy(pnode->m_pmesh->uniformBuffer.mapped, &world, sizeof(world));
             //memcpy((char *)pnode->m_pmesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
      
             //for (auto *primitive: pmesh->primitives)
             //{
             //auto ptextureTarget = pframe->m_pgpucommandbuffer->m_pgpurendertarget->current_texture(pframe);
             auto ealphamode = pmesh->m_pmaterial->alphaMode;
             if (ealphamode != alphamodeLast)
             {
                // Pick pipeline by alpha mode
                switch (ealphamode)
                {
                   case ::gpu::model::material::ALPHAMODE_OPAQUE:
                      pframe->m_pgpucommandbuffer->set_shader(m_pshaderOpaque);
                      break;
                   case ::gpu::model::material::ALPHAMODE_MASK:
                      pframe->m_pgpucommandbuffer->set_shader(m_pshaderMask);
                      break;
                   case ::gpu::model::material::ALPHAMODE_BLEND:
                   default:
                      pframe->m_pgpucommandbuffer->set_shader(m_pshaderBlend);
                      break;
                }


                pshader = pgpucontext->m_pshaderBound;

                auto pbindingslotsetGlobalUbo1 = pblockGlobalUbo1->binding_slot_set(
                   pframe->m_pgpucommandbuffer, pgpucontext->global_ubo1_binding_set());
                auto pbindingslotsetIbl = pscene->ibl_binding_slot_set();

                pframe->m_pgpucommandbuffer->bind_slot_set(0, pbindingslotsetGlobalUbo1);
                pframe->m_pgpucommandbuffer->bind_slot_set(1, pbindingslotsetIbl);
                // pgltfmodel->bind2(pframe->m_pgpucommandbuffer);
             }
          
                auto prendersystem = this;
      
                ::floating_sequence3 multiplier;
      
                if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
                {
      
                   if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
                   {
      
                      multiplier.y = -1.f;
                      multiplier.z = -1.f;
                      // x_multiplier = -1.f;
                   }
      
                }
                else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_z_minus)
                {
      
                   if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
                   {
      
                      // y_multiplier = -1.f;
                      multiplier.z = -1.f;
      
                   }
      
                }
                else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_y_up)
                {
      
                   if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
                   {
      
                      multiplier.y = -1.f;
                      multiplier.z = -1.f;
      
                   }
      
                }
      
                pshader->set_sequence3("multiplier", multiplier);
      
                pshader->set_matrix4("modelMatrix", world);
                pshader->set_matrix4("normalMatrix", normalMat);
                bool bAlbedo = pmesh->m_pmaterial->m_textureaPbr[::gpu::model::e_texture_albedo].is_set();
                bAlbedo = bAlbedo && !m_bDisableAlbedo;
                pshader->set_int("useTextureAlbedo", bAlbedo ? 1 : 0);

                if (m_erendersystem == ::graphics3d::e_render_system_gltf_scene)
                {
                   if (ealphamode == ::gpu::model::material::ALPHAMODE_MASK)
                   {
                      pshader->set_int("useAlphaMask", 1);
                      pshader->set_float("alphaMaskCutoff", 0.5f);
                   }
                   else
                   {
                      pshader->set_int("useAlphaMask", 0);
                      pshader->set_float("alphaMaskCutoff", 0.f);
                   }
                }

                pshader->set_float("prefilteredEnvMapMaxLod", pscene->m_ptexturePrefilteredCube->m_textureattributes.m_iMipCount - 1);
      
                    floating_sequence3 seq3Albedo = {};
                if (prendersystem->m_bForceDefaultAlbedo)
                {
      
                   seq3Albedo = prendersystem->m_seq3DefaultAlbedo;
                }
                else
                {
      
                   seq3Albedo = pmesh->m_pmaterial->m_seq3Albedo;
      
                }
      
                pshader->set_sequence3("albedo", seq3Albedo);
      
                bool bMetallicRoughness = pmesh->m_pmaterial->m_textureaPbr[::gpu::model::e_texture_metallic_roughness].is_set();
                bMetallicRoughness = bMetallicRoughness && !m_bDisableMetallicRoughness;
                if (m_bImplMetallic)
                {
                   pshader->set_int("useTextureMetallicRoughness", bMetallicRoughness ? 1 : 0);
                }
                bool bNormal = pmesh->m_pmaterial->m_textureaPbr[::gpu::model::e_texture_normal].is_set();
      
                float fMetallic = 0.0f;
                if (prendersystem->m_bForceDefaultMetallicFactor)
                {
      
                   fMetallic = prendersystem->m_fDefaultMetallicFactor;
                }
                else
                {
      
                   fMetallic = pmesh->m_pmaterial->m_fMetallic;
                }
                float fRoughness = 0.0f;
                if (prendersystem->m_bForceDefaultRoughnessFactor)
                {
      
                   fRoughness = prendersystem->m_fDefaultRoughnessFactor;
                }
                else
                {
      
                   fRoughness = pmesh->m_pmaterial->m_fRoughness;
                }
                pshader->set_float("metallic", fMetallic);
                pshader->set_float("roughness", fRoughness);
      
                bNormal = bNormal && !m_bDisableNormal;
                pshader->set_int("useTextureNormal", bNormal ? 1 : 0);
                bool bAmbientOcclusion = pmesh->m_pmaterial->m_textureaPbr[::gpu::model::e_texture_ambient_occlusion].is_set();
                bAmbientOcclusion = bAmbientOcclusion && !m_bDisableAmbientOcclusion;
                if (m_bImplAO)
                {
                   pshader->set_int("useTextureAmbientOcclusion", bAmbientOcclusion ? 1 : 0);
                }
      
      
      
                            float fAmbientOcclusion = 0.0f;
                if (prendersystem->m_bForceDefaultAmbientOcclusionFactor)
                {
      
                   fAmbientOcclusion = prendersystem->m_fDefaultAmbientOcclusionFactor;
                }
                else
                {
      
                   //fAmbientOcclusion = pmesh->m_pmaterial->occlusionTexture->m_fAmbientOcclusion;
                   fAmbientOcclusion = 1.f;
                }
                pshader->set_float("ambientOcclusion", fAmbientOcclusion);
      
                floating_sequence3 seq3Emission = {};
                if (prendersystem->m_bForceDefaultEmission)
                {
      
                   seq3Emission = prendersystem->m_seq3DefaultEmission;
                }
                else
                {
      
                   //seq3Emission = pmesh->m_pmaterial->m_seq3Emissive;
                }
                if (m_bImplEmissive)
                {
                   pshader->set_sequence3("emissive", seq3Emission);
                }

                ::floating_sequence3 seq3(1.f, 1.f, 1.f);

                pshader->set_sequence3("multiplier", seq3);
      
      
                bool bEmissive = pmesh->m_pmaterial->m_textureaPbr[::gpu::model::e_texture_emissive].is_set();
                bEmissive = bEmissive && !m_bDisableEmissive;
                if (m_bImplEmissive)
                {
                   pshader->set_int("useTextureEmissive", bEmissive ? 1 : 0);
                }
      
                //auto metallicFactor = pmesh->m_pmaterial->metallicFactor;
                //if (m_bForceDefaultMetallicFactor)
                //   metallicFactor = m_fDefaultMetallicFactor;
                //pshader->set_float("metallic", metallicFactor);
                //auto roughnessFactor = pmesh->m_pmaterial->roughnessFactor;
                //if (m_bForceDefaultRoughnessFactor)
                //   roughnessFactor = m_fDefaultRoughnessFactor;
                //pshader->set_float("roughness", roughnessFactor);
                //pshader->set_float("ambientOcclusion", pmesh->m_pmaterial->am);
                pshader->push_properties(pcommandbuffer);
      
      
                //// --- Bind sets 0 & 1 (global + node UBO) ---
                //std::array<VkDescriptorSet, 2> sets01 = {
                //   frame.globalDescriptorSet, // set 0
                //   pnode->m_pmesh->uniformBuffer.descriptorSet // set 1
                //};
                //// xxxxxxxxxxxxxxxxx
                ////// --- Bind sets 0 & 1 (global + pnode UBO) ---
                // std::array<VkDescriptorSet, 2> sets01 = {
                //   vkdescriptorsetGlobal, // set 0
                //    pnode->m_pmesh->uniformBuffer.descriptorSet // set 1
                // };
                //vkCmdBindDescriptorSets(
                //   pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                //   m_pipelineLayout, 0,
                //   (uint32_t)sets01.size(), sets01.data(), 0, nullptr);
                 //// --- Bind sets 0 (global) ---
                // pnode UBO transformed in Push Constants and set above
                 // std::array<VkDescriptorSet, 1> sets01 = {
                 //    vkdescriptorsetGlobal // set 0
                 // };
                 // vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                 //                         pshader->m_ppipeline->m_vkpipelinelayout, 0, (uint32_t)sets01.size(),
                //;
                //;
                //;sets01.data(), 0, nullptr);
      
                //// --- Bind our PBR set (set = 2) ---
                 // --- Bind our PBR set (set = 1) ---
                //if (pgltfmodel->m_materiala.size() <= 0)
                //{
      
                //   if (!warnedThisFrame)
                //   { /*spdlog::warn("PBR set null");*/
                //      warnedThisFrame = true;
                //   }
                //   continue;
                //}
      
                //               //// --- Bind IBL set (set = 3) ---
                //// --- Bind IBL set (set = 2) ---
                //VkDescriptorSet iblSet =
                //   m_vkdescriptorsetaIbl[pcontext->m_pgpurenderer->m_pgpurendertarget->get_frame_index()];
                //if (iblSet == VK_NULL_HANDLE)
                //{
                //   continue;
                //}
                //vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                //                        // m_pipelineLayout, 3, 1,
                //                        pshader->m_ppipeline->m_vkpipelinelayout, 1, 1, &iblSet, 0, nullptr);
      
                //if (pgltfmodel->m_materiala.has_element() &&
                //   pgltfmodel->m_materiala[0]
                //       .descriptor_set_array_gltf(pgltfmodel)
                //       .has_element())
                //{
                //   VkDescriptorSet pbrSet =
                //
                //pmesh->m_pmaterial->descriptor_set_array_gltf(pgltfmodel)[pframe->m_pgpucommandbuffer->m_iFrameIndex];
                //   if (pbrSet == VK_NULL_HANDLE)
                //   {
                //      if (!warnedThisFrame)
                //      { /*spdlog::warn("PBR set null");*/
                //         warnedThisFrame = true;
                //      }
                //      continue;
                //   }
                //   vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                //                           // m_pipelineLayout, 2, 1,
                //                           pshader->m_ppipeline->m_vkpipelinelayout, 2, 1, &pbrSet, 0, nullptr);
                //}
      

                                auto pgpubindingsetPbr = pbr_binding_set();

                // auto pbindingset = pshader->binding_set(2, pgpubindingset);
                auto pbindingslotsetPbr = pmesh->m_pmaterial->pbr_binding_slot_set(pgpubindingsetPbr, pgltfmodel);
                // if (pgpubindingset->size() == 2)
                // pgltfmodel

                pcommandbuffer->bind_slot_set(2, pbindingslotsetPbr);

                pcommandbuffer->draw(pmesh);
      
                //warnedThisFrame = false;
      
             //}
          }
       }

   }


} // namespace graphics3d



