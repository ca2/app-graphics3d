// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "camera.h"
#include "impact.h"
#include "main_scene.h"
#include "bred/gpu/context.h"
#include "bred/graphics3d/asset_manager.h"
#include "bred/graphics3d/camera.h"
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

         //floating_sequence3 camera = floating_sequence3(0.0f, 1.0f *m_pengine->m_fYScale, 3.0f);
         floating_sequence3 camera = floating_sequence3(0.0f, 1.0f, 3.0f);
         floating_sequence3 target = floating_sequence3(0.0f, 0.0f, 0.0f); // Look at origin
         //floating_sequence3 direction = glm::normalize(target - cameraPos);
         //camera camera{ floating_sequence3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
         auto pcamera = øcreate_new<::app_graphics3d_continuum::camera>();
         pcamera->m_pengine = m_pimmersionlayer->m_pengine;
         pcamera->initialize_camera(target, camera);

         float aspect = m_pimmersionlayer->m_pengine->m_pusergraphics3d->getAspectRatio();

         pcamera->setPerspectiveProjection(::radians(50.f), aspect, 0.1f, 100.f);
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


      m_strSkybox = m_papp->m_strSkybox;

      for (auto& strSkybox : m_papp->m_straSkybox)
      {

         auto& pskybox = m_mapSkybox[strSkybox];

         ødefer_construct_new(pskybox);

         pskybox->initialize_sky_box(this, strSkybox);

      }



      float fXScale;

      fXScale = m_pimmersionlayer->m_pengine->m_fYScale;

      {

         auto & flatVase = scene_renderable("matter://models/flat_vase.obj");
         flatVase.translate({ -.5f, 0.f, 0.f });
         flatVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.

      }

      {

         auto &floor = scene_renderable("matter://models/quad.obj");
         floor.translate({0.f, 0.f, 0.f});
         floor.scale({5.f, -1.f, 5.f * fXScale });

      }

      {

         auto &smoothVase = scene_renderable("matter://models/smooth_vase.obj");
         smoothVase.translate({.5f, .0f, 0.f});
         smoothVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.

      }

      {

         auto &stoneSphere = scene_renderable("matter://models/StoneSphere.obj");
         stoneSphere.translate({ .0f, 0.0f, 0.f });
         stoneSphere.scale({.25f, .25f, .25f });

      }

      {

         auto &woodBarrel = scene_renderable("matter://models/Barrel_OBJ.obj");
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
         auto ppointlight = create_point_light(0.2f, 0.1f);
         ppointlight->m_color = lightColors[i];
         auto rotateLight = 
            floating_matrix4(1.f).rotated(
            (i * ::two_π<float>) / lightColors.size(),
            { 0.f, 1.f, 0.f });
         ppointlight->m_fLightIntensity = 1.0f;
         ppointlight->transform().m_vec3Position = floating_sequence3(rotateLight * floating_sequence4(-1.f, 1.7f, 0.5f, 1.f));
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

      //m_pskyboxrendersystem->set_skybox(current_sky_box());

      auto& globalubo = this->global_ubo();

      //pgpucontext->clear(::argb(.5f, 0.f, 0.f, 0.5f));

      //::graphics3d::GlobalUbo ubo{};
      ::cast<::app_graphics3d_continuum::camera> pcamera = camera();

      
         floating_matrix4 matrixImpact;
         if (m_pimmersionlayer->m_pengine->m_fYScale < 0)
         {
            matrixImpact = glm::lookAtRH(pcamera->m_locationPosition,
                                         pcamera->m_locationPosition + pcamera->m_poleFront,
                             pcamera->m_poleWorldUp);
            // matrixImpact[2][0] = -matrixImpact[2][0];
            // matrixImpact[2][1] = -matrixImpact[2][1];
            // matrixImpact[2][2] = -matrixImpact[2][2];
            // matrixImpact[2][3] = -matrixImpact[2][3];
         }
         else
         {
            matrixImpact =
               glm::lookAtRH(pcamera->m_locationPosition, pcamera->m_locationPosition + pcamera->m_poleFront, pcamera->m_poleWorldUp);
         }
         pcamera->m_matrixImpact = matrixImpact;
         //}

         pcamera->m_matrixAntImpact = glm::inverse(pcamera->m_matrixImpact);



      auto projection = pcamera->getProjection();
      globalubo["projection"] = projection;

      auto view = pcamera->getView();
      globalubo["view"] = view;

      auto inverseView = pcamera->getInverseView();
      globalubo["invView"] = inverseView;

      if (m_ppointlightrendersystem)
      {

         m_ppointlightrendersystem->update(pgpucontext, this);

      }


   }


   //::graphics3d::skybox* main_scene::get_skybox()
   //{

   //   ::string strSkybox = m_papp->m_strSkybox;

   //   return m_mapSkybox[strSkybox];

   //}

   
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



