#include "platform.h"
// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include "approach.h"
#include "binding.h"
#include "block.h"
#include "command_buffer.h"
#include "context.h"
#include "descriptors.h"
#include "window_attachment.h"
#include "input_layout.h"
#include "pipeline.h"
#include "render_target.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/frame_storage.h"
#include "bred/gpu/texture_site.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"



namespace gpu_vulkan
{


   shader::shader()
   {

      m_bClearColor = false;
      // m_vktopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
      m_vkrenderpassCurrent = VK_NULL_HANDLE;
   }


   shader::~shader() {}


   bool shader::need_rebuild()
   {

      return false;
   }


   // void shader::_create_pipeline_layout(int iPushPropertiesSize)
   // {
   //
   //    // ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;
   //    //
   //    // ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;
   //    //
   //    // VkPushConstantRange pushConstantRange{};
   //    // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
   //    // pushConstantRange.offset = 0;
   //    // //pushConstantRange.size = sizeof(PointLightPushConstants);
   //    // pushConstantRange.size = iPushPropertiesSize;
   //    //
   //    // ::array<aaaVkDescriptorSetLayout> aaadescriptorSetLayouts;
   //    //
   //    // if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
   //    // {
   //    //
   //    //    auto globalSetLayout = pgpucontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
   //    //
   //    //    aaadescriptorSetLayouts.add(globalSetLayout);
   //    //
   //    // }
   //    //
   //    // if (m_pLocalDescriptorSet)
   //    // {
   //    //
   //    //    ::cast < ::gpu_vulkan::descriptor_set_layout > pset = m_pLocalDescriptorSet;
   //    //
   //    //    auto setLayout = pset->getDescriptorSetLayout();
   //    //
   //    //    aaadescriptorSetLayouts.add(setLayout);
   //    //
   //    // }
   //    //
   //    // if (m_psetdescriptorlayout)
   //    // {
   //    //
   //    //    auto samplerSetLayout = m_psetdescriptorlayout->getDescriptorSetLayout();
   //    //
   //    //    auto uSet = (unsigned int)aaadescriptorSetLayouts.add(samplerSetLayout);
   //    //
   //    //    if (m_bindingSampler.is_set())
   //    //       m_bindingSampler.m_uSet = uSet;
   //    //    else if (m_bindingCubeSampler.is_set())
   //    //       m_bindingCubeSampler.m_uSet = uSet;
   //    //
   //    // }
   //
   //
   // }


   void shader::on_initialize_shader() {}


