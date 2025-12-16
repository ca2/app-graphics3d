// Changed by camilo on 2025-12-06 21:36 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "scene_render_system.h"
#include "bred/gltf/vertex.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/renderer.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/global_ubo1.h"
#include "bred/graphics3d/scene_base.h"
#include "gpu/gltf/model.h"
#include "gpu/ibl/diffuse_irradiance_map.h"
#include "gpu/ibl/specular_map.h"
#include "app-graphics3d/graphics3d/scene.h"
//#include <spdlog/spdlog.h>
//#include "vulkan_wrapper/render_systems/scene_rs.h"


namespace graphics3d
{


   scene_render_system::scene_render_system()
   {


   }

   //scene_render_system::scene_render_system(VkSandboxDevice &device, VkRenderPass renderPass,
     //                                       aaaVkDescriptorSetLayout globalSetLayout, IAssetProvider &assets) :
       //m_device(device), m_globalSetLayout(globalSetLayout), m_assets(assets)
   //{
   //}

   scene_render_system::~scene_render_system()
   {
      //vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
   }


   //void scene_render_system::init(VkSandboxDevice &device, VkRenderPass renderPass,
   //                               aaaVkDescriptorSetLayout globalSetLayout, VkSandboxDescriptorPool &descriptorPool,
   //                               size_t frameCount)
   //{
   //   m_globalSetLayout = globalSetLayout;


   //   createPipelineLayout(globalSetLayout);
   //   createPipeline(renderPass);
   //}
   ::gpu::property *scene_render_system::push_constants_properties()
   {

      auto ppropertyPushConstantsProperties = ::gpu_properties<push_constants>();

      return ppropertyPushConstantsProperties;
   }


   ::gpu::binding_set *scene_render_system::pbr_binding_set()
   {
      auto pcontext = m_pengine->gpu_context();

      auto pbindingsetSceneGltfPbr = pcontext->scene_gltf_pbr_binding_set();

      return pbindingsetSceneGltfPbr;
   }


   ::memory scene_render_system::pbr_with_ibl_vert_memory() { return scene_gltf_vert_memory(); }
   ::memory scene_render_system::pbr_with_ibl_frag_memory() { return scene_gltf_frag_memory(); }

   ::memory scene_render_system::scene_gltf_vert_memory()
   {

      return {}; 

         }


         ::memory scene_render_system::scene_gltf_frag_memory()
         {

            return {};

   }



