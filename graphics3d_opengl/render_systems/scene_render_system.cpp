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
   //scene_render_system::scene_render_system(VkSandboxDevice &device, VkRenderPass renderPass,
   //                                     VkDescriptorSetLayout globalSetLayout, IAssetProvider &assets) :
   //    m_device(device), m_globalSetLayout(globalSetLayout), m_assets(assets)
   //{
   //}

   scene_render_system::~scene_render_system() 
   {
      //vkDestroyPipelineLayout(pcontext->logicalDevice(), m_pipelineLayout, nullptr); 
      }


   //void scene_render_system::init(VkSandboxDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout,
   //                             VkSandboxDescriptorPool &descriptorPool, size_t frameCount)
   //{
      void scene_render_system::on_prepare(::gpu::context *pgpucontext)
      {
      //m_globalSetLayout = globalSetLayout;

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

      //createPipelineLayout(pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout());
        // auto prenderpass = render_pass2();
      //createPipeline(prenderpass->m_vkrenderpass);
   }

   // void scene_render_system::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
   // {
   //    const ::array_base<VkDescriptorSetLayout> layouts = {
   //       globalSetLayout,
   //       ::gpu_opengl::gltf::ubo_descriptor_set_layout(),
   // ::gpu_opengl::gltf::image_descriptor_set_layout()};
   //
   //    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
   //    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
   //    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
   //    pipelineLayoutInfo.pSetLayouts = layouts.data();
   //
   //    ::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();
   //
   //
   //    if (vkCreatePipelineLayout(pcontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
   //    {
   //       throw std::runtime_error("Failed to create GLTF pipeline layout");
   //    }
   // }
   //
   // void scene_render_system::createPipeline(VkRenderPass renderPass)
   // {
   //    assert(m_pipelineLayout != VK_NULL_HANDLE);
   //    ::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();
   //    ::cast<::gpu_opengl::device> pgpudevice = pcontext->m_pgpudevice;
   //
   //    ::memory vert;
   //    ::memory frag;
   //
   //    pgpudevice->defer_shader_memory(vert, "matter://shaders/scene_vert.vert");
   //    pgpudevice->defer_shader_memory(frag, "matter://shaders/scene_frag.frag");
   //
   //    ::array_base<VkVertexInputBindingDescription> bindings = {
   //       vkinit::vertexInputBindingDescription(0, sizeof(::gpu_opengl::gltf::Vertex), VK_VERTEX_INPUT_RATE_VERTEX)};
   //
   //    ::array_base<VkVertexInputAttributeDescription> attributes = {
   //       vkinit::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT,
   //                                               offsetof(::gpu_opengl::gltf::Vertex, pos)),
   //       vkinit::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT,
   //                                               offsetof(::gpu_opengl::gltf::Vertex, normal)),
   //       vkinit::vertexInputAttributeDescription(0, 2, VK_FORMAT_R32G32_SFLOAT,
   //                                               offsetof(::gpu_opengl::gltf::Vertex, uv)),
   //       vkinit::vertexInputAttributeDescription(0, 3, VK_FORMAT_R32G32B32A32_SFLOAT,
   //                                               offsetof(::gpu_opengl::gltf::Vertex, color)),
   //       vkinit::vertexInputAttributeDescription(0, 4, VK_FORMAT_R32G32B32A32_SFLOAT,
   //                                               offsetof(::gpu_opengl::gltf::Vertex, tangent))};
   //
   //    {
   //    // OPAQUE
   //    ::vulkan::pipeline_configuration pipelineconfigurationOpaque{};
   //    ::vulkan::defaultPipelineConfigInfo2(pipelineconfigurationOpaque);
   //    pipelineconfigurationOpaque.pipelineLayout = m_pipelineLayout;
   //    pipelineconfigurationOpaque.renderPass = renderPass;
   //    pipelineconfigurationOpaque.bindingDescriptions = bindings;
   //    pipelineconfigurationOpaque.attributeDescriptions = attributes;
   //
   //    m_ppipelineOpaque = øcreate_new<::gpu_opengl::pipeline>();
   //
   //    m_ppipelineOpaque->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag,
   //                                                    pipelineconfigurationOpaque);
   // }
   //
   //    {
   //       // MASK
   //       ::vulkan::pipeline_configuration pipelineconfigurationMask{};
   //       ::vulkan::defaultPipelineConfigInfo2(pipelineconfigurationMask);
   //       pipelineconfigurationMask.pipelineLayout = m_pipelineLayout;
   //       pipelineconfigurationMask.renderPass = renderPass;
   //       pipelineconfigurationMask.bindingDescriptions = bindings;
   //       pipelineconfigurationMask.attributeDescriptions = attributes;
   //       pipelineconfigurationMask.colorBlendAttachments[0].blendEnable = VK_FALSE;
   //
   //       struct SpecData
   //       {
   //          VkBool32 alphaMask;
   //          float cutoff;
   //       };
   //       static SpecData specData{VK_TRUE, 0.5f};
   //       static VkSpecializationMapEntry mapEntries[2] = {{0, offsetof(SpecData, alphaMask), sizeof(VkBool32)},
   //                                                        {1, offsetof(SpecData, cutoff), sizeof(float)}};
   //       static VkSpecializationInfo specInfo{};
   //       specInfo.mapEntryCount = 2;
   //       specInfo.pMapEntries = mapEntries;
   //       specInfo.dataSize = sizeof(specData);
   //       specInfo.pData = &specData;
   //
   //       pipelineconfigurationMask.fragSpecInfo = &specInfo;
   //
   //       m_ppipelineMask = øcreate_new<::gpu_opengl::pipeline>();
   //
   //       m_ppipelineMask->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag, pipelineconfigurationMask);
   //    }
   //
   //    {
   //       // BLEND
   //       ::vulkan::pipeline_configuration pipelineconfigurationBlend{};
   //       ::vulkan::defaultPipelineConfigInfo2(pipelineconfigurationBlend);
   //       pipelineconfigurationBlend.pipelineLayout = m_pipelineLayout;
   //       pipelineconfigurationBlend.renderPass = renderPass;
   //       pipelineconfigurationBlend.bindingDescriptions = bindings;
   //       pipelineconfigurationBlend.attributeDescriptions = attributes;
   //
   //       pipelineconfigurationBlend.colorBlendAttachments[0].blendEnable = VK_TRUE;
   //       pipelineconfigurationBlend.colorBlendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
   //       pipelineconfigurationBlend.colorBlendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   //       pipelineconfigurationBlend.colorBlendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD;
   //       pipelineconfigurationBlend.colorBlendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
   //       pipelineconfigurationBlend.colorBlendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
   //       pipelineconfigurationBlend.colorBlendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD;
   //       // auto &state = pipelineconfigurationBlend.colorBlendAttachments[0];
   //       // state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
   //       // state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   //       // state.colorBlendOp = VK_BLEND_OP_ADD;
   //       // state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
   //       // state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
   //       // state.alphaBlendOp = VK_BLEND_OP_ADD;
   //
   //       pipelineconfigurationBlend.colorBlendAttachments[0].colorWriteMask =
   //          VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
   //
   //       m_ppipelineBlend = øcreate_new<::gpu_opengl::pipeline>();
   //
   //       m_ppipelineBlend->initialize_graphics_pipeline(pcontext->m_pgpurenderer, vert, frag,
   //                                                      pipelineconfigurationBlend);
   //    }
   //
   // }


   void scene_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscene)
   {

      static bool warnedThisFrame = false;

                  auto pframe = ::gpu::current_frame();

      ::cast<::gpu_opengl::command_buffer> pcommandbuffer = pframe->m_pgpucommandbuffer;

      pgpucontext->defer_bind(m_ppipelineOpaque);

      auto &scenerenderables = pscene->scene_renderables();
      ::cast < ::gpu_opengl::context > pcontext = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;
      auto pshader =pcontext->m_pshaderBound;

      //// IBL stuff
      //glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
      //pshader->set_int("diffuseIrradianceMap", TEXTURE_UNIT_DIFFUSE_IRRADIANCE_MAP);
      //::cast < ::gpu_opengl::ibl::diffuse_irradiance_map > pirradiancemap = pcontext->m_pibldiffuseirradiancemap;
      //glBindTexture(GL_TEXTURE_CUBE_MAP, pirradiancemap->getCubemapId());

      //glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_PREFILTERED_ENV_MAP);
      //pshader->set_int("prefilteredEnvMap", TEXTURE_UNIT_PREFILTERED_ENV_MAP);
      //::cast < ::gpu_opengl::ibl::specular_map > pspecularmap = pcontext->m_piblspecularmap;
      //glBindTexture(GL_TEXTURE_CUBE_MAP, pspecularmap->getPrefilteredEnvMapId());

      //glActiveTexture(GL_TEXTURE0 + TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
      //pshader->set_int("brdfConvolutionMap", TEXTURE_UNIT_BRDF_CONVOLUTION_MAP);
      //glBindTexture(GL_TEXTURE_2D, pspecularmap->getBrdfConvolutionMapId());

      //// post stuff for main shader
      //pshader->set_float("bloomBrightnessCutoff", pcontext->m_fBloomBrightnessCutoff);

      //   //// xxxxxxxxxxxxxxxxx
      //::cast<::gpu_opengl::context> pcontext = m_pengine->gpu_context();
      ::cast<::gpu_opengl::renderer> prenderer = pcontext->m_pgpurenderer;

      ////// xxxxxxxxxxxxxxxxx
      // auto globalSetLayout = pcontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      //auto vkdescriptorsetGlobal = pcontext->getGlobalDescriptorSet(prenderer);


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

            glm::mat4 world = pscenerenderable->transform().getMatrix() * pmesh->uniformBlock.matrix;
            glm::mat4 normalMat = glm::transpose(glm::inverse(world));

            m_ppipelineOpaque->set_mat4("modelMatrix", world);
            m_ppipelineOpaque->set_mat4("normalMatrix", normalMat);
            //m_ppipelineOpaque->set_mat4("view", pscene->global_ubo().mat4("view"));
            //m_ppipelineOpaque->set_mat4("projection", pscene->global_ubo().mat4("projection"));

            //memcpy(pmesh->uniformBuffer.mapped, &world, sizeof(world));
            //memcpy((char *)node->mesh->uniformBuffer.mapped + sizeof(world), &normalMat, sizeof(normalMat));
            pcommandbuffer->m_erendersystem = ::graphics3d::e_render_system_gltf_scene;
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
