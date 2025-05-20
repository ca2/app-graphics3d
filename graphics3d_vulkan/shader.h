// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#pragma once



#include "app-cube/cube/graphics3d/shader.h"


namespace graphics3d_vulkan
{


	class CLASS_DECL_GRAPHICS3D_VULKAN shader :
		virtual public ::graphics3d::shader
	{
	public:


		::pointer < pipeline > m_ppipeline;


		shader();
		~shader();


		//void bind(VkCommandBuffer commandBuffer);

		///static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
	///private:
///		static std::vector<char> readFile(const std::string& filepath);

		/*void createGraphicsPipeline(
			const std::string& vertFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& configInfo);

		void createShaderModule(const ::block& block, VkShaderModule* shaderModule);*/

		/*
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;*/
		VkPipelineLayout		m_vkpipelinelayout;


		void _create_pipeline_layout(int iSize);
		
		void draw() override;

		//void initialize_shader(
		//	::graphics3d::context* pcontext,
		//	const ::file::path& pathVert,
		//	const ::file::path& pathFrag,
		//	const ::graphics3d::property* pproperties) override;

		void on_initialize_shader() override;


		void bind() const override;


		void push_properties() override;


	};


} // namespace graphics3d_vulkan 



