#pragma once
////#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
////#include "Core/gl_container.h"
//
//
//namespace graphics3d_opengl
//{
//
//
//   enum CameraMovement 
//   {
//      FORWARD,
//      BACKWARD,
//      LEFT,
//      RIGHT
//   };
//
//   struct camera_t
//   {
//
//      float m_LastX; // Last mouse x position
//      float m_LastY; // Last mouse y position
//      float m_MouseSensitivity; // Sensitivity factor for mouse input
//      // Deadzone
//      float m_MouseDeadZone;
//      // Inertia
//      glm::vec3 m_Velocity;
//      // ::graphics3d::camera attributes
//      glm::vec3 m_Position;
//      glm::vec3 m_Front;
//      glm::vec3 m_Up;
//      glm::vec3 m_Right;
//      glm::vec3 m_WorldUp;
//
//      // Euler angles
//      float m_fYaw;
//      float m_fPitch;
//
//      // ::graphics3d::camera options
//      float m_Zoom;
//      float m_MovementSpeed;  // Added movement speed
//
//
//   };
//
//   class ::graphics3d::camera :
//      virtual public ::particle,
//      virtual public camera_t
//   {
//   public:
//
//      ::pointer < ::glc::GlContainer > m_pimpact;
//
//
//      ::graphics3d::camera(glc::GlContainer * pimpact, glm::vec3 position, float yaw, float pitch);
//
//      // Process input for mouse movement
//      void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
//
//      // Process input for keyboard movement
//      void ProcessKeyboardInput(int direction, float deltaTime);
//
//      // Get the view matrix
//      glm::mat4 GetViewMatrix() const;
//
//
//      glm::vec3 GetPosition() const;
//
//
//      void UpdateIdleMovement(float deltaTime); // Add this line
//
//      // Get zoom (field of view)
//      float GetZoom() const;
//
//      // Set movement speed
//      void SetMovementSpeed(float speed);
//
//      //void scroll_callback(glc::GlContainer* window, double xoffset, double yoffset);
//
//      void scroll_callback(glc::GlContainer* pglconatainer, double xoffset, double yoffset);
//
//
//      void Jump(float jumpHeight);
//
//      void TeleportDownward(float distance); // Moves the camera down
//
//      void TeleportInDirection(int direction);
//   //private:
//
//
//      // Update camera vectors
//      void UpdateCameraVectors();
//   };
//
//
//} // namespace graphics3d_opengl
//
//
