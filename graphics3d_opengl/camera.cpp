#include "framework.h"
//#include "::graphics3d::camera.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <cmath> // For sin and cos functions
//
//
//namespace graphics3d_opengl
//{
//
//
//   // Constructor
//   ::graphics3d::camera::::graphics3d::camera(glc::GlContainer* pimpact, glm::vec3 position, float fYaw, float fPitch)
//   {
//
//      m_pimpact = pimpact;
//
//      initialize(pimpact);
//
//      m_Position = position;
//      m_fYaw = fYaw;
//      m_fPitch = fPitch;
//      m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
//      m_WorldUp = glm::vec3(0.0f, -1.0f, 0.0f);
//      m_Zoom = 75.0f;
//      m_MovementSpeed = 20.5f;
//
//
//   }
//
//
//   // Mouse movement processing
//   void ::graphics3d::camera::ProcessMouseMovement(float Δx, float Δy, bool constrainPitch) 
//   {
//
//      const float sensitivity = 0.01f;  // Adjust this value to your liking
//      Δx *= sensitivity;
//      Δy *= sensitivity;
//
//      //m_Yaw += xoffset;
//      //m_Pitch += yoffset;
//
//      //if (constrainPitch) {
//      //   if (m_Pitch > 89.0f) m_Pitch = 89.0f;
//      //   if (m_Pitch < -89.0f) m_Pitch = -89.0f;
//      //}
//      // limit pitch values between about +/- 85ish degrees
//      //gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
//      //gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
//      if (m_pimpact->is_absolute_mouse_position())
//      {
//         m_fYaw = Δx;
//         m_fPitch = Δy;
//      }
//      else
//      {
//
//         m_fYaw += Δx;
//         m_fPitch += Δy;
//
//      }
//
//      // Clamp pitch to avoid flipping
//      m_fPitch = glm::clamp(m_fPitch, -1.5f, 1.5f);
//
//      // Optional: wrap yaw
//      //if (m_fYaw > 2.0f * MATH_PI) m_fYaw -= 2.0f * MATH_PI;
//      //if (m_fYaw < 0.0f) m_fYaw += 2.0f * MATH_PI;
//      m_fYaw= glm::mod(m_fYaw, glm::two_pi<float>());
//      //gameObject.transform.rotation.x = glm::radians(_pitch);
//      //gameObject.transform.rotation.y = glm::radians(_yaw);
//
//      UpdateCameraVectors();
//   }
//
//
//   // Keyboard input processing (with adjustable speed)
//   void ::graphics3d::camera::ProcessKeyboardInput(int direction, float deltaTime) {
//      float velocity = m_MovementSpeed * deltaTime;  // Use movement speed
//      if (direction == FORWARD)
//         m_Position += m_Front * velocity;
//      if (direction == BACKWARD)
//         m_Position -= m_Front * velocity;
//      if (direction == LEFT)
//         m_Position -= m_Right * velocity;
//      if (direction == RIGHT)
//         m_Position += m_Right * velocity;
//   }
//
//   void ::graphics3d::camera::Jump(float jumpHeight) {
//      m_Position.y += jumpHeight;  // Move up by jumpHeight units
//   }
//
//   void ::graphics3d::camera::TeleportDownward(float distance) {
//      m_Position.y -= distance;  // Move down by the specified distance
//   }
//
//   void ::graphics3d::camera::TeleportInDirection(int direction) {
//      const float teleportDistance = 0.40f; // Set teleport distance to 50 units
//
//      if (direction == FORWARD) {
//         m_Position += m_Front * teleportDistance;
//      }
//      else if (direction == BACKWARD) {
//         m_Position -= m_Front * teleportDistance;
//      }
//      else if (direction == LEFT) {
//         m_Position -= m_Right * teleportDistance;
//      }
//      else if (direction == RIGHT) {
//         m_Position += m_Right * teleportDistance;
//      }
//   }
//
//   // Function to update idle movement
//   void ::graphics3d::camera::UpdateIdleMovement(float deltaTime) {
//      static float elapsedTime = 0.0f;
//      elapsedTime += deltaTime;
//
//      // Subtle oscillation parameters
//      float amplitude = 0.1f; // Amplitude of the movement
//      float frequency = 1.0f; // Frequency of the oscillation
//
//      // Calculate oscillation
//      float offsetX = amplitude * sin(frequency * elapsedTime);
//      float offsetY = amplitude * cos(frequency * elapsedTime);
//
//      // Apply the oscillation to the camera position
//      m_Position.x += offsetX;
//      m_Position.y += offsetY;
//   }
//   // Update the camera vectors based on yaw and pitch
//   void ::graphics3d::camera::UpdateCameraVectors() {
//      glm::vec3 front;
//      front.x = cos(m_fYaw) * cos(m_fPitch);
//      front.y = sin(m_fPitch);
//      front.z = sin(m_fYaw) * cos(m_fPitch);
//      m_Front = glm::normalize(front);
//
//      // Update right and up vectors
//      m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
//      m_Up = glm::normalize(glm::cross(m_Right, m_Front));
//   }
//
//   // Get the camera's zoom (field of view)
//   float ::graphics3d::camera::GetZoom() const {
//      return m_Zoom;
//   }
//
//   // Set the movement speed of the camera
//   void ::graphics3d::camera::SetMovementSpeed(float speed) {
//      m_MovementSpeed = speed;
//   }
//   // Get the view matrix
//   glm::mat4 ::graphics3d::camera::GetViewMatrix() const {
//      return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
//   }
//   // Get the camera position
//   glm::vec3 ::graphics3d::camera::GetPosition() const {
//      return m_Position;
//   }
//
//
//   void ::graphics3d::camera::scroll_callback(glc::GlContainer* pimpact, double xoffset, double yoffset)
//   {
//      m_Zoom -= (float)yoffset;
//      if (m_Zoom < 1.0f)
//         m_Zoom = 1.0f;
//      if (m_Zoom > 45.0f)
//         m_Zoom = 45.0f;
//   }
//
//
//
//
//
//} // namespace graphics3d_opengl
//
//
//
//
//