   void scene_render_system::on_prepare(::gpu::context *pgpucontext)
   {

      ::graphics3d::pbr_with_ibl_render_system::on_prepare(pgpucontext);

//
//
//         //{
//
//         auto pcontext = m_pengine->gpu_context();
//
//         øconstruct(m_pshaderOpaque);
//         øconstruct(m_pshaderMask);
//         øconstruct(m_pshaderBlend);
//
//         auto memoryVert = scene_gltf_vert_memory();
//         auto memoryFrag = scene_gltf_frag_memory();
////         static unsigned int pvertshader[] = {
////#include "shader/scene.vert.spv.inl"
////         };
////
////
////         static unsigned int pfragshader[] = {
////#include "shader/scene.frag.spv.inl"
////         };
////         static unsigned int pfragshaderAlphaMask05[] = {
////#include "shader/scene_alpha_mask_0_5.frag.spv.inl"
////         };
//         //auto blockVert = ::as_memory_block(pvertshader);
//         //auto blockFrag = ::as_memory_block(pfragshader);
//         //auto blockFragAlphaMask05 = ::as_memory_block(pfragshaderAlphaMask05);
//
//         auto ppropertiesUbo = ::gpu_properties<::graphics3d::global_ubo1>();
//         auto ppropertiesPush = ::gpu_properties<push_constants>();
//         auto pinputlayout = pgpucontext->input_layout<::gpu::gltf::vertex>();
//
//         //øconstruct(m_pbindingsetIbl);
//
//         //m_pbindingsetIbl->binding(0)->m_ebinding = ::gpu::e
//
//         //m_pdescriptorsetlayoutIbl =
//            //::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
//            //   .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
//            //   .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
//            //   .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
//            //   .build();
//
//         //m_pdescriptorsetlayoutPbr =
//         //   ::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
//         //      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // albedo
//         //      .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // normal
//         //      .build();
//
//         auto pshaderOpaque = m_pshaderOpaque;
//         pshaderOpaque->m_bEnableBlend = false;
//         pshaderOpaque->binding_set(0, pgpucontext->global_ubo1_binding_set());
//         pshaderOpaque->binding_set(1, pgpucontext->ibl1_binding_set());
//         pshaderOpaque->binding_set(2, pgpucontext->scene_gltf_pbr_binding_set());
//         m_pshaderOpaque->m_propertiesPushShared.set_properties(ppropertiesPush);
//         pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushShared, false);
//         // m_pshaderOpaque->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
//         // m_pshaderOpaque->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
//         // pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushVertex);
//         // pgpucontext->layout_push_constants(m_pshaderOpaque->m_propertiesPushFragment);
//         m_pshaderOpaque->set_global_ubo();
//         m_pshaderOpaque->initialize_shader_with_block(pgpucontext->m_pgpurenderer, memoryVert, memoryFrag, {}, {},
//                                                       pinputlayout);
//
//         auto pshaderMask = m_pshaderMask;
//         pshaderMask->m_bEnableBlend = true;
//         pshaderMask->binding_set(0, pgpucontext->global_ubo1_binding_set());
//         pshaderMask->binding_set(1, pgpucontext->ibl1_binding_set());
//         pshaderMask->binding_set(2, pgpucontext->scene_gltf_pbr_binding_set());
//         m_pshaderMask->m_propertiesPushShared.set_properties(ppropertiesPush);
//         m_pshaderMask->set_global_ubo();
//         pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushShared, false);
//         // m_pshaderMask->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
//         // m_pshaderMask->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
//         // pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushVertex);
//         // pgpucontext->layout_push_constants(m_pshaderMask->m_propertiesPushFragment);
//         m_pshaderMask->initialize_shader_with_block(pgpucontext->m_pgpurenderer, memoryVert, memoryFrag, {}, {},
//                                                     pinputlayout);
//
//         auto pshaderBlend = m_pshaderBlend;
//
//         pshaderBlend->binding_set(0, pgpucontext->global_ubo1_binding_set());
//         pshaderBlend->binding_set(1, pgpucontext->ibl1_binding_set());
//         pshaderBlend->binding_set(2, pgpucontext->scene_gltf_pbr_binding_set());
//         pshaderBlend->m_bEnableBlend = true;
//         m_pshaderBlend->m_propertiesPushShared.set_properties(ppropertiesPush);
//         m_pshaderBlend->set_global_ubo();
//         pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushShared, false);
//         // m_pshaderBlend->m_propertiesPushVertex.set_properties(ppropertiesPushVertex);
//         // m_pshaderBlend->m_propertiesPushFragment.set_properties(ppropertiesPushFragment);
//         // pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushVertex);
//         // pgpucontext->layout_push_constants(m_pshaderBlend->m_propertiesPushFragment);
//         m_pshaderBlend->initialize_shader_with_block(pgpucontext->m_pgpurenderer, memoryVert, memoryFrag, {},
//                                                      {}, pinputlayout);
//

         // createPipelineLayout(pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout());

         // auto prenderpass = render_pass2();

         // createPipeline(prenderpass->m_vkrenderpass);

         //auto prenderer = pcontext->m_pgpurenderer;

         //auto prendertarget = prenderer->m_pgpurendertarget;

         //auto frameCount = prendertarget->get_frame_count();

         //auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

         //pdescriptorpoolbuilder->initialize_builder(pcontext);
         //pdescriptorpoolbuilder->setMaxSets(frameCount * 10);
         //pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, frameCount * 100);

         //m_pdescriptorpool = pdescriptorpoolbuilder->build();


         auto passetmanager = m_pengine->m_pimmersionlayer->m_passetmanager;

         //::cast<::graphics3d::scene> pscene = m_pengine->m_pimmersionlayer->m_pscene;

         // for (uint32_t i = 0; i < frameCount; i++)
         //{

         //   VkDescriptorSet set;

         //   // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set,
         //   //                                 /*setIndex=*/0);
         //   m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set, 0);

         //   ::cast<::gpu_vulkan::texture> ptextureIrrad = pscene->m_ptextureIrradianceCube;
         //   ::cast<::gpu_vulkan::texture> ptexturePrefltr = pscene->m_ptexturePrefilteredCube;
         //   ::cast<::gpu_vulkan::texture> ptextureBrdf = pscene->m_ptextureLuBrdf;
         //   // auto irradianceInfo = m_assets.getIrradianceDescriptor();
         //   // auto prefilterInfo = m_assets.getPrefilteredDescriptor();

         //   ptextureIrrad->set_state(::gpu::e_texture_state_shader_read);
         //   ptexturePrefltr->set_state(::gpu::e_texture_state_shader_read);
         //   ptextureBrdf->set_state(::gpu::e_texture_state_shader_read);

         //   ptextureIrrad->UpdateDescriptor();
         //   ptexturePrefltr->UpdateDescriptor();
         //   ptextureBrdf->UpdateDescriptor();

         //   auto irradianceInfo = ptextureIrrad->m_descriptor3;
         //   auto prefilterInfo = ptexturePrefltr->m_descriptor3;
         //   auto brdfInfo = ptextureBrdf->m_descriptor3;


         //   ::gpu_vulkan::descriptor_writer(*m_pdescriptorsetlayoutIbl, *m_pdescriptorpool)
         //      .writeImage(0, &irradianceInfo)
         //      .writeImage(1, &prefilterInfo)
         //      .writeImage(2, &brdfInfo)
         //      .build(set);

         //   m_vkdescriptorsetaIbl[i] = set;
         //}

         // for (uint32_t uFrameIndex = 0; uFrameIndex < frameCount; uFrameIndex++)
         //{
         //    //VkDescriptorSet set;
         //    // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutPbr->getDescriptorSetLayout(), set,
         //    // /*setIndex=*/0);
         //    m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutPbr->getDescriptorSetLayout(),
         //    // set);

         //   // auto logDescriptor = [&](const char *name, const VkDescriptorImageInfo &info)
         //   //{
         //   //    information("{} - sampler: {}, imageView: {}, layout: {}", name, (uint64_t)info.sampler,
         //   //                 (uint64_t)info.imageView, (int)info.imageLayout);
         //   // };

         //   if (1)
         //   {

         //      auto &scenerenderables = pscene->scene_renderables();

         //      //   //// xxxxxxxxxxxxxxxxx
         //      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();
         //      ::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;

         //      ////// xxxxxxxxxxxxxxxxx
         //      // auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
         //      auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer, uFrameIndex);


         //      for (auto &[id, pscenerenderable]: scenerenderables)
         //      {

         //         if (!pscenerenderable)
         //         {

         //            continue;
         //         }

         //         if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_ibl)
         //         {

         //            continue;
         //         }


         //         auto prenderable = pscenerenderable->renderable();
         //         if (!prenderable)
         //            continue;

         //         auto erenderabletype = prenderable->m_erenderabletype;

         //         if (erenderabletype != ::gpu::e_renderable_type_gltf)
         //         {
         //            continue; // not mine, skip
         //         }
         //         ::cast<::gpu_vulkan::gltf::Model> pgltfmodel = prenderable;

         //         if (!pgltfmodel)
         //            continue;

         //         // for (auto &material: pgltfmodel->m_materiala)
         //         //{
         //         //    if (material.baseColorTexture != nullptr)
         //         //    {
         //         //       material.addDescriptorSet(
         //         //          m_pdescriptorpool->m_vkdescriptorpool,
         //         //          m_pdescriptorsetlayoutPbr->m_vkdescriptorsetlayout,
         //         //          ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageBaseColor |
         //         //          ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageMetallicMap |
         //         //          ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageNormalMap |
         //         //          ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageAOMap |
         //         //          ::gpu_vulkan::gltf::DescriptorBindingFlags::ImageEmissiveMap,
         //         //          pgltfmodel->emptyTexture);
         //         //    }
         //         // }


         //         // pgltfmodel->m_vkdescriptorsetaPbr.resize(frameCount);


         //         // pgltfmodel->bind(pgpucommandbuffer);

         //         // for (auto *pnode: pgltfmodel->m_pgltfmodel->m_nodeaLinear)
         //         //{
         //         //    if (!pnode->m_pmesh)
         //         //       continue;


         //         //::cast<::gpu_vulkan::texture> ptextureAlbedo = pgltfmodel->y;
         //         //::cast<::gpu_vulkan::texture> ptextureNormal = passetmanager->getTexture("cerberus_normal");
         //         //::cast<::gpu_vulkan::texture> ptextureMetallic = passetmanager->getTexture("cerberus_metallic");
         //         //::cast<::gpu_vulkan::texture> ptextureRoughness = passetmanager->getTexture("cerberus_roughness");
         //         //::cast<::gpu_vulkan::texture> ptextureAo = passetmanager->getTexture("cerberus_ao");*/


         //         // VkDescriptorImageInfo albedoInfo = m_assets.getTextureDescriptor("cerberus_albedo");
         //         // VkDescriptorImageInfo normalInfo = m_assets.getTextureDescriptor("cerberus_normal");
         //         // VkDescriptorImageInfo metallicInfo = m_assets.getTextureDescriptor("cerberus_metallic");
         //         // VkDescriptorImageInfo roughnessInfo = m_assets.getTextureDescriptor("cerberus_roughness");
         //         // VkDescriptorImageInfo aoInfo = m_assets.getTextureDescriptor("cerberus_ao");

         //         // VkDescriptorImageInfo albedoInfo = ptextureAlbedo->m_descriptor3;
         //         // VkDescriptorImageInfo normalInfo = ptextureNormal->m_descriptor3;
         //         // VkDescriptorImageInfo metallicInfo = ptextureMetallic->m_descriptor3;
         //         // VkDescriptorImageInfo roughnessInfo = ptextureRoughness->m_descriptor3;
         //         // VkDescriptorImageInfo aoInfo = ptextureAo->m_descriptor3;

         //         // logDescriptor("albedo", albedoInfo);
         //         // logDescriptor("normal", normalInfo);
         //         // logDescriptor("metallic", metallicInfo);
         //         // logDescriptor("roughness", roughnessInfo);
         //         // logDescriptor("ao", aoInfo);

         //         //::gpu_vulkan::descriptor_writer(*m_pdescriptorsetlayoutPbr, *m_pdescriptorpool)
         //         //   .writeImage(0, &albedoInfo)
         //         //   .writeImage(1, &normalInfo)
         //         //   .writeImage(2, &metallicInfo)
         //         //   .writeImage(3, &roughnessInfo)
         //         //   .writeImage(4, &aoInfo)
         //         //   .build(set);

         //         // pgltfmodel->m_vkdescriptorsetaPbr[uFrameIndex] = set;
         //      }
         //   }
         //}
      //}

      //m_globalSetLayout = globalSetLayout;


      //createPipelineLayout(globalSetLayout);
      //createPipeline(renderPass);
   }


