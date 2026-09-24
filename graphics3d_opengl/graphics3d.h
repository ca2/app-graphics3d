// From engine_instance by camilo on 2026-09-04 10:40 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "bred/graphics3d/graphics3d.h"


////#include <memory>
////#include <vector>
////#include "Core/Window.h"
//#include "input.h"
////#include "renderer.h"
//#include "mesh.h"
////#include "shader.h"
//#include "camera.h" // Include the ::graphics3d::camera header
////#include "application.h"
////#include "apex/platform/app_consumer.h"
//#include "bred/gpu/render.h"
//#include "app/gpu_opengl/renderer.h"


namespace graphics3d_opengl
{
	//class SimpleRenderSystem;
	//class point_light_system;


	class CLASS_DECL_GRAPHICS3D_OPENGL graphics3d :
		virtual public ::graphics3d::graphics3d
	{
	public:

		////::pointer < ::user::graphics3d > m_pimpact;
		////::pointer < ::gpu_opengl::renderer > m_prenderer;
		////::pointer < ::graphics3d::camera > m_pcamera;
		////::pointer < glc::Application > m_pglcapplication;  // Game object that manages the scenes
		////::pointer < ::graphics3d::input > m_pinput;
		////::pointer<::graphics3d_opengl::render_data> m_prenderdataCurrentScene;
		////::pointer < ::gpu_opengl::frame_buffer	 >		m_pframebuffer;
		//::pointer < ::gpu::shader	 >		m_pshaderBlend;
		////double m_Δx;
		////double m_Δy;

		//double m_dMouseLastX = 0.0;
		//double m_dMouseLastY = 0.0;
		//double m_Δx = 0.;
		//double m_Δy = 0.;

		//bool m_Running;

		////bool m_bWireframeMode = false;
		////::pointer < SimpleRenderSystem > m_psimpleRenderSystem;
		////::pointer < point_light_system > m_ppointLightSystem;

		//::memory m_memoryBuffer;




		//bool m_bInitRenderData;


		//::pointer<::gpu::context>             m_pgpucontext;


		graphics3d();
		~graphics3d();


		//void defer_start(::user::graphics3d * pusergraphics3d, const ::i32_rectangle& rectanglePlacement) override;


		//void defer_update_engine(const ::i32_rectangle& rectanglePlacement) override;

		//void on_begin_frame() override;
		//void on_end_frame() override;

		////void create_global_ubo(::gpu::context* pgpucontext) override;
		////void update_global_ubo(::gpu::context* pgpucontext) override;
		////Application();
		////~Application();

		////void Init();       // Initialize the game and load the first scene
		/////void Update(float deltaTime, ::graphics3d::camera* pcamera);  // Update game logic
		//void Render(renderer* prenderer, ::graphics3d::camera* pcamera);

		////void run_application() override;
		////void resize(int cx, int cy) override;

		////void do_frame_step(::gpu::context * pgpucontextUpper) override;
  //    void do_draw_layer() override;


		////virtual void _do_frame_step();

		////void Init();

		void on_initialize_particle() override;

		////void run() override;

		////private:
		////void ProcessInput(float deltaTime);
		////static void MouseCallback(::user::graphics3d* pglContainer, double xpos, double ypos);
		void initialize_graphics3d() override;

		////virtual void handle_mouse_move(double dCursorX, double dCursorY) override;

		////
		////void on_layout(int cx, int cy) override;

		//virtual void on_layout(const ::i32_rectangle & rectanglePlacement) override;

		//virtual void on_mouse_move(float x, float y) override;


		////void on_render_frame() override;
  //    void on_render_layer() override;


  //    //floating_matrix4 ortho(float left, float right, float bottom, float top, float zNear = -1.0f,
  //      //                     float zFar = 1.0f) override;


  //    floating_matrix4 perspective(const f32_angle & angleFovY, float aspect, float zNear, float zFar) override;



	};



} // namespace graphics3d_opengl



