// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "impact.h"
#include "main_scene.h"
#include "bred/graphics3d/camera.h"
#include "bred/gpu/context.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/point_light.h"
#include "bred/graphics3d/scene_object.h"
#include "bred/graphics3d/render_systems/wavefront_obj_render_system.h"
#include "bred/graphics3d/render_systems/point_light_render_system.h"
#include "bred/graphics3d/render_systems/skybox_render_system.h"


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

      scene::on_initialize_particle();

      m_papp->m_pmainscene = this;

   }


   ::graphics3d::camera * main_scene::get_default_camera()
   {

      if (!m_pcameraDefault)
      {

         //glm::vec3 camera = glm::vec3(0.0f, 1.0f *m_pengine->m_fYScale, 3.0f);
         glm::vec3 camera = glm::vec3(0.0f, 1.0f, 3.0f);
         glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // Look at origin
         //glm::vec3 direction = glm::normalize(target - cameraPos);
         //camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
         auto pcamera = øcreate < ::graphics3d::camera>();
         pcamera->m_pengine = m_pimmersionlayer->m_pengine;
         pcamera->initialize_camera(target, camera);
         //pcamera->m_pimpact = m_pimpact;
         m_pcameraDefault = pcamera;

      }

      return m_pcameraDefault;

   }


   void main_scene::on_load_scene(::gpu::context* pgpucontext)
   {

      m_gpupropertiesGlobalUbo.set<::app_graphics3d_continuum::global_ubo>();


      //::graphics3d::sky_box::cube cube = {

      //   // Cloudy skybox
      //  /* "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_ft.jpg",
      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_bk.jpg",
      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_dn.jpg",
      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_up.jpg",
      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_rt.jpg",
      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CloudySkybox/bluecloud_lf.jpg",*/

      //   // Hell skybox
      //  /* "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Front.png",
      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Back.png",

      //    "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Top.png",
      //    "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Bottom.png",

      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Left.png",
      //   "D:/Users/Admin/source/repos/MyHell2024/MyHell2024/res/Textures/CoolSkybox/NightSky_Right.png",
      // */


      // // Space sky_box
      //   {"matter://textures/SpaceSkybox/right.png",
      //  "matter://textures/SpaceSkybox/left.png",
      //  "matter://textures/SpaceSkybox/top.png",
      //  "matter://textures/SpaceSkybox/bottom.png",
      //  "matter://textures/SpaceSkybox/front.png",
      //  "matter://textures/SpaceSkybox/back.png"
      //  }
      //};

//      m_Skybox = øallocate::graphics3d::sky_box();



      for (auto& strSkybox : m_papp->m_straSkybox)
      {

         auto& pskybox = m_mapSkybox[strSkybox];

         ødefer_construct_new(pskybox);

         pskybox->initialize_sky_box(m_pimmersionlayer->m_pengine, strSkybox);

      }



      float fXScale;

      fXScale = m_pimmersionlayer->m_pengine->m_fYScale;

      {

         auto & flatVase = scene_object("matter://models/flat_vase.obj");
         flatVase.translate({ -.5f, 0.f, 0.f });
         flatVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.

      }

      {

         auto & floor = scene_object("matter://models/quad.obj");
         floor.translate({0.f, 0.f, 0.f});
         floor.scale({5.f, -1.f, 5.f * fXScale });

      }

      {

         auto & smoothVase = scene_object("matter://models/smooth_vase.obj");
         smoothVase.translate({.5f, .0f, 0.f});
         smoothVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.

      }

      {

         auto & stoneSphere = scene_object("matter://models/StoneSphere.obj");
         stoneSphere.translate({ .0f, 0.0f, 0.f });
         stoneSphere.scale({.25f, .25f, .25f });

      }

      {

         auto & woodBarrel = scene_object("matter://models/Barrel_OBJ.obj");
         woodBarrel.translate({ 1.f, 0.f, 1.0f });
         woodBarrel.scale({1.f, 1.f, 1.f });

      }

      float fLo = 0.5f;

      ::array_base < ::color::color > lightColors={
           argb(1.f, 1.f, fLo, fLo),
           argb(1.f, fLo, fLo, 1.f),
           argb(1.f, fLo, 1.f, fLo),
           argb(1.f, 1.f, 1.f, fLo),
           argb(1.f, fLo, 1.f, 1.f),
           argb(1.f, 1.f, 1.f, 1.f)
      };

      for (int i = 0; i < lightColors.size(); i++) 
      {
         auto ppointlight = create_point_light(0.2f);
         ppointlight->m_color = lightColors[i];
         auto rotateLight = glm::rotate(
            glm::mat4(1.f),
            (i * glm::two_pi<float>()) / lightColors.size(),
            { 0.f, 1.f, 0.f });
         ppointlight->m_fLightIntensity = 1.0f;
         ppointlight->transform().m_vec3Translation = glm::vec3(rotateLight * glm::vec4(-1.f, 1.7f, 0.5f , 1.f));
         //m_pointlighta.add(ppointlight);

      }


      øconstruct_new(m_pskyboxrendersystem);

      m_pskyboxrendersystem->initialize_render_system(m_pimmersionlayer->m_pengine);

      m_pskyboxrendersystem->prepare(pgpucontext);



      øconstruct_new(m_pwavefrontobjrendersystem);

      m_pwavefrontobjrendersystem->initialize_render_system(m_pimmersionlayer->m_pengine);

      m_pwavefrontobjrendersystem->prepare(pgpucontext);
      //m_prenderer->getRenderPass(),
      //globalSetLayout->getDescriptorSetLayout() };

      øconstruct_new(m_ppointlightrendersystem);

      m_ppointlightrendersystem->initialize_render_system(m_pimmersionlayer->m_pengine);

      m_ppointlightrendersystem->prepare(pgpucontext);


   }


   void main_scene::on_update(::gpu::context* pgpucontext)
   {

      m_pskyboxrendersystem->set_skybox(get_skybox());

      auto& globalubo = this->global_ubo();

      //pgpucontext->clear(::argb(.5f, 0.f, 0.f, 0.5f));

      //::graphics3d::GlobalUbo ubo{};

      auto projection = m_pimmersionlayer->m_pscene->m_pcameraCurrent->getProjection();
      globalubo["projection"] = projection;

      auto view = m_pimmersionlayer->m_pscene->m_pcameraCurrent->getView();
      globalubo["view"] = view;

      auto inverseView = m_pimmersionlayer->m_pscene->m_pcameraCurrent->getInverseView();
      globalubo["invView"] = inverseView;

      if (m_ppointlightrendersystem)
      {

         m_ppointlightrendersystem->update(pgpucontext, this);

      }


   }


   ::graphics3d::skybox* main_scene::get_skybox()
   {

      ::string strSkybox = m_papp->m_strSkybox;

      return m_mapSkybox[strSkybox];

   }

   
   void main_scene::on_render(::gpu::context * pgpucontext)
   {

      //pgpucontext->clear(rgba(0.5f, 0.75f, 1.0f, 1.0f)); // Clear with a light blue color

      if (m_pskyboxrendersystem)
      {

         m_pskyboxrendersystem->render(pgpucontext, this);

      }

      //return;

      if (m_pwavefrontobjrendersystem)
      {

         m_pwavefrontobjrendersystem->render(pgpucontext, this);

      }

      if(m_ppointlightrendersystem)
      {

         m_ppointlightrendersystem->render(pgpucontext, this);

      }

   }


} // namespace app_graphics3d_continuum



