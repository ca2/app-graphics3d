// From graphics3d by camilo on 2026-09-04 10:40 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
//#include "context.h"
#include "graphics3d.h"
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


   graphics3d::graphics3d()
//      : m_Running(true)
   {

  //    m_bInitRenderData = true;

   }


   
   graphics3d::~graphics3d()
   {


   }
   
   
   
   void graphics3d::on_initialize_particle()
   {

      ::graphics3d::graphics3d::on_initialize_particle();

   }




   void graphics3d::initialize_graphics3d()
   {

      //m_fYScale = -1.0f;

      ::graphics3d::graphics3d::initialize_graphics3d();

   }


   //void graphics3d::on_render_layer()
   //{

   //   //auto pcontext = gpu_context();

   //   ::graphics3d::graphics3d::on_render_layer();

   //}


   //void graphics3d::defer_start(::user::graphics3d* pusergraphics3d, const ::i32_rectangle& rectanglePlacement)
   //{

   //   ::graphics3d::graphics3d::defer_start(pusergraphics3d, rectanglePlacement);

   //}


   //void graphics3d::defer_update_engine(const ::i32_rectangle& rectanglePlacement)
   //{

   //   ::graphics3d::graphics3d::defer_update_engine(rectanglePlacement);

   //}


   //void graphics3d::on_begin_frame()
   //{

   //   auto pcontext = gpu_context();

   //   ::gpu::context_lock contextlock(pcontext);

   //   //glPushMatrix();
   //   //::opengl::check_error("");
   //   //glPushAttrib(GL_ALL_ATTRIB_BITS);
   //   //::opengl::check_error("");

   //}


   //void graphics3d::on_end_frame()
   //{

   //   auto pcontext = gpu_context();

   //   ::gpu::context_lock contextlock(pcontext);

   //   //glPopAttrib();
   //   //::opengl::check_error("");
   //   //glPopMatrix();
   //   //::opengl::check_error("");

   //}


   //void graphics3d::do_draw_layer()
   //{

   //   ::graphics3d::graphics3d::do_draw_layer();

   //}


   //void graphics3d::on_mouse_move(float x, float y)
   //{


   //}


   //void graphics3d::on_layout(const ::i32_rectangle& rectanglePlacement)
   //{

   //   ::graphics3d::graphics3d::on_layout(rectanglePlacement);

   //}

   //
   //void graphics3d::Render(renderer* prenderer, ::graphics3d::camera* pcamera)
   //{


   //}


   //floating_matrix4 graphics3d::perspective(const f32_angle &angleFovY, float aspect, float zNear, float zFar)
   //{
   //   float f = 1.0f / tanf(angleFovY.radians() * 0.5f);

   //   floating_matrix4 M(0.0f); // initialize all elements to 0

   //   M[0][0] = f / aspect;
   //   M[1][1] = f;

   //   M[2][2] = (zFar + zNear) / (zNear - zFar);
   //   M[2][3] = -1.0f;

   //   M[3][2] = (2.0f * zFar * zNear) / (zNear - zFar);

   //   return M;
   //}



} // namespace graphics3d_opengl



