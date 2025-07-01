#pragma once



#include "bred/gpu/properties.h"
#include "bred/graphics3d/scene_system.h"
#include <glm/glm.hpp>


namespace app_graphics3d_continuum
{

//#define MAX_LIGHTS 10
//
//   struct PointLight 
//   {
//      glm::vec4 position{};  // ignore w
//      glm::vec4 color{};     // w is intensity
//   };
//


   //struct PointLightPushConstants {
   //   glm::vec4 position{};
   //   glm::vec4 color{};
   //   float radius;
   //};

   //inline ::gpu::properties point_light_properties()
   //{

   //   static ::gpu::property s_properties[] =
   //   {
   //      {"position", ::gpu::e_type_seq4},
   //      {"color", ::gpu::e_type_seq4},
   //      {"radius", ::gpu::e_type_float},
   //      { nullptr, ::gpu::e_type_none}

   //   };

   //   return s_properties;

   //}



   //inline ::gpu::properties global_ubo_properties()
   //{
   //   ::gpu::property s_properties[] = {
   //      {"projection", ::gpu::e_type_mat4},
   //      {"view", ::gpu::e_type_mat4},
   //      {"invVew", ::gpu::e_type_mat4},
   //      {"ambientLightColor", ::gpu::e_type_seq4},
   //      {"pointLights", ::gpu::e_type_propeties_array, point_light_properties(),  10},

   //   };


   //}


   class point_light_system :
      virtual public ::graphics3d::scene_system
   {
   public:


      ::pointer < ::graphics3d::engine >     m_pengine;

      ::pointer < ::gpu::shader >            m_pshader;
      //::pointer < ::graphics3d::engine >        m_pengine;
      ::pointer < ::graphics3d::model >      m_pmodelDummy;

      point_light_system();
      ~point_light_system();

      
      void initialize_point_light_system(::graphics3d::engine * pengine);


      virtual void update(::gpu::context* pgpucontext, ::graphics3d::scene * pscene) override;
      virtual void render(::gpu::context* pgpucontext, ::graphics3d::scene * pscene) override;
      

      //void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
      //void createPipeline(VkRenderPass renderPass);

      virtual void prepare(::gpu::context* pgpucontext) override;

      //VkPipelineLayout pipelineLayout;

   };


}  // namespace app_graphics3d_continuum