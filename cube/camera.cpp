// From V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-17 20:12 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "camera.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath> 
// std
#include <cassert>
#include <limits>

namespace cube
{


   camera::camera(glm::vec3 position, float yaw, float pitch)
   {
      
      m_locationPosition = position;

      m_fYaw = yaw;

      m_fPitch = pitch;

      m_poleWorldUp = { 0.0f, 1.0f, 0.0f };

      m_fZoom = 75.0f;

      m_fMovementSpeed = 8.0f;

      UpdateCameraVectors();

   }


   camera::camera(glm::vec3 position, glm::vec3 to, glm::vec3 from)
   {

      m_locationPosition = position;

      auto direction = glm::normalize(to - from);

      m_fYaw = atan2(direction.x, direction.z);

      m_fPitch = asin(-direction.y);

      m_poleWorldUp = { 0.0f, 1.0f, 0.0f };
      
      m_fZoom = 75.0f;
      
      m_fMovementSpeed = 8.0f;

      UpdateCameraVectors();

   }

   // Mouse movement processing
   void camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) 
   {

      const float sensitivity = 0.1f;  // Adjust this value to your liking
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      m_fYaw += xoffset;
      m_fPitch += yoffset;

      if (constrainPitch) 
      {
         
         m_fPitch = minimum_maximum(m_fPitch, glm::radians(-89.0f), glm::radians(89.0f));

      }

      UpdateCameraVectors();

   }


   void camera::ProcessKeyboardInput(int direction, float deltaTime) 
   {

      auto velocity = m_fMovementSpeed * deltaTime;  // Use movement speed

      if (direction == FORWARD)
         m_locationPosition += m_poleFront * velocity;
      if (direction == BACKWARD)
         m_locationPosition -= m_poleFront * velocity;
      if (direction == LEFT)
         m_locationPosition -= m_poleRight * velocity;
      if (direction == RIGHT)
         m_locationPosition += m_poleRight * velocity;

   }

   
   void camera::setOrthographicProjection(
      float left, float right, float top, float bottom, float fNear, float fFar) 
   {

      m_matrixProjection = glm::mat4{ 1.0f };
      m_matrixProjection[0][0] = 2.f / (right - left);
      m_matrixProjection[1][1] = 2.f / (bottom - top);
      m_matrixProjection[2][2] = 1.f / (fFar - fNear);
      m_matrixProjection[3][0] = -(right + left) / (right - left);
      m_matrixProjection[3][1] = -(bottom + top) / (bottom - top);
      m_matrixProjection[3][2] = -fNear / (fFar - fNear);

   }


   // This is the method being used below as opposed to ortho ^
   void camera::setPerspectiveProjection(float fovy, float aspect, float fNear, float fFar) 
   {

      assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
      const float tanHalfFovy = tan(fovy / 2.f);
      m_matrixProjection = glm::mat4{ 0.0f };
      m_matrixProjection[0][0] = 1.f / (aspect * tanHalfFovy);
      m_matrixProjection[1][1] = 1.f / (tanHalfFovy);
      m_matrixProjection[2][2] = fFar / (fFar - fNear);
      m_matrixProjection[2][3] = 1.f;
      m_matrixProjection[3][2] = -(fFar * fNear) / (fFar - fNear);

   }


   void camera::setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) 
   {

      const glm::vec3 w{ glm::normalize(direction) };
      const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
      const glm::vec3 v{ glm::cross(w, u) };

      m_matrixImpact = glm::mat4{ 1.f };
      m_matrixImpact[0][0] = u.x;
      m_matrixImpact[1][0] = u.y;
      m_matrixImpact[2][0] = u.z;
      m_matrixImpact[0][1] = v.x;
      m_matrixImpact[1][1] = v.y;
      m_matrixImpact[2][1] = v.z;
      m_matrixImpact[0][2] = w.x;
      m_matrixImpact[1][2] = w.y;
      m_matrixImpact[2][2] = w.z;
      m_matrixImpact[3][0] = -glm::dot(u, position);
      m_matrixImpact[3][1] = -glm::dot(v, position);
      m_matrixImpact[3][2] = -glm::dot(w, position);
      m_matrixAntImpact = glm::mat4{ 1.f };
      m_matrixAntImpact[0][0] = u.x;
      m_matrixAntImpact[0][1] = u.y;
      m_matrixAntImpact[0][2] = u.z;
      m_matrixAntImpact[1][0] = v.x;
      m_matrixAntImpact[1][1] = v.y;
      m_matrixAntImpact[1][2] = v.z;
      m_matrixAntImpact[2][0] = w.x;
      m_matrixAntImpact[2][1] = w.y;
      m_matrixAntImpact[2][2] = w.z;
      m_matrixAntImpact[3][0] = position.x;
      m_matrixAntImpact[3][1] = position.y;
      m_matrixAntImpact[3][2] = position.z;

   }

   
   void camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) 
   {

      setViewDirection(position, target - position, up);

   }


   void camera::setViewYXZ(glm::vec3 position, glm::vec3 rotation) 
   {

      const float c3 = glm::cos(rotation.z);
      const float s3 = glm::sin(rotation.z);
      const float c2 = glm::cos(rotation.x);
      const float s2 = glm::sin(rotation.x);
      const float c1 = glm::cos(rotation.y);
      const float s1 = glm::sin(rotation.y);
      const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
      const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
      const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };
      m_matrixImpact = glm::mat4{ 1.f };
      m_matrixImpact[0][0] = u.x;
      m_matrixImpact[1][0] = u.y;
      m_matrixImpact[2][0] = u.z;
      m_matrixImpact[0][1] = v.x;
      m_matrixImpact[1][1] = v.y;
      m_matrixImpact[2][1] = v.z;
      m_matrixImpact[0][2] = w.x;
      m_matrixImpact[1][2] = w.y;
      m_matrixImpact[2][2] = w.z;
      m_matrixImpact[3][0] = -glm::dot(u, position);
      m_matrixImpact[3][1] = -glm::dot(v, position);
      m_matrixImpact[3][2] = -glm::dot(w, position);

      m_matrixAntImpact = glm::mat4{ 1.f };
      m_matrixAntImpact[0][0] = u.x;
      m_matrixAntImpact[0][1] = u.y;
      m_matrixAntImpact[0][2] = u.z;
      m_matrixAntImpact[1][0] = v.x;
      m_matrixAntImpact[1][1] = v.y;
      m_matrixAntImpact[1][2] = v.z;
      m_matrixAntImpact[2][0] = w.x;
      m_matrixAntImpact[2][1] = w.y;
      m_matrixAntImpact[2][2] = w.z;
      m_matrixAntImpact[3][0] = position.x;
      m_matrixAntImpact[3][1] = position.y;
      m_matrixAntImpact[3][2] = position.z;

   }


   void camera::UpdateCameraVectors()
   {

      // Calculate the new front vector based on yaw and pitch
      glm::vec3 front;
      front.x = cos(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
      front.y = sin(glm::radians(m_fPitch));
      front.z = sin(glm::radians(m_fYaw)) * cos(glm::radians(m_fPitch));
      this->m_poleFront = glm::normalize(front);

      // Re-calculate the right and up vector
      this->m_poleRight = glm::normalize(glm::cross(this->m_poleFront, this->m_poleUp));
      this->m_poleUp = glm::normalize(glm::cross(this->m_poleRight, this->m_poleFront));

   }


   // Get the camera's zoom (field of view)
   float camera::GetZoom() const
   {

      return m_fZoom;

   }


   // Set the movement speed of the camera
   void camera::SetMovementSpeed(float speed)
   {

      m_fMovementSpeed = speed;

   }


   // Get the view matrix
   glm::mat4 camera::GetViewMatrix() const
   {

      return glm::lookAt(m_locationPosition, m_locationPosition + m_poleFront, m_poleUp);

   }

   // Get the camera position
   glm::vec3 camera::GetPosition() const
   {

      return m_locationPosition;

   }


}  // namespace cube


