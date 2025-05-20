#pragma once


#include "app-cube/cube/graphics3d/engine.h"


//#include <memory>
//#include <vector>
//#include "Core/Window.h"
#include "input.h"
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h" // Include the ::graphics3d::camera header
//#include "application.h"
//#include "apex/platform/app_consumer.h"
#include "aura/graphics/gpu/render.h"


namespace graphics3d_opengl
{
	class SimpleRenderSystem;
	class point_light_system;

	class engine :
		virtual public ::graphics3d::engine
	{
	public:

		void defer_start() override;
		//::pointer < ::cube::impact > m_pimpact;
		::pointer < ::graphics3d_opengl::renderer > m_prenderer;
		::pointer < ::graphics3d::camera > m_pcamera;
		//::pointer < glc::Application > m_pglcapplication;  // Game object that manages the scenes
		::pointer < ::graphics3d::input > m_pinput;
		//::pointer<::graphics3d_opengl::render_data> m_prenderdataCurrentScene;

		//double m_Δx;
		//double m_Δy;

		double m_dMouseLastX = 0.0;
		double m_dMouseLastY = 0.0;
		double m_Δx = 0.;
		double m_Δy = 0.;


		bool m_Running;

		//bool m_bWireframeMode = false;
		//::pointer < SimpleRenderSystem > m_psimpleRenderSystem;
		//::pointer < point_light_system > m_ppointLightSystem;

		::memory m_memoryBuffer;




		bool m_bInitRenderData;


		//::pointer<::gpu::context>             m_pgpucontext;

		GLuint m_globalUBO;

		engine();
		~engine();

		
		::file::path _translate_shader_path(const ::file::path& pathShader) override;

		void on_start_engine() override;

		void on_begin_frame() override;

		void create_global_ubo() override;
		//Application();
		//~Application();

		//void Init();       // Initialize the game and load the first scene
		///void Update(float deltaTime, ::graphics3d::camera* pcamera);  // Update game logic
		void Render(renderer* prenderer, ::graphics3d::camera* pcamera);

		//void run_application() override;
		//void resize(int cx, int cy) override;


		//void Init();

		void on_initialize_particle() override;

		//void run() override;

		//private:
		//void ProcessInput(float deltaTime);
		//static void MouseCallback(::cube::impact* pglContainer, double xpos, double ypos);
		virtual void initialize_engine(::cube::impact* pimpact);

		//virtual void handle_mouse_move(double dCursorX, double dCursorY) override;

		//
		//void on_layout(int cx, int cy) override;

		virtual void on_layout(int cx, int cy) override;

		virtual void on_mouse_move(int x, int y) override;


		void on_render_frame() override;


	};



} // namespace graphics3d_opengl



