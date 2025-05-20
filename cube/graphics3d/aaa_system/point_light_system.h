#pragma once


#include "app-cube/cube/graphics3d/properties.h"
#include <glm/glm.hpp>


namespace graphics3d
{



   //struct PointLightPushConstants {
   //   glm::vec4 position{};
   //   glm::vec4 color{};
   //   float radius;
   //};

   inline ::graphics3d::properties point_light_properties()
   {

      static ::graphics3d::property s_properties[] =
      {
         {"position", ::gpu::e_type_seq4},
         {"color", ::gpu::e_type_seq4},
         {"radius", ::gpu::e_type_float},
         { nullptr, ::gpu::e_type_none}

      };

      return s_properties;

   }



   //inline ::graphics3d::properties global_ubo_properties()
   //{
   //   ::graphics3d::property s_properties[] = {
   //      "projection", ::gpu::e_type_mat4,
   //      "view", ::gpu::e_type_mat4,
   //      "inverseVew", ::gpu::e_type_mat4,
   //      "vec4", ::gpu::e_type_seq4,

   //   };


   //}


   class point_light_system :
      virtual public ::particle
   {
   public:


      ::pointer < ::graphics3d::context >      m_pcontext;

      ::pointer < ::graphics3d::shader >        m_pshader;
      //::pointer < ::graphics3d::engine >        m_pengine;


      point_light_system();
      ~point_light_system();

      
      void initialize_point_light_system(::graphics3d::context* pcontext);


      virtual void render(::graphics3d::scene * pscene);
      virtual void update(::graphics3d::scene* pscene);

      //void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
      //void createPipeline(VkRenderPass renderPass);

      virtual void prepare();

      //VkPipelineLayout pipelineLayout;

   };


}  // namespace lve