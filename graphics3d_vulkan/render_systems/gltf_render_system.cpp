// From V0idsEmbrace.gltf_render_system by
// camilo on 2025-04-24 21:28 <3ThomasBorregaardSorensen!!
#include "framework.h"
// #include "bred/gpu/gltf_model.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "app-graphics3d/gpu_vulkan/command_buffer.h"
#include "app-graphics3d/gpu_vulkan/descriptors.h"
#include "app-graphics3d/gpu_vulkan/gltf_model.h"
#include "app-graphics3d/gpu_vulkan/pipeline.h"
#include "app-graphics3d/gpu_vulkan/render_pass.h"
#include "app-graphics3d/gpu_vulkan/render_target.h"
#include "app-graphics3d/gpu_vulkan/renderer.h"
#include "app-graphics3d/gpu_vulkan/texture.h"
#include "app-graphics3d/gpu_vulkan/vk_init.h"
#include "bred/gpu/frame.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene.h"
#include "gltf_render_system.h"
// #include "graphics3d/_.h"
// #include <stdexcept>


namespace graphics3d_vulkan
{

   //
   //   // gltf_render_system::gltf_render_system(
   //   //     graphics3d::device * pdevice,
   //   //     VkRenderPass renderPass,
   //   //     VkDescriptorSetLayout globalSetLayout,
   //   //     IAssetProvider& assets
   //   // ) :
   //   //     m_pgpudevice(pdevice),
   //   //     m_pdescriptorsetlayoutUbo(globalSetLayout),
   //   //     m_assets(assets)
   //   // {
   //   //
   //   // }
   //
   //   gltf_render_system::gltf_render_system() {}
   //
   //   gltf_render_system::~gltf_render_system()
   //   {
   //      // vkDestroyPipelineLayout(m_pgpudevice->device(), m_pipelineLayout, nullptr);
   //   }
   //
   //
   //   // void gltf_render_system::init(
   //   //    graphics3d::device * pdevice,
   //   //     VkRenderPass renderPass,
   //   //     VkDescriptorSetLayout globalSetLayout,
   //   //    graphics3d::sandbox_descriptor_pool& descriptorPool,
   //   //     size_t frameCount
   //   // ) {
   //   //     m_pdescriptorsetlayoutUbo = globalSetLayout;
   //   //
   //   //     m_pdescriptorsetlayoutIbl = graphics3d::sandbox_descriptor_set_layout::Builder{ pdevice }
   //   //     .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //   //     .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //   //     //.addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
   //   //     .build();
   //   //
   //   //
   //   //
   //   //     createPipelineLayout(globalSetLayout);
   //   //     createPipeline(renderPass);
   //   //
   //   //     // --- 2) Allocate & write per‐frame IBL descriptor sets ---
   //   //     m_vkdescriptorsetaIbl.resize(frameCount);
   //   //     for (uint32_t i = 0; i < frameCount; i++) {
   //   //         VkDescriptorSet set;
   //   //         descriptorPool.allocateDescriptor(
   //   //             m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(),
   //   //             set,
   //   //             /*setIndex=*/0
   //   //         );
   //   //         // grab descriptors straight from the provider:
   //   //         auto brdfInfo = m_assets.getBRDFLUTDescriptor();
   //   //         auto irradianceInfo = m_assets.getIrradianceDescriptor();
   //   //         // auto prefilterInfo = m_assets.getPrefilteredDescriptor();
   //   //
   //   //         graphics3d::sandbox_descriptor_writer(*m_pdescriptorsetlayoutIbl, descriptorPool)
   //   //             .writeImage(0, &brdfInfo)
   //   //             .writeImage(1, &irradianceInfo)
   //   //           //  .writeImage(2, &prefilterInfo)
   //   //             .build(set);
   //   //
   //   //         m_vkdescriptorsetaIbl[i] = set;
   //   //     }
   //   //
   //   //
   //   // }
   //
   //   void gltf_render_system::on_prepare(::gpu::context *pgpucontext)
   //   {
   //
   //
   //      m_pdescriptorsetlayoutUbo;
   //      VkDescriptorSetLayout m_iblSetLayout;
   //      VkDescriptorSet m_iblDescriptorSet;
   //
   //      auto prenderer = pgpucontext->m_pgpurenderer;
   //
   //      m_pshader = prenderer->create_shader(
   //         "matter://shaders/vert.vert", "matter://shaders/frag.frag",
   //         {::gpu::shader::e_descriptor_set_slot_global, ::gpu::shader::e_descriptor_set_slot_local}, {},
   //         simple_render_properties(), pgpucontext->input_layout<::graphics3d::Vertex>()
   //
   //      );
   //
   //      // m_pshader->m_bClearColor = true;
   //      // m_pshader->m_colorClear = argb(0.8f, 0.1f, 0.5f, 0.1f);
   //   }
   //
   //
   //   // void gltf_render_system::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
   //   //     const ::array_base<VkDescriptorSetLayout> layouts = {
   //   //         globalSetLayout,
   //   //         graphics3d::gltf::descriptorSetLayoutUbo,
   //   //         graphics3d::gltf::descriptorSetLayoutImage,
   //   //         m_pdescriptorsetlayoutIbl->getDescriptorSetLayout()
   //   //     };
   //   //
   //   //     VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
   //   //     pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   //   //     pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
   //   //     pipelineLayoutInfo.pSetLayouts = layouts.data();
   //   //
   //   //     if (vkCreatePipelineLayout(m_pgpudevice->device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) !=
   //   //     VK_SUCCESS) {
   //   //         throw std::runtime_error("Failed to create GLTF pipeline layout");
   //   //     }
   //   // }
   //
   //   // void gltf_render_system::createPipeline(VkRenderPass renderPass) {
   //   //     ASSERT(m_pipelineLayout != VK_NULL_HANDLE);
   //   //
   //   //     auto vertSpv = "matter://shaders/spirV/gltf_vert.vert.spv";
   //   //     auto fragSpv = "matter://shaders/spirV/gltf_frag.frag.spv";
   //   //
   //   //     ::array_base<VkVertexInputBindingDescription> bindings = {
   //   //         vkinit::vertexInputBindingDescription(0, sizeof(graphics3d::gltf::Vertex),
   //   VK_VERTEX_INPUT_RATE_VERTEX)
   //   //     };
   //   //
   //   //     ::array_base<VkVertexInputAttributeDescription> attributes = {
   //   //         vkinit::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT,
   //   //         offsetof(graphics3d::gltf::Vertex, pos)), vkinit::vertexInputAttributeDescription(0, 1,
   //   //         VK_FORMAT_R32G32B32_SFLOAT, offsetof(graphics3d::gltf::Vertex, normal)),
   //   //         vkinit::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT,
   //   offsetof(graphics3d::gltf::Vertex,
   //   //         uv)), vkinit::vertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT,
   //   //         offsetof(graphics3d::gltf::Vertex, color)), vkinit::vertexInputAttributeDescription(0, 4,
   //   //         VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(graphics3d::gltf::Vertex, tangent))
   //   //     };
   //   //
   //   //     // OPAQUE
   //   //     // graphics3d::pipeline_configuration_information opaqueConfig{};
   //   //     // graphics3d::pipeline::default_pipeline_configuration(opaqueConfig);
   //   //     // opaqueConfig.pipelineLayout = m_pipelineLayout;
   //   //     // opaqueConfig.renderPass = renderPass;
   //   //     // opaqueConfig.bindingDescriptions = bindings;
   //   //     // opaqueConfig.attributeDescriptions = attributes;
   //   //
   //   //     m_opaquePipeline = øcreate_pointer<graphics3d::pipeline>(
   //   //         //m_pgpudevice, vertSpv, fragSpv, opaqueConfig);
   //   //         m_pgpudevice, vertSpv, fragSpv);
   //   //
   //   //     // MASK
   //   //     // graphics3d::pipeline_configuration_information maskConfig{};
   //   //     // graphics3d::pipeline::default_pipeline_configuration(maskConfig);
   //   //     // maskConfig.pipelineLayout = m_pipelineLayout;
   //   //     // maskConfig.renderPass = renderPass;
   //   //     // maskConfig.bindingDescriptions = bindings;
   //   //     // maskConfig.attributeDescriptions = attributes;
   //   //     // maskConfig.colorBlendAttachment.blendEnable = VK_FALSE;
   //   //
   //   //     // struct SpecData { VkBool32 alphaMask; float cutoff; };
   //   //     // static SpecData specData{ VK_TRUE, 0.5f };
   //   //     // static VkSpecializationMapEntry mapEntries[2] = {
   //   //     //     { 0, offsetof(SpecData, alphaMask), sizeof(VkBool32) },
   //   //     //     { 1, offsetof(SpecData, cutoff),    sizeof(float) }
   //   //     // };
   //   //     // static VkSpecializationInfo specInfo{};
   //   //     // specInfo.mapEntryCount = 2;
   //   //     // specInfo.pMapEntries = mapEntries;
   //   //     // specInfo.dataSize = sizeof(specData);
   //   //     // specInfo.pData = &specData;
   //   //     //
   //   //     // maskConfig.fragSpecInfo = &specInfo;
   //   //
   //   //     m_maskPipeline = øcreate_pointer<graphics3d::pipeline>(
   //   //         //m_pgpudevice, vertSpv, fragSpv, maskConfig);
   //   //         m_pgpudevice, vertSpv, fragSpv);
   //   //
   //   //     // // BLEND
   //   //     // graphics3d::pipeline_configuration_information blendConfig{};
   //   //     // graphics3d::pipeline::default_pipeline_configuration(blendConfig);
   //   //     // blendConfig.pipelineLayout = m_pipelineLayout;
   //   //     // blendConfig.renderPass = renderPass;
   //   //     // blendConfig.bindingDescriptions = bindings;
   //   //     // blendConfig.attributeDescriptions = attributes;
   //   //     //
   //   //     // blendConfig.colorBlendAttachment.blendEnable = VK_TRUE;
   //   //     // blendConfig.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
   //   //     // blendConfig.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   //   //     // blendConfig.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
   //   //     // blendConfig.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
   //   //     // blendConfig.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
   //   //     // blendConfig.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
   //   //     //
   //   //     // blendConfig.colorBlendAttachment.colorWriteMask =
   //   //     //     VK_COLOR_COMPONENT_R_BIT |
   //   //     //     VK_COLOR_COMPONENT_G_BIT |
   //   //     //     VK_COLOR_COMPONENT_B_BIT |
   //   //     //     VK_COLOR_COMPONENT_A_BIT;
   //   //
   //   //     m_blendPipeline = øcreate_pointer<graphics3d::pipeline>(
   //   //     m_pgpudevice, vertSpv, fragSpv);
   //   //     //m_pgpudevice, vertSpv, fragSpv, blendConfig);
   //   // }
   //
   //
   //   void gltf_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene *pscene)
   //   {
   //
   //      // vkCmdBindDescriptorSets(
   //      //     frame.m_pcommandbuffer,
   //      //     VK_PIPELINE_BIND_POINT_GRAPHICS,
   //      //     m_pipelineLayout,
   //      //     0, 1,
   //      //     &frame.globalDescriptorSet,
   //      //     0, nullptr);
   //
   //      auto pframe = ::gpu::current_frame();
   //
   //      auto &sceneobjects = pscene->scene_objects();
   //
   //      for (auto &[id, pgameobject]: sceneobjects)
   //      {
   //
   //         auto prenderable = pgameobject->renderable();
   //
   //         if (!prenderable || prenderable->m_erenderabletype != ::gpu::e_renderabled_type_gltf)
   //) continue;
   //
   //         ::cast<::gpu::gltf::Model> model = baseModel;
   //
   //         if (!prenderable)
   //            continue;
   //
   //         prenderable->bind(pframe->m_pgpucommandbuffer);
   //
   //         for (auto *node: prenderable->m_linearNodes)
   //         {
   //
   //            if (!node->mesh)
   //               continue;
   //
   //            glm::mat4 world = pgameobject->getTransform().mat4() * node->getMatrix();
   //            glm::mat4 normalMat = glm::transpose(glm::inverse(world));
   //            memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
   //            memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
   //
   //            vkCmdBindDescriptorSets(frame.m_pcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 1,
   //            1,
   //                                    &node->mesh->uniformBuffer.descriptorSet, 0, nullptr);
   //
   //            const auto &mat = node->mesh->primitives[0]->material;
   //            switch (mat.alphaMode)
   //            {
   //               case ::graphics3d::gltf::Material::ALPHAMODE_OPAQUE:
   //                  m_opaquePipeline->bind(frame.m_pcommandbuffer);
   //                  break;
   //               case ::graphics3d::gltf::Material::ALPHAMODE_MASK:
   //                  m_maskPipeline->bind(frame.m_pcommandbuffer);
   //                  break;
   //               case ::graphics3d::gltf::Material::ALPHAMODE_BLEND:
   //               default:
   //                  m_blendPipeline->bind(frame.m_pcommandbuffer);
   //                  break;
   //            }
   //
   //
   //            model->drawNode(node, frame.m_pcommandbuffer, ::graphics3d::gltf::RenderFlags::BindImages,
   //            m_pipelineLayout,
   //                            2 // bindImageSet
   //            );
   //         }
   //      }
   //   }
   //} // namespace graphics3d
   //
   //
   //
   //
   //
   // #include"vulkan_wrapper/render_systems/gltf_render_system.h"
   // #include <spdlog/spdlog.h>
   //
   //


