// From V0idsEmbrace.gltf_render_system by
// camilo on 2025-04-24 21:28 <3ThomasBorregaardSorensen!!
#include "framework.h"
// #include "bred/gpu/gltf_model.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "app-graphics3d/gpu_vulkan/command_buffer.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"
//#include "app-graphics3d/gpu_vulkan/gltf_model.h"
#include "app-graphics3d/gpu_vulkan/gltf/model.h"
#include "app-graphics3d/gpu_vulkan/pipeline.h"
#include "app-graphics3d/gpu_vulkan/render_pass.h"
#include "app-graphics3d/gpu_vulkan/render_target.h"
#include "app-graphics3d/gpu_vulkan/renderer.h"
#include "app-graphics3d/gpu_vulkan/texture.h"
#include "app-graphics3d/gpu_vulkan/vk_init.h"
#include "app-graphics3d/graphics3d/scene.h"
#include "bred/gpu/frame.h"
#include "bred/graphics3d/asset_manager.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/global_ubo1.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/scene_renderable.h"
#include "gltf_render_system.h"
//#include "gpu_vulkan/gltf_model.h"
#include "gpu_vulkan/shader.h"
#include "bred/gltf/vertex.h"


namespace graphics3d_vulkan
{


   gltf_render_system::gltf_render_system()
   {
   
      int iSize = sizeof(push_constants);

      ::information() << "size:" << iSize;

   
   }


   gltf_render_system::~gltf_render_system()
   {

   }


   ::memory gltf_render_system::gltf_vert_memory()
   {

      static unsigned int pvertexshader[] = {
#include "render_system/shader/gltf.vert.spv.inl"

      };

      return ::as_memory_block(pvertexshader);
   }


   ::memory gltf_render_system::gltf_frag_memory()
   {

      static unsigned int pfragmentshader[] = {
#include "render_system/shader/gltf.frag.spv.inl"

      };

      return ::as_memory_block(pfragmentshader);
   }


