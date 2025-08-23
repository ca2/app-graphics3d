#pragma once
//#include <string>
//#include <vector>
//
//
//#include "context.h"
//
//
//namespace graphics3d_opengl 
//{
//
//
//	struct pipeline_configuration
//	{
//
//
//		pipeline_configuration() = default;
//		pipeline_configuration(const pipeline_configuration&) = delete;
//		pipeline_configuration& operator=(const pipeline_configuration&) = delete;
//
//
//		::array<VkVertexInputBindingDescription> bindingDescriptions{};
//		::array<VkVertexInputAttributeDescription> attributeDescriptions{};
//		VkPipelineViewportStateCreateInfo viewportInfo;
//		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
//		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
//		VkPipelineMultisampleStateCreateInfo multisampleInfo;
//		VkPipelineColorBlendAttachmentState colorBlendAttachment;
//		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
//		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
//		::array<VkDynamicState> dynamicStateEnables;
//		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
//		VkPipelineLayout pipelineLayout = nullptr;
//		VkRenderPass renderPass = nullptr;
//		uint32_t subpass = 0;
//	};
//
//	class CLASS_DECL_GRAPHICS3D_OPENGL pipeline :
//	virtual public ::particle
//	{
//	public:
//
//
//		pipeline();
//		virtual void initialize_pipeline(
//			context * pvkcdevice,
//			const std::string& vertFilepath,
//			const std::string& fragFilepath,
//			const pipeline_configuration& configInfo);
//		~pipeline();
//
//		pipeline(const pipeline&) = delete;
//		void operator=(const pipeline&) = delete;
//
//		void bind(VkCommandBuffer commandBuffer);
//		
//		static void default_pipeline_configuration(pipeline_configuration& configInfo);
//	private:
//		static ::array<char> readFile(const std::string& filepath);
//
//		void createGraphicsPipeline(
//			const std::string& vertFilepath,
//			const std::string& fragFilepath,
//			const pipeline_configuration& configInfo);
//
//		void createShaderModule(const ::block & block, VkShaderModule* shaderModule);
//
//		::pointer < context > m_pgpucontext;
//		VkPipeline graphicsPipeline;
//		VkShaderModule vertShaderModule;
//		VkShaderModule fragShaderModule;
//	};
//
//
//} // namespace graphics3d_opengl 
//
//
//