   gltf_render_system::gltf_render_system() {}


   gltf_render_system::~gltf_render_system()
   {

      //   vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
   }


   // void gltf_render_system::initialize_GltfRenderSystem(

   //   ::gpu_vulkan::descriptor_set_layout *psetdescriptorlayoutGlobal)
   //{
   //   //m_device = device;

   //   m_pdescriptorsetlayoutUbo = psetdescriptorlayoutGlobal;


   //   //m_assets = assets;
   //}


   // void gltf_render_system::init(::gpu_vulkan::descriptor_set_layout *psetdescriptorlayoutGlobal,
   //::pointer<::gpu_vulkan::descriptor_set_layout> &descriptorPool, size_t frameCount)
   void gltf_render_system::on_prepare(::gpu::context *pgpucontext)
   {
      // m_pdescriptorsetlayoutUbo = globalSetLayout;

      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();

      m_pdescriptorsetlayoutIbl = ::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
                       .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                       .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                       .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                       .build();

      m_pdescriptorsetlayoutPbr =
         ::gpu_vulkan::descriptor_set_layout::Builder(pcontext)
            .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // albedo
            .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // normal
            .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // metallic
            .addBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // roughness
            .addBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // ao
            .build();

      createPipelineLayout(pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout());

      auto prenderpass = render_pass2();

      createPipeline(prenderpass->m_vkrenderpass);

      ::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;

      ::cast<::gpu_vulkan::render_target> prendertarget = prenderer->m_pgpurendertarget;

      auto frameCount = prendertarget->get_frame_count();

      m_vkdescriptorsetaIbl.resize(frameCount);

      auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

      pdescriptorpoolbuilder->initialize_builder(pcontext);
      pdescriptorpoolbuilder->setMaxSets(frameCount * 10);
      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, frameCount * 100);

