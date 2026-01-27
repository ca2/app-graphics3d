#include "framework.h"
//#include "context.h"
#include "engine.h"
#include "bred/graphics3d/input.h"
#include "bred/graphics3d/scene_base.h"
#include "mesh.h"
//#include "shader.h"
#include <iostream>
#include "camera.h"
#include "bred/gpu/bred_approach.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/shader.h"
#include "aura/platform/application.h"
#include "gpu_opengl/context.h"
#ifdef WINDOWS_DESKTOP
#include "gpu_opengl/device_win32.h"
#endif
#include "gpu_opengl/frame_buffer.h"
#include "gpu_opengl/lock.h"
#include "system/basic_render_system.h"
#include "system/point_light_system.h"
#include "bred/user/user/graphics3d.h"

//
//
//


#pragma comment( lib, "glu32" )
#pragma comment( lib, "opengl32" )


namespace graphics3d_opengl
{


   engine::engine()
      : m_Running(true)
   {

      m_bInitRenderData = true;

   }


   void engine::on_initialize_particle()
   {

      ::graphics3d::engine::on_initialize_particle();

   }

   
   engine::~engine()
   {


   }


   void engine::initialize_engine(::user::graphics3d* pimpact)
   {

      //m_fYScale = -1.0f;

      ::graphics3d::engine::initialize_engine(pimpact);

   }


   void engine::on_render_frame()
   {

      //auto pcontext = gpu_context();

      ::graphics3d::engine::on_render_frame();

   }


   void engine::defer_start(::user::graphics3d* pusergraphics3d, const ::int_rectangle& rectanglePlacement)
   {

      ::graphics3d::engine::defer_start(pusergraphics3d, rectanglePlacement);

   }


   void engine::defer_update_engine(const ::int_rectangle& rectanglePlacement)
   {

      ::graphics3d::engine::defer_update_engine(rectanglePlacement);

   }


   void engine::on_begin_frame()
   {

      auto pcontext = gpu_context();

      ::gpu::context_lock contextlock(pcontext);

      //glPushMatrix();
      //::opengl::check_error("");
      //glPushAttrib(GL_ALL_ATTRIB_BITS);
      //::opengl::check_error("");

   }


   void engine::on_end_frame()
   {

      auto pcontext = gpu_context();

      ::gpu::context_lock contextlock(pcontext);

      //glPopAttrib();
      //::opengl::check_error("");
      //glPopMatrix();
      //::opengl::check_error("");

   }


   void engine::do_frame_step(::gpu::context* pcontext)
   {

      ::graphics3d::engine::do_frame_step(pcontext);

   }


   void engine::on_mouse_move(float x, float y)
   {


   }


   void engine::on_layout(const ::int_rectangle& rectanglePlacement)
   {

      ::graphics3d::engine::on_layout(rectanglePlacement);

   }

   
   void engine::Render(renderer* prenderer, ::graphics3d::camera* pcamera)
   {


   }


   floating_matrix4 engine::perspective(const float_angle &angleFovY, float aspect, float zNear, float zFar)
   {
      float f = 1.0f / tanf(angleFovY.radians() * 0.5f);

      floating_matrix4 M(0.0f); // initialize all elements to 0

      M[0][0] = f / aspect;
      M[1][1] = f;

      M[2][2] = (zFar + zNear) / (zNear - zFar);
      M[2][3] = -1.0f;

      M[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);

      return M;
   }



} // namespace graphics3d_opengl



