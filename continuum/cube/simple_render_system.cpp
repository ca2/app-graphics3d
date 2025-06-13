#include "framework.h"
#include "application.h"
#include "bred/graphics3d/scene.h"
#include "bred/gpu/approach.h"
#include "bred/gpu/context.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/shader.h"
#include "aura/user/user/graphics3d.h"
#include "simple_render_system.h"
// libs
#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <string>
#include <array>
#include <cassert>
#include <stdexcept>


namespace app_graphics3d_continuum
{
	//struct SimplePushConstantData {
	//	glm::mat4 modelMatrix{ 1.f };
	//	glm::mat4 normalMatrix{ 1.f };
	//};

	///::gpu::property * simple_

	//simple_render_system::simple_render_system(context * pdevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
	simple_render_system::simple_render_system()
	{

	}
		



	simple_render_system::~simple_render_system() {
		/*vkDestroyPipelineLayout(m_pgpucontext->logicalDevice(), pipelineLayout, nullptr);*/
	}


	void simple_render_system::initialize_simple_render_system(::graphics3d::engine* pengine)
	{

		initialize_scene_system(pengine);
		
		//m_pgpucontext = pgpucontext;

	/*	m_pshader = m_pgpucontext->create_shader(
			pgpucontext,
			"matter://Shaders/vert.vert",
			"matter://Shaders/frag.frag",
			simple_render_properties()
			);*/

		//std::string vertShaderPath = 
		//std::string fragShaderPath = "matter://Shaders/SpirV/frag.frag.spv";


		//createPipelineLayout(globalSetLayout);
		//createPipeline(renderPass);

	//}
		//std::string vertShaderPath = 
		//std::string fragShaderPath = "matter://Shaders/SpirV/frag.frag.spv";


		//createPipelineLayout(globalSetLayout);
		//createPipeline(renderPass);

	}


	void simple_render_system::prepare(::gpu::context* pgpucontext)
	{

		m_pshader = pgpucontext->m_pgpurenderer->create_shader(
			"matter://shaders/vert.vert",
			"matter://shaders/frag.frag",
			{ ::gpu::shader::e_descriptor_set_slot_global,
			::gpu::shader::e_descriptor_set_slot_local }, {}, {},
			simple_render_properties()
		);

		//std::string vertShaderPath = 
		//std::string fragShaderPath = "matter://Shaders/SpirV/frag.frag.spv";


		//createPipelineLayout(globalSetLayout);
		//createPipeline(renderPass);

	}
	//void simple_render_system::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {

	//	VkPushConstantRange pushConstantRange{};
	//	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	//	pushConstantRange.offset = 0;
	//	pushConstantRange.size = sizeof(SimplePushConstantData);

	//	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

	//	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	//	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	//	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	//	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	//	pipelineLayoutInfo.pushConstantRangeCount = 1;
	//	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	//	if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
	//		VK_SUCCESS) {
	//		throw ::exception(error_failed, "Failed to create pipeline layout");
	//	}

	//}


	//void simple_render_system::createPipeline(VkRenderPass renderPass) 
	//{

	//	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

	//	PipelineConfigInfo pipelineConfig{};
	//	pipeline::defaultPipelineConfigInfo(pipelineConfig);

	//	pipelineConfig.renderPass = renderPass;
	//	pipelineConfig.pipelineLayout = pipelineLayout;

	//	// Construct paths using PROJECT_ROOT_DIR
	//	std::string vertShaderPath = "matter://Shaders/SpirV/vert.vert.spv";
	//	std::string fragShaderPath = "matter://Shaders/SpirV/frag.frag.spv";

	//	m_ppipeline = __allocate pipeline();
	//	
	//	m_ppipeline->initialize_pipeline(
	//		m_pgpucontext,
	//		vertShaderPath.c_str(),
	//		fragShaderPath.c_str(),
	//		pipelineConfig
	//	);
	//}

	void simple_render_system::update(::gpu::context* pgpucontext, ::graphics3d::scene* pscene)
	{


	}


	void simple_render_system::render(::gpu::context * pgpucontext, ::graphics3d::scene* pscene)
	{

		m_pshader->bind();

	////	vkCmdBindDescriptorSets(
	////		frameInfo.commandBuffer,
	////		VK_PIPELINE_BIND_POINT_GRAPHICS,
	////		pipelineLayout,
	////		0, 1,
	////		&frameInfo.globalDescriptorSet,
	////		0, nullptr
	////	);

		for (auto& kv : pscene->m_mapObjects) 
		{

			auto& obj = kv.element2();

			//SimplePushConstantData push{};
			//push.modelMatrix = obj->m_transform.mat4();
			//push.normalMatrix = obj->m_transform.normalMatrix();

			//vkCmdPushConstants(
			//	frameInfo.commandBuffer,
			//	pipelineLayout,
			//	VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			//	0,
			//	sizeof(SimplePushConstantData),
			//	&push);

			if (obj && obj->m_pmodel)
			{

				auto pszPath = obj->m_strPath.c_str();

				auto modelMatrix = m_pengine->model_matrix(obj->m_transform);

				m_pshader->m_properties["modelMatrix"] = modelMatrix;

				auto normalMatrix = m_pengine->normal_matrix(obj->m_transform);

				m_pshader->m_properties["normalMatrix"] = normalMatrix;

				m_pshader->push_properties();
				
				obj->m_pmodel->bind(pgpucontext);

				obj->m_pmodel->draw(pgpucontext);

				obj->m_pmodel->unbind(pgpucontext);

			}

		}

		m_pshader->unbind();

	}


} // namespace app_graphics3d_continuum 



