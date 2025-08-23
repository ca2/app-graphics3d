#pragma once


#include "context.h"


namespace gpu_directx12
{


   struct pipeline_configuration
   {

      /*::array<VkVertexInputBindingDescription> bindingDescriptions{};
      ::array<VkVertexInputAttributeDescription> attributeDescriptions{};
      VkPipelineViewportStateCreateInfo viewportInfo;
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
      VkPipelineRasterizationStateCreateInfo rasterizationInfo;
      VkPipelineMultisampleStateCreateInfo multisampleInfo;
      ::array < VkPipelineColorBlendAttachmentState > colorBlendAttachments;
      VkPipelineColorBlendStateCreateInfo colorBlendInfo;
      VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
      ::comparable_array<VkDynamicState> dynamicStateEnables;
      VkPipelineDynamicStateCreateInfo dynamicStateInfo;
      VkPipelineLayout pipelineLayout = nullptr;
      VkRenderPass renderPass = nullptr;*/
      uint32_t subpass = 0;
   };

   class CLASS_DECL_GPU_DIRECTX12 pipeline :
      virtual public ::particle
   {
   public:


      ::pointer < renderer > m_pgpurenderer;
      //VkPipeline graphicsPipeline;
      //VkShaderModule vertShaderModule;
      //VkShaderModule fragShaderModule;



      pipeline();
      //virtual void initialize_pipeline(
      //   ::gpu::context * pgpucontext,
      //   const ::file::path & pathVert,
      //   const ::file::path & pathFrag,
      //   const pipeline_configuration & configInfo);
      virtual void initialize_pipeline(
         ::gpu::renderer * pgpurenderer,
         const ::block & blockVertex,
         const ::block & blockFragment,
         const pipeline_configuration & configInfo);
      ~pipeline();


      //void bind(VkCommandBuffer commandBuffer);

      static void default_pipeline_configuration(pipeline_configuration & configInfo);
   //private:
      //static ::array<char> readFile(const ::string & filepath);

      void createGraphicsPipeline(
         const ::block & blockVertex,
         const ::block & blockFragment,
         const pipeline_configuration & configInfo);

      //void createShaderModule(const ::block & block, VkShaderModule * shaderModule);
      void createShaderModule(const ::block& block, void* shaderModule);

   };


} // namespace gpu_directx12 