      m_pdescriptorpool = pdescriptorpoolbuilder->build();


      auto passetmanager = m_pengine->m_pimmersionlayer->m_passetmanager;


      for (uint32_t i = 0; i < frameCount; i++)
      {

         VkDescriptorSet set;

         // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set,
         //                                 /*setIndex=*/0);
         m_pdescriptorpool->allocateDescriptor(
            m_pdescriptorsetlayoutIbl->getDescriptorSetLayout(), set, 0);

         ::cast<::gpu_vulkan::texture> ptextureBrdf = passetmanager->m_ptextureLuBrdf;
         ::cast<::gpu_vulkan::texture> ptextureIrrad = passetmanager->m_ptextureIrradianceCube;
         ::cast<::gpu_vulkan::texture> ptexturePrefltr = passetmanager->m_ptexturePrefilteredCube;
         // auto irradianceInfo = m_assets.getIrradianceDescriptor();
         // auto prefilterInfo = m_assets.getPrefilteredDescriptor();

         auto brdfInfo = ptextureBrdf->m_descriptor3;
         auto irradianceInfo = ptextureIrrad->m_descriptor3;
         auto prefilterInfo = ptexturePrefltr->m_descriptor3;

         ::gpu_vulkan::descriptor_writer(*m_pdescriptorsetlayoutIbl, *m_pdescriptorpool)
            .writeImage(0, &brdfInfo)
            .writeImage(1, &irradianceInfo)
            .writeImage(2, &prefilterInfo)
            .build(set);

         m_vkdescriptorsetaIbl[i] = set;
      }

