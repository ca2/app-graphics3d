#pragma once


#include "bred/user/user/graphics3d.h"


namespace app_graphics3d_continuum
{


#define MAX_LIGHTS 10


   //struct PointLight
   //   glm::vec4 position{};  // ignore w
   // glm::vec4 color{};     // w is intensity

BEGIN_GPU_PROPERTIES(point_light)
GPU_PROPERTY("position", ::gpu::e_type_seq4)
GPU_PROPERTY("color", ::gpu::e_type_seq4)
END_GPU_PROPERTIES()

//GPU_PROPERTY("radius", ::gpu::e_type_float)

   // GlobalUbo :
   // glm::mat4 projection{ 1.f };
   // glm::mat4 view{ 1.f };
   // glm::mat4 invView{ 1.f };
   // glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
   // PointLight pointLights[MAX_LIGHTS];
   // int numLights;
   // 3XINT PADDING

BEGIN_GPU_PROPERTIES(global_ubo)
GPU_PROPERTY("projection", ::gpu::e_type_mat4)
GPU_PROPERTY("view", ::gpu::e_type_mat4)
GPU_PROPERTY("invView", ::gpu::e_type_mat4)
GPU_PROPERTY("ambientLightColor", ::gpu::e_type_seq4)
GPU_PROPERTY("pointLights", point_light_properties(), MAX_LIGHTS)
GPU_PROPERTY("numLights", ::gpu::e_type_int)
GPU_PROPERTY("padding1", ::gpu::e_type_float)
GPU_PROPERTY("padding2", ::gpu::e_type_float)
GPU_PROPERTY("padding3", ::gpu::e_type_float)
END_GPU_PROPERTIES()

} // namespace app_graphics3d_continuum


