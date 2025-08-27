#include "framework.h"
#include "command_buffer.h"
#include "pipeline.h"
#include "renderer.h"
#include "acme/platform/application.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/file_context.h"
#include <assert.h>

using namespace vulkan;

namespace gpu_vulkan
{


   pipeline::pipeline()
   {

      m_vkpipelineGraphics = VK_NULL_HANDLE;
      m_vkshadermoduleVertex = VK_NULL_HANDLE;
      m_vkshadermoduleFragment = VK_NULL_HANDLE;
      m_vkpipelinelayout = VK_NULL_HANDLE;

   }


   pipeline::~pipeline()
   {

      ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;


      vkDestroyPipelineLayout(pgpucontext->logicalDevice(), m_vkpipelinelayout, nullptr);
      vkDestroyShaderModule(pgpucontext->logicalDevice(), m_vkshadermoduleFragment, nullptr);
      vkDestroyShaderModule(pgpucontext->logicalDevice(), m_vkshadermoduleVertex, nullptr);
      vkDestroyPipeline(pgpucontext->logicalDevice(), m_vkpipelineGraphics, nullptr);

   }



   VkPipelineLayout pipeline::_pipeline_layout()
   {

      return m_vkpipelinelayout;

   }


   void pipeline::initialize_graphics_pipeline(
      ::gpu::renderer * pgpurenderer,
      const ::block & blockVertexShader,
      const ::block & blockFragmentShader,
      const pipeline_configuration & pipelineconfiguration)
   {

      initialize(pgpurenderer);


      m_pgpurenderer = pgpurenderer;

      create_graphics_pipeline(
         blockVertexShader,
         blockFragmentShader,
         pipelineconfiguration);


   }



   
   void pipeline::bind(command_buffer * pcommandbuffer)
   {

      vkCmdBindPipeline(pcommandbuffer->m_vkcommandbuffer,
         VK_PIPELINE_BIND_POINT_GRAPHICS, m_vkpipelineGraphics);

   }

   //::array<char> pipeline::readFile(const ::string & filepath) {


   //   std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

   //   if (!file.is_open()) {
   //      throw::exception(error_failed, "failed to open file: " + filepath);
   //   }

   //   size_t fileSize = static_cast<size_t>(file.tellg());
   //   ::array<char> buffer(fileSize);

   //   file.seekg(0);
   //   file.read(buffer.data(), fileSize);

   //   file.close();
   //   return buffer;
   //}
   void pipeline::create_graphics_pipeline(
      const ::block & blockVertexShader,
      const ::block & blockFragmentShader,
      const pipeline_configuration & pipelineconfiguration)
   {
      assert(
         pipelineconfiguration.renderPass != VK_NULL_HANDLE &&
         "Cannot create graphics pipeline: no renderPass provided in pipelineconfiguration");

      ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;
      ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;

      create_shader_module(blockVertexShader, &m_vkshadermoduleVertex);
      create_shader_module(blockFragmentShader, &m_vkshadermoduleFragment);
      VkPipelineShaderStageCreateInfo shaderStages[2];
      shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
      shaderStages[0].module = m_vkshadermoduleVertex;
      shaderStages[0].pName = "main";
      shaderStages[0].flags = 0;
      shaderStages[0].pNext = nullptr;
      shaderStages[0].pSpecializationInfo = nullptr;
      shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
      shaderStages[1].module = m_vkshadermoduleFragment;
      shaderStages[1].pName = "main";
      shaderStages[1].flags = 0;
      shaderStages[1].pNext = nullptr;
      shaderStages[1].pSpecializationInfo = pipelineconfiguration.fragSpecInfo;

      auto & bindingDescriptions = pipelineconfiguration.bindingDescriptions;
      auto & attributeDescriptions = pipelineconfiguration.attributeDescriptions;

      VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
      vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
      vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
      vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
      vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

      if (pipelineconfiguration.pipelineLayout)
      {

         m_vkpipelinelayout = pipelineconfiguration.pipelineLayout;

      }
      else
      {

         VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
         pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
         pipelineLayoutInfo.setLayoutCount = (uint32_t)pipelineconfiguration.descriptorSetLayouts.size();
         pipelineLayoutInfo.pSetLayouts = pipelineconfiguration.descriptorSetLayouts.data();
         pipelineLayoutInfo.pushConstantRangeCount = (uint32_t)pipelineconfiguration.pushConstantRanges.size();
         pipelineLayoutInfo.pPushConstantRanges = pipelineconfiguration.pushConstantRanges.data();

         VkPipelineLayout vkpipelinelayout = VK_NULL_HANDLE;

         auto vkresultCreatePipelineLayout =
            vkCreatePipelineLayout(pgpucontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &vkpipelinelayout);

         pgpudevice->_defer_throw_vkresult(vkresultCreatePipelineLayout);

         m_vkpipelinelayout = vkpipelinelayout;

      }

      VkGraphicsPipelineCreateInfo graphicspipelinecreateinfo{};
      graphicspipelinecreateinfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      graphicspipelinecreateinfo.stageCount = 2;
      graphicspipelinecreateinfo.pStages = shaderStages;
      graphicspipelinecreateinfo.pVertexInputState = &vertexInputInfo;
      graphicspipelinecreateinfo.pInputAssemblyState = &pipelineconfiguration.inputAssemblyInfo;
      graphicspipelinecreateinfo.pViewportState = &pipelineconfiguration.viewportInfo;
      graphicspipelinecreateinfo.pRasterizationState = &pipelineconfiguration.rasterizationInfo;
      graphicspipelinecreateinfo.pMultisampleState = &pipelineconfiguration.multisampleInfo;
      graphicspipelinecreateinfo.pColorBlendState = &pipelineconfiguration.colorBlendInfo;
      graphicspipelinecreateinfo.pDepthStencilState = &pipelineconfiguration.depthStencilInfo;
      graphicspipelinecreateinfo.pDynamicState = &pipelineconfiguration.dynamicStateInfo;
      

      graphicspipelinecreateinfo.layout = m_vkpipelinelayout;
      graphicspipelinecreateinfo.renderPass = pipelineconfiguration.renderPass;
      graphicspipelinecreateinfo.subpass = pipelineconfiguration.subpass;

      graphicspipelinecreateinfo.basePipelineIndex = -1;
      graphicspipelinecreateinfo.basePipelineHandle = VK_NULL_HANDLE;

      auto vkresultCreateGraphicsPipelines = vkCreateGraphicsPipelines(
         pgpucontext->logicalDevice(),
         VK_NULL_HANDLE,
         1,
         &graphicspipelinecreateinfo,
         nullptr,
         &m_vkpipelineGraphics);

      pgpudevice->_defer_throw_vkresult(
         vkresultCreateGraphicsPipelines,
         "Failed to create graphics pipeline");

   }

   void pipeline::create_shader_module(const block & block, VkShaderModule * shaderModule)
   {

      VkShaderModuleCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
      createInfo.codeSize = block.size();
      createInfo.pCode = reinterpret_cast<const uint32_t *>(block.data());
      ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;
      ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;
      if (vkCreateShaderModule(pgpucontext->logicalDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
         throw ::exception(error_failed, "failed to create shader module");
      }

   }



} // namespace gpu_vulkan 



