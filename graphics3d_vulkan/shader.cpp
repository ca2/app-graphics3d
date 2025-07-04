// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include "framework.h"
//#include "shader.h"
//#include "context.h"
//#include "descriptors.h"
//#include "engine.h"
//#include "pipeline.h"
//#include "renderer.h"
//#include "bred/user/user/graphics3d.h"
//
//
//namespace graphics3d_vulkan
//{
//
//
//   shader::shader()
//   {
//
//
//
//   }
//
//
//   shader::~shader()
//   {
//
//      ::cast < context > pgpucontext = m_pgpucontext;
//
//      vkDestroyPipelineLayout(pgpucontext->logicalDevice(), m_vkpipelinelayout, nullptr);
//
//   }
//
//
//   void shader::_create_pipeline_layout(int iSize)
//   {
//
//      ::cast < context > pgpucontext = m_pgpucontext;
//
//      ::cast < engine > pengine = pgpucontext->m_pimpact->m_pengine;
//
//      VkPushConstantRange pushConstantRange{};
//      pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//      pushConstantRange.offset = 0;
//      //pushConstantRange.size = sizeof(PointLightPushConstants);
//      pushConstantRange.size = iSize;
//
//      auto globalSetLayout = pengine->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();
//
//      ::array<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };
//
//      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
//      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//      pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
//      pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
//      pipelineLayoutInfo.pushConstantRangeCount = 1;
//      pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
//      //pipelineLayoutInfo.pPushConstantRanges = nullptr;
//      if (vkCreatePipelineLayout(
//         pgpucontext->logicalDevice(),
//         &pipelineLayoutInfo,
//         nullptr,
//         &m_vkpipelinelayout) !=
//         VK_SUCCESS)
//      {
//         throw ::exception(error_failed, "failed to create pipeline layout!");
//      }
//
//   }
//
//
//   void shader::on_initialize_shader()
//   {
//
//      _create_pipeline_layout(m_properties.m_memory.size());
//
//      __construct_new(m_ppipeline);
//
//      ::cast <engine> pengine = m_pgpucontext->m_pimpact->m_pengine;
//
//      PipelineConfigInfo pipelineConfig{};
//      pipeline::defaultPipelineConfigInfo(pipelineConfig);
//      if (m_eflag & e_flag_clear_default_bindings_and_attributes_descriptions)
//      {
//         pipelineConfig.attributeDescriptions.clear();
//         pipelineConfig.bindingDescriptions.clear();
//      }
//      pipelineConfig.renderPass = pengine->m_prenderer->m_pvkcrenderpass->m_vkrenderpass;
//      pipelineConfig.pipelineLayout = m_vkpipelinelayout;
//
//
//      m_ppipeline->initialize_pipeline(m_pgpucontext,
//         pengine->_translate_shader_path(m_pathVert),
//         pengine->_translate_shader_path(m_pathFrag),
//         pipelineConfig);
//
//
//   }
//
//
//   void shader::bind() const
//   {
//
//      ::cast <engine> pengine = m_pgpucontext->m_pimpact->m_pengine;
//
//      auto pcommandbuffer = pengine->m_prenderer->getCurrentCommandBuffer();
//
//      m_ppipeline->bind(pcommandbuffer->m_vkcommandbuffer);
//
//      auto globalDescriptorSet = pengine->getcurrentDescriptorSet();
//
//      vkCmdBindDescriptorSets(
//         pcommandbuffer->m_vkcommandbuffer,
//         VK_PIPELINE_BIND_POINT_GRAPHICS,
//         m_vkpipelinelayout,
//         0,
//         1,
//         &globalDescriptorSet,
//         0,
//         nullptr);
//
//   }
//
//
//   void shader::push_properties()
//   {
//
//      ::cast <engine> pengine = m_pgpucontext->m_pimpact->m_pengine;
//
//      auto pcommandbuffer = pengine->m_prenderer->getCurrentCommandBuffer();
//
//      vkCmdPushConstants(
//         pcommandbuffer->m_vkcommandbuffer,
//         m_vkpipelinelayout,
//         VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
//         0,
//         m_properties.size(),
//         m_properties.data());
//
//   }
//
//
//   void shader::draw()
//   {
//
//      ::cast <engine> pengine = m_pgpucontext->m_pimpact->m_pengine;
//
//      auto pcommandbuffer = pengine->m_prenderer->getCurrentCommandBuffer();
//
//      vkCmdDraw(pcommandbuffer->m_vkcommandbuffer, 6, 1, 0, 0);
//
//   }
//
//
//} // namespace graphics3d_vulkan
//
//