   //void scene_render_system::createPipelineLayout(aaaVkDescriptorSetLayout globalSetLayout)
   //{
   //   const std::vector<aaaVkDescriptorSetLayout> layouts = {globalSetLayout, vkglTF::descriptorSetLayoutUbo,
   //                                                       vkglTF::descriptorSetLayoutImage};

   //   VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
   //   pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   //   pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
   //   pipelineLayoutInfo.pSetLayouts = layouts.data();

   //   if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
   //   {
   //      throw std::runtime_error("Failed to create GLTF pipeline layout");
   //   }
   //}

   //void scene_render_system::createPipeline(VkRenderPass renderPass)
   //{
   //   assert(m_pipelineLayout != VK_NULL_HANDLE);

   //   auto vertSpv = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/scene_vert.vert.spv";
   //   auto fragSpv = std::string(PROJECT_ROOT_DIR) + "/res/shaders/spirV/scene_frag.frag.spv";

   //   std::vector<VkVertexInputBindingDescription> bindings = {
   //      vkinit::vertexInputBindingDescription(0, sizeof(vkglTF::Vertex), VK_VERTEX_INPUT_RATE_VERTEX)};

   //   std::vector<VkVertexInputAttributeDescription> attributes = {
   //      vkinit::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vkglTF::Vertex, pos)),
   //      vkinit::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vkglTF::Vertex, normal)),
   //      vkinit::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(vkglTF::Vertex, uv)),
   //      vkinit::vertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(vkglTF::Vertex, color)),
   //      vkinit::vertexInputAttributeDescription(0, 4, VK_FORMAT_R32G32B32A32_SFLOAT,
   //                                              offsetof(vkglTF::Vertex, tangent))};