      m_vkdescriptorsetaPbr.resize(frameCount);
      for (uint32_t i = 0; i < frameCount; i++)
      {
         VkDescriptorSet set;
         // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutPbr->getDescriptorSetLayout(), set, /*setIndex=*/0);
         // m_pdescriptorpool->allocateDescriptor(m_pdescriptorsetlayoutPbr->getDescriptorSetLayout(), set);

         // auto logDescriptor = [&](const char *name, const VkDescriptorImageInfo &info)
         //{
         //    information("{} - sampler: {}, imageView: {}, layout: {}", name, (uint64_t)info.sampler,
         //                 (uint64_t)info.imageView, (int)info.imageLayout);
         // };

         if (1)
         {

            ::cast<::gpu_vulkan::texture> ptextureAlbedo = passetmanager->getTexture("cerberus_albedo");
            ::cast<::gpu_vulkan::texture> ptextureNormal = passetmanager->getTexture("cerberus_normal");
            ::cast<::gpu_vulkan::texture> ptextureMetallic = passetmanager->getTexture("cerberus_metallic");
            ::cast<::gpu_vulkan::texture> ptextureRoughness = passetmanager->getTexture("cerberus_roughness");
            ::cast<::gpu_vulkan::texture> ptextureAo = passetmanager->getTexture("cerberus_ao");


            // VkDescriptorImageInfo albedoInfo = m_assets.getTextureDescriptor("cerberus_albedo");
            // VkDescriptorImageInfo normalInfo = m_assets.getTextureDescriptor("cerberus_normal");
            // VkDescriptorImageInfo metallicInfo = m_assets.getTextureDescriptor("cerberus_metallic");
            // VkDescriptorImageInfo roughnessInfo = m_assets.getTextureDescriptor("cerberus_roughness");
            // VkDescriptorImageInfo aoInfo = m_assets.getTextureDescriptor("cerberus_ao");

            VkDescriptorImageInfo albedoInfo = ptextureAlbedo->m_descriptor3;
            VkDescriptorImageInfo normalInfo = ptextureNormal->m_descriptor3;
            VkDescriptorImageInfo metallicInfo = ptextureMetallic->m_descriptor3;
            VkDescriptorImageInfo roughnessInfo = ptextureRoughness->m_descriptor3;
            VkDescriptorImageInfo aoInfo = ptextureAo->m_descriptor3;

            // logDescriptor("albedo", albedoInfo);
            // logDescriptor("normal", normalInfo);
            // logDescriptor("metallic", metallicInfo);
            // logDescriptor("roughness", roughnessInfo);
            // logDescriptor("ao", aoInfo);

            ::gpu_vulkan::descriptor_writer(*m_pdescriptorsetlayoutPbr, *m_pdescriptorpool)
               .writeImage(0, &albedoInfo)
               .writeImage(1, &normalInfo)
               .writeImage(2, &metallicInfo)
               .writeImage(3, &roughnessInfo)
               .writeImage(4, &aoInfo)
               .build(set);

            m_vkdescriptorsetaPbr[i] = set;
         }
      }
   }


   void gltf_render_system::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
   {

      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();

      auto descriptorsetlayout0 = globalSetLayout;
      auto descriptorsetlayout1 = ::gpu_vulkan::gltf::ubo_descriptor_set_layout();
      auto descriptorsetlayout2 = m_pdescriptorsetlayoutPbr->getDescriptorSetLayout();
      auto descriptorsetlayout3 = m_pdescriptorsetlayoutIbl->getDescriptorSetLayout();

      const std::vector<VkDescriptorSetLayout> layouts = {
         descriptorsetlayout0,
         descriptorsetlayout1,
         descriptorsetlayout2,
         descriptorsetlayout3,

      };


      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
      pipelineLayoutInfo.pSetLayouts = layouts.data();


      if (vkCreatePipelineLayout(pcontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) !=
          VK_SUCCESS)
      {

         throw ::exception(error_failed, "Failed to create GLTF pipeline layout");
      }
   }


   void gltf_render_system::createPipeline(VkRenderPass renderPass)
   {

      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();
      ::cast<::gpu_vulkan::device> pgpudevice = pcontext->m_pgpudevice;

      assert(m_pipelineLayout != VK_NULL_HANDLE);

      auto vertSpv = "matter://shaders/gltf_vert.vert";
      auto fragSpv = "matter://shaders/gltf_frag.frag";

      ::memory vert;
      ::memory frag;

      pgpudevice->defer_shader_memory(vert, vertSpv);
      pgpudevice->defer_shader_memory(frag, fragSpv);

      std::vector<VkVertexInputBindingDescription> bindings = {
         vkinit::vertexInputBindingDescription(0, sizeof(::gpu_vulkan::gltf::Vertex), VK_VERTEX_INPUT_RATE_VERTEX)};

      std::vector<VkVertexInputAttributeDescription> attributes = {
         vkinit::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT,
                                                 offsetof(::gpu_vulkan::gltf::Vertex, pos)),
         vkinit::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT,
                                                 offsetof(::gpu_vulkan::gltf::Vertex, normal)),
         vkinit::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT,
                                                 offsetof(::gpu_vulkan::gltf::Vertex, uv)),
         vkinit::vertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT,
                                                 offsetof(::gpu_vulkan::gltf::Vertex, color)),
         vkinit::vertexInputAttributeDescription(0, 4, VK_FORMAT_R32G32B32A32_SFLOAT,
                                                 offsetof(::gpu_vulkan::gltf::Vertex, tangent))};

      // OPAQUE
      ::vulkan::pipeline_configuration opaqueConfig{};
      ::vulkan::defaultPipelineConfigInfo2(opaqueConfig);

      opaqueConfig.pipelineLayout = m_pipelineLayout;
      opaqueConfig.renderPass = renderPass;
      opaqueConfig.bindingDescriptions = bindings;
      opaqueConfig.attributeDescriptions = attributes;

      m_opaquePipeline = øcreate_new<::gpu_vulkan::pipeline>();

      m_opaquePipeline->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag, opaqueConfig);

      // MASK
      ::vulkan::pipeline_configuration maskConfig{};
      ::vulkan::defaultPipelineConfigInfo2(maskConfig);
      maskConfig.pipelineLayout = m_pipelineLayout;
      maskConfig.renderPass = renderPass;
      maskConfig.bindingDescriptions = bindings;
      maskConfig.attributeDescriptions = attributes;
      maskConfig.colorBlendAttachments[0].blendEnable = VK_FALSE;

      struct SpecData
      {
         VkBool32 alphaMask;
         float cutoff;
      };
      static SpecData specData{VK_TRUE, 0.5f};
      static VkSpecializationMapEntry mapEntries[2] = {{0, offsetof(SpecData, alphaMask), sizeof(VkBool32)},
                                                       {1, offsetof(SpecData, cutoff), sizeof(float)}};
      static VkSpecializationInfo specInfo{};
      specInfo.mapEntryCount = 2;
      specInfo.pMapEntries = mapEntries;
      specInfo.dataSize = sizeof(specData);
      specInfo.pData = &specData;

      maskConfig.fragSpecInfo = &specInfo;

      // m_maskPipeline = std::make_unique<VkSandboxPipeline>(m_device, vertSpv, fragSpv, maskConfig);
      m_maskPipeline = øcreate_new<::gpu_vulkan::pipeline>();

      m_maskPipeline->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag, maskConfig);

      // BLEND
      ::vulkan::pipeline_configuration blendConfig{};
      ::vulkan::defaultPipelineConfigInfo2(blendConfig);
      blendConfig.pipelineLayout = m_pipelineLayout;
      blendConfig.renderPass = renderPass;
      blendConfig.bindingDescriptions = bindings;
      blendConfig.attributeDescriptions = attributes;

      blendConfig.colorBlendAttachments[0].blendEnable = VK_TRUE;
      blendConfig.colorBlendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
      blendConfig.colorBlendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
      blendConfig.colorBlendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD;
      blendConfig.colorBlendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
      blendConfig.colorBlendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
      blendConfig.colorBlendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD;

      blendConfig.colorBlendAttachments[0].colorWriteMask =
         VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

      // m_blendPipeline = std::make_unique<VkSandboxPipeline>(m_device, vertSpv, fragSpv, blendConfig);
      m_blendPipeline = øcreate_new<::gpu_vulkan::pipeline>();

      m_blendPipeline->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag, blendConfig);
   }

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
   //          glm::mat4 world = go->getTransform().mat4() * node->getMatrix();
   //          glm::mat4 normalMat = glm::transpose(glm::inverse(world));
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


   //void gltf_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene *pscene)
   //{

   //   // vkCmdBindDescriptorSets(
   //   //     frame.m_pcommandbuffer,
   //   //     VK_PIPELINE_BIND_POINT_GRAPHICS,
   //   //     m_pipelineLayout,
   //   //     0, 1,
   //   //     &frame.globalDescriptorSet,
   //   //     0, nullptr);

   //   auto pframe = ::gpu::current_frame();

   //   ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

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

   //      ::cast<::gpu_vulkan::gltf::Model> pmodel = prenderable;

   //      if (!pmodel)
   //         continue;

   //      pmodel->bind(pframe->m_pgpucommandbuffer);

   //      for (auto *node: pmodel->m_linearNodes)
   //      {

   //         if (!node->mesh)
   //            continue;

   //         glm::mat4 world = psceneobject->transform().getMatrix() * node->getMatrix();
   //         glm::mat4 normalMat = glm::transpose(glm::inverse(world));
   //         memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
   //         memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));

   //         vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
   //                                 m_pipelineLayout, 1, 1, &node->mesh->uniformBuffer.descriptorSet, 0, nullptr);

   //         auto pmaterial = node->mesh->primitives[0]->m_pmaterial;
   //         switch (pmaterial->alphaMode)
   //         {
   //            case ::gpu_vulkan::gltf::Material::ALPHAMODE_OPAQUE:
   //               m_opaquePipeline->bind(pcommandbuffer);
   //               break;
   //            case ::gpu_vulkan::gltf::Material::ALPHAMODE_MASK:
   //               m_maskPipeline->bind(pcommandbuffer);
   //               break;
   //            case ::gpu_vulkan::gltf::Material::ALPHAMODE_BLEND:
   //            default:
   //               m_blendPipeline->bind(pcommandbuffer);
   //               break;
   //         }


   //         pmodel->gltfDraw(pcommandbuffer->m_vkcommandbuffer, ::gpu_vulkan::gltf::RenderFlags::RenderNone,
   //                          m_pipelineLayout, 2);
   //         // pmodel->drawNode(node,
   //         //  pcommandbuffer->m_vkcommandbuffer,
   //         //::gpu_vulkan::gltf::RenderFlags::BindImages,
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
//            glm::mat4 world = go->getTransform().mat4() * node->getMatrix();
//            glm::mat4 normalMat = glm::transpose(glm::inverse(world));
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

