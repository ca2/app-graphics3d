#include "framework.h"
#include "render.h"
#include "application.h"
#include <math.h>
#ifndef __APPLE__
#include "aura/graphics/draw2d/graphics.h"
#include "acme/prototype/geometry2d/matrix.h"
#endif
#include "aura/graphics/image/drawing.h"
#include "aura/graphics/image/image.h"
#include "aura/platform/system.h"
#include "cube/gpu/approach.h"
#include "cube/gpu/cpu_buffer.h"
#include "cube/gpu/context.h"
#include "cube/gpu/object.h"
#include "cube/gpu/program.h"
#include "cube/gpu/shader.h"
#define GLAD_GLAPI_EXPORT
#include <glad.h>


void image_gl_set(::image::image* pimage);


namespace graphics3d_hello3d
{


   render::render()
   {

      m_iRender = 0;
      //m_bPendingShaderUpdate = false;
      //m_bPrepared = false;

   }


   render::~render()
   {


   }


//   void render::initialize(::particle * pparticle)
//   {
//
//      ::particle::initialize(pparticle);
//
//      m_papplication = system()->get_gpu()->create_context();
//
//      m_papplication->initialize(this);
//
//   }
//
//
//#ifdef _DEBUG
//
//   long long render::increment_reference_count()
//   {
//
//      return ::particle::increment_reference_count();
//
//   }
//
//
//   long long render::decrement_reference_count()
//   {
//
//      return ::particle::decrement_reference_count();
//
//   }
//
//
//#endif
//
//
//   void render::on_layout(::draw2d::graphics_pointer & pgraphics)
//   {
//
//      if (m_rectangle.area() <= 0)
//      {
//
//         return;
//
//      }
//
//      if (m_papplication)
//      {
//
//         m_papplication->resize_offscreen_buffer(m_rectangle.size());
//
//      }
//
//      defer_prepare();
//
//   }
//
//
//   void render::defer_prepare()
//   {
//
//      defer_update_shader();
//
//      if (m_bPrepared)
//      {
//
//         return;
//
//      }
//
//      if (::is_set(m_papplication) && ::is_set(m_papplication->m_pbuffer)
//         && m_papplication->m_pbuffer->m_pimage.ok()
//         && ::is_set(m_pprogram))
//      {
//
//         if (::is_set(m_pobject))
//         {
//
//            if (m_pobject->m_bPrepared)
//            {
//
//               return;
//
//            }
//
//            _prepare();
//
//         }
//
//      }
//
//
//   }
//
//
//   void render::_prepare()
//   {
//
//      auto pcontext = m_papplication;
//
//      auto uniformMatrix = m_pprogram->m_pshader->get_uniform(m_strMatrixUniform);
//
//      pcontext->set_matrix_uniform(uniformMatrix);
//
//      auto uniformTexture = m_pprogram->m_pshader->get_uniform(m_strTextureUniform);
//
//      m_pobject->set_texture_uniform(uniformTexture);
//
//      m_pobject->prepare(system()->gpu());
//
//      m_bPrepared = true;
//
//   }
//
//
//   void render::set_vertex_source_code(const ::string & strVertexSourceCode)
//   {
//
//      m_strProjection = strVertexSourceCode;
//      
//      set_update_shader();
//      
//   }
//
//
//   void render::set_fragment_source_code(const ::string & strFragmentSourceCode)
//   {
//
//      m_strFragment = strFragmentSourceCode;
//
//      set_update_shader();
//
//   }
//
//
//   void render::set_update_shader()
//   {
//
//      m_bPendingShaderUpdate = true;
//
//   }
//
//
//   void render::defer_update_shader()
//   {
//
//      auto pcontext = m_papplication;
//
//      if (::is_null(pcontext))
//      {
//
//         return;
//
//      }
//
//      auto pbuffer = pcontext->m_pbuffer;
//
//      if (::is_null(pbuffer))
//      {
//
//         return;
//
//      }
//
//      if (m_bPendingShaderUpdate)
//      {
//
//         m_bPendingShaderUpdate = false;
//
//         _update_shader();
//
//      }
//
//   }
//
//
//   ::e_status render::_update_shader()
//   {
//
//      color32_t crText = argb(255, 55, 210, 120);
//
//      string strProjection = m_strProjection;
//
//      string strFragment = m_strFragment;
//
//      auto pcontext = m_papplication;
//
//      ::gpu::context_lock lock(pcontext);
//
//      pcontext->make_current();
//
//      if (!m_pprogram)
//      {
//
//         ::__øconstruct(this,m_pprogram);
//
//      }
//
//      //pcontext->translate_shader(strProjection);
//
//      //pcontext->translate_shader(strFragment);
//
//      m_pprogram->create_program(this, strProjection, strFragment);
//
//      ::color::color color(crText);
//
//      pcontext->m_pprogram = m_pprogram;
//
//      pcontext->draw();
//
//      //unsigned int texture1;
//
//      //glGenTextures(1, &texture1);
//
//      ////            glActiveTexture(GL_TEXTURE0);
//
//      //glBindTexture(GL_TEXTURE_2D, texture1);
//      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//      m_timeStart.Now();
//
//      return ::success;
//
//   }
//
//
//
//
//   ::e_status render::set_font(const ::string & strFont)
//   {
//
//      if (strFont.is_empty())
//      {
//
//         return error_bad_argument;
//
//      }
//
//      //string strDataId;
//
//      //strDataId = m_pimpact->id();
//
//      //if (!papp->datastream()->set(strDataId + ".font_family", m_strFont1))
//      //{
//
//      //   return error_failed;
//
//      //}
//
//      m_strFont1 = strFont;
//
//      return ::success;
//
//   }
//
//
//   string render::get_font()
//   {
//
//      string strFont;
//
//      //strFont = papp->get_hover_font();
//
//      //if (strFont.has_character())
//      //{
//
//      //   return strFont;
//
//      //}
//
//      //if (m_strFont1.is_empty())
//      //{
//
//      //   string strDataId;
//
//      //   strDataId = m_pimpact->id();
//
//      //   if (!papp->datastream()->get(strDataId + ".font_family", m_strFont1)
//      //      || m_strFont1.is_empty())
//      //   {
//
//      //      m_strFont1 = pnode->font_name(e_font_sans_ex);
//
//      //   }
//
//      //}
//
//      return m_strFont1;
//
//   }
//
//
//   void render::set_model_path(const ::payload & payloadFile)
//   {
//
//      ::__øconstruct(this, m_pobject);
//
//      auto pparticle = m_pobject;
//
//      auto path = payloadFile.as_file_path();
//
//      pparticle->load(path);
//
//   }
//
//


