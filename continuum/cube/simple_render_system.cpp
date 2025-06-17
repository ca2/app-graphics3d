#include "framework.h"
#include "application.h"
#include "bred/graphics3d/scene.h"
#include "bred/gpu/approach.h"
#include "bred/gpu/context.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/shader.h"
#include "bred/user/user/graphics3d.h"
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
		
	}


	void simple_render_system::prepare(::gpu::context* pgpucontext)
	{

		auto prenderer = pgpucontext->m_pgpurendererOutput2;

		m_pshader = prenderer->create_shader(
			"matter://shaders/vert.vert",
			"matter://shaders/frag.frag",
			{ ::gpu::shader::e_descriptor_set_slot_global,
			::gpu::shader::e_descriptor_set_slot_local }, {}, {},
			simple_render_properties()
		);



	}


	void simple_render_system::update(::gpu::context* pgpucontext, ::graphics3d::scene* pscene)
	{


	}


	void simple_render_system::render(::gpu::context * pgpucontext, ::graphics3d::scene* pscene)
	{

		m_pshader->bind();

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

				m_pshader->m_propertiesPush["modelMatrix"] = modelMatrix;

				auto normalMatrix = m_pengine->normal_matrix(obj->m_transform);

				m_pshader->m_propertiesPush["normalMatrix"] = normalMatrix;

				m_pshader->push_properties();
				
				obj->m_pmodel->bind();

				obj->m_pmodel->draw();

				obj->m_pmodel->unbind();

			}

		}

		m_pshader->unbind();

	}


} // namespace app_graphics3d_continuum 