   void gltf_render_system::on_prepare(::gpu::context *pgpucontext)
   {
      
      ::graphics3d::pbr_with_ibl_render_system::on_prepare(pgpucontext);


      //::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();

      //øconstruct(m_pshaderOpaque);
      //øconstruct(m_pshaderMask);
      //øconstruct(m_pshaderBlend);


      //auto ppropertiesUbo = ::gpu_properties<::graphics3d::global_ubo1>();
      //auto ppropertiesPush = ::gpu_properties<push_constants>();
      //auto pinputlayout = pgpucontext->input_layout < ::gpu::gltf::vertex >();

      //m_pdescriptorsetlayoutIbl =
      //   ::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
      //      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //      .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //      .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      //      .build();

      //m_pdescriptorsetlayoutPbr =
      //   ::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
      //      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // albedo
      //      .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // normal
      //      .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // metallicRoughness
      //      .addBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // ao
      //      .addBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // emissive
      //      .build();

      //::cast<::gpu_vulkan::shader> pshaderOpaque = m_pshaderOpaque;

      //pshaderOpaque->m_mapDescriptorSetLayout[1] = m_pdescriptorsetlayoutIbl;
      //pshaderOpaque->m_mapDescriptorSetLayout[2] = m_pdescriptorsetlayoutPbr;
      //m_pshaderOpaque->m_propertiesPushShared.set_properties(ppropertiesPush);
      //pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushShared, false);
      ////m_pshaderOpaque->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      ////m_pshaderOpaque->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      ////pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushVertex);
      ////pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushFragment);
      //m_pshaderOpaque->set_global_ubo();
      //m_pshaderOpaque->initialize_shader_with_block(
      //   pgpucontext->m_pgpurenderer, 
      //   embedded_pbr_vert(),
      //   embedded_pbr_frag(), 
      //   {}, 
      //   {}, 
      //   pinputlayout);

      //::cast<::gpu_vulkan::shader> pshaderMask = m_pshaderMask;

      //pshaderMask->m_mapDescriptorSetLayout[1] = m_pdescriptorsetlayoutIbl;
      //pshaderMask->m_mapDescriptorSetLayout[2] = m_pdescriptorsetlayoutPbr;
      //m_pshaderMask->m_propertiesPushShared.set_properties(ppropertiesPush);
      //m_pshaderMask->set_global_ubo();
      //pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushShared, false);
      ////m_pshaderMask->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      ////m_pshaderMask->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      ////pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushVertex);
      ////pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushFragment);
      //m_pshaderMask->initialize_shader_with_block(
      //   pgpucontext->m_pgpurenderer, 
      //   embedded_pbr_vert(), 
      //   embedded_pbr_frag(),
      //   {}, 
      //   {}, 
      //   pinputlayout);

      //::cast<::gpu_vulkan::shader> pshaderBlend = m_pshaderBlend;

      //pshaderBlend->m_mapDescriptorSetLayout[1] = m_pdescriptorsetlayoutIbl;
      //pshaderBlend->m_mapDescriptorSetLayout[2] = m_pdescriptorsetlayoutPbr;
      //m_pshaderBlend->m_propertiesPushShared.set_properties(ppropertiesPush);
      //m_pshaderBlend->set_global_ubo();
      //pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushShared, false);
      ////m_pshaderBlend->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      ////m_pshaderBlend->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      ////pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushVertex);
      ////pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushFragment);
      //m_pshaderBlend->initialize_shader_with_block(
      //   pgpucontext->m_pgpurenderer, 
      //   embedded_pbr_vert(),
      //   embedded_pbr_frag(),
      //   {}, 
      //   {},
      //   pinputlayout);



      ////createPipelineLayout(pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout());

      ////auto prenderpass = render_pass2();

      ////createPipeline(prenderpass->m_vkrenderpass);

      ////::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;

      ////::cast<::gpu_vulkan::render_target> prendertarget = prenderer->m_pgpurendertarget;

      ////auto frameCount = prendertarget->get_frame_count();

      ////m_vkdescriptorsetaIbl.resize(frameCount);

      ////auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

      ////pdescriptorpoolbuilder->initialize_builder(pcontext);
      ////pdescriptorpoolbuilder->setMaxSets(frameCount * 10);
      ////pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, frameCount * 100);

      ////m_pdescriptorpool = pdescriptorpoolbuilder->build();


      ////auto passetmanager = m_pengine->m_pimmersionlayer->m_passetmanager;

      ////::cast<::graphics3d::scene> pscene = m_pengine->m_pimmersionlayer->m_pscene;

      ////for (uint32_t i = 0; i < frameCount; i++)
      ////{

      ////   VkDescriptorSet set;

      ////   // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set,
      ////   //                                 /*setIndex=*/0);
      ////   m_pdescriptorpool->allocateDescriptor(
      ////      m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set, 0);

      ////   ::cast<::gpu_vulkan::texture> ptextureIrrad = pscene->m_ptextureIrradianceCube;
      ////   ::cast<::gpu_vulkan::texture> ptexturePrefltr = pscene->m_ptexturePrefilteredCube;
      ////   ::cast<::gpu_vulkan::texture> ptextureBrdf = pscene->m_ptextureLuBrdf;
      ////   // auto irradianceInfo = m_assets.getIrradianceDescriptor();
      ////   // auto prefilterInfo = m_assets.getPrefilteredDescriptor();

      ////   auto irradianceInfo = ptextureIrrad->m_descriptor3;
      ////   auto prefilterInfo = ptexturePrefltr->m_descriptor3;
      ////   auto brdfInfo = ptextureBrdf->m_descriptor3;


      ////   ::gpu_vulkan::descriptor_writer(*m_pdescriptorsetlayoutIbl, *m_pdescriptorpool)
      ////      .writeImage(0, &irradianceInfo)
      ////      .writeImage(1, &prefilterInfo)
      ////      .writeImage(2, &brdfInfo)
      ////      .build(set);

      ////   m_vkdescriptorsetaIbl[i] = set;
      ////}

      ////for (uint32_t uFrameIndex = 0; uFrameIndex < frameCount; uFrameIndex++)
      ////{
      ////   VkDescriptorSet set;
      ////   // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutPbr->getDescriptorSetLayout(), set, /*setIndex=*/0);
      ////   // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutPbr->getDescriptorSetLayout(), set);

      ////   // auto logDescriptor = [&](const char *name, const VkDescriptorImageInfo &info)
      ////   //{
      ////   //    information("{} - sampler: {}, imageView: {}, layout: {}", name, (uint64_t)info.sampler,
      ////   //                 (uint64_t)info.imageView, (int)info.imageLayout);
      ////   // };

      ////   if (1)
      ////   {

      ////      auto &scenerenderables = pscene->scene_renderables();

      ////      //   //// xxxxxxxxxxxxxxxxx
      ////      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();
      ////      ::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;

      ////      ////// xxxxxxxxxxxxxxxxx
      ////      // auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      ////      auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer, uFrameIndex);


      ////      for (auto &[id, pscenerenderable]: scenerenderables)
      ////      {

      ////         if (!pscenerenderable)
      ////         {

      ////            continue;
      ////         }

      ////         if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_ibl)
      ////         {

      ////            continue;
      ////         }


      ////         auto prenderable = pscenerenderable->renderable();
      ////         if (!prenderable)
      ////            continue;

      ////         auto erenderabletype = prenderable->m_erenderabletype;

      ////         if (erenderabletype != ::gpu::e_renderable_type_gltf)
      ////         {
      ////            continue; // not mine, skip
      ////         }
      ////         ::cast<::gpu_vulkan::gltf::Model> pgltfmodel = prenderable;

      ////         if (!pgltfmodel)
      ////            continue;

      ////         //for (auto &material: pgltfmodel->m_materiala)
      ////         //{
      ////         //   if (material.baseColorTexture != nullptr)
      ////         //   {
      ////         //      material.addDescriptorSet(
      ////         //         m_pdescriptorpool->m_vkdescriptorpool, 
      ////         //         m_pdescriptorsetlayoutPbr->m_vkdescriptorsetlayout, 
      ////         //         ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageBaseColor |
      ////         //         ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageMetallicMap |
      ////         //         ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageNormalMap |
      ////         //         ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageAOMap |
      ////         //         ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageEmissiveMap,
      ////         //         pgltfmodel->emptyTexture);
      ////         //   }
      ////         //}


      ////         //pgltfmodel->m_vkdescriptorsetaPbr.resize(frameCount);


      ////         // pgltfmodel->bind(pgpucommandbuffer);

      ////         // for (auto *pnode: pgltfmodel->m_pgltfmodel->m_nodeaLinear)
      ////         //{
      ////         //    if (!pnode->m_pmesh)
      ////         //       continue;


      ////         //::cast<::gpu_vulkan::texture> ptextureAlbedo = pgltfmodel->y;
      ////         //::cast<::gpu_vulkan::texture> ptextureNormal = passetmanager->getTexture("cerberus_normal");
      ////         //::cast<::gpu_vulkan::texture> ptextureMetallic = passetmanager->getTexture("cerberus_metallic");
      ////         //::cast<::gpu_vulkan::texture> ptextureRoughness = passetmanager->getTexture("cerberus_roughness");
      ////         //::cast<::gpu_vulkan::texture> ptextureAo = passetmanager->getTexture("cerberus_ao");*/


      ////         // VkDescriptorImageInfo albedoInfo = m_assets.getTextureDescriptor("cerberus_albedo");
      ////         // VkDescriptorImageInfo normalInfo = m_assets.getTextureDescriptor("cerberus_normal");
      ////         // VkDescriptorImageInfo metallicInfo = m_assets.getTextureDescriptor("cerberus_metallic");
      ////         // VkDescriptorImageInfo roughnessInfo = m_assets.getTextureDescriptor("cerberus_roughness");
      ////         // VkDescriptorImageInfo aoInfo = m_assets.getTextureDescriptor("cerberus_ao");

      ////         //VkDescriptorImageInfo albedoInfo = ptextureAlbedo->m_descriptor3;
      ////         //VkDescriptorImageInfo normalInfo = ptextureNormal->m_descriptor3;
      ////         //VkDescriptorImageInfo metallicInfo = ptextureMetallic->m_descriptor3;
      ////         //VkDescriptorImageInfo roughnessInfo = ptextureRoughness->m_descriptor3;
      ////         //VkDescriptorImageInfo aoInfo = ptextureAo->m_descriptor3;

      ////         // logDescriptor("albedo", albedoInfo);
      ////         // logDescriptor("normal", normalInfo);
      ////         // logDescriptor("metallic", metallicInfo);
      ////         // logDescriptor("roughness", roughnessInfo);
      ////         // logDescriptor("ao", aoInfo);

      ////         //::gpu_vulkan::descriptor_writer(*m_pdescriptorsetlayoutPbr, *m_pdescriptorpool)
      ////         //   .writeImage(0, &albedoInfo)
      ////         //   .writeImage(1, &normalInfo)
      ////         //   .writeImage(2, &metallicInfo)
      ////         //   .writeImage(3, &roughnessInfo)
      ////         //   .writeImage(4, &aoInfo)
      ////         //   .build(set);

      ////         //pgltfmodel->m_vkdescriptorsetaPbr[uFrameIndex] = set;
      ////      }
      ////   }
      ////}
   }



void gltf_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene)
   {

      ::graphics3d::gltf_render_system::on_render(pgpucontext, pscene);
      //
      // static bool warnedThisFrame = false;
      //
      //       auto pframe = ::gpu::current_frame();
      //
      //    ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;
      //
      //    auto &scenerenderables = pscene->scene_renderables();
      // //   //// xxxxxxxxxxxxxxxxx
      // ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();
      //    ::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;
      //
      // ////// xxxxxxxxxxxxxxxxx
      // //auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      //    //auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);
      //
      // int iRenderable = -1;
      //
      // for (auto &[id, pscenerenderable]: scenerenderables)
      // {
      //
      //    iRenderable++;
      //
      //    if (!pscenerenderable)
      //    {
      //
      //       continue;
      //
      //    }
      //
      //    if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_ibl)
      //    {
      //
      //       continue;
      //    }
      //
      //
      //    auto prenderable = pscenerenderable->renderable();
      //    if (!prenderable)
      //       continue;
      //
      //    auto erenderabletype = prenderable->m_erenderabletype;
      //
      //    if (erenderabletype != ::gpu::e_renderable_type_gltf)
      //    {
      //       continue; // not mine, skip
      //    }
      //    ::cast<::gpu_vulkan::gltf::model> pgltfmodel = prenderable;
      //
      //    if (!pgltfmodel)
      //       continue;
      //
      //    ::string strName = pscenerenderable->m_strName;
      //
      //    pgltfmodel->bind(pframe->m_pgpucommandbuffer);
      //
      //    for (auto *pnode: pgltfmodel->m_nodeaLinear)
      //    {
      //       if (!pnode->m_pmesh)
      //          continue;
      //
      //       auto matrixObject = pscenerenderable->model_matrix();
      //
      //       auto matrixNode = pnode->getMatrix();
      //
      //       floating_matrix4 world =  matrixObject * matrixNode;
      //
      //       // Extract upper-left 3×3 from world
      //       floating_matrix3 world3x3 = floating_matrix3(world);
      //
      //       float det2 = floating_matrix3(world3x3).determinant();
      //       information("deteterminant of model matrix is %f\n", det2);
      //
      //
      //       floating_matrix4 normalMat(world3x3);
      //
      //       // Compute normal matrix correctly
      //       normalMat = normalMat.inversed().transposed();
      //
      //
      //       //memcpy(pnode->m_pmesh->uniformBuffer.mapped, &world, sizeof(world));
      //       //memcpy((char *)pnode->m_pmesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
      //
      //       for (auto *primitive: pnode->m_pmesh->primitives)
      //       {
      //
      //                         // Pick pipeline by alpha mode
      //          switch (primitive->m_pmaterial->alphaMode)
      //          {
      //             case ::gpu_vulkan::gltf::Material::ALPHAMODE_OPAQUE:
      //                pgpucontext->defer_bind(m_pshaderOpaque);
      //                break;
      //             case ::gpu_vulkan::gltf::Material::ALPHAMODE_MASK:
      //                pgpucontext->defer_bind(m_pshaderMask);
      //                break;
      //             case ::gpu_vulkan::gltf::Material::ALPHAMODE_BLEND:
      //             default:
      //                pgpucontext->defer_bind(m_pshaderBlend);
      //                break;
      //          }
      //
      //          ::cast<::gpu_vulkan::shader> pshader = pgpucontext->m_pshaderBound;
      //
      //          auto prendersystem = this;
      //
      //          ::floating_sequence3 multiplier;
      //
      //          if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
      //          {
      //
      //             if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
      //             {
      //
      //                multiplier.y = -1.f;
      //                multiplier.z = -1.f;
      //                // x_multiplier = -1.f;
      //             }
      //
      //          }
      //          else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_z_minus)
      //          {
      //
      //             if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
      //             {
      //
      //                // y_multiplier = -1.f;
      //                multiplier.z = -1.f;
      //
      //             }
      //
      //          }
      //          else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_y_up)
      //          {
      //
      //             if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
      //             {
      //
      //                multiplier.y = -1.f;
      //                multiplier.z = -1.f;
      //
      //             }
      //
      //          }
      //
      //          pshader->set_sequence3("multiplier", multiplier);
      //
      //          pshader->set_matrix4("modelMatrix", world);
      //          pshader->set_matrix4("normalMatrix", normalMat);
      //          bool bAlbedo = pgltfmodel->m_materiala[0].baseColorTexture.is_set();
      //          bAlbedo = bAlbedo && !m_bDisableAlbedo;
      //          pshader->set_int("useTextureAlbedo", bAlbedo ? 1 : 0);
      //
      //              floating_sequence3 seq3Albedo = {};
      //          if (prendersystem->m_bForceDefaultAlbedo)
      //          {
      //
      //             seq3Albedo = prendersystem->m_seq3DefaultAlbedo;
      //          }
      //          else
      //          {
      //
      //             seq3Albedo = ::floating_sequence3(pgltfmodel->m_materiala[0].baseColorFactor.r,
      //                                       pgltfmodel->m_materiala[0].baseColorFactor.g,
      //                                       pgltfmodel->m_materiala[0].baseColorFactor.b);
      //
      //          }
      //
      //          pshader->set_sequence3("albedo", seq3Albedo);
      //
      //          bool bMetallicRoughness = pgltfmodel->m_materiala[0].metallicRoughnessTexture.is_set();
      //          bMetallicRoughness = bMetallicRoughness && !m_bDisableMetallicRoughness;
      //          pshader->set_int("useTextureMetallicRoughness", bMetallicRoughness ? 1 : 0);
      //          bool bNormal = pgltfmodel->m_materiala[0].normalTexture.is_set();
      //
      //
      //                      float fMetallic = 0.0f;
      //          if (prendersystem->m_bForceDefaultMetallicFactor)
      //          {
      //
      //             fMetallic = prendersystem->m_fDefaultMetallicFactor;
      //          }
      //          else
      //          {
      //
      //             fMetallic = pgltfmodel->m_materiala[0].metallicFactor;
      //          }
      //          float fRoughness = 0.0f;
      //          if (prendersystem->m_bForceDefaultRoughnessFactor)
      //          {
      //
      //             fRoughness = prendersystem->m_fDefaultRoughnessFactor;
      //          }
      //          else
      //          {
      //
      //             fRoughness = pgltfmodel->m_materiala[0].roughnessFactor;
      //          }
      //          pshader->set_float("metallic", fMetallic);
      //          pshader->set_float("roughness", fRoughness);
      //
      //          bNormal = bNormal && !m_bDisableNormal;
      //          pshader->set_int("useTextureNormal", bNormal ? 1 : 0);
      //          bool bAmbientOcclusion = pgltfmodel->m_materiala[0].occlusionTexture.is_set();
      //          bAmbientOcclusion = bAmbientOcclusion && !m_bDisableAmbientOcclusion;
      //          pshader->set_int("useTextureAmbientOcclusion", bAmbientOcclusion ? 1 : 0);
      //
      //
      //
      //                      float fAmbientOcclusion = 0.0f;
      //          if (prendersystem->m_bForceDefaultAmbientOcclusionFactor)
      //          {
      //
      //             fAmbientOcclusion = prendersystem->m_fDefaultAmbientOcclusionFactor;
      //          }
      //          else
      //          {
      //
      //             //fAmbientOcclusion = pgltfmodel->m_materiala[0].occlusionTexture->m_fAmbientOcclusion;
      //             fAmbientOcclusion = 1.f;
      //          }
      //          pshader->set_float("ambientOcclusion", fAmbientOcclusion);
      //
      //          floating_sequence3 seq3Emission = {};
      //          if (prendersystem->m_bForceDefaultEmission)
      //          {
      //
      //             seq3Emission = prendersystem->m_seq3DefaultEmission;
      //          }
      //          else
      //          {
      //
      //             //seq3Emission = pgltfmodel->m_materiala[0].m_seq3Emissive;
      //          }
      //          pshader->set_sequence3("emissive", seq3Emission);
      //
      //
      //          bool bEmissive = pgltfmodel->m_materiala[0].emissiveTexture.is_set();
      //          bEmissive = bEmissive && !m_bDisableEmissive;
      //          pshader->set_int("useTextureEmissive", bEmissive ? 1 : 0);
      //
      //          //auto metallicFactor = pgltfmodel->m_materiala[0].metallicFactor;
      //          //if (m_bForceDefaultMetallicFactor)
      //          //   metallicFactor = m_fDefaultMetallicFactor;
      //          //pshader->set_float("metallic", metallicFactor);
      //          //auto roughnessFactor = pgltfmodel->m_materiala[0].roughnessFactor;
      //          //if (m_bForceDefaultRoughnessFactor)
      //          //   roughnessFactor = m_fDefaultRoughnessFactor;
      //          //pshader->set_float("roughness", roughnessFactor);
      //          //pshader->set_float("ambientOcclusion", pgltfmodel->m_materiala[0].am);
      //          pshader->push_properties(pcommandbuffer);
      //
      //
      //          //// --- Bind sets 0 & 1 (global + node UBO) ---
      //          //std::array<VkDescriptorSet, 2> sets01 = {
      //          //   frame.globalDescriptorSet, // set 0
      //          //   pnode->m_pmesh->uniformBuffer.descriptorSet // set 1
      //          //};
      //          //// xxxxxxxxxxxxxxxxx
      //          ////// --- Bind sets 0 & 1 (global + pnode UBO) ---
      //          // std::array<VkDescriptorSet, 2> sets01 = {
      //          //   vkdescriptorsetGlobal, // set 0
      //          //    pnode->m_pmesh->uniformBuffer.descriptorSet // set 1
      //          // };
      //          //vkCmdBindDescriptorSets(
      //          //   pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //          //   m_pipelineLayout, 0,
      //          //   (uint32_t)sets01.size(), sets01.data(), 0, nullptr);
      //           //// --- Bind sets 0 (global) ---
      //          // pnode UBO transformed in Push Constants and set above
      //           // std::array<VkDescriptorSet, 1> sets01 = {
      //           //    vkdescriptorsetGlobal // set 0
      //           // };
      //           // vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //           //                         pshader->m_ppipeline->m_vkpipelinelayout, 0, (uint32_t)sets01.size(), sets01.data(), 0, nullptr);
      //
      //          //// --- Bind our PBR set (set = 2) ---
      //           // --- Bind our PBR set (set = 1) ---
      //          if (pgltfmodel->m_materiala.size() <= 0)
      //          {
      //
      //             if (!warnedThisFrame)
      //             { /*spdlog::warn("PBR set null");*/
      //                warnedThisFrame = true;
      //             }
      //             continue;
      //          }
      //
      //          //               //// --- Bind IBL set (set = 3) ---
      //          //// --- Bind IBL set (set = 2) ---
      //          //VkDescriptorSet iblSet =
      //          //   m_vkdescriptorsetaIbl[pcontext->m_pgpurenderer->m_pgpurendertarget->get_frame_index()];
      //          //if (iblSet == VK_NULL_HANDLE)
      //          //{
      //          //   continue;
      //          //}
      //          //vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //          //                        // m_pipelineLayout, 3, 1,
      //          //                        pshader->m_ppipeline->m_vkpipelinelayout, 1, 1, &iblSet, 0, nullptr);
      //
      //          //if (pgltfmodel->m_materiala.has_element() &&
      //          //   pgltfmodel->m_materiala[0]
      //          //       .descriptor_set_array_gltf(pgltfmodel)
      //          //       .has_element())
      //          //{
      //          //   VkDescriptorSet pbrSet =
      //          //      pgltfmodel->m_materiala[0].descriptor_set_array_gltf(pgltfmodel)[pframe->m_pgpucommandbuffer->m_iFrameIndex];
      //          //   if (pbrSet == VK_NULL_HANDLE)
      //          //   {
      //          //      if (!warnedThisFrame)
      //          //      { /*spdlog::warn("PBR set null");*/
      //          //         warnedThisFrame = true;
      //          //      }
      //          //      continue;
      //          //   }
      //          //   vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //          //                           // m_pipelineLayout, 2, 1,
      //          //                           pshader->m_ppipeline->m_vkpipelinelayout, 2, 1, &pbrSet, 0, nullptr);
      //          //}
      //          auto pgpubindingset = pcontext->gltf_pbr_binding_set();
      //
      //          auto pbindingset = pshader->binding_set(2, pgpubindingset);
      //
      //          pgltfmodel->gltfDraw(pcommandbuffer, 0);
      //
      //          warnedThisFrame = false;
      //
      //       }
      //    }
      // }
   }

} // namespace graphics3d_vulkan
//
//      struct fragment_push_constants
//{
//
//   int useTextureAlbedo;
//   int useTextureMetallicRoughness;
//   int useTextureNormal;
//   int useTextureAmbientOcclusion;
//   int useTextureEmissive;
//
//   ::floating_sequence3 albedo;
//   float metallic;
//   float roughness;
//   float ambientOcclusion;
//   ::floating_sequence3 emissive;
//
//
//   //    vec4 _pad1;
//
//   ::floating_sequence3 cameraPosition;
//   float bloomBrightnessCutoff;
//   // pad to 16-byte boundary
//   float _pad0;
//};