   void render::on_draw(::gpu::context * pcontext)
   {

      if (m_strRender.is_empty())
      {

         return;

      }

      if (m_strRender == "triangle")
      {

         on_draw_triangle(pcontext);

      }
      else if (m_strRender == "triangle2")
      {

         on_draw_triangle2(pcontext);

      }
      else if (m_strRender == "triangle_uniform")
      {

         on_draw_triangle_uniform(pcontext);

      }
      else if (m_strRender == "triangle_more_attributes")
      {

         on_draw_triangle_more_attributes(pcontext);

      }
      else if (m_strRender == "rectangle_wireframe")
      {

         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

         on_draw_rectangle(pcontext);

      }
      else if (m_strRender == "rectangle")
      {

         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

         on_draw_rectangle(pcontext);

      }
      else if (m_strRender == "rectangle_texture")
      {

         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

         on_draw_rectangle_texture(pcontext);

      }
      else
      {

         on_draw_triangle(pcontext);

      }

   }


//   ////void computeMatricesFromInputs() {
//
//   ////   // glfwGetTime is called only once, the first time this function is called
//   ////   static double lastTime = glfwGetTime();
//
//   ////   // Compute time difference between current and last frame
//   ////   double currentTime = glfwGetTime();
//   ////   float deltaTime = float(currentTime - lastTime);
//
//   //   // Get mouse position
//   //   double xpos, ypos;
//   //   glfwGetCursorPos(window, &xpos, &ypos);
//
//   //   // Reset mouse position for next frame
//   //   glfwSetCursorPos(window, 1024 / 2, 768 / 2);
//
//   //   // Compute ___new orientation
//   //   horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
//   //   verticalAngle += mouseSpeed * float(768 / 2 - ypos);
//
//   //   // Direction : Spherical coordinates to Cartesian coordinates conversion
//   //   glm::vec3 direction(
//   //      cos(verticalAngle) * sin(horizontalAngle),
//   //      sin(verticalAngle),
//   //      cos(verticalAngle) * cos(horizontalAngle)
//   //   );
//
//   //   // Right vector
//   //   glm::vec3 right = glm::vec3(
//   //      sin(horizontalAngle - 3.14f / 2.0f),
//   //      0,
//   //      cos(horizontalAngle - 3.14f / 2.0f)
//   //   );
//
//   //   // Up vector
//   //   glm::vec3 up = glm::cross(right, direction);
//
//   //   // Move forward
//   //   if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//   //      position += direction * deltaTime * speed;
//   //   }
//   //   // Move backward
//   //   if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//   //      position -= direction * deltaTime * speed;
//   //   }
//   //   // Strafe right
//   //   if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//   //      position += right * deltaTime * speed;
//   //   }
//   //   // Strafe left
//   //   if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//   //      position -= right * deltaTime * speed;
//   //   }
//
//   //   float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.
//
//   //   // Projection matrix : 45degrees Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//   //   ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
//   //   // Camera matrix
//   //   ViewMatrix = glm::lookAt(
//   //      position,           // Camera is here
//   //      position + direction, // and looks here : at the same position, plus "direction"
//   //      up                  // Head is up (set to 0,-1,0 to look upside-down)
//   //   );
//
//   //   // For the next frame, the "last time" will be "now"
//   //   lastTime = currentTime;
//   //}
//   //getProjectionMatrix
//




} // namespace graphics3d_hello3d



