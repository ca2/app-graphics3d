// From V0idsEmbrace.gltf_render_system by
// camilo on 2025-04-24 21:28 <3ThomasBorregaardSorensen!!
#include "framework.h"
// #include "bred/gpu/gltf_model.h"
#include "acme/filesystem/filesystem/file_context.h"
//#include "gpu_directx11/command_buffer.h"
#include "gpu_directx12/descriptors.h"
//#include "gpu_directx11/gltf_model.h"
#include "gpu_directx12/gltf/model.h"
#include "gpu_directx12/ibl/diffuse_irradiance_map.h"
//#include "gpu_directx11/ibl/brdf_convolution_framebuffer.h"
#include "gpu_directx12/ibl/specular_map.h"
//#include "gpu_directx11/pipeline.h"
//#include "gpu_directx11/render_pass.h"
//#include "gpu_directx11/render_target.h"
#include "gpu_directx12/renderer.h"
#include "gpu_directx12/texture.h"
//#include "gpu_directx11/vk_init.h"
#include "app-graphics3d/graphics3d/scene.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/frame.h"
#include "bred/graphics3d/asset_manager.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/global_ubo1.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/scene_renderable.h"
#include "gltf_render_system.h"
#include "gpu_directx12/gltf/model.h"
#include "gpu_directx12/shader.h"
#include "bred/gltf/vertex.h"
// #include "graphics3d/_.h"
// #include <stdexcept>
#include "gpu/ibl/_hlsl.h"


namespace graphics3d_directx12
{


   gltf_render_system::gltf_render_system()
   {
   
   
   }


   gltf_render_system::~gltf_render_system()
   {

   }


   ::memory gltf_render_system::pbr_with_ibl_vert_memory()
   {

      return hlsl_embedded_gltf_vert();

   }