void gltf_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene *pscene)
   {
      static bool warnedThisFrame = false;

            auto pframe = ::gpu::current_frame();

         ::cast<::gpu_vulkan::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

         auto &sceneobjects = pscene->scene_objects();

      //   //// xxxxxxxxxxxxxxxxx
      ::cast<::gpu_vulkan::context> pcontext = m_pengine->gpu_context();
         ::cast<::gpu_vulkan::renderer> prenderer = pcontext->m_pgpurenderer;

      ////// xxxxxxxxxxxxxxxxx
      //auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
         auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);


      for (auto &[id, psceneobject]: sceneobjects)
      {

         if (!psceneobject)
         {

            continue;

         }

         if (psceneobject->m_erendersystem != ::graphics3d::e_render_system_gltf)
         {

            continue;
         }


         auto prenderable = psceneobject->renderable();
         if (!prenderable)
            continue;

         auto erenderabletype = prenderable->m_erenderabletype;

         if (erenderabletype != ::gpu::e_renderable_type_gltf)
         {
            continue; // not mine, skip
         }
         ::cast<::gpu_vulkan::gltf::Model> pgltfmodel = prenderable;

         if (!pgltfmodel)
            continue;

         pgltfmodel->bind(pframe->m_pgpucommandbuffer);

         for (auto *node: pgltfmodel->m_linearNodes)
         {
            if (!node->mesh)
               continue;

            glm::mat4 world = psceneobject->transform().getMatrix() * node->getMatrix();
            glm::mat4 normalMat = glm::transpose(glm::inverse(world));

            memcpy(node->mesh->uniformBuffer.mapped, &world, sizeof(world));
            memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));

            for (auto *primitive: node->mesh->primitives)
            {
               //// --- Bind sets 0 & 1 (global + node UBO) ---
               //std::array<VkDescriptorSet, 2> sets01 = {
               //   frame.globalDescriptorSet, // set 0
               //   node->mesh->uniformBuffer.descriptorSet // set 1
               //};
               //// xxxxxxxxxxxxxxxxx
               //// --- Bind sets 0 & 1 (global + node UBO) ---
                std::array<VkDescriptorSet, 2> sets01 = {
                  vkdescriptorsetGlobal, // set 0
                   node->mesh->uniformBuffer.descriptorSet // set 1
                };
               vkCmdBindDescriptorSets(
                  pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                  m_pipelineLayout, 0,
                  (uint32_t)sets01.size(), sets01.data(), 0, nullptr);

               // --- Bind our PBR set (set = 2) ---
                VkDescriptorSet pbrSet =
                  m_vkdescriptorsetaPbr[pcontext->m_pgpurenderer->m_pgpurendertarget->get_frame_index()];
               if (pbrSet == VK_NULL_HANDLE)
               {
                  if (!warnedThisFrame)
                  { /*spdlog::warn("PBR set null");*/
                     warnedThisFrame = true;
                  }
                  continue;
               }
               vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                       m_pipelineLayout, 2, 1,
                                       &pbrSet, 0, nullptr);

               // --- Bind IBL set (set = 3) ---
               VkDescriptorSet iblSet =
                  m_vkdescriptorsetaIbl[pcontext->m_pgpurenderer->m_pgpurendertarget->get_frame_index()];
               if (iblSet == VK_NULL_HANDLE)
               {
                  continue;
               }
               vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 3, 1,
                                       &iblSet, 0, nullptr);

               // Pick pipeline by alpha mode
               switch (primitive->m_pmaterial->alphaMode)
               {
                  case ::gpu_vulkan::gltf::Material::ALPHAMODE_OPAQUE:
                     m_opaquePipeline->bind(pcommandbuffer);
                     break;
                  case ::gpu_vulkan::gltf::Material::ALPHAMODE_MASK:
                     m_maskPipeline->bind(pcommandbuffer);
                     break;
                  case ::gpu_vulkan::gltf::Material::ALPHAMODE_BLEND:
                  default:
                     m_blendPipeline->bind(pcommandbuffer);
                     break;
               }

               pgltfmodel->gltfDraw(pcommandbuffer->m_vkcommandbuffer, ::gpu_vulkan::gltf::RenderNone, m_pipelineLayout, 2);
               warnedThisFrame = false;
            }
         }
      }
   }

} // namespace graphics3d_vulkan
