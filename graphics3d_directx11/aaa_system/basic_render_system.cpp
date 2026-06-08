#include "framework.h"
#include "basic_render_system.h"
// libs
	







#include <string>
#include <array>
#include <cassert>
#include <stdexcept>


namespace graphics3d_directx11 {
	struct SimplePushConstantData {
		floating_matrix4 modelMatrix{ 1.f };
		floating_matrix4 normalMatrix{ 1.f };
	};


	SimpleRenderSystem::SimpleRenderSystem(::gpu::context * pdevice, VkRenderPass renderPass, aaaVkDescriptorSetLayout globalSetLayout)
		: m_pgpucontext{ pdevice } {
		createPipelineLayout(globalSetLayout);
		createPipeline(renderPass);

	}

	SimpleRenderSystem::~SimpleRenderSystem() {
		vkDestroyPipelineLayout(m_pgpucontext->logicalDevice(), pipelineLayout, nullptr);
	}



	void SimpleRenderSystem::createPipelineLayout(aaaVkDescriptorSetLayout globalSetLayout) {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		::array<aaaVkDescriptorSetLayout> aaadescriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(aaadescriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = aaadescriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(m_pgpucontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw ::exception(error_failed, "Failed to create pipeline layout");
		}

	}


	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) 
	{

		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		pipeline_configuration pipelineconfiguration{};
		pipeline::default_pipeline_configuration(pipelineconfiguration);

		pipelineconfiguration.renderPass = renderPass;
		pipelineconfiguration.pipelineLayout = pipelineLayout;

		// Construct paths using PROJECT_ROOT_DIR
		std::string vertShaderPath = "matter://Shaders/SpirV/vert.vert.spv";
		std::string fragShaderPath = "matter://Shaders/SpirV/frag.frag.spv";

		m_ppipeline = allocateø pipeline();
		
		m_ppipeline->initialize_pipeline(
			m_pgpucontext,
			vertShaderPath.c_str(),
			fragShaderPath.c_str(),
			pipelineconfiguration
		);
	}


	void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo) {

		m_ppipeline->bind(frameInfo.commandBuffer);
		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0, 1,
			&frameInfo.globalDescriptorSet,
			0, nullptr
		);

		for (auto& kv : frameInfo.gameObjects) {
			auto& obj = kv.element2();
			SimplePushConstantData push{};
			push.modelMatrix = obj->m_transform.mat4();
			push.normalMatrix = obj->m_transform.normalMatrix();

			vkCmdPushConstants(
				frameInfo.commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);

			if (obj->m_pmodel)
			{
				obj->m_pmodel->bind(&frameInfo);
				obj->m_pmodel->draw(&frameInfo);
			}
		}
	}
}