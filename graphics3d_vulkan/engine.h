// From application_object by camilo on 2025-05-17 01:09 <3ThomasBorregaardSorensen!!
#pragma once


#include "app-cube/cube/graphics3d/engine.h"


#include "scene_object.h"


// libs
#include <glm/gtc/matrix_transform.hpp>

//// std
//#include <memory>
//#include <unordered_map>


namespace graphics3d_vulkan 
{

	//class SimpleRenderSystem;
	//class point_light_system;
	class buffer;


	class CLASS_DECL_GRAPHICS3D_VULKAN engine :
		virtual public ::graphics3d::engine
	{
	public:


		//::pointer < context >             m_pgpucontext;
		::pointer < ::gpu_vulkan::renderer >		m_prenderer;



		engine();
		~engine() override;


		//::file::path _translate_shader_path(const ::file::path& pathShader);


		void run() override;

		void on_begin_frame() override;
		void on_start_engine() override;

		void create_global_ubo(::gpu::context* pgpucontext) override;
		void update_global_ubo(::gpu::context* pgpucontext) override;
		//void on_render_frame() override;



		
	};

}