//
//BEGIN_GPU_PROPERTIES(::graphics3d_vulkan::gltf_render_system::push_constants)
//GPU_PROPERTY("modelMatrix", ::gpu::e_type_mat4)
//GPU_PROPERTY("normalMatrix", ::gpu::e_type_mat4)
//GPU_PROPERTY("useTextureAlbedo", ::gpu::e_type_int)
//GPU_PROPERTY("useTextureMetallicRoughness", ::gpu::e_type_int)
//GPU_PROPERTY("useTextureNormal", ::gpu::e_type_int)
//GPU_PROPERTY("useTextureAmbientOcclusion", ::gpu::e_type_int)
//GPU_PROPERTY("useTextureEmissive", ::gpu::e_type_int)
//GPU_PROPERTY("albedo", ::gpu::e_type_seq3)
//GPU_PROPERTY("metallic", ::gpu::e_type_float)
//GPU_PROPERTY("roughness", ::gpu::e_type_float)
//GPU_PROPERTY("ambientOcclusion", ::gpu::e_type_float)
//GPU_PROPERTY("emissive", ::gpu::e_type_seq3)
////GPU_PROPERTY("cameraPosition", ::gpu::e_type_seq3)
//GPU_PROPERTY("bloomBrightnessCutoff", ::gpu::e_type_float)
//GPU_PROPERTY("multiplier", ::gpu::e_type_seq3)
//END_GPU_PROPERTIES()
