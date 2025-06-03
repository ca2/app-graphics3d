// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "main_scene.h"
#include "simple_render_system.h"
#include "point_light_system.h"
#include "app-graphics3d/continuum/application.h"
#include "app-graphics3d/continuum/impact.h"
#include "core/platform/application.h"
#include "aura/graphics/graphics3d/camera.h"
#include "aura/graphics/gpu/context.h"
#include "aura/graphics/graphics3d/engine.h"
#include "aura/graphics/graphics3d/scene_object.h"


namespace app_graphics3d_continuum
{


   main_scene::main_scene()
   {

   }


   main_scene::~main_scene()
   {

   }


   void main_scene::on_initialize_scene()
   {

      m_pimpact = m_pengine->m_pimpact;



   }


   ::pointer < ::graphics3d::camera > main_scene::get_default_camera()
   {
      glm::vec3 camera = glm::vec3(0.0f, 1.0f, 3.0f);
      glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // Look at origin
      //glm::vec3 direction = glm::normalize(target - cameraPos);
      //camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      auto pcamera = __øcreate < ::graphics3d::camera>();
      pcamera->initialize_camera(target, camera);
      //pcamera->m_pimpact = m_pimpact;
      return pcamera;


   }


   void main_scene::on_load_scene(::gpu::context* pgpucontext)
   {

      m_propertiesGlobalUbo.set(::app_graphics3d_continuum::global_ubo_properties());


      {

         auto & flatVase = tinyobjloader_object("matter://models/flat_vase.obj");
         flatVase.translate({ -.5f, 0.f, 0.f });
         flatVase.scale({3.f, -1.5f, 3.f}); // The vase is upside down.

      }

      {

         auto & floor = tinyobjloader_object("matter://models/quad.obj");
         floor.translate({0.f, 0.f, 0.f});
         floor.scale({5.f, -1.f, 5.f});

      }

      {

         auto & smoothVase = tinyobjloader_object("matter://models/smooth_vase.obj");
         smoothVase.translate({.5f, .0f, 0.f});
         smoothVase.scale({3.f, -1.5f, 3.f}); // The vase is upside down.

      }

      {

         auto & stoneSphere = tinyobjloader_object("matter://models/StoneSphere.obj");
         stoneSphere.translate({ .0f, 0.5f, 0.f });
         stoneSphere.scale({.25f, .25f, .25f});

      }

      {

         auto & woodBarrel = tinyobjloader_object("matter://models/Barrel_OBJ.obj");
         woodBarrel.translate({ 1.f, 0.f, 1.0f });
         woodBarrel.scale({1.f, 1.f, 1.f});

      }

      float fLo = 0.5f;

      std::vector<glm::vec3> lightColors{
            {1.f, fLo, fLo},
            {fLo, fLo, 1.f},
            {fLo, 1.f, fLo},
            {1.f, 1.f, fLo},
            {fLo, 1.f, 1.f},
            {1.f, 1.f, 1.f}
      };

      for (int i = 0; i < lightColors.size(); i++) 
      {
         auto pointLight = __allocate ::graphics3d::point_light (0.2f);
         pointLight->m_color = lightColors[i];
         auto rotateLight = glm::rotate(
            glm::mat4(1.f),
            (i * glm::two_pi<float>()) / lightColors.size(),
            { 0.f, 1.f, 0.f });
         pointLight->m_pointlightcomponent.lightIntensity = 1.0f;
         pointLight->m_transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, 1.7f, 0.5f, 1.f));
         add_object(pointLight);

      }


      __construct_new(m_psimplerendersystem);

      m_psimplerendersystem->initialize_simple_render_system(m_pengine);

      m_psimplerendersystem->prepare(pgpucontext);
      //m_prenderer->getRenderPass(),
      //globalSetLayout->getDescriptorSetLayout() };

      __construct_new(m_ppointlightsystem);

      m_ppointlightsystem->initialize_point_light_system(m_pengine);

      m_ppointlightsystem->prepare(pgpucontext);


   }


   void main_scene::on_update_global_ubo(::gpu::context* pgpucontext)
   {

      auto& globalubo = this->global_ubo();

      //::graphics3d::GlobalUbo ubo{};
      globalubo["projection"] = m_pengine->m_pcamera->getProjection();
      globalubo["view"] = m_pengine->m_pcamera->getView();
      globalubo["invView"] = m_pengine->m_pcamera->getInverseView();
      if (m_ppointlightsystem)
      {
         m_ppointlightsystem->update(pgpucontext, this);
      }

   }

   
   void main_scene::on_render(::gpu::context * pgpucontext)
   {


      //pgpucontext->clear(rgba(0.5f, 0.75f, 1.0f, 1.0f)); // Clear with a light blue color

      //return;

      //if (pgpucontext->m_eoutput == ::gpu::e_output_cpu_buffer)
      //{
      //   auto colorLightBlue = rgba(0.5f, 0.75f, 1.0f, 1.0f);
      //   pgpucontext->clear(colorLightBlue);
      //}

      //m_pengine->update_global_ubo();

      if (m_psimplerendersystem && m_ppointlightsystem)
      {

         m_psimplerendersystem->render(pgpucontext, this);
         m_ppointlightsystem->render(pgpucontext, this);

      }


   }


} // namespace app_graphics3d_continuum