   ::pointer<pipeline> shader::_create_pipeline(::gpu::texture_site *pgputexturesiteTarget, ::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;

      //::cast < command_buffer > pcommandbuffer = ::gpu::current_command_buffer();

      //::cast < render_target > prendertarget = prenderer->m_pgpurendertarget;

      auto ppipeline = create_newø<::gpu_vulkan::pipeline>();

      // prenderer->current_frame_particle_array()->add(m_ppipeline);

      //::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      //::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      //::cast <renderer> prenderer = m_pgpurenderer;

      ::vulkan::pipeline_configuration pipelineconfiguration{};

      // if (m_propertiesInputLayout.m_pproperties)
      //{

      //   if (m_pVertexInput)
      //   {

      //      throw ::exception(error_wrong_state, "Shader vertex input shouldn't be set if there are input layout
      //      properties, they are alternatives to describe the same thing");

      //   }

      //   defer_construct_newø(m_pVertexInput);

      //   m_pVertexInput->initialize_from_properties(m_propertiesInputLayout);

      //}

      ::cast<input_layout> pinputlayout = m_pinputlayout;

      if (pinputlayout)
      {

         pipelineconfiguration.bindingDescriptions.copy(pinputlayout->m_bindings);
         pipelineconfiguration.attributeDescriptions.copy(pinputlayout->m_attribs);
      }

      pgpudevice->default_pipeline_configuration(pipelineconfiguration);

      if (m_pinputlayout)
      {

         ::cast<input_layout> pinputlayout = m_pinputlayout;

         pipelineconfiguration.attributeDescriptions = pinputlayout->m_attribs;
         pipelineconfiguration.bindingDescriptions = pinputlayout->m_bindings;
      }

      if (m_eflag & e_flag_clear_default_bindings_and_attributes_descriptions)
      {

         pipelineconfiguration.attributeDescriptions.clear();
         pipelineconfiguration.bindingDescriptions.clear();
      }

      if (m_bDepthTestButNoDepthWrite)
      {

         pipelineconfiguration.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
         pipelineconfiguration.depthStencilInfo.depthTestEnable = VK_TRUE;
         pipelineconfiguration.depthStencilInfo.depthWriteEnable = VK_FALSE;
         pipelineconfiguration.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
         pipelineconfiguration.depthStencilInfo.stencilTestEnable = VK_FALSE;
      }
      else if (m_bDisableDepthTest)
      {

         pipelineconfiguration.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
         pipelineconfiguration.depthStencilInfo.depthTestEnable = VK_FALSE;
         pipelineconfiguration.depthStencilInfo.depthWriteEnable = VK_FALSE;
         pipelineconfiguration.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS;
         // doesn't matter since test is disabled
         pipelineconfiguration.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
         pipelineconfiguration.depthStencilInfo.stencilTestEnable = VK_FALSE;
      }

      if (!m_bDisableDepthTest)
      {

         if (m_bLequalDepth)
         {

            pipelineconfiguration.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
         }
         else
         {

            pipelineconfiguration.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
         }
      }

      pipelineconfiguration.colorBlendAttachments.clear();

      if (m_bAccumulationEnable)
      {

         VkPipelineColorBlendAttachmentState state;
         state.blendEnable = VK_TRUE;
         state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // use alpha blending
         state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
         state.colorBlendOp = VK_BLEND_OP_ADD;
         state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
         state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
         state.alphaBlendOp = VK_BLEND_OP_ADD;
         state.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
         pipelineconfiguration.colorBlendAttachments.add(state);
         if (m_iColorAttachmentCount > 1)
         {

            state.blendEnable = VK_TRUE;
            state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // use alpha blending
            state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
            state.colorBlendOp = VK_BLEND_OP_ADD;
            state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            state.alphaBlendOp = VK_BLEND_OP_ADD;
            state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT;
            pipelineconfiguration.colorBlendAttachments.add(state);
         }

         pipelineconfiguration.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineconfiguration.colorBlendInfo.logicOpEnable = VK_FALSE;
         pipelineconfiguration.colorBlendInfo.attachmentCount =
            (uint32_t)pipelineconfiguration.colorBlendAttachments.get_count();
         pipelineconfiguration.colorBlendInfo.pAttachments = pipelineconfiguration.colorBlendAttachments.data();
      }
      else if (m_bEnableBlend)
      {

         for (int i = 0; i < m_iColorAttachmentCount; i++)
         {
            VkPipelineColorBlendAttachmentState state;
            state.blendEnable = VK_TRUE;
            state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            state.colorBlendOp = VK_BLEND_OP_ADD;
            state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            state.alphaBlendOp = VK_BLEND_OP_ADD;
            // state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // use alpha blending
            // state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            // state.colorBlendOp = VK_BLEND_OP_ADD;
            // state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            // state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            // state.alphaBlendOp = VK_BLEND_OP_ADD;
            state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                                   VK_COLOR_COMPONENT_A_BIT;
            pipelineconfiguration.colorBlendAttachments.add(state);
         }

         pipelineconfiguration.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineconfiguration.colorBlendInfo.logicOpEnable = VK_FALSE;
         pipelineconfiguration.colorBlendInfo.attachmentCount =
            (uint32_t)pipelineconfiguration.colorBlendAttachments.get_count();
         pipelineconfiguration.colorBlendInfo.pAttachments = pipelineconfiguration.colorBlendAttachments.data();
      }
      else
      {

         for (int i = 0; i < m_iColorAttachmentCount; i++)
         {
            VkPipelineColorBlendAttachmentState state;
            state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                                   VK_COLOR_COMPONENT_A_BIT;
            state.blendEnable = VK_FALSE;
            state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            state.colorBlendOp = VK_BLEND_OP_ADD; // Optional
            state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
            state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
            state.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
            pipelineconfiguration.colorBlendAttachments.add(state);
         }

         pipelineconfiguration.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineconfiguration.colorBlendInfo.logicOpEnable = VK_FALSE;
         pipelineconfiguration.colorBlendInfo.attachmentCount =
            (uint32_t)pipelineconfiguration.colorBlendAttachments.get_count();
         pipelineconfiguration.colorBlendInfo.pAttachments = pipelineconfiguration.colorBlendAttachments.data();
      }

      pipelineconfiguration.inputAssemblyInfo.topology = ::vulkan::as_vk_topology(m_etopology);
      pipelineconfiguration.dynamicStateEnables.append_unique(m_dynamicstateaEnable);
      pipelineconfiguration.dynamicStateInfo.dynamicStateCount =
         (uint32_t)pipelineconfiguration.dynamicStateEnables.size();

      auto pgpurendertarget = m_pgpurenderer->render_target();

      ::cast<render_target> prendertarget = pgpurendertarget;

      ::cast<::gpu_vulkan::texture> ptextureTarget = pgputexturesiteTarget->gpu_texture();

      auto prenderpass = ptextureTarget->get_render_pass();

      //::cast<texture> ptexture = prendertarget->current_texture(::gpu::current_layer());

      // if (ptexture->m_state.m_vkimagelayout == VK_IMAGE_LAYOUT_UNDEFINED)
      //{

      //   warning() << "what?";

      //}

      // if (has_sampler())
      //{

      //   pipelineconfiguration.renderPass = shader_sampler()->get_render_pass();

      //}
      // else
      {

         pipelineconfiguration.renderPass = prenderpass->m_vkrenderpass;
      }

      // pipelineconfiguration.pipelineLayout = m_vkpipelinelayout;

      // if (m_bindingUbo.is_set())
      //{

      // if (m_bindingSampler.is_set())
      //{

      //   auto globalSetLayout = pgpucontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      //   pipelineconfiguration.aaadescriptorSetLayouts.add(globalSetLayout);

      ::pointer_array<::gpu_vulkan::descriptor_set_layout> descriptorsetlayouta;
      ::pointer_array<::gpu_vulkan::descriptor_pool> descriptorpoola;

      auto pgpuwindowattachment = ::gpu::window_attachment::get(pgpucommandbuffer->m_pgpurendertarget);

      int iFrameCount = pgpuwindowattachment->get_frame_count();

      if (m_pbindingslotseta)
      {

         update_binding_slots();

         int iBindingSlotSet = -1;

         for (auto &pbindingslotset: *m_pbindingslotseta)
         {

            iBindingSlotSet++;
            
            if (pbindingslotset->m_pbindingset->first()->is_global_ubo())
            {

               // auto pscene = pgpucontext->m_pengine->m_pimmersionlayer->m_pscene;

               // auto pblockGlobalUbo1 = pscene->global_ubo1(pgpucontext);
               //    auto globalSetLayout =
               //    pgpucontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
               //    pipelineconfiguration.aaadescriptorSetLayouts.add(globalSetLayout);
               auto pgpubindingsetGlobalUbo = pgpucontext->global_ubo1_binding_set();

               ::cast<::gpu_vulkan::binding_set> pbindingsetGlobalUbo = pgpubindingsetGlobalUbo;

               auto pdescriptorsetlayout = pbindingsetGlobalUbo->descriptor_set_layout(pgpucommandbuffer);

               descriptorsetlayouta.atø(pbindingslotset->m_iSet) = pdescriptorsetlayout;

               continue;

            }

            ::cast<::gpu_vulkan::binding_set> pbindingset = pbindingslotset->m_pbindingset;

            descriptorsetlayouta.atø(pbindingslotset->m_iSet) = pbindingset->descriptor_set_layout(pgpucommandbuffer);

            descriptorpoola.atø(pbindingslotset->m_iSet) = pbindingset->m_pdescriptorsetlayout1;

         }

      }

      //      //m_psetdescriptorlayout =
      //      //
      //      //      .addBinding(m_bindingUbo.m_uBinding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      //      //                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
      //      //      .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      //      //                  VK_SHADER_STAGE_FRAGMENT_BIT)
      //      //      .build();

      //      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

      //      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

      //      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
      //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount * 10);
      //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //      m_pdescriptorpool = pdescriptorpoolbuilder->build();
      //   }
      //   else
      //   {

      //      m_psetdescriptorlayout =
      //         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
      //            .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      //                        VK_SHADER_STAGE_FRAGMENT_BIT)
      //            .build();

      //      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

      //      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

      //      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
      //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //      m_pdescriptorpool = pdescriptorpoolbuilder->build();
      //   }
      //}
      // else if (m_bindingSampler.is_set())
      //{

      //   {

      //      m_psetdescriptorlayout =
      //         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
      //            .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      //                        VK_SHADER_STAGE_FRAGMENT_BIT)
      //            .build();

      //      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

      //      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

      //      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
      //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //      m_pdescriptorpool = pdescriptorpoolbuilder->build();
      //   }
      //}
      // else if (m_bindingCubeSampler.is_set())
      //{

      //   {

      //      m_psetdescriptorlayout =
      //         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
      //            .addBinding(m_bindingCubeSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
      //                        VK_SHADER_STAGE_FRAGMENT_BIT)
      //            .build();

      //      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

      //      auto pdescriptorpoolbuilder = allocateø::gpu_vulkan::descriptor_pool::Builder();

      //      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
      //      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      //      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      //      m_pdescriptorpool = pdescriptorpoolbuilder->build();
      //   }
      //}

      //_create_pipeline_layout((int)m_propertiesPush.m_memory.size());

      // ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;
      //
      // ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;

      int iPushConstantOffset = 0;

      int iSharedPushPropertiesSize = (int)m_propertiesPushShared.m_blockWithoutSamplers.size();

      if (iSharedPushPropertiesSize > 0)
      {
         VkPushConstantRange pushConstantRange{};
         pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
         pushConstantRange.offset = iPushConstantOffset;
         pushConstantRange.size = iSharedPushPropertiesSize;

         pipelineconfiguration.pushConstantRanges.add(pushConstantRange);
         // pushConstantRange.size = iPushPropertiesSize;
         iPushConstantOffset += iSharedPushPropertiesSize;
      }

      int iVertexPushPropertiesSize = (int)m_propertiesPushVertex.m_blockWithoutSamplers.size();

      if (iVertexPushPropertiesSize > 0)
      {
         VkPushConstantRange pushConstantRange{};
         pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
         pushConstantRange.offset = iPushConstantOffset;
         pushConstantRange.size = iVertexPushPropertiesSize;

         // iPushConstantOffset += iVertexPushPropertiesSie;

         pipelineconfiguration.pushConstantRanges.add(pushConstantRange);
         // pushConstantRange.size = iPushPropertiesSize;
      }

      int iFragmentPushPropertiesSize = (int)m_propertiesPushFragment.m_blockWithoutSamplers.size();

      if (iFragmentPushPropertiesSize > 0)
      {
         VkPushConstantRange pushConstantRange{};
         pushConstantRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
         pushConstantRange.offset = iPushConstantOffset;
         pushConstantRange.size = iFragmentPushPropertiesSize;

         // iPushConstantOffset += iFragmentPushPropertiesSize;

         pipelineconfiguration.pushConstantRanges.add(pushConstantRange);
         // pushConstantRange.size = iPushPropertiesSize;
      }
      //::array<aaaVkDescriptorSetLayout> aaadescriptorSetLayouts;

      // if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      //{

      //   auto globalSetLayout = pgpucontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
      //   pipelineconfiguration.aaadescriptorSetLayouts.add(globalSetLayout);
      //}

      //if (m_pLocalDescriptorSet)
      //{

      //   ::cast<::gpu_vulkan::descriptor_set_layout> pset = m_pLocalDescriptorSet;

      //   auto setLayout = pset->getDescriptorSetLayout();

      //   pipelineconfiguration.aaadescriptorSetLayouts.add(setLayout);
      //}

      int iSet = -1;

      for (auto &pdescriptorsetlayout: descriptorsetlayouta)
      {

         iSet++;

         if (pdescriptorsetlayout)
         {

            auto samplerSetLayout = pdescriptorsetlayout->getDescriptorSetLayout();

            pipelineconfiguration.descriptorSetLayouts.atø(iSet) = samplerSetLayout;
         }

         // if (m_bindingSampler.is_set())
         //  m_bindingSampler.m_uSet = uSet;
         // else if (m_bindingCubeSampler.is_set())
         //  m_bindingCubeSampler.m_uSet = uSet;
      }

      //for (auto &pair: m_mapDescriptorSetLayout)
      //{

      //   auto uSet = (unsigned int)pair.element1();

      //   auto descriptorsetlayout = pair.element2()->getDescriptorSetLayout();

      //   pipelineconfiguration.aaadescriptorSetLayouts.element_at_grow(uSet) = descriptorsetlayout;
      //}


      pgpudevice->defer_shader_memory(m_memoryVertex, m_pathVertex);
      pgpudevice->defer_shader_memory(m_memoryFragment, m_pathFragment);

      ppipeline->initialize_graphics_pipeline(m_pgpurenderer, m_memoryVertex, m_memoryFragment,
                                                pipelineconfiguration);

      //m_vkrenderpassCurrent = pipelineconfiguration.renderPass;

      return ppipeline;

   }


