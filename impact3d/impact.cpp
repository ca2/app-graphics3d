#include "framework.h"
#include "impact.h"
#include "document.h"
#include "render.h"
#include "acme/constant/id.h"
#include "acme/constant/message.h"
#include "acme/constant/user_key.h"
#include "acme/handler/topic.h"
#include "base/user/user/split_impact.h"
#include "cube/gpu/context.h"
#include "aura/message/user.h"
#include "aura/user/user/frame_interaction.h"
#include "glm/gtc/matrix_transform.hpp"


#include <math.h>



namespace graphics3d_impact3d
{


   impact::impact()
   {

      m_pointLastMouse.x() = -100'000.0;
      m_pointLastMouse.y() = -100'000.0;

      m_iView = -1;

      //payload(FONTSEL_IMPACT) = true;

      m_flagNonClient.erase(e_non_client_background);

      m_flagNonClient.erase(e_non_client_focus_rect);

   }


   impact::~impact()
   {

   }


   //void impact::assert_ok() const
   //{

   //   user::box::assert_ok();

   //}


   //void impact::dump(dump_context & dumpcontext) const
   //{

   //   user::box::dump(dumpcontext);

   //}


#ifdef _DEBUG

   long long impact::increment_reference_count()
   {

      return  ::user::impact::increment_reference_count();

   }


   long long impact::decrement_reference_count()
   {

      return  ::user::impact::decrement_reference_count();

   }

#endif

   void impact::install_message_routing(::channel * psender)
   {

      ::user::impact::install_message_routing(psender);
      //::user::interaction::install_simple_ui_default_mouse_handling(psender);
      MESSAGE_LINK(e_message_create, psender, this, &impact::on_message_create);
      MESSAGE_LINK(e_message_destroy, psender, this, &impact::on_message_destroy);
      MESSAGE_LINK(e_message_mouse_move, psender, this, &impact::on_message_mouse_move);
      MESSAGE_LINK(e_message_key_down, psender, this, &impact::on_message_key_down);

   }


   void impact::on_message_create(::message::message * pmessage)
   {

      ::pointer<::message::create> pcreate(pmessage);

      pcreate->previous();

      if (pcreate->m_bRet)
      {

         return;

      }

      add_graphical_output_purpose(this, ::graphics::e_output_purpose_screen_fps);

      //top_level_frame()->set_fps_interest();

//      auto predraw = [this]()
//      {
//
//         set_need_redraw();
//
//         post_redraw();
//
//      };

      //__construct_new(m_prender);

      //if(!estatus)
      //{

      //   pcreate->set_fail();

      //   return;

      //}

      //m_prender->initialize_application_consumer();

      //m_prender->m_pimpact = this;

      //string strId = get_document()->m_pimpactsystem->m_strMatter;

      string strText;

      if (get_typed_parent<::user::split_impact>() != nullptr)
      {

         if (get_typed_parent<::user::split_impact>()->get_child_by_id("top_edit_impact") != nullptr)
         {

            auto pinteraction = get_typed_parent<::user::split_impact>()->get_child_by_id("top_edit_impact");

            pinteraction->set_text(strText, ::e_source_initialize);

         }

      }

      //auto prender = get_document()->m_prender;

      //if (::is_set(prender))
      //{

      //   prender->set_update_shader();

      //}

      //{

      //   ::identification identification = id_simple_text;

      //   auto pproperty = papp->fetch_property(identification);

      //   ::payload payload;

      //   if (papp->datastream()->get(identification, payload))
      //   {

      //      pproperty->convert(payload);

      //   }

      //   auto idRunnable = papp->translate_property_id(identification);

      //   papp->m_mapRunnable[idRunnable] += [this, identification]()
      //   {

      //      auto pproperty = fetch_property(identification);

      //      m_prender->defer_load_fragment(pproperty->get_string());

      //   };

      //}


   }


   // Direction : Spherical coordinates to Cartesian coordinates conversion
   ::pole_double impact::camera_position()
   {

      return
      {

         -sin(m_angleHorizontal) * 5.0 * cos(m_angleVertical),
         -sin(m_angleVertical) * 5.0,
         -cos(m_angleHorizontal) * 5.0 * cos(m_angleVertical),

      };

   }


   ::pole_double impact::right()
   {

      // Right vector
      return
      {

         -cos(m_angleHorizontal.m_fAngle) * 5.0,
         0,
         sin(m_angleHorizontal.m_fAngle) * 5.0

      };

   }


   void impact::compute_matrices()
   {

//      auto FoV = m_angleImpact;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

      auto fRadian = (float)m_angleImpact.radian();

   // Projection matrix : 45degrees Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
      auto matrixProjection = ::glm::perspective(fRadian, 4.0f / 3.0f, 0.1f, 100.0f);
       //auto matrixProjection = ::glm::mat4(fRadian, 4.0f / 3.0f, 0.1f, 100.0f);
        //auto matrixProjection = ::glm::mat4();
      // Camera matrix
      // 
      
      //auto loc = ::glm::vec3(m_location.x(), m_location.y(), m_location.z);
      auto campos = ::glm::vec3(camera_position().x(), camera_position().y(), camera_position().z());
      //sauto locdir = loc + 2;
      auto r = ::glm::vec3(right().x(), right().y(), right().z());
      auto up = ::glm::cross(campos, r);
      auto matrixImpact = ::glm::lookAt(
         campos, // Camera is here
         //::glm::vec3(m_location.x(), m_location.y(), m_location.z),           // Camera is here
         ::glm::vec3(0.f, 0.f, 0.f),          // and looks here : at the same position, plus "direction"
         up
         //::glm::vec3(0.f, 1.f, 0.f)// Head is up (set to 0,-1,0 to look upside-down)
      );

      auto prender = get_document()->m_prender;

      if (::is_set(prender))
      {

         auto pgpucontext = prender->m_pgpucontext;

         if (::is_set(pgpucontext))
         {

            pgpucontext->m_matrixProjection = (const ::draw3d::matrix &)matrixProjection;

            pgpucontext->m_matrixImpact = (const ::draw3d::matrix &)matrixImpact;

         }

      }

   }


