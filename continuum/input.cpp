// From mnk_controller from V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "camera.h"
#include "immersion.h"
#include "input.h"
//#include "scene_object.h"
#include "bred/gpu/context.h"
#include "bred/gpu/renderer.h"
#include "bred/graphics3d/camera.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/types.h"
#include "aura/platform/application.h"
#include "aura/platform/session.h"
#include "bred/user/user/graphics3d.h"
#include <limits>

//#include "immersion_layer.h"
//#include "scene.h"


namespace app_graphics3d_continuum
{

   input::input()
   {


   }

   input::~input()
   {


   }


   void input::initialize_input(
      const float_angle &angleCursorPixel,
      const float_angle &angleYaw,
      const float_angle &anglePitch)
   {

      m_dCursorX = 0.;
      m_dCursorY = 0.;
      m_bFirstMouse = true;
      m_bLastMouse = false;

      m_angleCursorPixel = angleCursorPixel;

      //m_angleYaw = angleYaw;
      //m_anglePitch = anglePitch;

      //_cameraDirection = floating_sequence3(0.0f, 0.0f, -1.0f);
      //_cameraPosition = floating_sequence3(0.0f, 0.0f, 3.0f);

   }

   //void input::moveInPlaneXZ(
   //    ::user::graphics3d * pimpact, float dt, application_object& gameObject) {
   //    floating_sequence3 rotate{ 0 };
   //    if (key(e_key_lookRight) == ::user::e_key_state_pressed) rotate.y += 1.f;
   //    if (key(e_key_lookLeft) == ::user::e_key_state_pressed) rotate.y -= 1.f;
   //    if (key(e_key_lookUp) == ::user::e_key_state_pressed) rotate.x += 1.f;
   //    if (key(e_key_lookDown) == ::user::e_key_state_pressed) rotate.x -= 1.f;

   //    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
   //        gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
   //    }

   //    // limit pitch values between about +/- 85ish degrees
   //    gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
   //    gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

   //    float yaw = gameObject.transform.rotation.y;
   //    const floating_sequence3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
   //    const floating_sequence3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
   //    const floating_sequence3 upDir{ 0.f, -1.f, 0.f };

   //    floating_sequence3 moveDir{ 0.f };
   //    if (key(e_key_moveForward) == ::user::e_key_state_pressed) moveDir += forwardDir;
   //    if (key(e_key_moveBackward) == ::user::e_key_state_pressed) moveDir -= forwardDir;
   //    if (key(e_key_moveRight) == ::user::e_key_state_pressed) moveDir += rightDir;
   //    if (key(e_key_moveLeft) == ::user::e_key_state_pressed) moveDir -= rightDir;
   //    if (key(e_key_moveUp) == ::user::e_key_state_pressed) moveDir += upDir;
   //    if (key(e_key_moveDown) == ::user::e_key_state_pressed) moveDir -= upDir;

   //    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
   //        gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
   //    }

   //    if (key(e_key_Exit) == ::user::e_key_state_pressed)
   //    {
   //       //glfwSetWindowShouldClose(window, GLFW_TRUE);

   //    }
   //}


   //void input::_001OnMouseOut()
   //{

   //   reset_mouse_last_position();

   //   ::cast<::app_graphics3d_continuum::immersion> pimmersion = m_pengine->m_pimmersionlayer;


   //               if (pimmersion->m_emouse == ::app_graphics3d_continuum::e_mouse_updateLook)
   //   {

   //      //m_pengine->m_pinput->_001OnMouseOut();
   //   }



   //}


   void input::reset_mouse_last_position()
   {



      if (m_b_001AbsoluteMousePosition)
      {

         m_dCursorX = 0.;
         m_dCursorY = 0.;

      }

      m_bLastMouse = true;

   }


   //void input::process_mouse_input_updateLook()
   //{

   //   auto xOffset = m_Δx;
   //   auto yOffset = m_Δy;

   //   // Apply sensitivity factor
   //   xOffset *= m_f_001UpdateLookSensitivity;
   //   yOffset *= m_f_001UpdateLookSensitivity;

   //   // Update yaw and pitch based on offsets
   //   _yaw += xOffset;
   //   _pitch += yOffset;

   //   // Constrain the pitch if necessary
   //   if (_pitch > 89.0f)
   //      _pitch = 89.0f;
   //   if (_pitch < -89.0f)
   //      _pitch = -89.0f;

