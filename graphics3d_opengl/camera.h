// Changed by camilo on 2025-12-07 08:38 <3ThomasBorregaardSørensen!!
#pragma once


#include "bred/graphics3d/camera.h"


namespace graphics3d_opengl
{


   class camera :
      virtual public ::graphics3d::camera
   {
   public:


      camera();
      ~camera();

//      void setPerspectiveProjection(float fovy, float aspect, float near, float far);
////
////      ::pointer < ::glc::GlContainer > m_pimpact;
////
////
////      ::graphics3d::camera(glc::GlContainer * pimpact, const ::floating_sequence3 & position, float yaw, float pitch);
////
////      // Process input for mouse movement
////      void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
////
////      // Process input for keyboard movement
////      void ProcessKeyboardInput(int direction, float deltaTime);
////
////      // Get the view matrix
////      floating_matrix4 GetViewMatrix() const;
////
////
////      floating_sequence3 GetPosition() const;
////
////
////      void UpdateIdleMovement(float deltaTime); // Add this line
////
////      // Get zoom (field of view)
////      float GetZoom() const;
////
////      // Set movement speed
////      void SetMovementSpeed(float speed);
////
////      //void scroll_callback(glc::GlContainer* window, double xoffset, double yoffset);
////
////      void scroll_callback(glc::GlContainer* pglconatainer, double xoffset, double yoffset);
////
////
////      void Jump(float jumpHeight);
////
////      void TeleportDownward(float distance); // Moves the camera down
////
////      void TeleportInDirection(int direction);
////   //private:
////
////
////      // Update camera vectors
////      void UpdateCameraVectors();
   };

} // namespace graphics3d_opengl