   //void shader::bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget,
   //                  ::gpu::texture *pgputextureSource)
   //{

   //   _defer_prepare_pipeline(pgpucommandbuffer, pgputextureTarget);

   //   _bind(pgpucommandbuffer, pgputextureTarget);

   //   bind_source(pgpucommandbuffer, pgputextureSource, 0);
   //}


   void shader::bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture_site *pgputexturesiteTarget)
   {

      _defer_set_current_pipeline(pgpucommandbuffer, pgputexturesiteTarget);

      _bind(pgpucommandbuffer, pgputexturesiteTarget);

      //_bind(pgpucommandbuffer, ::gpu::e_scene_none);
   }


   void shader::_bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture_site *pgputexturesiteTarget)
   {

      //m_ptextureTarget = pgputextureTarget;

      //::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      //::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      //::cast<renderer> prenderer = m_pgpurenderer;

      //::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //auto pgpurendertarget = m_pgpurenderer->render_target();

      //::cast<render_target> prendertarget = pgpurendertarget;

      //::cast<::gpu_vulkan::texture> ptextureTarget = m_ptextureTarget;

      //auto prenderpass = ptextureTarget->get_render_pass();

      //VkRenderPassBeginInfo renderPassBeginInfo{};

      //renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

      //// if (!has_shader_sampler())
      ////{

      ////   throw ::exception(error_wrong_state, "use bind()");

      ////}

      //// bind_source(pgputextureSource);

      //::cast<texture> ptextureDst = pgputextureTarget;

      //// auto pshadertextureDst = shader_texture(ptextureDst, false);

      //renderPassBeginInfo.renderPass = prenderpass->m_vkrenderpass;

      //auto &layer = ptextureDst->current_layer(prenderpass);

      //renderPassBeginInfo.framebuffer = layer.m_vkframebufferLayer;

      //uint32_t w = ptextureDst->width();
      //uint32_t h = ptextureDst->height();
      //int x = ptextureDst->rectangle().left;
      ////int bottom = ptextureDst->rectangle().bottom;
      ////int y = h - bottom;
      //int y = ptextureDst->rectangle().top;

      //renderPassBeginInfo.renderArea.offset = {x, y};
      ////renderPassBeginInfo.renderArea.offset = {0, 0};
      //renderPassBeginInfo.renderArea.extent = {w, h};

      //debug() << "has_shader_sampler";

      ////}
      //// else
      ////{

      ////   ::cast <render_pass> prenderpass = m_pgpurenderer->m_pgpurendertarget;

      //VkClearValue clearValues[2];

      //if (prenderpass->m_bLoadClearOp)
      //{

      //   clearValues[0].color = {m_colorClear.f32_red() * m_colorClear.f32_opacity(),
      //                           m_colorClear.f32_green() * m_colorClear.f32_opacity(),
      //                           m_colorClear.f32_blue() * m_colorClear.f32_opacity(), m_colorClear.f32_opacity()};

      //   if (m_bDisableDepthTest)
      //   {

      //      renderPassBeginInfo.clearValueCount = 1;
      //   }
      //   else
      //   {

      //      clearValues[1].depthStencil = {1.0f, 0};
      //      renderPassBeginInfo.clearValueCount = 2;
      //   }

      //   renderPassBeginInfo.pClearValues = clearValues;
      //}
      //else
      //{

      //   renderPassBeginInfo.clearValueCount = 0;
      //   renderPassBeginInfo.pClearValues = nullptr;
      //}

      ////   renderPassBeginInfo.renderPass = prenderpass->getRenderPass();
      ////   renderPassBeginInfo.framebuffer = prenderpass->getFrameBuffer(prenderer->get_frame_index());
      ////   renderPassBeginInfo.renderArea.offset = { 0, 0 };
      ////   renderPassBeginInfo.renderArea.extent =
      ////   {
      ////      (uint32_t)pgpucontext->width(),
      ////      (uint32_t)pgpucontext->height()
      ////   };

      ////}

      //::cast<texture> ptexture = ptextureDst;

      //if (ptexture->mip_layer_state(0, 0).m_vkimagelayout == VK_IMAGE_LAYOUT_UNDEFINED)
      //{

      //   warning() << "what?";
      //}


      //// vkCmdBeginRenderPass(
      ////  pcommandbuffer->m_vkcommandbuffer,
      ////&renderPassBeginInfo,
      //// VK_SUBPASS_CONTENTS_INLINE);

      ////_bind();
   }


   //void shader::bind(::gpu::command_buffer *pgpucommandbuffer)
   //{

   //   ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

   //   ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

   //   ::cast<renderer> prenderer = m_pgpurenderer;

   //   ::cast<command_buffer> pcommandbuffer = ::gpu::current_command_buffer();

   //   VkRenderPassBeginInfo renderPassBeginInfo{};

   //   renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

   //   // if (has_sampler())
   //   //{

   //   //   throw ::exception(error_wrong_state, "use bind(txtDst, txtDsr)");

   //   //}

   //   ::cast<render_target> prendertarget = m_pgpurenderer->m_pgpurendertarget;

   //   ::cast<::gpu_vulkan::texture> ptextureTarget = m_ptextureTarget;

   //   if (!ptextureTarget)
   //   {

   //      m_ptextureTarget = m_pgpurenderer->m_pgpurendertarget->current_texture(::gpu::current_layer());

   //      ptextureTarget = m_ptextureTarget;
   //   }

   //   auto prenderpass = ptextureTarget->get_render_pass();


   //   renderPassBeginInfo.renderPass = prenderpass->getRenderPass();
   //   // if (prenderer->m_pgpulayer)
   //   {

   //      ::cast<texture> ptexture = prendertarget->current_texture(::gpu::current_layer());

   //      if (ptexture->m_state.m_vkimagelayout == VK_IMAGE_LAYOUT_UNDEFINED)
   //      {

   //         warning() << "what?";
   //      }

   //      renderPassBeginInfo.framebuffer = ptexture->framebuffer(prenderpass);
   //   }
   //   // else
   //   //{

   //   //   renderPassBeginInfo.framebuffer =
   //   //   prenderpass->getFrameBuffer(prenderer->m_pgpurendertarget->get_frame_index());

   //   //}

   //   VkClearValue clearValues[2]{};
   //   // clearValues[0].color = { 0.5f* 0.5f, 0.75f*0.5f, 0.95f* 0.5f, 0.5f };

   //   if (m_bClearColor)
   //   {

   //      auto fR = m_colorClear.f32_red();
   //      auto fG = m_colorClear.f32_green();
   //      auto fB = m_colorClear.f32_blue();
   //      auto fA = m_colorClear.f32_opacity();

   //      clearValues[0].color = {fR * fA, fG * fA, fB * fA, fA};
   //      clearValues[1].depthStencil = {1.0f, 0};
   //      renderPassBeginInfo.clearValueCount = 2;
   //      renderPassBeginInfo.pClearValues = nullptr;
   //   }
   //   else
   //   {

   //      renderPassBeginInfo.clearValueCount = 0;
   //      renderPassBeginInfo.pClearValues = nullptr;
   //   }


   //   renderPassBeginInfo.renderArea.offset = {0, 0};
   //   renderPassBeginInfo.renderArea.extent = {(uint32_t)pgpucontext->width(),
   //                                            (uint32_t)pgpucontext->height()};


   //   /*vkCmdBeginRenderPass(
   //      pcommandbuffer->m_vkcommandbuffer,
   //      &renderPassBeginInfo,
   //      VK_SUBPASS_CONTENTS_INLINE);*/
   //   _defer_prepare_pipeline(pgpucommandbuffer, m_ptextureTarget);
   //   _bind(pgpucommandbuffer, m_ptextureTarget);
   //}

   
   void shader::_defer_set_current_pipeline(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture_site *pgputexturesite)
   {

      ::cast<texture> ptexture = pgputexturesite->gpu_texture();

      auto prenderpass = ptexture->get_render_pass();

      auto vkrenderpass = prenderpass->m_vkrenderpass;

      auto &ppipeline = m_mapRenderPassPipeline[vkrenderpass];

      if (!ppipeline)
      {

         ppipeline = _create_pipeline(pgputexturesite, pgpucommandbuffer);

      }

      m_ppipelineCurrent = ppipeline;

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;

      //::cast<command_buffer> pcommandbuffer = ::gpu::current_command_buffer();
      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      m_ppipelineCurrent->bind(pcommandbuffer);

      //auto rectangle = pgputexture->m_textureattributes.m_rectangleTarget;
      //auto size = rectangle.size();
      //VkViewport vp = {(float)rectangle.left, (float)rectangle.top, (float)size.width(), (float)size.height(), 0.0f, 1.0f};

      //VkRect2D sc = {{
      //                  rectangle.left,
      //                  rectangle.top,
      //               },
      //               {
      //                  (uint32_t)size.width(),
      //                  (uint32_t)size.height(),
      //               }};

      //vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);

      //vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);


      // if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      //{

      //   auto globalDescriptorSet = pgpucontext->getGlobalDescriptorSet(prenderer);

      //   vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
      //                           m_ppipeline->_pipeline_layout(), 0, 1, &globalDescriptorSet, 0, nullptr);
      //}
   }


   // void shader::_bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::enum_scene escene)
   //{

   //   ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

   //   ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

   //   ::cast<renderer> prenderer = m_pgpurenderer;

   //   //::cast<command_buffer> pcommandbuffer = ::gpu::current_command_buffer();
   //   ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

   //   if (!m_ppipeline)
   //   {

   //      _create_pipeline(pgput);

   //   }

   //   m_ppipeline->bind(pcommandbuffer);

   //   auto rectangle = pgpucontext->get_placement();
   //   auto size = rectangle.size();
   //   VkViewport vp = {
   //      (float)0.f,
   //      (float)0.f,
   //      (float)size.width(),
   //      (float)size.height(),
   //      0.0f, 1.0f};

   //   VkRect2D sc = {
   //      {
   //         0,
   //         0,
   //      },
   //      {
   //         (uint32_t)size.width(),
   //         (uint32_t)size.height(),
   //      }
   //   };

   //   vkCmdSetViewport(pcommandbuffer->m_vkcommandbuffer, 0, 1, &vp);

   //   vkCmdSetScissor(pcommandbuffer->m_vkcommandbuffer, 0, 1, &sc);


   //   if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
   //   {

   //      auto globalDescriptorSet = pgpucontext->getGlobalDescriptorSet(prenderer);

   //      vkCmdBindDescriptorSets(
   //         pcommandbuffer->m_vkcommandbuffer,
   //         VK_PIPELINE_BIND_POINT_GRAPHICS,
   //         m_ppipeline->_pipeline_layout(),
   //         0,
   //         1,
   //         &globalDescriptorSet,
   //         0,
   //         nullptr);

   //   }

   //}


   void shader::unbind(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;
      //
      //      {
      //
      //   {
      //
      //      if (pgpucontext->m_pgpudevice->m_iLayer == 0)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue={.color = {0.5f, 0.0f, 0.0f, 0.5f}} // Red
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {100, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //      }
      //      else if (pgpucontext->m_pgpudevice->m_iLayer == 1)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.5f, 0.0f, 0.5f} } // Green
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {200, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //
      //      }
      //      else if (pgpucontext->m_pgpudevice->m_iLayer == 2)
      //      {
      //
      //         VkClearAttachment clearAttachment = {
      //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      //            .colorAttachment = 0,
      //            .clearValue = {.color = {0.0f, 0.0f, 0.5f, 0.5f} } // Blue
      //         };
      //
      //         VkClearRect clearRect = {
      //             .rect = {
      //                 .offset = {300, 100},
      //                 .extent = {100, 100}
      //             },
      //             .baseArrayLayer = 0,
      //             .layerCount = 1
      //         };
      //
      //         vkCmdClearAttachments(
      //            pcommandbuffer->m_vkcommandbuffer,
      //            1,
      //            &clearAttachment,
      //            1,
      //            &clearRect);
      //
      //      }
      //
      //   }
      //
      //}


      // vkCmdEndRenderPass(pcommandbuffer->m_vkcommandbuffer);
   }


   void shader::push_properties(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      int iPushConstantsOffset = 0;

      if (m_propertiesPushShared.m_blockWithoutSamplers.size() > 0)
      {

         auto uBlockSizeWithoutSamplers = (uint32_t)m_propertiesPushShared.m_blockWithoutSamplers.size();

         auto pmemory = allocateø ::memory();

         pmemory->assign(m_propertiesPushShared.m_blockWithoutSamplers.data(),
                         m_propertiesPushShared.m_blockWithoutSamplers.size());
         auto f0 = ((float *)pmemory->data())[0];
         auto f1 = ((float *)pmemory->data())[1];
         auto f2 = ((float *)pmemory->data())[2];
         vkCmdPushConstants(pcommandbuffer->m_vkcommandbuffer, m_ppipelineCurrent->_pipeline_layout(),
                            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, iPushConstantsOffset,
                            uBlockSizeWithoutSamplers, pmemory->data());

         auto pgpuwindowattachment = ::gpu::window_attachment::get(pgpucommandbuffer->m_pgpurendertarget);

         auto pframestorage = pgpuwindowattachment->current_frame_storage();

         pframestorage->m_memorya.add(pmemory);

         iPushConstantsOffset += uBlockSizeWithoutSamplers;
      }

      if (m_propertiesPushVertex.m_blockWithoutSamplers.size() > 0)
      {

         vkCmdPushConstants(pcommandbuffer->m_vkcommandbuffer, m_ppipelineCurrent->_pipeline_layout(),
                            VK_SHADER_STAGE_VERTEX_BIT, iPushConstantsOffset,
                            (uint32_t)m_propertiesPushVertex.m_blockWithoutSamplers.size(),
                            m_propertiesPushVertex.m_blockWithoutSamplers.data());

         // iPushConstantsOffset += m_propertiesPushVertex.m_blockWithoutSamplers.size();
      }

      if (m_propertiesPushFragment.m_blockWithoutSamplers.size() > 0)
      {

         vkCmdPushConstants(pcommandbuffer->m_vkcommandbuffer, m_ppipelineCurrent->_pipeline_layout(),
                            VK_SHADER_STAGE_FRAGMENT_BIT, iPushConstantsOffset,
                            (uint32_t)m_propertiesPushFragment.m_blockWithoutSamplers.size(),
                            m_propertiesPushFragment.m_blockWithoutSamplers.data());

         // iPushConstantsOffset += m_propertiesPushFragment.m_blockWithoutSamplers.size();
      }

      // set_push_properties(pgpucommandbuffer, m_propertiesPushShared.m_blockWithoutSamplers);
      // set_push_properties(pgpucommandbuffer, m_propertiesPushVertex.m_blockWithoutSamplers);
      // set_push_properties(pgpucommandbuffer, m_propertiesPushFragment.m_blockWithoutSamplers);
      //::gpu::shader::push_properties();
      // set_push_properties(m_propertiesPush.m_block);
      /*     ::cast < renderer > prenderer = m_pgpurenderer;

           ::cast < command_buffer > pcommandbuffer = ::gpu::current_command_buffer();

           vkCmdPushConstants(
              pcommandbuffer->m_vkcommandbuffer,
              m_vkpipelinelayout,
              VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
              0,
              (uint32_t)m_propertiesPush.size(),
              m_propertiesPush.data());*/
   }
   // bool shader::has_sampler() { return m_bindingSampler.is_set() || m_bindingCubeSampler.is_set(); }

   // bool shader::has_sampler() { return ::gpu::shader::has_sampler(); }


   // void shader::set_push_properties(::gpu::command_buffer *pgpucommandbuffer, const ::block &block)
   //{::gpu::    binding &binding(int iSet = 0, int iSlot = 0);

   //   ::cast<renderer> prenderer = m_pgpurenderer;

   //   ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

   //   vkCmdPushConstants(
   //      pcommandbuffer->m_vkcommandbuffer,
   //      m_ppipeline->_pipeline_layout(),
   //      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
   //      0,
   //      (uint32_t)block.size(),
   //      block.data());
   //   //::cast < renderer > prenderer = m_pgpurenderer;

   //   //::cast < command_buffer > pcommandbuffer = ::gpu::current_command_buffer();

   //   //vkCmdPushConstants(
   //   //   pcommandbuffer->m_vkcommandbuffer,


   //   //vkCmdPushConstants(cmdBuf, irradiancePipeline.getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT |
   //   VK_SHADER_STAGE_FRAGMENT_BIT, 0, block.size(), block.data());
   //}


   // class shader_sampler* shader::shader_sampler()
   //{

   //   if (nok(m_pshadersampler))
   //   {

   //      construct_newø(m_pshadersampler);

   //      m_pshadersampler->m_pshader = this;

   //   }

   //   return m_pshadersampler;

   //}


   // shader_sampler::shader_sampler() :
   //    m_descriptorsetlayout{}
   //{

   //   m_uSet = 0;
   //   m_pshader = nullptr;
   //   m_vksampler = VK_NULL_HANDLE;
   //   m_vkrenderpass2 = VK_NULL_HANDLE;

   //}


   // shader_sampler::~shader_sampler()
   //{

   //   ::cast < context > pcontext = m_pshader->m_pgpurenderer->m_pgpucontext;

   //   if (m_vkrenderpass2)
   //   {

   //      vkDestroyRenderPass(
   //         pcontext->logicalDevice(),
   //         m_vkrenderpass2,
   //         nullptr
   //      );

   //   }

   //   if (m_vksampler)
   //   {

   //      vkDestroySampler(
   //         pcontext->logicalDevice(),
   //         m_vksampler,
   //         nullptr
   //      );

   //   }

   //}


   // VkRenderPass shader_sampler::get_render_pass()
   //{

   //   if (!m_pshader->has_shader_sampler())
   //   {

   //      throw ::exception(error_wrong_state);

   //   }

   //   if (m_vkrenderpass2 != VK_NULL_HANDLE)
   //   {

   //      return m_vkrenderpass2;

   //   }

   //   ::cast < context > pcontext = m_pshader->m_pgpurenderer->m_pgpucontext;

   //   VkAttachmentDescription colorAttachment = {
   //      .format = pcontext->m_formatImageDefault,
   //      .samples = VK_SAMPLE_COUNT_1_BIT,
   //      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
   //      .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
   //   };

   //   if (m_pshader->m_bClearColor)
   //   {

   //      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //// ← clears to clearColor at start
   //      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

   //   }
   //   else
   //   {

   //      colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // Blend onto existing dstImage
   //      colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

   //   }

   //   VkAttachmentReference colorRef =
   //   {

   //      .attachment = 0,
   //      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL

   //   };

   //   VkSubpassDescription subpass =
   //   {

   //      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
   //      .colorAttachmentCount = 1,
   //      .pColorAttachments = &colorRef

   //   };

   //   VkRenderPassCreateInfo renderPassInfo =
   //   {

   //      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
   //      .attachmentCount = 1,
   //      .pAttachments = &colorAttachment,
   //      .subpassCount = 1,
   //      .pSubpasses = &subpass

   //   };

   //   VkCheckResult(vkCreateRenderPass(
   //      pcontext->logicalDevice(),
   //      &renderPassInfo, NULL,
   //      &m_vkrenderpass2));

   //   return m_vkrenderpass2;

   //}


   // shader_texture::shader_texture() :
   //    m_vkdescriptorset{}
   //{

   //   //m_vkframebuffer2 = VK_NULL_HANDLE;
   //   m_vkimageview = VK_NULL_HANDLE;
   //   m_pshadersampler = nullptr;

   //}


   // shader_texture::~shader_texture()
   //{

   //   ::cast < context > pcontext = m_pshadersampler->m_pshader->m_pgpurenderer->m_pgpucontext;

   //   for(auto & vkframebuffer2 : m_mapVkFramebuffer2.payloads())
   //   {

   //      vkDestroyFramebuffer(
   //         pcontext->logicalDevice(),
   //         vkframebuffer2,
   //         nullptr);

   //   }

   //   if (m_vkimageview)
   //   {

   //      vkDestroyImageView(
   //         pcontext->logicalDevice(),
   //         m_vkimageview,
   //         nullptr);

   //   }

   //}


   // void shader_texture::initialize_shader_texture(shader_sampler* pshadersampler, texture* ptexture)
   //{

   //   m_pshadersampler = pshadersampler;

   //   m_ptexture = ptexture;

   //}


   // VkImageView shader_texture::get_image_view()
   //{

   //   if (!m_vkimageview)
   //   {

   //      ::cast < context > pcontext = m_pshadersampler->m_pshader->m_pgpurenderer->m_pgpucontext;
   //      VkImageViewCreateInfo viewInfo =
   //      {
   //         .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //         .image = m_ptexture->m_vkimage,
   //         .viewType = VK_IMAGE_VIEW_TYPE_2D,
   //         .format = pcontext->m_formatImageDefault,
   //         .subresourceRange =
   //         {
   //            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //            .baseMipLevel = 0,
   //            .levelCount = 1,
   //            .baseArrayLayer = 0,
   //            .layerCount = 1
   //         }
   //      };

   //      VkCheckResult(vkCreateImageView(
   //         pcontext->logicalDevice(),
   //         &viewInfo,
   //         NULL,
   //         &m_vkimageview));

   //   }

   //   return m_vkimageview;

   //}


   // VkFramebuffer shader_texture::get_framebuffer(::gpu_vulkan::render_pass * prenderpass)
   //{

   //   auto& vkframebuffer2 = m_mapVkFramebuffer2[prenderpass->m_vkrenderpass];

   //   if (!vkframebuffer2)
   //   {

   //      ::cast < context > pcontext = m_pshadersampler->m_pshader->m_pgpurenderer->m_pgpucontext;

   //      VkImageView imageviewa[] = { get_image_view() };

   //      uint32_t w = m_ptexture->size().cx;

   //      uint32_t h = m_ptexture->size().cy;

   //      VkFramebufferCreateInfo framebufferCreateInfo =
   //      {
   //         .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
   //         .renderPass = prenderpass->m_vkrenderpass,
   //         .attachmentCount = 1,
   //         .pAttachments = imageviewa,
   //         .width = w,
   //         .height = h,
   //         .layers = 1
   //      };

   //      VkCheckResult(vkCreateFramebuffer(
   //         pcontext->logicalDevice(),
   //         &framebufferCreateInfo, NULL,
   //         &vkframebuffer2));

   //   }

   //   return vkframebuffer2;

   //}


   void shader::create_descriptor_layout() {}


   //void shader::_update_vk_descriptor_set(int iFrameCount) 
   //{

   //   defer_construct_newø(m_pvkdescriptorseta);
   //   
   //   m_pvkdescriptorseta->set_size(iFrameCount); 
   //
   //}


   // render_pass *shader::render_pass2(::gpu::texture * pgputextureTarget)
   //{

   //   ::cast<renderer> prenderer = m_pgpurenderer;

   //   return prenderer->render_pass2(pgputextureTarget);

   //}
   //void shader::on_before_draw(::gpu::command_buffer *pgpucommandbuffer)
   //{

   //   ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

   //   ::raw_array<VkDescriptorSet> descriptorseta;

   //   for (auto &pbindingslotset: *m_pbindingslotseta)
   //   {

   //      ::cast<::gpu_vulkan::binding_slot_set> pbindingset = pbindingslotset;

   //      auto vkdescriptorset = pbindingslotset->descriptor_set(pgpucommandbuffer);

   //      descriptorseta.add(vkdescriptorset);

   //      //if(pbindingslotset->m_pbindingset->first()->is_global_ubo())
   //      //{
   //      //   ASSERT(pbindingslotset->size() == 1);
   //      //      ::cast<context> pcontext = m_pgpurenderer->m_pgpucontext;

   //      //      auto pengine = pcontext->m_pengine;

   //      //      auto pscene = pcontext->m_pengine->m_pimmersionlayer->m_pscene;

   //      //      ::cast<::gpu_vulkan::block> pblockGlobalUbo1 = pscene->global_ubo1(pcontext);

   //      //      ::cast<renderer> prenderer = m_pgpurenderer;

   //      //      auto vkdescriptorset = pblockGlobalUbo1->descriptor_set(pbindingslotset, pgpucommandbuffer);

   //      //      // aaaVkDescriptorSetLayout vkdescriptorsetlayout = pdescriptorsetlayout->m_vkdescriptorsetlayout;

   //      //      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
   //      //                              m_ppipelineCurrent->_pipeline_layout(), 0, 1, &vkdescriptorset, 0, nullptr);
   //      //
   //      //}
   //      //else if (pbindingslotset->m_pbindingset->first()->is_image_sampler())
   //      //{

   //      //      ::cast<texture> ptexture = bindingslot.m_ptexture;

   //      //      if (!ptexture)
   //      //      {

   //      //         warning("missing texture attachment \"{}\" ({}, {})", 
   //      //            bindingslot.m_pbinding->m_strUniform,
   //      //            bindingslot.m_iSet, bindingslot.m_iSlot);

   //      //         throw ::exception(error_wrong_state);

   //      //      }

   //      //      int iFrameSerial = m_pgpurenderer->m_pgpucontext->m_pgpudevice->m_iFrameSerial2;

   //      //      auto vkdescriptorset = ptexture->descriptor_set(this, pgpucommandbuffer);

   //      //      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer,
   //      //                              VK_PIPELINE_BIND_POINT_GRAPHICS, // Bind point
   //      //                              m_ppipelineCurrent->_pipeline_layout(), // Layout used when pipeline was created
   //      //                              pbindingslotset->m_iSet, // First set (set = 0)
   //      //                              1, // Descriptor set count
   //      //                              &vkdescriptorset, // Pointer to descriptor set
   //      //                              0, // Dynamic offset count
   //      //                              NULL // Dynamic offsets
   //      //      );
   //      //   }
   //      //}
   //   }

   //   // if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
   //   //{

   //   //   auto globalDescriptorSet = pgpucontext->getGlobalDescriptorSet(prenderer);

   //   vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
   //               m_ppipeline->_pipeline_layout(), 0, descriptorseta.size(), descriptorseta.data(), 0, nullptr);
   //   //}
   //}

   void shader::draw()
   {

      ::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<command_buffer> pcommandbuffer = ::gpu::current_command_buffer();

      vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0);
   }


   // class shader_texture* shader::shader_texture(::gpu::texture* pgputexture, bool bSampler)
   //{

   //   ::cast < texture > ptexture = pgputexture;

   //   auto pshadersampler = shader_sampler();

   //   auto& pshadertexture = pshadersampler->m_shadertexturemap[ptexture->m_vkimage];

   //   ::cast < renderer >prenderer = m_pgpurenderer;

   //   if (!pshadertexture)
   //   {

   //      construct_newø(pshadertexture);

   //      pshadertexture->initialize_shader_texture(pshadersampler, ptexture);

   //      //imagestructa.set_size(prenderer->get_frame_count());

   //      //for (int i = 0; i < imagestructa.size(); i++)
   //      //{

   //         //auto& imagestruct = *pshadertexture;

   //      VkImageViewCreateInfo viewInfo =
   //      {
   //   .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
   //   .image = ptexture->m_vkimage,  // <-- Your existing VkImage
   //   .viewType = ptexture->_get_image_view_type(),
   //   .format = VK_FORMAT_B8G8R8A8_UNORM,  // <-- Match your image's format
   //   .components = {
   //      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
   //      .a = VK_COMPONENT_SWIZZLE_IDENTITY,
   //   },
   //   .subresourceRange = {
   //      .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
   //      .baseMipLevel = 0,
   //      .levelCount = 1,
   //      .baseArrayLayer = 0,
   //      .layerCount = ptexture->_get_layer_count(),
   //   },
   //      };

   //      ::cast < context > pcontext = m_pgpurenderer->m_pgpucontext;

   //      //VkImageView imageView;
   //      if (vkCreateImageView(
   //         pcontext->logicalDevice(),
   //         &viewInfo, NULL,
   //         &pshadertexture->m_vkimageview) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //      ::cast < device > pgpudevice = pcontext->m_pgpudevice;

   //      if (bSampler)
   //      {

   //         VkDescriptorImageInfo imageinfo;

   //         imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
   //         imageinfo.imageView = pshadertexture->m_vkimageview;
   //         imageinfo.sampler = pcontext->_001VkSampler();

   //         auto& playout = shader_sampler()->m_psetdescriptorlayout;

   //         auto& ppool = shader_sampler()->m_pdescriptorpool;

   //         unsigned int uSamplerBinding = 0;

   //         if (m_bindingSampler.is_set())
   //            uSamplerBinding = m_bindingSampler.m_uBinding;
   //         else if (m_bindingCubeSampler.is_set())
   //            uSamplerBinding = m_bindingCubeSampler.m_uBinding;

   //         descriptor_writer(*playout, *ppool)
   //            .writeImage(uSamplerBinding, &imageinfo)
   //            .build(pshadertexture->m_vkdescriptorset);


   //      }
   //      //   auto descriptorSetLayout = s1()->m_psetdescriptorlayout->getDescriptorSetLayout();

   //      //   VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
   //      //.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
   //      //.setLayoutCount = 1,
   //      //.pSetLayouts = &descriptorSetLayout,
   //      //   };

   //      //   //VkPipelineLayout pipelineLayout;
   //      //   if (vkCreatePipelineLayout(pcontext->logicalDevice(),
   //      //      &pipelineLayoutInfo,
   //      //      NULL,

   //      //      &m_vkpipelinelayout) != VK_SUCCESS) {
   //      //      // Handle error
   //      //   }

   //      //}

   //   }

   //   return pshadertexture;

   //}


   void shader::bind_source(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture_site *pgputexturesite, int iSlot)
   {

      ::cast<texture> ptexture = pgputexturesite->gpu_texture();

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      auto vkimage = ptexture->m_vkimage;

      //ptexture->set_state(pgpucommandbuffer, ::gpu::e_texture_state_shader_read);

      auto vkimagelayoutSourceTexture = ptexture->mip_layer_state(0, 0).m_vkimagelayout;

      if (vkimagelayoutSourceTexture != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
      {

         throw ::exception(error_wrong_state);

      }

      // auto pshadertexture = this->shader_texture(pgputexture, true);

      // for (int i = 0; i < prenderer->get_frame_count(); i++)
      //{

      // auto& pdescriptorset = s1()->m_imagedescriptorset[image];
      // auto pcommandbuffer = this->getCurrentCommandBuffer();

      //::cast<renderer> prenderer = m_pgpurenderer;

      ::cast < ::gpu_vulkan::renderer> prenderer = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer;

      ::cast<::gpu_vulkan::shader> pshader = prenderer->m_pgpucontext->m_pshaderBound;

      //::cast<command_buffer> pcommandbuffer = ::gpu::current_command_buffer();
      //::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      unsigned int uSet = 0;

      auto pbindingslot = get_first_image_sampler_binding_slot();

      uSet = pbindingslot->m_iSet;

      pbindingslot->m_ptexturesite = ptexture;

      // if (m_bindingSampler.is_set())
      //    uSet = m_bindingSampler.m_uSet;
      // else if (m_bindingCubeSampler.is_set())
      //    uSet = m_bindingCubeSampler.m_uSet;

      auto vkdescriptorset = ptexture->_001DescriptorSet(this, pgpucommandbuffer);

      if ((((::uptr)vkdescriptorset) & 0xffff) == 0x357)
      {

         //vkdescriptorset = ptexture->descriptor_set(this, pgpucommandbuffer);

      }

      // Bind pipeline and descriptor sets
      //      vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //    vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, ...);
      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer,
                              VK_PIPELINE_BIND_POINT_GRAPHICS, // Bind point
                              pshader->m_ppipelineCurrent->_pipeline_layout(), // Layout used when pipeline was created
                              uSet, // First set (set = 0)
                              1, // Descriptor set count
                              &vkdescriptorset, // Pointer to descriptor set
                              0, // Dynamic offset count
                              NULL // Dynamic offsets
      );

   }


   void shader::bind_slot_set(::gpu::command_buffer *pgpucommandbuffer, int iSet,
                              ::gpu::binding_slot_set *pgpubindingslotset)
   {

      ::cast<::gpu_vulkan::binding_slot_set> pbindingslotset = pgpubindingslotset;

      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      ::cast<renderer> prenderer = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer;

      ::cast<::gpu_vulkan::shader> pshader = prenderer->m_pgpucontext->m_pshaderBound;

      auto & vkdescriptorseta = pbindingslotset->descriptor_set(pgpucommandbuffer);

      int iFrameIndex = pgpucommandbuffer->m_iCommandBufferFrameIndex2;

      auto vkpipelinelayout = pshader->m_ppipelineCurrent->_pipeline_layout();

      VkDescriptorSet vkdescriptorset = vkdescriptorseta[iFrameIndex];

      VkDescriptorSet vkdescriptorsetaBind[1];

      vkdescriptorsetaBind[0] = vkdescriptorset;

      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer,
                              VK_PIPELINE_BIND_POINT_GRAPHICS, // Bind point
                              vkpipelinelayout, // Layout used when pipeline was created
                              iSet, // First set (set = 0)
                              1, // Descriptor set count
                              vkdescriptorsetaBind, // Pointer to descriptor set
                              0, // Dynamic offset count
                              NULL // Dynamic offsets
      );

   }


   void shader::bind_block(::gpu::command_buffer *pgpucommandbuffer, ::gpu::block *pgpublock, int iSet)
   {

      
      ::cast<block> pblock = pgpublock;
      ::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      //if (ptexture->m_state.m_vkimagelayout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
      //{
      //   throw ::exception(error_wrong_state);
      //}
      // auto pshadertexture = this->shader_texture(pgputexture, true);

      // for (int i = 0; i < prenderer->get_frame_count(); i++)
      //{


      // auto& pdescriptorset = s1()->m_imagedescriptorset[image];
      // auto pcommandbuffer = this->getCurrentCommandBuffer();


      ::cast<renderer> prenderer = m_pgpurenderer;

      ::cast<::gpu_vulkan::shader> pshader = prenderer->m_pgpucontext->m_pshaderBound;

      //::cast<command_buffer> pcommandbuffer = ::gpu::current_command_buffer();
      //::cast<command_buffer> pcommandbuffer = pgpucommandbuffer;

      unsigned int uSet = iSet;

      auto pbindingslot = binding_slot(iSet, 0);

      uSet = pbindingslot->m_iSet;

      if (pbindingslot->m_pbinding->m_ebinding != ::gpu::e_binding_global_ubo1
         &&
          pbindingslot->m_pbinding->m_ebinding != ::gpu::e_binding_uniform_buffer)
      {

         throw ::exception(error_wrong_state);

      }

      pbindingslot->m_pblock = pblock;

      auto pbindingslotset = binding_slot_set(iSet);

      // if (m_bindingSampler.is_set())
      //    uSet = m_bindingSampler.m_uSet;
      // else if (m_bindingCubeSampler.is_set())
      //    uSet = m_bindingCubeSampler.m_uSet;

      auto vkdescriptorset = pblock->descriptor_set(pbindingslotset, pgpucommandbuffer);

      if ((((::uptr)vkdescriptorset) & 0xffff) == 0x357)
      {

         vkdescriptorset = pblock->descriptor_set(pbindingslotset, pgpucommandbuffer);
      }

      // Bind pipeline and descriptor sets
      //      vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
      //    vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer, ...);
      vkCmdBindDescriptorSets(pcommandbuffer->m_vkcommandbuffer,
                              VK_PIPELINE_BIND_POINT_GRAPHICS, // Bind point
                              pshader->m_ppipelineCurrent->_pipeline_layout(), // Layout used when pipeline was created
                              uSet, // First set (set = 0)
                              1, // Descriptor set count
                              &vkdescriptorset, // Pointer to descriptor set
                              0, // Dynamic offset count
                              NULL // Dynamic offsets
      );

   }





} // namespace gpu_vulkan