   //   // Update camera direction based on yaw and pitch
   //   floating_sequence3 direction;
   //   direction.x = cos(::radians(_yaw)) * cos(::radians(_pitch));
   //   direction.y = sin(::radians(_pitch));
   //   direction.z = sin(::radians(_yaw)) * cos(::radians(_pitch));
   //   _cameraDirection = glm::normalize(direction);

   //}

   //void input::processKeyboardInput(GLFWwindow* window, float deltaTime) {
   //    float cameraSpeed = 2.5f * deltaTime; // adjust speed as necessary

   //    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
   //        _cameraPosition += _cameraDirection * cameraSpeed;
   //    }
   //    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
   //        _cameraPosition -= _cameraDirection * cameraSpeed;
   //    }
   //    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
   //        _cameraPosition -= glm::normalize(glm::cross(_cameraDirection, floating_sequence3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
   //    }
   //    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
   //        _cameraPosition += glm::normalize(glm::cross(_cameraDirection, floating_sequence3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
   //    }
   //}


   void input::process_mouse_input_updateLook()
   {

      auto xOffset = m_sequence2MouseRawΔ.x;
      auto yOffset = m_sequence2MouseRawΔ.y;

      auto angleΔYaw = xOffset * m_angleCursorPixel;
      auto angleΔPitch = yOffset * m_angleCursorPixel;

      auto pengine = m_pengine;

      auto pimmersionlayer = pengine->m_pimmersionlayer;

      auto pscene = pimmersionlayer->m_pscene;

      auto pgpucamera = pscene->camera();

      ::cast<::app_graphics3d_continuum::camera> pcamera = pgpucamera;

      if (pcamera)
      {

         // limit pitch values between about +/- 85ish degrees
         auto rotation = pcamera->rotation();
         //;
         //;
         //auto pitch = pcamera->pitch();

         //if (_yaw > _2πf)
         //   _yaw -= _2πf;
         //if (_yaw < 0.0f)
         //   _yaw += _2πf;

         //_pitch = geometry::clamp(_pitch, -1.5f, 1.5f);

         if (m_b_001AbsoluteMousePosition)
         {

            rotation.set(angleΔYaw, angleΔPitch);

         }
         else
         {

            rotation.offset(angleΔPitch, angleΔPitch, -89f_degrees, 89f_degrees);

         }

         //pitch = ::geometry::clamp(pitch, -89f_degrees, 89f_degrees);

         pcamera->m_rotation = rotation;

         pcamera->update_vectors();

      }

   }


   
   void input::process_mouse_input()
   {

      //m_pengine->m_pusergraphics3d->process_mouse_input();
      //_001PrepareMouseInput();

                     ::cast<::app_graphics3d_continuum::immersion> pimmersion = m_pengine->m_pimmersionlayer;


      if (pimmersion->m_emouse == ::app_graphics3d_continuum::e_mouse_updateLook)
      {

         process_mouse_input_updateLook();
      }


   }


   void input::process_keyboard_input()
   {

      //m_pengine->m_pusergraphics3d->process_keyboard_input();

      
      ::cast<::app_graphics3d_continuum::immersion> pimmersion = m_pengine->m_pimmersionlayer;
      if (pimmersion->m_ekeyboard == ::app_graphics3d_continuum::e_keyboard_updateMovement)
      {

         process_keyboard_input_updateMovement();
      }
      else if (pimmersion->m_ekeyboard == ::app_graphics3d_continuum::e_keyboard_spaceExplorer)
      {

         ::cast<::app_graphics3d_continuum::immersion> pimmersion = m_pengine->m_pimmersionlayer;
         process_keyboard_input_spaceExplorer();
      }


   }


