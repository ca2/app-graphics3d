#include "framework.h"
#include "context.h"
#include "engine.h"
#include "bred/graphics3d/input.h"
#include "bred/graphics3d/scene.h"
#include "mesh.h"
#include "shader.h"
#include <iostream>
#include "camera.h"
#include "bred/gpu/approach.h"
#include "bred/gpu/shader.h"
#include "aura/platform/application.h"
#include "gpu_opengl/context.h"
#include "gpu_opengl/device_win32.h"
#include "gpu_opengl/frame_buffer.h"
#include "system/basic_render_system.h"
#include "system/point_light_system.h"
#include "bred/user/user/graphics3d.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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

      m_fYScale = 1.0f;

      ::graphics3d::engine::initialize_engine(pimpact);

   }


   void engine::on_render_frame()
   {

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

      glPushMatrix();
      glPushAttrib(GL_ALL_ATTRIB_BITS);

   }


   void engine::on_end_frame()
   {

      glPopAttrib();
      glPopMatrix();

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


} // namespace graphics3d_opengl