   //   // OPAQUE
   //   PipelineConfigInfo opaqueConfig{};
   //   VkSandboxPipeline::defaultPipelineConfigInfo(opaqueConfig);
   //   opaqueConfig.pipelineLayout = m_pipelineLayout;
   //   opaqueConfig.renderPass = renderPass;
   //   opaqueConfig.bindingDescriptions = bindings;
   //   opaqueConfig.attributeDescriptions = attributes;

   //   m_opaquePipeline = std::make_unique<VkSandboxPipeline>(m_device, vertSpv, fragSpv, opaqueConfig);

   //   // MASK
   //   PipelineConfigInfo maskConfig{};
   //   VkSandboxPipeline::defaultPipelineConfigInfo(maskConfig);
   //   maskConfig.pipelineLayout = m_pipelineLayout;
   //   maskConfig.renderPass = renderPass;
   //   maskConfig.bindingDescriptions = bindings;
   //   maskConfig.attributeDescriptions = attributes;
   //   maskConfig.colorBlendAttachment.blendEnable = VK_FALSE;

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

   //   m_maskPipeline = std::make_unique<VkSandboxPipeline>(m_device, vertSpv, fragSpv, maskConfig);

   //   // BLEND
   //   PipelineConfigInfo blendConfig{};
   //   VkSandboxPipeline::defaultPipelineConfigInfo(blendConfig);
   //   blendConfig.pipelineLayout = m_pipelineLayout;
   //   blendConfig.renderPass = renderPass;
   //   blendConfig.bindingDescriptions = bindings;
   //   blendConfig.attributeDescriptions = attributes;

   //   blendConfig.colorBlendAttachment.blendEnable = VK_TRUE;
   //   blendConfig.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
   //   blendConfig.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   //   blendConfig.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
   //   blendConfig.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
   //   blendConfig.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
   //   blendConfig.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

   //   blendConfig.colorBlendAttachment.colorWriteMask =
   //      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

   //   m_blendPipeline = std::make_unique<VkSandboxPipeline>(m_device, vertSpv, fragSpv, blendConfig);
   //}


