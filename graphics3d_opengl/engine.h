#pragma once


#include "aura/graphics/graphics3d/engine.h"


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
#include "app-graphics3d/gpu_opengl/renderer.h"


namespace graphics3d_opengl
{
	class SimpleRenderSystem;
	class point_light_system;


	class engine :
		virtual public ::graphics3d::engine
	{
	public:

		//::pointer < ::user::graphics3d > m_pimpact;
		//::pointer < ::gpu_opengl::renderer > m_prenderer;
		::pointer < ::graphics3d::camera > m_pcamera;
		//::pointer < glc::Application > m_pglcapplication;  // Game object that manages the scenes
		::pointer < ::graphics3d::input > m_pinput;
		//::pointer<::graphics3d_opengl::render_data> m_prenderdataCurrentScene;
		//::pointer < ::gpu_opengl::frame_buffer	 >		m_pframebuffer;
		::pointer < ::gpu::shader	 >		m_pshaderBlend;
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


		engine();
		~engine();


		void defer_start(::user::graphics3d * pusergraphics3d, const ::int_rectangle& rectanglePlacement) override;


		void defer_update_engine(const ::int_rectangle& rectanglePlacement) override;

		void on_begin_frame() override;

		//void create_global_ubo(::gpu::context* pgpucontext) override;
		//void update_global_ubo(::gpu::context* pgpucontext) override;
		//Application();
		//~Application();

		//void Init();       // Initialize the game and load the first scene
		///void Update(float deltaTime, ::graphics3d::camera* pcamera);  // Update game logic
		void Render(renderer* prenderer, ::graphics3d::camera* pcamera);

		//void run_application() override;
		//void resize(int cx, int cy) override;

		void do_frame_step(::gpu::context * pgpucontextUpper) override;


		//virtual void _do_frame_step();

		//void Init();

		void on_initialize_particle() override;

		//void run() override;

		//private:
		//void ProcessInput(float deltaTime);
		//static void MouseCallback(::user::graphics3d* pglContainer, double xpos, double ypos);
		virtual void initialize_engine(::user::graphics3d* pimpact);

		//virtual void handle_mouse_move(double dCursorX, double dCursorY) override;

		//
		//void on_layout(int cx, int cy) override;

		virtual void on_layout(const ::int_rectangle & rectanglePlacement) override;

		virtual void on_mouse_move(float x, float y) override;


		void on_render_frame() override;


	};



} // namespace graphics3d_opengl