   void input::process_keyboard_input_updateMovement()
   {

      //auto& transform = m_pengine->m_transform;

      auto pgpucamera = m_pengine->m_pimmersionlayer->m_pscene->m_pcameraCurrent;

      ::cast<::app_graphics3d_continuum::camera> pcamera = pgpucamera;

      if (pcamera)
      {

         auto forwardDir = pcamera->front();
         auto rightDir = pcamera->right();
         auto upDir = pcamera->up();
         //const floating_sequence3 forwardDir{cos(yaw), 0.f, sin(yaw)};
         //const floating_sequence3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
         //const floating_sequence3 upDir{0.f, -1.f, 0.f};

         auto pinput = m_pengine->m_pinput;

         floating_sequence3 moveDir{0.f};
         {
            using namespace ::graphics3d;
            if (pinput->key(e_key_moveForward) == ::user::e_key_state_pressed)
               moveDir += forwardDir;
            if (pinput->key(e_key_moveBackward) == ::user::e_key_state_pressed)
               moveDir -= forwardDir;
            if (pinput->key(e_key_moveRight) == ::user::e_key_state_pressed)
               moveDir += rightDir;
            if (pinput->key(e_key_moveLeft) == ::user::e_key_state_pressed)
               moveDir -= rightDir;
            if (pinput->key(e_key_moveUp) == ::user::e_key_state_pressed)
               moveDir += upDir;
            if (pinput->key(e_key_moveDown) == ::user::e_key_state_pressed)
               moveDir -= upDir;

            if (moveDir.squared_modulus() > std::numeric_limits<float>::epsilon())
            {

               ::cast<camera> pcameraCurrent = m_pengine->m_pimmersionlayer->m_pscene->m_pcameraCurrent;

               pcameraCurrent->m_locationPosition +=
                  m_fMoveSpeed * m_pengine->dt() * moveDir.normalized();
            }

            if (pinput->key(e_key_Exit) == ::user::e_key_state_pressed)
            {

               m_pengine->gpu_context()->set_finish();
            }
         }
      }

   }


   
   void input::process_keyboard_input_spaceExplorer()
   {

      ::cast<camera> pcamera = m_pengine->m_pimmersionlayer->m_pscene->m_pcameraCurrent;

      if (key(::graphics3d::e_key_moveForward) == ::user::e_key_state_pressed)
      {

         pcamera->ProcessKeyboardInput(::graphics3d::e_key_moveForward, m_pengine->dt());

         if (IsKeyPressed(::user::e_key_left_shift))
         {

            pcamera->TeleportInDirection(::graphics3d::e_key_moveForward);

         }

      }

      if (key(::graphics3d::e_key_moveBackward) == ::user::e_key_state_pressed)
      {

         pcamera->ProcessKeyboardInput(
            ::graphics3d::e_key_moveBackward, m_pengine->dt());

         if (IsKeyPressed(::user::e_key_left_shift))
         {

            pcamera->TeleportInDirection(
               ::graphics3d::e_key_moveBackward);

         }

      }

      if (key(::graphics3d::e_key_moveLeft) == ::user::e_key_state_pressed)
      {

         pcamera->ProcessKeyboardInput(::graphics3d::e_key_moveLeft,
                                                                                        m_pengine->dt());

         if (IsKeyPressed(::user::e_key_left_shift))
         {

            pcamera->TeleportInDirection(::graphics3d::e_key_moveLeft);

         }

      }

      if (key(::graphics3d::e_key_moveRight) == ::user::e_key_state_pressed)
      {

         pcamera->ProcessKeyboardInput(::graphics3d::e_key_moveRight,
                                                                                        m_pengine->dt());

         if (IsKeyPressed(::user::e_key_left_shift))
         {

            pcamera->TeleportInDirection(
               ::graphics3d::e_key_moveRight);

         }
      }

      if (IsKeyPressed(::user::e_key_escape))
      {

         m_pengine->gpu_context()->set_finish();

      }

      if (IsKeyPressed(::user::e_key_4))
      {
         m_pengine->m_bWireframeMode = !m_pengine->m_bWireframeMode;

      }
      // Check for jump (Space key)
      if (IsKeyPressed(::user::e_key_space))
      {
         float jumpHeight = 0.20f;  // Define how high the jump should be
         pcamera->Jump(jumpHeight);
      }
      //// Teleport down (Left Control key)
      //if (IsKeyPressed(::user::e_key_left_shift))
      //{
      //   float teleportDistance = 0.1081f;  // Define the downward distance
      //   m_pengine->m_pimmersionlayer->m_pscene->m_pcameraCurrent->TeleportDownward(teleportDistance);
      //}

      // Idle movement
      if (!IsAnyKeyPressed()) { // You might want to implement IsAnyKeyPressed to check for movement keys
         // m_pcamera->UpdateIdleMovement(deltaTime);
      }


   }


   floating_sequence3 input::getCameraDirection() const
   {
      return _cameraDirection;
   }


