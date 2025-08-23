#pragma once
//#include <string>
//#include <vector>
//
//
//#include "context.h"
//
//
//namespace graphics3d_directx12 
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
//	class CLASS_DECL_GRAPHICS3D_DIRECTX12 pipeline :
//	virtual public ::particle
//	{
//	public:
//
//
//		pipeline();
//		virtual void initialize_pipeline(
//			::gpu::context * pgpucontext,
//			const ::file::path& pathVert,
//			const ::file::path& pathFrag,
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
//			const ::file::path& pathVert,
//			const ::file::path& pathFrag,
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
//} // namespace graphics3d_directx12 
//
//
//