// #include "shader.h"
//
//
// namespace gpu_vulkan
//{
//
//
//    const char* shader_type_c_str(VkShaderStageFlagBits etype)
//    {
//
//       switch (etype)
//       {
//       case VK_SHADER_STAGE_VERTEX_BIT:
//          return "VERTEX";
//       case VK_SHADER_STAGE_FRAGMENT_BIT:
//          return "FRAGMENT";
// #if !defined(__APPLE__) && !defined(__ANDROID__)
//       case VK_SHADER_STAGE_GEOMETRY_BIT:
//          return "GEOMETRY";
// #endif
//       default:
//          return "(Unknown Shader Type)";
//       }
//
//    }
//
//
//    shader::shader()
//    {
//
//
//    }
//
//
//    shader::~shader()
//    {
//
//
//    }
//
//
//    unsigned int shader::create_shader(const ::string & strSource, VkShaderStageFlagBits type)
//    {
//
//       //unsigned int uShader;
//
//       //uShader = glCreateShader(type);
//
//       //if(!uShader)
//       //{
//
//       //   GLenum eerror = glGetError();
//       //
//       //   auto errString = vulkan_error_string(eerror);
//
//       //   informationf("error %d \"%s\"", eerror, errString);
//
//       //   return ::error_failed;
//
//       //}
//
//       //const char * sza[1];
//
//       //sza[0] = strSource;
//
//       //glShaderSource(uShader, 1, sza, NULL);
//
//       //glCompileShader(uShader);
//
//       //information() << "compiling shader : " << sza[0];
//
//       ////string strSummary;
//
//       ////::e_status estatus =
//       //
//       //shader_compile_errors(uShader, type, m_strError);
//
//       ////if (!estatus)
//       ////{
//
//       ////   return 0;
//
//       ////}
//       //m_strError.empty();
//
//       //return uShader;
//
//       return 0;
//
//
//    }
//
//
//    void shader::create_shader(const ::string & pszVertex, const ::string & pszFragment, const ::string & pszGeometry)
//    {
//
//       unsigned int uVertex = create_shader(pszVertex, VK_SHADER_STAGE_VERTEX_BIT);
//
//       unsigned int uFragment = create_shader(pszFragment, VK_SHADER_STAGE_FRAGMENT_BIT);
//
// #if !defined(__APPLE__) && !defined(__ANDROID__)
//
//       unsigned int uGeometry = 0;
//
//       bool bGeometry = pszGeometry.trimmed().has_character();
//
//       if (bGeometry)
//       {
//
//          uGeometry = create_shader(pszGeometry, VK_SHADER_STAGE_GEOMETRY_BIT);
//
//       }
//
// #endif
//
////      m_uId = glCreateProgram();
////
////      glAttachShader(m_uId, uVertex);
////
////      glAttachShader(m_uId, uFragment);
////
////#if !defined(__APPLE__) && !defined(__ANDROID__)
////
////      if (bGeometry)
////      {
////
////         glAttachShader(m_uId, uGeometry);
////
////      }
////
////#endif
////
////      glLinkProgram(m_uId);
////
////      // Validate program
//////      glValidateProgram(m_uId);
////
////      string strSummary;
////
////      ///::e_status estatus =
////
////      program_compile_errors(m_uId, strSummary);
////
////      //if(estatus.succeeded())
////      //{
////
////      //   return estatus;
////
////      //}
////
////      // delete the shaders
////      // they're linked into our program
////
////      glDeleteShader(uVertex);
////
////      glDeleteShader(uFragment);
////
////#if !defined(__APPLE__) && !defined(__ANDROID__)
////
////      if (bGeometry)
////      {
////
////         glDeleteShader(uGeometry);
////
////      }
////
////#endif
////
//      //return ::success;
//
//   }
//
//
//   // activate the shader
//   // ------------------------------------------------------------------------
//   void shader::use()
//   {
//
//      //glUseProgram(m_uId);
//
//   }
//
//
//   void shader::setBool(const ::scoped_string & scopedstrName, bool b)
//   {
//
//      setInt(scopedstrName, b != false);
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1i(i, (int)value);
//
//   }
//
//
//   void shader::setInt(const ::scoped_string & scopedstrName, int i)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if(!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &i, sizeof(int));
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1i(i, value);
//
//      //int i = value != 0;
//      //m_memoryPushConstants.append(&i, sizeof(int));
//
//
//   }
//
//
//   void shader::setFloat(const ::scoped_string & scopedstrName, float f)
//   {
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1f(i, value);
//      //int i = value != 0;
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &f, sizeof(float));
//
//   }
//
//
////   void shader::setVec2(const ::scoped_string & scopedstrName, const floating_sequence2& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform2fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec2(const ::scoped_string & scopedstrName, float x, float y)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//
//   }
//
//
////   void shader::setVec3(const ::scoped_string & scopedstrName, const floating_sequence3& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform3fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec3(const ::scoped_string & scopedstrName, float x, float y, float z)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 2*sizeof(float), &z, sizeof(float));
//
//   }
//
//
////   void shader::setVec4(const ::scoped_string & scopedstrName, const floating_sequence4& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform4fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec4(const ::scoped_string & scopedstrName, float x, float y, float z, float w)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 2 * sizeof(float), &z, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 3 * sizeof(float), &w, sizeof(float));
//
//   }
//
//
//   void shader::setMat2(const ::scoped_string & scopedstrName, const float a[2*2])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, (void *) a, 2*2*sizeof(float));
//
//   }
//
//
//   void shader::setMat3(const ::scoped_string & scopedstrName, const float a[3*3])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, a, 3 * 3 * sizeof(float));
//
//   }
//
//
//   void shader::setMat4(const ::scoped_string & scopedstrName, const float a[4*4])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, a, 4 * 4 * sizeof(float));
//
//   }
//
//
//   void shader::shader_compile_errors(int shader, VkShaderStageFlagBits type, string & strSummary)
//   {
//
//      //::e_status estatus = ::success;
//
//      //GLint success = 0;
//
//      //GLchar infoLog[1024];
//
//      //glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//
//      //if (success)
//      //{
//
//      //   strSummary.formatf("SUCCESS::SHADER_COMPILATION (%s) \n --
//      --------------------------------------------------- -- \n", shader_type_c_str(type));
//
//      //}
//      //else
//      //{
//
//      //   glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
//
//      //   const char * psz = shader_type_c_str(type);
//
//      //   const char * pszLog = infoLog;
//
//      //   strSummary.formatf("error::SHADER_COMPILATION_ERROR of type: %s \n %s \n --
//      --------------------------------------------------- -- \n", psz, pszLog);
//
//      //   warning() << strSummary;
//
//      //   throw ::exception(error_failed, "Shader Compilation Error", strSummary);
//
//      //}
//
//      //information() << strSummary;
//
//      ////return estatus;
//
//   }
//
//
//   void shader::program_compile_errors(int program, string & strSummary)
//   {
//
//      ////::e_status estatus = ::success;
//
//      //GLint success;
//
//      //GLchar infoLog[1024];
//      //
//      //glGetProgramiv(program, GL_LINK_STATUS, &success);
//
//      //if (success)
//      //{
//
//      //   strSummary.formatf("SUCCESS::PROGRAM_LINKING \n -- --------------------------------------------------- --
//      \n");
//
//      //}
//      //else
//      //{
//
//      //   glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
//
//      //   strSummary.formatf("error::PROGRAM_LINKING_ERROR : \n %s \n --
//      --------------------------------------------------- -- \n", infoLog);
//
//      //   throw ::exception(error_failed);
//
//      //}
//
//      //information() << strSummary;
//
//      ////return estatus;
//
//   }
//
//
//   //::gpu::payload * shader::get_payload(const ::scoped_string & scopedstrPayload)
//   //{
//   //
//   //   auto uniform = glGetUniformLocation(m_uId, strUniform);
//
//   //   return uniform;
//
//   //}
//
//
//} // namespace gpu_vulkan
//
//
//