   floating_sequence3 input::getCameraPosition() const
   {
      return _cameraPosition;
   }


//   void input::_001OnMouseMove(const ::int_point& point)
//   {
//
//      //m_mousestate.position.x = point.x;
////m_mousestate.position.y = point.y;
////m_mousestate.m_buttons.left = true;
////         pmouse->m_p
//
//      double w = m_pengine->m_pusergraphics3d->m_iWidth;
//
//      double h = m_pengine->m_pusergraphics3d->m_iHeight;
//
//      if (m_bLastMouse)
//      {
//
//         m_bLastMouse = false;
//         m_bFirstMouse = true;
//
//      }
//
//      double xCursor;
//      double yCursor;
//
//      if (m_b_001AbsoluteMousePosition)
//      {
//
//         xCursor = ((point.x - (w / 2.0)) * 2.0);
//         yCursor = ((point.y - (h / 2.0)) * 2.0);
//
//      }
//      else
//      {
//
//         //xCursor = point.x;
//         //yCursor = point.y;
//
//         xCursor = point.x;
//         yCursor = point.y;
//
//      }
//
//      m_dCursorX = xCursor;
//
//      m_dCursorY = yCursor;
//
//      m_pengine->m_pusergraphics3d->track_mouse_leave();
//
//      m_pengine->m_pusergraphics3d->m_pengine->on_mouse_move((float)xCursor, (float)yCursor);
//
//   }


   void input::_001PrepareMouseInput()
   {

      floating_sequence2 sequence2;
      floating_sequence2 sequence2New;

      if (m_b_001AbsoluteMousePosition)
      {

         sequence2New.x = m_dCursorX * 1.25 * MATH_PI;
         sequence2New.y = m_dCursorY * 1.25 * MATH_PI / 2.0;

      }
      else
      {

         sequence2New.x = m_dCursorX;
         sequence2New.y = m_dCursorY;

      }
      //glfwGetCursorPos(window, &xpos, &ypos);

      //if (m_pimpact->m_bFirstMouse) {
      //   _lastX = newx;
      //   _lastY = newy;
      //   m_pimpact->m_bFirstMouse = false;
      //   xpos = _lastX;
      //   ypos = _lastY;
      //}
      //else
      if (!m_b_001AbsoluteMousePosition)
      {

         //if (m_bFirstMouse)
         if (!m_bWasLeft && m_mousestate.m_buttons.left)
         {
            //m_dMouseLastX = newx;
            //m_dMouseLastY = newy;
            m_bWasLeft = true;

         }
         else if (m_bWasLeft && !m_mousestate.m_buttons.left)
         {

            m_bWasLeft = false;

         }

      }

      if (m_mousestate.m_buttons.left)
      {

         if (m_b_001AbsoluteMousePosition)
         {

            sequence2 = m_sequence2MouseLast + (sequence2New - m_sequence2MouseLast) * 0.05;

         }
         else
         {

            sequence2 = sequence2New;

         }

         if (m_b_001AbsoluteMousePosition)
         {

            m_sequence2MouseRawΔ.x = sequence2.x;
            m_sequence2MouseRawΔ.y = -sequence2.y; // reversed Y

         }
         else
         {

            //m_Δx = x - m_dMouseLastX;
            //m_Δy = m_dMouseLastY - y;  // reversed Y
            m_sequence2MouseRawΔ.x =
               m_sequence2MouseRawΔ.x + static_cast<float>(m_sequence2MouseLast.x - sequence2.x - m_sequence2MouseRawΔ.x) * 0.1;
            m_sequence2MouseRawΔ.y = m_sequence2MouseRawΔ.y + static_cast<float>(sequence2.y - m_sequence2MouseLast.y - m_sequence2MouseRawΔ.y) * 0.1; // reversed Y

         }

         //m_dMouseLastX = x;
         //m_dMouseLastY = y;

         m_sequence2MouseLast = sequence2;

      }
      else
      {
         
         if (!m_b_001AbsoluteMousePosition)
         {

            m_sequence2MouseRawΔ.x *= 0.90;
            m_sequence2MouseRawΔ.y *= 0.90;

         }

      }

   }


   //::block input::as_block()
   //{
   //   
   //   return input_t::as_block();

   //}


   void input::prepare_mouse_input()
   {

         //m_pengine->m_pusergraphics3d->prepare_mouse_input();


      //   ::cast<::app_graphics3d_continuum::immersion> pimmersion = m_pengine->m_pimmersionlayer;
   
      //if (pimmersion->m_emouse == ::app_graphics3d_continuum::e_mouse_updateLook)
      //{

      //   _001PrepareMouseInput();
      //}


   }





} // namespace app_graphics3d_continuum