   void scene_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscenebase)
   {

      graphics3d::pbr_with_ibl_render_system::on_render(pgpucontext, pscenebase);

      //static bool warnedThisFrame = false;

      //auto pframe = ::gpu::current_frame();

      //auto pcommandbuffer = pframe->m_pgpucommandbuffer;

      //// pgpucontext->defer_bind(m_pshaderOpaque);

      //auto &scenerenderables = pscenebase->scene_renderables();

      //auto pcontext = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      //::gpu::shader *pshader = nullptr;

      //auto prenderer = pcontext->m_pgpurenderer;

      //for (auto &[id, pscenerenderable]: scenerenderables)
      //{

      //   if (!pscenerenderable)
      //   {

      //      debug("Hey, there is a null object named '{}' in scene renderables map.", id);

      //      continue;
      //   }

      //   if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_scene)
      //   {

      //      continue;
      //   }

      //   auto prenderable = pscenerenderable->renderable();

      //   if (!prenderable)
      //   {

      //      continue;
      //   }

      //   auto erenderabletype = prenderable->m_erenderabletype;

      //   if (erenderabletype != ::gpu::e_renderable_type_gltf)
      //   {

      //      continue;
      //   }

      //   ::cast<::gpu::gltf::model> pgltfmodel = prenderable;

      //   if (!pgltfmodel)
      //   {

      //      continue;
      //   }

      //   pgltfmodel->bind(pframe->m_pgpucommandbuffer);

      //   for (auto pmesh: pgltfmodel->m_mesha)
      //   {

      //      floating_matrix4 matrixObject = pscenerenderable->model_matrix();

      //      auto matrixNode = pmesh->uniformBlock.matrix;

      //      floating_matrix4 world = matrixObject * matrixNode;

      //      floating_matrix3 matrix3World(world);
      //      auto matrix3Normal = matrix3World.inversed().transposed();
      //      floating_matrix4 normalMat = matrix3Normal;
      //      bool bChangedShader = false;
      //      // Pick pipeline by alpha mode
      //      switch (pmesh->m_pmaterial->alphaMode)
      //      {
      //         case ::gpu::gltf::material::ALPHAMODE_OPAQUE:
      //            bChangedShader = pgpucontext->defer_bind(m_pshaderOpaque);
      //            m_pshaderOpaque->set_int("useAlphaMask", 0);
      //            break;
      //         case ::gpu::gltf::material::ALPHAMODE_MASK:
      //            bChangedShader = pgpucontext->defer_bind(m_pshaderMask);
      //            m_pshaderBlend->set_int("useAlphaMask", 1);
      //            break;
      //         case ::gpu::gltf::material::ALPHAMODE_BLEND:
      //         default:
      //            bChangedShader = pgpucontext->defer_bind(m_pshaderBlend);
      //            m_pshaderBlend->set_int("useAlphaMask", 0);
      //            break;
      //      }

      //      auto pshader = pgpucontext->m_pshaderBound;

      //      if (bChangedShader)
      //      {
      //         ::cast<::graphics3d::scene> pscene = pscenebase;
      //         //// xxxxxxxxxxxxxxxxx
      //         // auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      //         // auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);

      //         // pshader->bind_source2(TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP, "diffuseIrradianceMap",
      //         //                  pscene->m_pibldiffuseirradiancemap->m_pframebufferDiffuseIrradiance->m_ptexture);
      //         auto pirradiancemap = pscene->m_pibldiffuseirradiancemap;
      //         //               int iCubemapId = pirradiancemap->getCubemapId();

      //         pshader->bind_source2(pcommandbuffer, TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP, "diffuseIrradianceMap",
      //                               pirradiancemap->m_ptextureDiffuseIrradianceCubemap);
      //         // //  IBL stuff
      //         // glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
      //         // GLCheckError("");
      //         // pshader->set_int("diffuseIrradianceMap", TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
      //         // ::cast<::gpu_opengl::ibl::diffuse_irradiance_map> pirradiancemap = pscene->m_pibldiffuseirradiancemap;
      //         // int iCubemapId = pirradiancemap->getCubemapId();
      //         // glBindTexture(GL_TEXTURE_CUBE_MAP, iCubemapId);
      //         // GLCheckError("");

      //         // glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_PREFILTERED_ENV_MAP);
      //         // GLCheckError("");
      //         // pshader->set_int("prefilteredEnvMap", TEXTURE_UNIT_PREFILTERED_ENV_MAP);
      //         // ::cast<::gpu_opengl::ibl::specular_map> pspecularmap = pscene->m_piblspecularmap;
      //         // ::cast<::gpu_opengl::texture> pspecularmap = pscene->m_piblspecularmap;
      //         // int iPrefilteredEnvMapId = pspecularmap->m_pframebufferPrefilteredEnvMap->m_ptexture();
      //         // glBindTexture(GL_TEXTURE_CUBE_MAP, iPrefilteredEnvMapId);
      //         // GLCheckError("");

      //         //::cast<::gpu_opengl::texture> pspecularmap = pscene->m_piblspecularmap;
      //         auto pspecularmap = pscene->m_piblspecularmap;
      //         // int iPrefilteredEnvMapId = pspecularmap->m_pframebufferPrefilteredEnvMap->m_ptexture();

      //         pshader->bind_source2(pcommandbuffer, TEXTURE_UNIT_PREFILTERED_ENV_MAP, "prefilteredEnvMap",
      //                               pspecularmap->m_ptexturePrefilteredEnvMapCubemap);


      //         // glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
      //         // GLCheckError("");
      //         // pshader->set_int("brdfConvolutionMap", TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
      //         // int iBrdfConvolutionMapId = pspecularmap->getBrdfConvolutionMapId();
      //         // glBindTexture(GL_TEXTURE_2D, iBrdfConvolutionMapId);
      //         // GLCheckError("");

      //         pshader->bind_source2(pcommandbuffer, TEXTURE_UNIT_BRDF_CONVOLUTION_MAP, "brdfConvolutionMap",
      //                               pspecularmap->m_ptextureBrdfConvolutionMap);
      //      }

      //      pshader->set_matrix4("modelMatrix", world);
      //      pshader->set_matrix4("normalMatrix", normalMat);

      //      m_erendersystem = ::graphics3d::e_render_system_gltf_scene;
      //      pcommandbuffer->m_prendersystem = this;
      //      pmesh->draw(pcommandbuffer);
      //   }
      //}

      //if (pshader)
      //{
      //   pgpucontext->defer_unbind(pshader);
      //}



      ////static bool warnedThisFrame = false;

      ////for (auto &[id, go]: frame.gameObjects)
      ////{

      ////   if (go->getPreferredRenderTag() != RenderTag::Scene)
      ////   {
      ////      continue; // not mine, skip
      ////   }
      ////   auto baseModel = go->getModel();
      ////   if (!baseModel)
      ////      continue;

      ////   auto model = std::dynamic_pointer_cast<vkglTF::Model>(baseModel);
      ////   if (!model)
      ////      continue;

      ////   model->bind(frame.commandBuffer);

      ////   for (auto *node: model->m_linearNodes)
      ////   {
      ////      if (!node->mesh)
      ////         continue;

      ////      floating_matrix4 world = go->getTransform().floating_matrix4() * node->getMatrix();
      ////      floating_matrix4 normalMat = glm::transpose(glm::inverse(world));

      ////      memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
      ////      memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));

      ////      for (auto *prototype: node->mesh->prototypes)
      ////      {


      ////         std::array<VkDescriptorSet, 2> sets = {frame.globalDescriptorSet, // set 0
      ////                                                node->mesh->uniformBuffer.descriptorSet};


      ////         vkCmdBindDescriptorSets(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0,
      ////                                 static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);

      ////         switch (prototype->material.alphaMode)
      ////         {
      ////            case vkglTF::Material::ALPHAMODE_OPAQUE:
      ////               m_opaquePipeline->bind(frame.commandBuffer);
      ////               break;
      ////            case vkglTF::Material::ALPHAMODE_MASK:
      ////               m_maskPipeline->bind(frame.commandBuffer);
      ////               break;
      ////            case vkglTF::Material::ALPHAMODE_BLEND:
      ////            default:
      ////               m_blendPipeline->bind(frame.commandBuffer);
      ////               break;
      ////         }

      ////         model->drawNode(node, frame.commandBuffer, vkglTF::RenderFlags::BindImages, m_pipelineLayout, 2);
      ////         warnedThisFrame = false;
      ////      }
      ////   }
      ////}





















      // static bool warnedThisFrame = false;

      //            auto pframe = ::gpu::current_frame();

      //auto pcommandbuffer = pframe->m_pgpucommandbuffer;

      // auto &scenerenderables = pscenebase->scene_renderables();

      ////   //// xxxxxxxxxxxxxxxxx
      //auto pcontext = m_pengine->gpu_context();
      //auto prenderer = pcontext->m_pgpurenderer;

      //////// xxxxxxxxxxxxxxxxx
      //// auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      ////auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);

      ////auto pdescriptorset


      // for (auto &[id, pscenerenderable]: scenerenderables)
      //{

      //   if (!pscenerenderable)
      //   {

      //      debug("Hey, there is a null object named '{}' in scene objects map.", id);

      //      continue;

      //   }

      //   if (pscenerenderable->m_erendersystem != ::graphics3d::e_render_system_gltf_scene)
      //   {

      //      continue;

      //   }

      //           auto prenderable = pscenerenderable->renderable();
      //   if (!prenderable)
      //      continue;

      //   auto erenderabletype = prenderable->m_erenderabletype;

      //   if (erenderabletype != ::gpu::e_renderable_type_gltf)
      //   {
      //      continue; // not mine, skip
      //   }
      //   ::cast<::gpu::gltf::model> pgltfmodel = prenderable;

      //   if (!pgltfmodel)
      //      continue;

      //   pgltfmodel->bind(pframe->m_pgpucommandbuffer);

      //   for (auto *pmesh: pgltfmodel->m_mesha)
      //   {
      //      if (!pmesh)
      //         continue;

      //      auto matrixObject = pscenerenderable->model_matrix();

      //      auto matrixNode = pnode->getMatrix();

      //      floating_matrix4 world = matrixObject * matrixNode;

      //      // Extract upper-left 3×3 from world
      //      floating_matrix3 world3x3 = floating_matrix3(world);

      //      float det2 = floating_matrix3(world3x3).determinant();
      //      information("deteterminant of model matrix is %f\n", det2);

      //      auto normal3x3 = world3x3.inversed().transposed();

      //      floating_matrix4 normalMat(normal3x3);

      //      // Compute normal matrix correctly
      //      //normalMat = normalMat.inversed().transposed();

      //      //memcpy(pnode->m_pmesh->uniformBuffer.mapped, &world, sizeof(world));
      //      //memcpy((char *)pnode->m_pmesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));

      //      for (auto *primitive: pnode->m_pmesh->primitives)
      //      {


      //         //std::array<VkDescriptorSet, 2> sets = {vkdescriptorsetGlobal, // set 0
      //         //                                       pnode->m_pmesh->uniformBuffer.descriptorSet};


      //         //vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //         m_pipelineLayout, 0,
      //         //                        static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);

      //         //switch (primitive->m_pmaterial->alphaMode)
      //         //{
      //         //   case ::gpu_vulkan::gltf::Material::ALPHAMODE_OPAQUE:
      //         //      m_ppipelineOpaque->bind(pcommandbuffer);
      //         //      break;
      //         //   case ::gpu_vulkan::gltf::Material::ALPHAMODE_MASK:
      //         //      m_ppipelineMask->bind(pcommandbuffer);
      //         //      break;
      //         //   case ::gpu_vulkan::gltf::Material::ALPHAMODE_BLEND:
      //         //   default:
      //         //      m_ppipelineBlend->bind(pcommandbuffer);
      //         //      break;
      //         //}

      //         // Pick pipeline by alpha mode
      //         switch (primitive->m_pmaterial->alphaMode)
      //         {
      //            case ::gpu_vulkan::gltf::Material::ALPHAMODE_OPAQUE:
      //               pgpucontext->defer_bind(m_pshaderOpaque);
      //               break;
      //            case ::gpu_vulkan::gltf::Material::ALPHAMODE_MASK:
      //               pgpucontext->defer_bind(m_pshaderMask);
      //               break;
      //            case ::gpu_vulkan::gltf::Material::ALPHAMODE_BLEND:
      //            default:
      //               pgpucontext->defer_bind(m_pshaderBlend);
      //               break;
      //         }

      //         ::cast<::gpu_vulkan::shader> pshader = pgpucontext->m_pshaderBound;

      //         auto prendersystem = this;

      //         ::floating_sequence3 multiplier;

      //         if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_vulkan)
      //         {

      //            if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
      //            {

      //               multiplier.y = -1.f;
      //               multiplier.z = -1.f;
      //               // x_multiplier = -1.f;
      //            }
      //         }
      //         else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_z_minus)
      //         {

      //            if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
      //            {

      //               // y_multiplier = -1.f;
      //               multiplier.z = -1.f;
      //            }
      //         }
      //         else if (pgltfmodel->m_ecoordinatesystem == ::gpu::e_coordinate_system_y_up)
      //         {

      //            if (pgpucontext->m_eapi == ::gpu::e_api_vulkan)
      //            {

      //               multiplier.y = -1.f;
      //               multiplier.z = -1.f;
      //            }
      //         }

      //         pshader->set_sequence3("multiplier", multiplier);

      //         pshader->set_matrix4("modelMatrix", world);
      //         pshader->set_matrix4("normalMatrix", normalMat);
      //         bool bAlbedo = pgltfmodel->m_materiala[0].baseColorTexture.is_set();
      //         bAlbedo = bAlbedo && !m_bDisableAlbedo;
      //         pshader->set_int("useTextureAlbedo", bAlbedo ? 1 : 0);

      //         floating_sequence3 seq3Albedo = {};
      //         if (prendersystem->m_bForceDefaultAlbedo)
      //         {

      //            seq3Albedo = prendersystem->m_seq3DefaultAlbedo;
      //         }
      //         else
      //         {

      //            seq3Albedo = ::floating_sequence3(pgltfmodel->m_materiala[0].baseColorFactor.r,
      //                                              pgltfmodel->m_materiala[0].baseColorFactor.g,
      //                                              pgltfmodel->m_materiala[0].baseColorFactor.b);
      //         }

      //         pshader->set_sequence3("albedo", seq3Albedo);

      //         bool bMetallicRoughness = pgltfmodel->m_materiala[0].metallicRoughnessTexture.is_set();
      //         bMetallicRoughness = bMetallicRoughness && !m_bDisableMetallicRoughness;
      //         //pshader->set_int("useTextureMetallicRoughness", bMetallicRoughness ? 1 : 0);
      //         bool bNormal = pgltfmodel->m_materiala[0].normalTexture.is_set();


      //         float fMetallic = 0.0f;
      //         if (prendersystem->m_bForceDefaultMetallicFactor)
      //         {

      //            fMetallic = prendersystem->m_fDefaultMetallicFactor;
      //         }
      //         else
      //         {

      //            fMetallic = pgltfmodel->m_materiala[0].metallicFactor;
      //         }
      //         float fRoughness = 0.0f;
      //         if (prendersystem->m_bForceDefaultRoughnessFactor)
      //         {

      //            fRoughness = prendersystem->m_fDefaultRoughnessFactor;
      //         }
      //         else
      //         {

      //            fRoughness = pgltfmodel->m_materiala[0].roughnessFactor;
      //         }
      //         pshader->set_float("metallic", fMetallic);
      //         pshader->set_float("roughness", fRoughness);

      //         bNormal = bNormal && !m_bDisableNormal;
      //         pshader->set_int("useTextureNormal", bNormal ? 1 : 0);
      //         bool bAmbientOcclusion = pgltfmodel->m_materiala[0].occlusionTexture.is_set();
      //         bAmbientOcclusion = bAmbientOcclusion && !m_bDisableAmbientOcclusion;
      //         //pshader->set_int("useTextureAmbientOcclusion", bAmbientOcclusion ? 1 : 0);


      //         float fAmbientOcclusion = 0.0f;
      //         if (prendersystem->m_bForceDefaultAmbientOcclusionFactor)
      //         {

      //            fAmbientOcclusion = prendersystem->m_fDefaultAmbientOcclusionFactor;
      //         }
      //         else
      //         {

      //            // fAmbientOcclusion = pgltfmodel->m_materiala[0].occlusionTexture->m_fAmbientOcclusion;
      //            fAmbientOcclusion = 1.f;
      //         }
      //         pshader->set_float("ambientOcclusion", fAmbientOcclusion);

      //         //floating_sequence3 seq3Emission = {};
      //         //if (prendersystem->m_bForceDefaultEmission)
      //         //{

      //         //   seq3Emission = prendersystem->m_seq3DefaultEmission;
      //         //}
      //         //else
      //         //{

      //         //   // seq3Emission = pgltfmodel->m_materiala[0].m_seq3Emissive;
      //         //}
      //         //pshader->set_sequence3("emissive", seq3Emission);


      //         ////bool bEmissive = pgltfmodel->m_materiala[0].emissiveTexture.is_set();
      //         ////bEmissive = bEmissive && !m_bDisableEmissive;
      //         //pshader->set_int("useTextureEmissive", bEmissive ? 1 : 0);
      //         pshader->push_properties(pcommandbuffer);


      //         //// --- Bind sets 0 & 1 (global + node UBO) ---
      //         // std::array<VkDescriptorSet, 2> sets01 = {
      //         //    frame.globalDescriptorSet, // set 0
      //         //    pnode->m_pmesh->uniformBuffer.descriptorSet // set 1
      //         // };
      //         //// xxxxxxxxxxxxxxxxx
      //         ////// --- Bind sets 0 & 1 (global + pnode UBO) ---
      //         // std::array<VkDescriptorSet, 2> sets01 = {
      //         //   vkdescriptorsetGlobal, // set 0
      //         //    pnode->m_pmesh->uniformBuffer.descriptorSet // set 1
      //         // };
      //         // vkCmdBindDescriptorSets(
      //         //   pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //         //   m_pipelineLayout, 0,
      //         //   (uint32_t)sets01.size(), sets01.data(), 0, nullptr);
      //         //// --- Bind sets 0 (global) ---
      //         // pnode UBO transformed in Push Constants and set above
      //         //std::array<VkDescriptorSet, 1> sets01 = {
      //           // vkdescriptorsetGlobal // set 0
      //         //};
      //         //vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //           //                      pshader->m_ppipeline->m_vkpipelinelayout, 0, (uint32_t)sets01.size(),
      //             //                    sets01.data(), 0, nullptr);

      //         //// --- Bind our PBR set (set = 2) ---
      //         // --- Bind our PBR set (set = 1) ---
      //         if (pgltfmodel->m_materiala.size() <= 0)
      //         {

      //            if (!warnedThisFrame)
      //            { /*spdlog::warn("PBR set null");*/
      //               warnedThisFrame = true;
      //            }
      //            continue;
      //         }


      //         ////// --- Bind IBL set (set = 3) ---
      //         //// --- Bind IBL set (set = 2) ---
      //         //VkDescriptorSet iblSet =
      //         //   m_vkdescriptorsetaIbl[pcontext->m_pgpurenderer->m_pgpurendertarget->get_frame_index()];
      //         //if (iblSet == VK_NULL_HANDLE)
      //         //{
      //         //   continue;
      //         //}
      //         //vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //         //                        // m_pipelineLayout, 3, 1,
      //         //                        pshader->m_ppipeline->m_vkpipelinelayout, 1, 1, &iblSet, 0, nullptr);

      //         //if (pgltfmodel->m_materiala[0].descriptor_set_array_scene_gltf(pgltfmodel).has_element())
      //         //{
      //         //   VkDescriptorSet pbrSet = pgltfmodel->m_materiala[0].descriptor_set_array_scene_gltf(
      //         //      pgltfmodel)[pframe->m_pgpucommandbuffer->m_iFrameIndex];
      //         //   if (pbrSet == VK_NULL_HANDLE)
      //         //   {
      //         //      if (!warnedThisFrame)
      //         //      { /*spdlog::warn("PBR set null");*/
      //         //         warnedThisFrame = true;
      //         //      }
      //         //      continue;
      //         //   }
      //         //   vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //         //                           // m_pipelineLayout, 2, 1,
      //         //                           pshader->m_ppipeline->m_vkpipelinelayout, 2, 1, &pbrSet, 0, nullptr);
      //         //}

      //         auto pgpubindingset = pcontext->scene_gltf_pbr_binding_set();

      //         auto pbindingset = pshader->binding_set(2, pgpubindingset);

      //         pgltfmodel->gltfDraw(pcommandbuffer,0);

      //         warnedThisFrame = false;

      //      }
      //   }
      //}

   }


} // namespace graphics3d
 