   void impact::on_message_destroy(::message::message * pmessage)
   {

   }


   void impact::on_message_key_down(::message::message* pmessage)
   {

      auto pkey = pmessage->m_union.m_pkey;

      if (pkey->m_ekey == ::user::e_key_s)
      {

         m_bSaveFrame = true;

      }
      else if (pkey->m_ekey == ::user::e_key_up)
      {

         m_location += camera_position() * (m_timeDelta * m_frequencySpeed);

      }
      else if (pkey->m_ekey == ::user::e_key_down)
      {

         m_location -= camera_position() * (m_timeDelta * m_frequencySpeed);

      }
      else if (pkey->m_ekey == ::user::e_key_right)
      {

         m_location += right() * (m_timeDelta * m_frequencySpeed);

      }
      else if (pkey->m_ekey == ::user::e_key_left)
      {

         m_location -= right() * (m_timeDelta * m_frequencySpeed);

      }

   }


   void impact::on_message_mouse_move(::message::message * pmessage)
   {

      auto pmouse = pmessage->m_union.m_pmouse;

      if (::is_set(pmouse))
      {

         //auto size = this->get_size();

         bool bOk = m_pointLastMouse.x() > -10'000.0 && m_pointLastMouse.y() > -10'000.0;

         m_sizeMouseDelta = double_point(pmouse->m_pointHost) - m_pointLastMouse;

         m_pointLastMouse = pmouse->m_pointHost;

         //// Get mouse position
         //double xpos, ypos;
         //glfwGetCursorPos(window, &xpos, &ypos);

         //// Reset mouse position for next frame
         //glfwSetCursorPos(window, 1024 / 2, 768 / 2);

         // Compute ___new orientation
         if (bOk)
         {

            m_doubleRatSpeedHpp = 0.01;

            //m_angleHorizontal.m_d += m_doubleRatSpeedHpp * (size.cx() / 2.0 - m_sizeMouseDelta.cx());
            //m_angleVertical.m_d += m_doubleRatSpeedHpp * (size.cy() / 2.0 - m_sizeMouseDelta.cy());
            m_angleHorizontal.m_fAngle += m_doubleRatSpeedHpp * (- m_sizeMouseDelta.cx());
            m_angleVertical.m_fAngle += m_doubleRatSpeedHpp * ( - m_sizeMouseDelta.cy());

         }

      }

   }


   void impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      if (ptopic->id() == id_initial_update)
      {

         set_need_layout();

         set_need_redraw();

         post_redraw();

      }

      ::user::impact::handle(ptopic, phandlercontext);

   }


   document * impact::get_document()
   {

      return dynamic_cast < document * > (::user::impact::get_document());

   }


   bool impact::keyboard_focus_is_focusable()
   {

      return true;

   }


   void impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      m_timeDelta = m_timeLastDraw.update_elapsed();

      if (m_bSaveFrame)
      {

         m_bSaveFrame = false;

         //auto rectangleX = this->rectangle();

         //auto pimage = create_image(rectangleX);

         //::draw2d::graphics_pointer pgraphics = pimage->get_graphics();

         //m_prender->_001OnDraw(pgraphics);

         //fork([this, pimage]()
         //   {

         //      ::image::encoding_options encodingoptions;

         //      encodingoptions.m_eformat = ::image::e_format_png;

         //      string strDate = pdatetime->international().get_gmt_date_time(INTERNATIONAL_DATE_TIME_FORMAT_FOR_FILE);

         //      pcontext->save_image("image://app_simple_shader-" + strDate + ".png", pimage, encodingoptions);

         //   });


      }

      compute_matrices();

      auto prender = get_document()->m_prender;

      if (::is_set(prender))
      {

         prender->_001OnDraw(pgraphics);

      }

   }


   void impact::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      if(rectangleX.is_empty())
      {

         return;

      }

      auto prender = get_document()->m_prender;

      if (::is_set(prender))
      {

         prender->m_rectangle = rectangleX;

         prender->on_layout(pgraphics);

      }

   }

   //// Direction : Spherical coordinates to Cartesian coordinates conversion
   //::poled impact::direction()
   //{

   //   cos(m_angleVertical) * sin(m_angleHorizontal),
   //   sin(m_angleVertical),
   //   cos(m_angleVertical) * cos(m_angleHorizontal)

   //);


   //void computeMatricesFromInputs() {

   //   // glfwGetTime is called only once, the first time this function is called
   //   static double lastTime = glfwGetTime();

   //   // Compute time difference between current and last frame
   //   double currentTime = glfwGetTime();
   //   float deltaTime = float(currentTime - lastTime);

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
   //   //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
   //   //   m_location += direction * deltaTime * m_frequency;
   //   //}
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



