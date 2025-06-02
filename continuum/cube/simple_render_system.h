#pragma once


#include "aura/graphics/gpu/properties.h"
#include "aura/graphics/graphics3d/scene_system.h"
//#include "context.h"
//#include "pipeline.h"
//#include "scene_object.h"
//#include "frame_info.h"
//#include "aura/graphics/graphics3d/camera.h"


//
//#include <memory>
//#include <vector>



namespace app_graphics3d_continuum
{


	//struct SimplePushConstantData {
	//	glm::mat4 modelMatrix{ 1.f };
	//	glm::mat4 normalMatrix{ 1.f };
	//};


	inline ::gpu::properties simple_render_properties()
	{

		static ::gpu::property s_properties[] =
		{
			{"modelMatrix", ::gpu::e_type_mat4},
			{"normalMatrix", ::gpu::e_type_mat4},
			{ nullptr, ::gpu::e_type_none}

		};

		return s_properties;

	}
	

	class simple_render_system:
		virtual public ::graphics3d::scene_system 
	{
	public:
		

		simple_render_system();
		~simple_render_system();


		virtual void initialize_simple_render_system(::graphics3d::engine* pengine);



		virtual void update(::gpu::context* pgpucontext, ::graphics3d::scene* pscene) override;
		virtual void render(::gpu::context* pgpucontext, ::graphics3d::scene* pscene) override;


		//void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		//void createPipeline(VkRenderPass renderPass);

		virtual void prepare(::gpu::context* pgpucontext) override;


		//virtual void renderGameObjects(::graphics3d::scene* pscene);
	//private:
		//virtual void createPipelineLayout();
		//virtual void createPipeline();

	};


} // namespace app_graphics3d_continuum