BEGIN_GPU_PROPERTIES(::graphics3d::scene_render_system::push_constants)
GPU_PROPERTY("modelMatrix", ::gpu::e_type_mat4)
GPU_PROPERTY("normalMatrix", ::gpu::e_type_mat4)
GPU_PROPERTY("useTextureAlbedo", ::gpu::e_type_int)
GPU_PROPERTY("useTextureNormal", ::gpu::e_type_int)
// GPU_PROPERTY("useTextureMetallicRoughness", ::gpu::e_type_int)
// GPU_PROPERTY("useTextureAmbientOcclusion", ::gpu::e_type_int)
// GPU_PROPERTY("useTextureEmissive", ::gpu::e_type_int)
GPU_PROPERTY("albedo", ::gpu::e_type_seq3)
GPU_PROPERTY("metallic", ::gpu::e_type_float)
GPU_PROPERTY("roughness", ::gpu::e_type_float)
GPU_PROPERTY("ambientOcclusion", ::gpu::e_type_float)
GPU_PROPERTY("emissive", ::gpu::e_type_seq3)
// GPU_PROPERTY("cameraPosition", ::gpu::e_type_seq3)
GPU_PROPERTY("bloomBrightnessCutoff", ::gpu::e_type_float)
// GPU_PROPERTY("useAlphaMask", ::gpu::e_type_int)
// GPU_PROPERTY("alphaMaskCutOff", ::gpu::e_type_float)
GPU_PROPERTY("multiplier", ::gpu::e_type_seq3)
END_GPU_PROPERTIES()
