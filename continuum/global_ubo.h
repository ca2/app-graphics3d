#pragma once


#include "cube/impact.h"


namespace app_graphics3d_continuum
{


#define MAX_LIGHTS 10

   struct PointLight
   {
      glm::vec4 position{};  // ignore w
      glm::vec4 color{};     // w is intensity
   };



   class GlobalUbo
   {
   public:

      
      glm::mat4 projection{ 1.f };
      glm::mat4 view{ 1.f };
      glm::mat4 inverseView{ 1.f };
      glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
      PointLight pointLights[MAX_LIGHTS];
      int numLights;

   };


} // namespace app_graphics3d_continuum


