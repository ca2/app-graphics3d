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
#include "bred/gpu/approach.h"
#include "bred/gpu/cpu_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/object.h"
#include "bred/gpu/program.h"
#include "bred/gpu/shader.h"


void image_gl_set(::image::image* pimage);


namespace graphics3d_impact3d
{


   render::render()
   {

      m_bPendingShaderUpdate = false;
      m_bPrepared = false;

   }


   render::~render()
   {


   }


   void render::initialize(::particle * pparticle)
   {

      ::gpu::render::initialize(pparticle);

      //m_pgpucontext = system()->get_gpu()->create_context(this);

//      m_pgpucontext->initialize(this);

   }


#ifdef _DEBUG

   long long render::increment_reference_count()
   {

      return ::particle::increment_reference_count();

   }


   long long render::decrement_reference_count()
   {

      return ::particle::decrement_reference_count();

   }


#endif


   void render::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      m_pgpucontext->send([this, &pgraphics]()
         {

            if (m_rectangle.area() <= 0)
            {

               return;

            }

            if (m_pgpucontext)
            {

               m_pgpucontext->resize_offscreen_buffer(m_rectangle.size());

            }

            defer_prepare();

      });

   }


   void render::defer_prepare()
   {

      defer_update_shader();

      if (m_bPrepared)
      {

         return;

      }

      if (::is_set(m_pgpucontext) && ::is_set(m_pgpucontext->m_pcpubuffer)
         && m_pgpucontext->m_pcpubuffer->m_pixmap.is_ok()
         && ::is_set(m_pgpuprogram))
      {

         if (::is_set(m_pobject))
         {

            if (m_pobject->m_bPrepared)
            {

               return;

            }

            _prepare();

         }

      }


   }


   void render::_prepare()
   {

      auto pgpucontext = m_pgpucontext;

      auto uniformMatrix = m_pgpuprogram->m_pshader->get_uniform(m_strMatrixUniform);

      pgpucontext->set_matrix_uniform(uniformMatrix);

      auto uniformTexture = m_pgpuprogram->m_pshader->get_uniform(m_strTextureUniform);

      m_pobject->set_texture_uniform(uniformTexture);

      m_pobject->prepare(system()->gpu());

      m_bPrepared = true;

   }


   void render::set_vertex_source_code(const ::string & strVertexSourceCode)
   {

      m_strProjection = strVertexSourceCode;
      
      set_update_shader();
      
   }


   void render::set_fragment_source_code(const ::string & strFragmentSourceCode)
   {

      m_strFragment = strFragmentSourceCode;

      set_update_shader();

   }


   void render::set_update_shader()
   {

      m_bPendingShaderUpdate = true;

   }


   void render::defer_update_shader()
   {

      auto pgpucontext = m_pgpucontext;

      if (::is_null(pgpucontext))
      {

         return;

      }

      auto pcpubuffer = pgpucontext->m_pcpubuffer;

      if (::is_null(pcpubuffer))
      {

         return;

      }

      if (m_bPendingShaderUpdate)
      {

         m_bPendingShaderUpdate = false;

         _update_shader();

      }

   }


   ::e_status render::_update_shader()
   {

      color32_t crText = argb(255, 55, 210, 120);

      string strProjection = m_strProjection;

      string strFragment = m_strFragment;

      auto pgpucontext = m_pgpucontext;

      ::gpu::context_lock lock(pgpucontext);

      pgpucontext->make_current();

      if (!m_pgpuprogram)
      {

         __øconstruct(m_pgpuprogram);

      }

      //pgpucontext->translate_shader(strProjection);

      //pgpucontext->translate_shader(strFragment);

      m_pgpuprogram->create_program(this, strProjection, strFragment);

      ::color::color color(crText);

      pgpucontext->m_pprogram = m_pgpuprogram;

      pgpucontext->draw();

      //unsigned int texture1;

      //glGenTextures(1, &texture1);

      ////            glActiveTexture(GL_TEXTURE0);

      //glBindTexture(GL_TEXTURE_2D, texture1);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      m_timeStart.Now();

      return ::success;

   }


   void render::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      m_pgpucontext->send([this, & pgraphics]()
         {
      
      ::gpu::context_lock lock(m_pgpucontext);

      if (::is_set(m_pgpucontext) && ::is_set(m_pgpucontext->m_pcpubuffer)
         && m_pgpucontext->m_pcpubuffer->m_pixmap.is_ok()
         && ::is_set(m_pgpuprogram))
      {

         if (::is_set(m_pobject))
         {

            ::gpu::context_lock lock(m_pgpucontext);

            m_pgpucontext->make_current();

            // global_transform

            ////{

            ////   float x = (float) psession->get_cursor_position().x();

            ////   float y = (float) psession->get_cursor_position().y();

            ////   m_pgpucontext->m_pgpuprogram->m_pshader->setVec2("mouse", x, y);
            ////   m_pgpucontext->m_pgpuprogram->m_pshader->setVec2("iMouse", x, y);

            ////}

            //{

            //   float cx = (float) m_pgpucontext->m_pbuffer->m_pimage->width();

            //   float cy = (float) m_pgpucontext->m_pbuffer->m_pimage->height();

            //   m_pgpucontext->m_pgpuprogram->m_pshader->setVec2("resolution", cx, cy);
            //   m_pgpucontext->m_pgpuprogram->m_pshader->setVec2("iResolution", cx, cy);

            //}

            //{

            //   double dElapsed = m_timeStart.elapsed().floating_second().m_d;

            //   double dTime = dElapsed / 1000.0;

            //   float time = (float) dTime;

            //   m_pgpucontext->m_pgpuprogram->m_pshader->setFloat("time", time);
            //   m_pgpucontext->m_pgpuprogram->m_pshader->setFloat("iTime", time);

            //}

            m_pgpucontext->start_drawing();

            m_pgpucontext->global_transform();

            m_pobject->draw();

            //m_pgpucontext->render();

            read_to_cpu_buffer();

            to_draw2d_graphics(pgraphics);


         }

//            m_pgpucontext->prepare_for_gpu_read();
//
//            m_pgpucontext->m_pcpubuffer->gpu_read();
//
//         }
//
//#if !defined(__APPLE__)
//         ::geometry2d::matrix matrixOriginal;
//         pgraphics->get(matrixOriginal);
//         ::geometry2d::matrix matrix(matrixOriginal);
//         matrix.scale(1.0, -1.0);
//         matrix.translate(0, m_rectangle.height());
//         pgraphics->set(matrix);
//#endif
//
//         auto pimage = m_pgpucontext->m_pcpubuffer->m_pimage;
//
//         if (::is_set(pimage))
//         {
//
//            //pimage->map();
//
//            //pimage->fill_channel(255, ::color::e_channel_opacity);
//
//         }
//
//         ::image::image_source imagesource(m_pgpucontext->m_pbuffer->m_pimage);
//
//         ::double_rectangle rectangleTarget(m_rectangle);
//
//         ::image::image_drawing_options imagedrawingoptions(rectangleTarget);
//
//         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);
//
//         pgraphics->draw(imagedrawing);
//
//#if !defined(__APPLE__)
//
//         pgraphics->set(matrixOriginal);
//
//#endif
//
      }

      //_001OnDraw1Through3(pgraphics);
      });

   }


   ::e_status render::set_font(const ::string & strFont)
   {

      if (strFont.is_empty())
      {

         return error_bad_argument;

      }

      //string strDataId;

      //strDataId = m_pimpact->id();

      //if (!papp->datastream()->set(strDataId + ".font_family", m_strFont1))
      //{

      //   return error_failed;

      //}

      m_strFont1 = strFont;

      return ::success;

   }


   string render::get_font()
   {

      string strFont;

      //strFont = papp->get_hover_font();

      //if (strFont.has_character())
      //{

      //   return strFont;

      //}

      //if (m_strFont1.is_empty())
      //{

      //   string strDataId;

      //   strDataId = m_pimpact->id();

      //   if (!papp->datastream()->get(strDataId + ".font_family", m_strFont1)
      //      || m_strFont1.is_empty())
      //   {

      //      m_strFont1 = pnode->font_name(e_font_sans_ex);

      //   }

      //}

      return m_strFont1;

   }


   void render::set_model_path(const ::payload & payloadFile)
   {

      __øconstruct(m_pobject);

      auto pparticle = m_pobject;

      auto path = payloadFile.as_file_path();

      pparticle->load(path);

   }


   void render::draw()
   {

      m_pobject->draw();

   }


   ////void computeMatricesFromInputs() {

   ////   // glfwGetTime is called only once, the first time this function is called
   ////   static double lastTime = glfwGetTime();

   ////   // Compute time difference between current and last frame
   ////   double currentTime = glfwGetTime();
   ////   float deltaTime = float(currentTime - lastTime);

   //   // Get mouse position
   //   double xpos, ypos;
   //   glfwGetCursorPos(window, &xpos, &ypos);

   //   // Reset mouse position for next frame
   //   glfwSetCursorPos(window, 1024 / 2, 768 / 2);

   //   // Compute ___new orientation
   //   horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
   //   verticalAngle += mouseSpeed * float(768 / 2 - ypos);

   //   // Direction : Spherical coordinates to Cartesian coordinates conversion
   //   glm::vec3 direction(
   //      cos(verticalAngle) * sin(horizontalAngle),
   //      sin(verticalAngle),
   //      cos(verticalAngle) * cos(horizontalAngle)
   //   );

   //   // Right vector
   //   glm::vec3 right = glm::vec3(
   //      sin(horizontalAngle - 3.14f / 2.0f),
   //      0,
   //      cos(horizontalAngle - 3.14f / 2.0f)
   //   );

   //   // Up vector
   //   glm::vec3 up = glm::cross(right, direction);

   //   // Move forward
   //   if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
   //      position += direction * deltaTime * speed;
   //   }
   //   // Move backward
   //   if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
   //      position -= direction * deltaTime * speed;
   //   }
   //   // Strafe right
   //   if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
   //      position += right * deltaTime * speed;
   //   }
   //   // Strafe left
   //   if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
   //      position -= right * deltaTime * speed;
   //   }

   //   float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

   //   // Projection matrix : 45degrees Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
   //   ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
   //   // Camera matrix
   //   ViewMatrix = glm::lookAt(
   //      position,           // Camera is here
   //      position + direction, // and looks here : at the same position, plus "direction"
   //      up                  // Head is up (set to 0,-1,0 to look upside-down)
   //   );

   //   // For the next frame, the "last time" will be "now"
   //   lastTime = currentTime;
   //}
   //getProjectionMatrix


} // namespace graphics3d_impact3d