   ::memory gltf_render_system::pbr_with_ibl_frag_memory()
   {

      return hlsl_embedded_gltf_frag();

   }

   
   void gltf_render_system::on_prepare(::gpu::context *pgpucontext)
   {
      // m_pdescriptorsetlayoutUbo = globalSetLayout;

      ::graphics3d::gltf_render_system::on_prepare(pgpucontext);
      
      //::cast<::gpu_directx11::context> pcontext = m_pengine->gpu_context();

      //constructø(m_pshaderOpaque);
      //constructø(m_pshaderMask);
      //constructø(m_pshaderBlend);

      //// m_ppipelineOpaque->initialize_shader(pgpucontext->m_pgpurenderer, "matter://shaders/gltf_vert.vert",
      ////                                      "matter://shaders/gltf_frag.frag");

      //// m_ppipelineMask->initialize_shader(pgpucontext->m_pgpurenderer, "matter://shaders/gltf_vert.vert",
      ////                                    "matter://shaders/gltf_frag.frag");

      //// m_ppipelineBlend->initialize_shader(pgpucontext->m_pgpurenderer, "matter://shaders/gltf_vert.vert",
      ////                                     "matter://shaders/gltf_frag.frag");

      //auto ppropertiesUbo = ::gpu_properties<::graphics3d::global_ubo1>();
      ////auto ppropertiesPushVertex = ::gpu_properties<::gpu::model_normal>();
      ////auto ppropertiesPushFragment = ::gpu_properties<fragment_push_constants>();
      //auto ppropertiesPush = ::gpu_properties<push_constants>();
      //auto pinputlayout = pgpucontext->input_layout < ::gpu::gltf::vertex >();

      ////::cast<::gpu_directx11::context> pcontext = m_pengine->gpu_context();

      ////m_pdescriptorsetlayoutIbl =
      ////   ::gpu_directx11::descriptor_set_layout::Builder(pcontext)
      ////      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      ////      .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      ////      .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      ////      .build();

      ////m_pdescriptorsetlayoutPbr =
      ////   ::gpu_directx11::descriptor_set_layout::Builder(pcontext)
      ////      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // albedo
      ////      .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // normal
      ////      .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // metallicRoughness
      ////      .addBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // ao
      ////      .addBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // emissive
      ////      .build();

      //::cast<::gpu_directx11::shader> pshaderOpaque = m_pshaderOpaque;



      ////pshaderOpaque->m_mapDescriptorSetLayout[1] = m_pdescriptorsetlayoutIbl;
      ////pshaderOpaque->m_mapDescriptorSetLayout[2] = m_pdescriptorsetlayoutPbr;
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
      //   //{}, 
      //   //{}, 
      //   pinputlayout);

      //::cast<::gpu_directx11::shader> pshaderMask = m_pshaderMask;

      ////pshaderMask->m_mapDescriptorSetLayout[1] = m_pdescriptorsetlayoutIbl;
      ////pshaderMask->m_mapDescriptorSetLayout[2] = m_pdescriptorsetlayoutPbr;
      //m_pshaderMask->m_propertiesPushShared.set_properties(ppropertiesPush);
      //pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushShared, false);
      ////m_pshaderMask->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      ////m_pshaderMask->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      ////pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushVertex);
      ////pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushFragment);
      //m_pshaderMask->set_global_ubo();
      //m_pshaderMask->initialize_shader_with_block(
      //   pgpucontext->m_pgpurenderer, 
      //   embedded_pbr_vert(), 
      //   embedded_pbr_frag(),
      //   //{}, 
      //   //{}, 
      //   pinputlayout);

      //::cast<::gpu_directx11::shader> pshaderBlend = m_pshaderBlend;

      ////pshaderBlend->m_mapDescriptorSetLayout[1] = m_pdescriptorsetlayoutIbl;
      ////pshaderBlend->m_mapDescriptorSetLayout[2] = m_pdescriptorsetlayoutPbr;
      //m_pshaderBlend->m_propertiesPushShared.set_properties(ppropertiesPush);
      //pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushShared, false);
      ////m_pshaderBlend->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
      ////m_pshaderBlend->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
      ////pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushVertex);
      ////pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushFragment);
      //m_pshaderBlend->set_global_ubo();
      //m_pshaderBlend->initialize_shader_with_block(
      //   pgpucontext->m_pgpurenderer, 
      //   embedded_pbr_vert(),
      //   embedded_pbr_frag(),
      //   //{}, 
      //   //{},
      //   pinputlayout);



      ////createPipelineLayout(pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout());

      ////auto prenderpass = render_pass2();

      ////createPipeline(prenderpass->m_vkrenderpass);

      //::cast<::gpu_directx11::renderer> prenderer = pcontext->m_pgpurenderer;

      ////::cast<::gpu_directx11::render_target> prendertarget = prenderer->m_pgpurendertarget;

      ////auto frameCount = prenderer->get_frame_count();

      ////m_vkdescriptorsetaIbl.resize(frameCount);

      ////auto pdescriptorpoolbuilder = allocateø::gpu_directx11::descriptor_pool::Builder();

      ////pdescriptorpoolbuilder->initialize_builder(pcontext);
      ////pdescriptorpoolbuilder->setMaxSets(frameCount * 10);
      ////pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, frameCount * 100);

      ////m_pdescriptorpool = pdescriptorpoolbuilder->build();


      //auto passetmanager = m_pengine->m_pimmersionlayer->m_passetmanager;

      //::cast<::graphics3d::scene> pscene = m_pengine->m_pimmersionlayer->m_pscene;

      ////for (uint32_t i = 0; i < frameCount; i++)
      ////{

      ////   VkDescriptorSet set;

      ////   // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set,
      ////   //                                 /*setIndex=*/0);
      ////   m_pdescriptorpool->allocateDescriptor(
      ////      m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set, 0);

      ////   ::cast<::gpu_directx11::texture> ptextureIrrad = pscene->m_ptextureIrradianceCube;
      ////   ::cast<::gpu_directx11::texture> ptexturePrefltr = pscene->m_ptexturePrefilteredCube;
      ////   ::cast<::gpu_directx11::texture> ptextureBrdf = pscene->m_ptextureLuBrdf;
      ////   // auto irradianceInfo = m_assets.getIrradianceDescriptor();
      ////   // auto prefilterInfo = m_assets.getPrefilteredDescriptor();

      ////   auto irradianceInfo = ptextureIrrad->m_descriptor3;
      ////   auto prefilterInfo = ptexturePrefltr->m_descriptor3;
      ////   auto brdfInfo = ptextureBrdf->m_descriptor3;


      ////   ::gpu_directx11::descriptor_writer(*m_pdescriptorsetlayoutIbl, *m_pdescriptorpool)
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
      ////      ::cast<::gpu_directx11::context> pcontext = m_pengine->gpu_context();
      ////      ::cast<::gpu_directx11::renderer> prenderer = pcontext->m_pgpurenderer;

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
      ////         ::cast<::gpu_directx11::gltf::Model> pgltfmodel = prenderable;

      ////         if (!pgltfmodel)
      ////            continue;

      ////         //for (auto &material: pgltfmodel->m_materials)
      ////         //{
      ////         //   if (material.baseColorTexture != nullptr)
      ////         //   {
      ////         //      material.addDescriptorSet(
      ////         //         m_pdescriptorpool->m_vkdescriptorpool, 
      ////         //         m_pdescriptorsetlayoutPbr->m_vkdescriptorsetlayout, 
      ////         //         ::gpu_directx11::gltf::DescriptorBindingFlags::ImageBaseColor |
      ////         //         ::gpu_directx11::gltf::DescriptorBindingFlags::ImageMetallicMap |
      ////         //         ::gpu_directx11::gltf::DescriptorBindingFlags::ImageNormalMap |
      ////         //         ::gpu_directx11::gltf::DescriptorBindingFlags::ImageAOMap |
      ////         //         ::gpu_directx11::gltf::DescriptorBindingFlags::ImageEmissiveMap,
      ////         //         pgltfmodel->emptyTexture);
      ////         //   }
      ////         //}


      ////         //pgltfmodel->m_vkdescriptorsetaPbr.resize(frameCount);


      ////         // pgltfmodel->bind(pgpucommandbuffer);

      ////         // for (auto *node: pgltfmodel->m_pgltfmodel->m_linearNodes)
      ////         //{
      ////         //    if (!node->mesh)
      ////         //       continue;


      ////         //::cast<::gpu_directx11::texture> ptextureAlbedo = pgltfmodel->y;
      ////         //::cast<::gpu_directx11::texture> ptextureNormal = passetmanager->getTexture("cerberus_normal");
      ////         //::cast<::gpu_directx11::texture> ptextureMetallic = passetmanager->getTexture("cerberus_metallic");
      ////         //::cast<::gpu_directx11::texture> ptextureRoughness = passetmanager->getTexture("cerberus_roughness");
      ////         //::cast<::gpu_directx11::texture> ptextureAo = passetmanager->getTexture("cerberus_ao");*/


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

      ////         //::gpu_directx11::descriptor_writer(*m_pdescriptorsetlayoutPbr, *m_pdescriptorpool)
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


   //void gltf_render_system::createPipelineLayout(aaaVkDescriptorSetLayout globalSetLayout)
   //{

   //   ::cast<::gpu_directx11::context> pcontext = m_pengine->gpu_context();

   //   auto descriptorsetlayout0 = globalSetLayout;
   //   auto descriptorsetlayout1 = pcontext->m_psetdescriptorlayoutGlobal->m_vkdescriptorsetlayout;
   //   auto descriptorsetlayout2 = m_pdescriptorsetlayoutPbr->getDescriptorSetLayout();
   //   auto descriptorsetlayout3 = m_pdescriptorsetlayoutIbl->getDescriptorSetLayout();

   //   const std::vector<aaaVkDescriptorSetLayout> layouts = {
   //      descriptorsetlayout0,
   //      descriptorsetlayout1,
   //      descriptorsetlayout2,
   //      descriptorsetlayout3,

   //   };


   //   VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
   //   pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   //   pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
   //   pipelineLayoutInfo.pSetLayouts = layouts.data();


   //   if (vkCreatePipelineLayout(pcontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) !=
   //       VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "Failed to create GLTF pipeline layout");
   //   }
   //}


   //void gltf_render_system::createPipeline(VkRenderPass renderPass)
   //{

   //   ::cast<::gpu_directx11::context> pcontext = m_pengine->gpu_context();
   //   ::cast<::gpu_directx11::device> pgpudevice = pcontext->m_pgpudevice;

   //   assert(m_pipelineLayout != VK_NULL_HANDLE);

   //   auto vertSpv = "matter://shaders/gltf_vert.vert";
   //   auto fragSpv = "matter://shaders/gltf_frag.frag";

   //   ::memory vert;
   //   ::memory frag;

   //   pgpudevice->defer_shader_memory(vert, vertSpv);
   //   pgpudevice->defer_shader_memory(frag, fragSpv);

   //   std::vector<VkVertexInputBindingDescription> bindings = {
   //      vkinit::vertexInputBindingDescription(0, sizeof(::gpu_directx11::gltf::Vertex), VK_VERTEX_INPUT_RATE_VERTEX)};

   //   std::vector<VkVertexInputAttributeDescription> attributes = {
   //      vkinit::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT,
   //                                              offsetof(::gpu_directx11::gltf::Vertex, pos)),
   //      vkinit::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT,
   //                                              offsetof(::gpu_directx11::gltf::Vertex, normal)),
   //      vkinit::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT,
   //                                              offsetof(::gpu_directx11::gltf::Vertex, uv)),
   //      vkinit::vertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT,
   //                                              offsetof(::gpu_directx11::gltf::Vertex, color)),
   //      vkinit::vertexInputAttributeDescription(0, 4, VK_FORMAT_R32G32B32A32_SFLOAT,
   //                                              offsetof(::gpu_directx11::gltf::Vertex, tangent))};

   //   // OPAQUE
   //   ::vulkan::pipeline_configuration opaqueConfig{};
   //   ::vulkan::defaultPipelineConfigInfo2(opaqueConfig);

   //   opaqueConfig.pipelineLayout = m_pipelineLayout;
   //   opaqueConfig.renderPass = renderPass;
   //   opaqueConfig.bindingDescriptions = bindings;
   //   opaqueConfig.attributeDescriptions = attributes;

   //   m_opaquePipeline = create_newø<::gpu_directx11::pipeline>();

   //   m_opaquePipeline->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag, opaqueConfig);

   //   // MASK
   //   ::vulkan::pipeline_configuration maskConfig{};
   //   ::vulkan::defaultPipelineConfigInfo2(maskConfig);
   //   maskConfig.pipelineLayout = m_pipelineLayout;
   //   maskConfig.renderPass = renderPass;
   //   maskConfig.bindingDescriptions = bindings;
   //   maskConfig.attributeDescriptions = attributes;
   //   maskConfig.colorBlendAttachments[0].blendEnable = VK_FALSE;

   //   struct SpecData
   //   {
   //      VkBool32 alphaMask;
   //      float cutoff;
   //   };
   //   static SpecData specData{VK_TRUE, 0.5f};
   //   static VkSpecializationMapEntry mapEntries[2] = {{0, offsetof(SpecData, alphaMask), sizeof(VkBool32)},
   //                                                    {1, offsetof(SpecData, cutoff), sizeof(float)}};
   //   static VkSpecializationInfo specInfo{};
   //   specInfo.mapEntryCount = 2;
   //   specInfo.pMapEntries = mapEntries;
   //   specInfo.dataSize = sizeof(specData);
   //   specInfo.pData = &specData;

   //   maskConfig.fragSpecInfo = &specInfo;

   //   // m_maskPipeline = std::make_unique<VkSandboxPipeline>(m_device, vertSpv, fragSpv, maskConfig);
   //   m_maskPipeline = create_newø<::gpu_directx11::pipeline>();

   //   m_maskPipeline->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag, maskConfig);

   //   // BLEND
   //   ::vulkan::pipeline_configuration blendConfig{};
   //   ::vulkan::defaultPipelineConfigInfo2(blendConfig);
   //   blendConfig.pipelineLayout = m_pipelineLayout;
   //   blendConfig.renderPass = renderPass;
   //   blendConfig.bindingDescriptions = bindings;
   //   blendConfig.attributeDescriptions = attributes;

   //   blendConfig.colorBlendAttachments[0].blendEnable = VK_TRUE;
   //   blendConfig.colorBlendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
   //   blendConfig.colorBlendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   //   blendConfig.colorBlendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD;
   //   blendConfig.colorBlendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
   //   blendConfig.colorBlendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
   //   blendConfig.colorBlendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD;

   //   blendConfig.colorBlendAttachments[0].colorWriteMask =
   //      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

   //   // m_blendPipeline = std::make_unique<VkSandboxPipeline>(m_device, vertSpv, fragSpv, blendConfig);
   //   m_blendPipeline = create_newø<::gpu_directx11::pipeline>();

   //   m_blendPipeline->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag, blendConfig);
   //}

   // void gltf_render_system::on_prepare(::gpu::context *pgpucontext)
   //{
   //

   //   initialize_GltfRenderSystem()
   //
   //}


   // void gltf_render_system::render(FrameInfo &frame)
   //{
   //    static bool warnedThisFrame = false;
   //
   //    for (auto &[id, go]: frame.gameObjects)
   //    {
   //
   //       if (go->getPreferredRenderTag() != RenderTag::Gltf)
   //       {
   //          continue; // not mine, skip
   //       }
   //       auto baseModel = go->getModel();
   //       if (!baseModel)
   //          continue;
   //
   //       auto model = std::dynamic_pointer_cast<vkglTF::Model>(baseModel);
   //       if (!model)
   //          continue;
   //
   //       model->bind(frame.commandBuffer);
   //
   //       for (auto *node: model->m_linearNodes)
   //       {
   //          if (!node->mesh)
   //             continue;
   //
   //          floating_matrix4 world = go->getTransform().mat4() * node->getMatrix();
   //          floating_matrix4 normalMat = glm::transpose(glm::inverse(world));
   //
   //          memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
   //          memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
   //
   //          for (auto *primitive: node->mesh->primitives)
   //          {
   //             // --- Bind sets 0 & 1 (global + node UBO) ---
   //             std::array<VkDescriptorSet, 2> sets01 = {
   //                frame.globalDescriptorSet, // set 0
   //                node->mesh->uniformBuffer.descriptorSet // set 1
   //             };
   //             vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0,
   //                                     static_cast<uint32_t>(sets01.size()), sets01.data(), 0, nullptr);
   //
   //             // --- Bind our PBR set (set = 2) ---
   //             VkDescriptorSet pbrSet = m_pdescriptorsetlayoutUbo[frame.frameIndex];
   //             if (pbrSet == VK_NULL_HANDLE)
   //             {
   //                if (!warnedThisFrame)
   //                { /*spdlog::warn("PBR set null");*/
   //                   warnedThisFrame = true;
   //                }
   //                continue;
   //             }
   //             vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 2, 1,
   //                                     &pbrSet, 0, nullptr);
   //
   //             // --- Bind IBL set (set = 3) ---
   //             VkDescriptorSet iblSet = m_vkdescriptorsetaIbl[frame.frameIndex];
   //             if (iblSet == VK_NULL_HANDLE)
   //             {
   //                continue;
   //             }
   //             vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 3, 1,
   //                                     &iblSet, 0, nullptr);
   //
   //             // Pick pipeline by alpha mode
   //             switch (primitive->material.alphaMode)
   //             {
   //                case vkglTF::Material::ALPHAMODE_OPAQUE:
   //                   m_opaquePipeline->bind(frame.commandBuffer);
   //                   break;
   //                case vkglTF::Material::ALPHAMODE_MASK:
   //                   m_maskPipeline->bind(frame.commandBuffer);
   //                   break;
   //                case vkglTF::Material::ALPHAMODE_BLEND:
   //                default:
   //                   m_blendPipeline->bind(frame.commandBuffer);
   //                   break;
   //             }
   //
   //             model->gltfDraw(frame.commandBuffer, vkglTF::RenderFlags::RenderNone, m_pipelineLayout, 2);
   //             warnedThisFrame = false;
   //          }
   //       }
   //    }


   //void gltf_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene)
   //{

   //   // vkCmdBindDescriptorSets(
   //   //     frame.m_pcommandbuffer,
   //   //     VK_PIPELINE_BIND_POINT_GRAPHICS,
   //   //     m_pipelineLayout,
   //   //     0, 1,
   //   //     &frame.globalDescriptorSet,
   //   //     0, nullptr);

   //   auto pframe = ::gpu::current_frame();

   //   ::cast<::gpu_directx11::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

   //   auto &sceneobjects = pscene->scene_objects();

   //   for (auto &[id, pobject]: sceneobjects)
   //   {

   //      ::cast<::graphics3d::scene_object> psceneobject = pobject;

   //      if (!psceneobject)
   //      {

   //         continue;
   //      }

   //      auto prenderable = psceneobject->renderable();

   //      if (!prenderable || prenderable->m_erenderabletype != ::gpu::e_renderable_type_gltf)
   //      {

   //         continue;
   //      }

   //      ::cast<::gpu_directx11::gltf::Model> pmodel = prenderable;

   //      if (!pmodel)
   //         continue;

   //      pmodel->bind(pframe->m_pgpucommandbuffer);

   //      for (auto *node: pmodel->m_linearNodes)
   //      {

   //         if (!node->mesh)
   //            continue;

   //         floating_matrix4 world = psceneobject->transform().getMatrix() * node->getMatrix();
   //         floating_matrix4 normalMat = glm::transpose(glm::inverse(world));
   //         memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
   //         memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));

   //         vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
   //                                 m_pipelineLayout, 1, 1, &node->mesh->uniformBuffer.descriptorSet, 0, nullptr);

   //         auto pmaterial = node->mesh->primitives[0]->m_pmaterial;
   //         switch (pmaterial->alphaMode)
   //         {
   //            case ::gpu_directx11::gltf::Material::ALPHAMODE_OPAQUE:
   //               m_opaquePipeline->bind(pcommandbuffer);
   //               break;
   //            case ::gpu_directx11::gltf::Material::ALPHAMODE_MASK:
   //               m_maskPipeline->bind(pcommandbuffer);
   //               break;
   //            case ::gpu_directx11::gltf::Material::ALPHAMODE_BLEND:
   //            default:
   //               m_blendPipeline->bind(pcommandbuffer);
   //               break;
   //         }


   //         pmodel->gltfDraw(pcommandbuffer->m_vkcommandbuffer, ::gpu_directx11::gltf::RenderFlags::RenderNone,
   //                          m_pipelineLayout, 2);
   //         // pmodel->drawNode(node,
   //         //  pcommandbuffer->m_vkcommandbuffer,
   //         //::gpu_directx11::gltf::RenderFlags::BindImages,
   //         // m_pipelineLayout,
   //         //              2 // bindImageSet
   //         //);
   //      }
   //   }
   //}


//void GltfRenderSystem::render(FrameInfo &frame)
//   {
//      static bool warnedThisFrame = false;
//
//      for (auto &[id, go]: frame.gameObjects)
//      {
//
//         if (go->getPreferredRenderTag() != RenderTag::Gltf)
//         {
//            continue; // not mine, skip
//         }
//         auto baseModel = go->getModel();
//         if (!baseModel)
//            continue;
//
//         auto model = std::dynamic_pointer_cast<vkglTF::Model>(baseModel);
//         if (!model)
//            continue;
//
//         model->bind(frame.commandBuffer);
//
//         for (auto *node: model->m_linearNodes)
//         {
//            if (!node->mesh)
//               continue;
//
//            floating_matrix4 world = go->getTransform().mat4() * node->getMatrix();
//            floating_matrix4 normalMat = glm::transpose(glm::inverse(world));
//
//            memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
//            memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
//
//            for (auto *primitive: node->mesh->primitives)
//            {
//                --- Bind sets 0 & 1 (global + node UBO) ---
//               std::array<VkDescriptorSet, 2> sets01 = {
//                  frame.globalDescriptorSet, // set 0
//                  node->mesh->uniformBuffer.descriptorSet // set 1
//               };
//               vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0,
//                                       static_cast<uint32_t>(sets01.size()), sets01.data(), 0, nullptr);
//
//                --- Bind our PBR set (set = 2) ---
//               VkDescriptorSet pbrSet = m_pdescriptorsetlayoutUbo[frame.frameIndex];
//               if (pbrSet == VK_NULL_HANDLE)
//               {
//                  if (!warnedThisFrame)
//                  { /*spdlog::warn("PBR set null");*/
//                     warnedThisFrame = true;
//                  }
//                  continue;
//               }
//               vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 2, 1,
//                                       &pbrSet, 0, nullptr);
//
//                --- Bind IBL set (set = 3) ---
//               VkDescriptorSet iblSet = m_vkdescriptorsetaIbl[frame.frameIndex];
//               if (iblSet == VK_NULL_HANDLE)
//               {
//                  continue;
//               }
//               vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 3, 1,
//                                       &iblSet, 0, nullptr);
//
//                Pick pipeline by alpha mode
//               switch (primitive->material.alphaMode)
//               {
//                  case vkglTF::Material::ALPHAMODE_OPAQUE:
//                     m_opaquePipeline->bind(frame.commandBuffer);
//                     break;
//                  case vkglTF::Material::ALPHAMODE_MASK:
//                     m_maskPipeline->bind(frame.commandBuffer);
//                     break;
//                  case vkglTF::Material::ALPHAMODE_BLEND:
//                  default:
//                     m_blendPipeline->bind(frame.commandBuffer);
//                     break;
//               }
//
//               model->gltfDraw(frame.commandBuffer, vkglTF::RenderFlags::RenderNone, m_pipelineLayout, 2);
//               warnedThisFrame = false;
//            }
//         }
//      }
//   }

void gltf_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscenebase)
   {

   ::graphics3d::gltf_render_system::on_render(pgpucontext, pscenebase);

//      static bool warnedThisFrame = false;
//
//            auto pframe = ::gpu::current_frame();
//
//         //::cast<::gpu_directx11::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;
//            ::cast<::graphics3d::scene> pscene = pscenebase;
//      
//         auto &scenerenderables = pscene->scene_renderables();
//      //   //// xxxxxxxxxxxxxxxxx
//      ::cast<::gpu_directx11::context> pcontext = m_pengine->gpu_context();
//
//      ::cast<::gpu_directx11::renderer> prenderer = pcontext->m_pgpurenderer;
//      
//   
//      ////// xxxxxxxxxxxxxxxxx
//      //auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
//         //auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);
//
//      for (auto &[id, pscenerenderable]: scenerenderables)
//         {
//
//            if (!pscenerenderable)
//            {
//
//               continue;
//            }
//
//            if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_ibl)
//            {
//
//               continue;
//            }
//
//
//            auto prenderable = pscenerenderable->renderable();
//            if (!prenderable)
//               continue;
//
//            auto erenderabletype = prenderable->m_erenderabletype;
//
//            if (erenderabletype != ::gpu::e_renderable_type_gltf)
//            {
//               continue; // not mine, skip
//            }
//            ::cast<::gpu_directx11::gltf::model> pgltfmodel = prenderable;
//
//            if (!pgltfmodel)
//               continue;
//
//            m_erendersystem = ::graphics3d::e_render_system_gltf_ibl;
//
//            pframe->m_pgpucommandbuffer->m_prendersystem = this;
//
//
//            // for (auto *node: pgltfmodel->m_linearNodes)
//            //{
//            //    if (!node->mesh)
//            //       continue;
//
//               //floating_matrix4 world = pscenerenderable->transform().getMatrix() * node->getMatrix();
////              floating_matrix4 normalMat = glm::transpose(glm::inverse(world));
//            //   //memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
//            //   //memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
//
//             //for (auto pmesh: pgltfmodel->m_mesha)
//               //{
//
//            //                     // Pick pipeline by alpha mode
//                  /*switch (pmesh->m_pmaterial->alphaMode)
//                  {
//                     case ::gpu_directx11::gltf::Material::ALPHAMODE_OPAQUE:
//                  */      
//            if (pgpucontext->defer_bind(m_pshaderOpaque))
//            {
//
//                  ::cast<::gpu_directx11::ibl::diffuse_irradiance_map> pirradiancemap = pscene->m_pibldiffuseirradiancemap;
//               ::cast<::gpu_directx11::ibl::specular_map> pspecularmap = pscene->m_piblspecularmap;
//
//               ::cast<::gpu_directx11::texture> ptextureIrradiance =
//                  pirradiancemap->m_ptextureDiffuseIrradianceCubemap;
//
//               ID3D11SamplerState *sampler = nullptr;
//               ID3D11ShaderResourceView *srv[3] = {};
//
//               if (ptextureIrradiance)
//               {
//                  if (!ptextureIrradiance->m_pshaderresourceview)
//                  {
//                     warning("irradiance shader resource view not set");
//
//                  }
//                  srv[0] = ptextureIrradiance->m_pshaderresourceview;
//                  //pcontext->m_pcontext->PSSetShaderResources(0, 1, srv);
//                  // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_albedo);
//                  // shader.setInt("material.textureAlbedo", e_gltf_texture_albedo);
//                  // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAlbedo->mId);
//                  if (!sampler && ptextureIrradiance->m_psamplerstate)
//                  {
//                     sampler = ptextureIrradiance->m_psamplerstate;
//                  }
//               }
//
//               ::cast<::gpu_directx11::texture> ptextureEnvMap =
//                  pspecularmap->m_ptexturePrefilteredEnvMapCubemap;
//
//               if (ptextureEnvMap)
//               {
//
//                  if (!ptextureEnvMap->m_pshaderresourceview)
//                  {
//                     
//                     warning("env map shader resource view not set");
//
//                  }
//
//                  srv[1] = ptextureEnvMap->m_pshaderresourceview;
//
//                  //pcontext->m_pcontext->PSSetShaderResources(1, 1, srv);
//
//                  if (!sampler && ptextureEnvMap->m_psamplerstate)
//                  {
//
//                     sampler = ptextureEnvMap->m_psamplerstate;
//
//                  }
//
//               }
//
//               ::cast<::gpu_directx11::texture> ptextureBrdf = pspecularmap->m_ptextureBrdfConvolutionMap;
//
//               if (ptextureBrdf)
//               {
//                  if (!ptextureBrdf->m_pshaderresourceview)
//                  {
//                     warning("brdf shader resource view not set");
//                  }
//
//                  srv[2] = ptextureBrdf->m_pshaderresourceview;
//                  //pcontext->m_pcontext->PSSetShaderResources(2, 1, srv);
//                  // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_albedo);
//                  // shader.setInt("material.textureAlbedo", e_gltf_texture_albedo);
//                  // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAlbedo->mId);
//                  if (!sampler && ptextureBrdf->m_psamplerstate)
//                  {
//                     sampler = ptextureBrdf->m_psamplerstate;
//                  }
//               }
//
//               if (sampler)
//               {
//                  
//                  ID3D11SamplerState *samplers[1] = {sampler};
//                  
//                  pcontext->m_pcontext->PSSetSamplers(0, 1, samplers);
//
//                  pcontext->m_pcontext->PSSetShaderResources(0, 3, srv);
//
//               }
//
//            }
//                       /* break;
//                     case ::gpu_directx11::gltf::Material::ALPHAMODE_MASK:
//                        pgpucontext->defer_bind(m_pshaderMask);
//                        break;
//                     case ::gpu_directx11::gltf::Material::ALPHAMODE_BLEND:
//                     default:
//                        pgpucontext->defer_bind(m_pshaderBlend);
//                        break;
//                  }*/
//            m_pscenerenderableCurrent = pscenerenderable;
//                              pgltfmodel->bind(pframe->m_pgpucommandbuffer);
//
//
//            //      ::cast<::gpu_directx11::shader> pshader = pgpucontext->m_pshaderBound;
//
//            //      auto prendersystem = this;
//
//                  //pshader->set_matrix4("modelMatrix", world);
//                  //pshader->set_matrix4("normalMatrix", normalMat);
//            //      bool bAlbedo = pgltfmodel->m_materials[0].baseColorTexture.is_set();
//            //      bAlbedo = bAlbedo && !m_bDisableAlbedo;
//            //      pshader->set_i32("useTextureAlbedo", bAlbedo ? 1 : 0);
//
//            //          floating_sequence3 seq3Albedo = {};
//            //      if (prendersystem->m_bForceDefaultAmbientOcclusionFactor)
//            //      {
//
//            //         seq3Albedo = prendersystem->m_seq3DefaultAlbedo;
//            //      }
//            //      else
//            //      {
//
//            //         seq3Albedo = ::floating_sequence3(pgltfmodel->m_materials[0].baseColorFactor.r,
//            //                                   pgltfmodel->m_materials[0].baseColorFactor.g,
//            //                                   pgltfmodel->m_materials[0].baseColorFactor.b);
//
//            //      }
//
//            //      pshader->set_sequence3("albedo", seq3Albedo);
//
//            //      bool bMetallicRoughness = pgltfmodel->m_materials[0].metallicRoughnessTexture.is_set();
//            //      bMetallicRoughness = bMetallicRoughness && !m_bDisableMetallicRoughness;
//            //      pshader->set_i32("useTextureMetallicRoughness", bMetallicRoughness ? 1 : 0);
//            //      bool bNormal = pgltfmodel->m_materials[0].normalTexture.is_set();
//
//
//            //                  float fMetallic = 0.0f;
//            //      if (prendersystem->m_bForceDefaultMetallicFactor)
//            //      {
//
//            //         fMetallic = prendersystem->m_fDefaultMetallicFactor;
//            //      }
//            //      else
//            //      {
//
//            //         fMetallic = pgltfmodel->m_materials[0].metallicFactor;
//            //      }
//            //      float fRoughness = 0.0f;
//            //      if (prendersystem->m_bForceDefaultRoughnessFactor)
//            //      {
//
//            //         fRoughness = prendersystem->m_fDefaultRoughnessFactor;
//            //      }
//            //      else
//            //      {
//
//            //         fRoughness = pgltfmodel->m_materials[0].roughnessFactor;
//            //      }
//            //      pshader->set_f32("metallic", fMetallic);
//            //      pshader->set_f32("roughness", fRoughness);
//
//            //      bNormal = bNormal && !m_bDisableNormal;
//            //      pshader->set_i32("useTextureNormal", bNormal ? 1 : 0);
//            //      bool bAmbientOcclusion = pgltfmodel->m_materials[0].occlusionTexture.is_set();
//            //      bAmbientOcclusion = bAmbientOcclusion && !m_bDisableAmbientOcclusion;
//            //      pshader->set_i32("useTextureAmbientOcclusion", bAmbientOcclusion ? 1 : 0);
//
//
//            //                  float fAmbientOcclusion = 0.0f;
//            //      if (prendersystem->m_bForceDefaultAmbientOcclusionFactor)
//            //      {
//
//            //         fAmbientOcclusion = prendersystem->m_fDefaultAmbientOcclusionFactor;
//            //      }
//            //      else
//            //      {
//
//            //         //fAmbientOcclusion = pgltfmodel->m_materials[0].occlusionTexture->m_fAmbientOcclusion;
//            //      }
//            //      pshader->set_f32("ambientOcclusion", fAmbientOcclusion);
//
//            //      floating_sequence3 seq3Emission = {};
//            //      if (prendersystem->m_bForceDefaultEmission)
//            //      {
//
//            //         seq3Emission = prendersystem->m_seq3DefaultEmission;
//            //      }
//            //      else
//            //      {
//
//            //         //seq3Emission = pgltfmodel->m_materials[0].m_seq3Emissive;
//            //      }
//            //      pshader->set_sequence3("emissive", seq3Emission);
//
//
//            //      bool bEmissive = pgltfmodel->m_materials[0].emissiveTexture.is_set();
//            //      bEmissive = bEmissive && !m_bDisableEmissive;
//            //      pshader->set_i32("useTextureEmissive", bEmissive ? 1 : 0);
//
//            //      //auto metallicFactor = pgltfmodel->m_materials[0].metallicFactor;
//            //      //if (m_bForceDefaultMetallicFactor)
//            //      //   metallicFactor = m_fDefaultMetallicFactor;
//            //      //pshader->set_f32("metallic", metallicFactor);
//            //      //auto roughnessFactor = pgltfmodel->m_materials[0].roughnessFactor;
//            //      //if (m_bForceDefaultRoughnessFactor)
//            //      //   roughnessFactor = m_fDefaultRoughnessFactor;
//            //      //pshader->set_f32("roughness", roughnessFactor);
//            //      //pshader->set_f32("ambientOcclusion", pgltfmodel->m_materials[0].am);
//            //      pshader->push_properties(pcommandbuffer);
//
//
//            //      //// --- Bind sets 0 & 1 (global + node UBO) ---
//            //      //std::array<VkDescriptorSet, 2> sets01 = {
//            //      //   frame.globalDescriptorSet, // set 0
//            //      //   node->mesh->uniformBuffer.descriptorSet // set 1
//            //      //};
//            //      //// xxxxxxxxxxxxxxxxx
//            //      ////// --- Bind sets 0 & 1 (global + node UBO) ---
//            //      // std::array<VkDescriptorSet, 2> sets01 = {
//            //      //   vkdescriptorsetGlobal, // set 0
//            //      //    node->mesh->uniformBuffer.descriptorSet // set 1
//            //      // };
//            //      //vkCmdBindDescriptorSets(
//            //      //   pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//            //      //   m_pipelineLayout, 0,
//            //      //   (uint32_t)sets01.size(), sets01.data(), 0, nullptr);
//            //       //// --- Bind sets 0 (global) ---
//            //      // node UBO transformed in Push Constants and set above
//            //       std::array<VkDescriptorSet, 1> sets01 = {
//            //          vkdescriptorsetGlobal // set 0
//            //       };
//            //       vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//            //                               pshader->m_ppipeline->m_vkpipelinelayout, 0, (uint32_t)sets01.size(),
//            //                               sets01.data(), 0, nullptr);
//
//            //      //// --- Bind our PBR set (set = 2) ---
//            //       // --- Bind our PBR set (set = 1) ---
//            //      if (pgltfmodel->m_materials.size() <= 0)
//            //      {
//
//            //         if (!warnedThisFrame)
//            //         { /*spdlog::warn("PBR set null");*/
//            //            warnedThisFrame = true;
//            //         }
//            //         continue;
//            //      }
//
//            //                     //// --- Bind IBL set (set = 3) ---
//            //      // --- Bind IBL set (set = 2) ---
//            //      VkDescriptorSet iblSet =
//            //         m_vkdescriptorsetaIbl[pcontext->m_pgpurenderer->m_pgpurendertarget->get_frame_index()];
//            //      if (iblSet == VK_NULL_HANDLE)
//            //      {
//            //         continue;
//            //      }
//            //      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//            //                              // m_pipelineLayout, 3, 1,
//            //                              pshader->m_ppipeline->m_vkpipelinelayout, 1, 1, &iblSet, 0, nullptr);
//
//
//            //       VkDescriptorSet pbrSet =
//            //         pgltfmodel->m_materials[0].m_descriptorseta[pframe->m_pgpucommandbuffer->m_iFrameIndex];
//            //      if (pbrSet == VK_NULL_HANDLE)
//            //      {
//            //         if (!warnedThisFrame)
//            //         { /*spdlog::warn("PBR set null");*/
//            //            warnedThisFrame = true;
//            //         }
//            //         continue;
//            //      }
//            //      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
//            //                              //m_pipelineLayout, 2, 1,
//            //                              pshader->m_ppipeline->m_vkpipelinelayout, 2, 1,
//            //                              &pbrSet, 0, nullptr);
//
//            //      pgltfmodel->gltfDraw(pcommandbuffer->m_vkcommandbuffer,
//            //         pframe->m_pgpucommandbuffer->m_iFrameIndex,::gpu_directx11::gltf::RenderNone,
//            //         pshader->m_ppipeline->m_vkpipelinelayout, 2);
//            //      warnedThisFrame = false;
//         //}
//         //   }
//         //}
//
//			pgltfmodel->draw(pframe->m_pgpucommandbuffer);
//
//            pgltfmodel->unbind(pframe->m_pgpucommandbuffer);
//
//      }
   }

} // namespace graphics3d_directx12
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
//
//BEGIN_GPU_PROPERTIES(::graphics3d_directx11::gltf_render_system::push_constants)
//GPU_PROPERTY("modelMatrix", ::gpu::e_type_mat4)
//GPU_PROPERTY("normalMatrix", ::gpu::e_type_mat4)
//GPU_PROPERTY("useTextureAlbedo", ::gpu::e_type_i32)
//GPU_PROPERTY("useTextureMetallicRoughness", ::gpu::e_type_i32)
//GPU_PROPERTY("useTextureNormal", ::gpu::e_type_i32)
//GPU_PROPERTY("useTextureAmbientOcclusion", ::gpu::e_type_i32)
//GPU_PROPERTY("useTextureEmissive", ::gpu::e_type_i32)
////GPU_PROPERTY("padding1", ::gpu::e_type_i32)
////GPU_PROPERTY("padding2", ::gpu::e_type_i32)
////GPU_PROPERTY("padding3", ::gpu::e_type_i32)
//GPU_PROPERTY("albedo", ::gpu::e_type_seq3)
//GPU_PROPERTY("metallic", ::gpu::e_type_f32)
//GPU_PROPERTY("roughness", ::gpu::e_type_f32)
//GPU_PROPERTY("ambientOcclusion", ::gpu::e_type_f32)
//GPU_PROPERTY("emissive", ::gpu::e_type_seq3)
////GPU_PROPERTY("fPadding4", ::gpu::e_type_f32)
////GPU_PROPERTY("cameraPosition", ::gpu::e_type_seq3)
//GPU_PROPERTY("bloomBrightnessCutoff", ::gpu::e_type_f32)
//GPU_PROPERTY("multiplier", ::gpu::e_type_seq3)
//END_GPU_PROPERTIES()
