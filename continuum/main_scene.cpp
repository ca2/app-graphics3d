// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application.h"
#include "camera.h"
#include "impact.h"
#include "input.h"
#include "main_scene.h"
#include "bred/gpu/block.h"
#include "bred/gpu/context.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/render_target.h"
#include "bred/graphics3d/asset_manager.h"
#include "bred/graphics3d/camera.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/point_light.h"
#include "bred/graphics3d/scene_object.h"
#include "bred/graphics3d/render_system/wavefront_obj_render_system.h"
#include "bred/graphics3d/render_system/point_light_render_system.h"
#include "bred/graphics3d/render_system/skybox_render_system.h"


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
         auto pcamera = create_newø<::app_graphics3d_continuum::camera>();
         pcamera->m_pengine = m_pimmersionlayer->m_pengine;
         pcamera->initialize_camera(target, camera);

         //float aspect = m_pimmersionlayer->m_pengine->m_pusergraphics3d->getAspectRatio();

         pcamera->m_angleFovY = 50_f_degrees;
         //pcamera->m_fAspectRatio = aspect;
         pcamera->m_fNearZ  = 0.1f;
         pcamera->m_fFarZ = 100.f;

         //pcamera->m_pimpact = m_pimpact;
         m_pcameraDefault = pcamera;

      }

      return m_pcameraDefault;

   }


   void main_scene::on_load_scene(::gpu::context* pgpucontext)
   {

      ///m_gpupropertiesGlobalUbo.set<::graphics3d::global_ubo1>();

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

      // m_Skybox = allocateø::graphics3d::sky_box();

      ::string strSkybox = m_papp->m_strSkybox;

      m_strSkybox = strSkybox;

      for (auto& strSkybox : m_papp->m_straSkybox)
      {

         auto& pskybox = m_mapSkybox[strSkybox];

         defer_construct_newø(pskybox);

         pskybox->initialize_sky_box(this, strSkybox);

      }

      m_pskyboxCurrent2 = m_mapSkybox[m_strSkybox];

      ///float fXScale;

      ///fXScale = m_pimmersionlayer->m_pengine->m_fYScale;

      {

         auto & flatVase = scene_renderable("matter://models/flat_vase.obj", true);
         flatVase.translate({ -.5f, 0.f, 0.f });
         //flatVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.
         flatVase.scale({3.f, 1.5f, 3.f}); // The vase is upside down.
         //flatVase.m_matrixRotation = ::floating_matrix4(1.f).rotate(::floating_sequence3(1, 0, 0), 180.f_degrees);
         flatVase.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
         flatVase.m_strName = "Flat Vase";

      }

      {

         auto &floor = scene_renderable("matter://models/quad.obj", true);
         floor.translate({0.f, 0.f, 0.f});
         //floor.scale({5.f, -1.f, 5.f * fXScale });
         floor.scale({5.f, 1.f, 5.f});
         ///floor.m_matrixRotation = ::floating_matrix4(1.f).rotate(::floating_sequence3(1, 0, 0), 180.f_degrees);
         floor.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
         floor.m_strName = "Floor";

      }

      {

         auto &smoothVase = scene_renderable("matter://models/smooth_vase.obj", true);
         smoothVase.translate({.5f, .0f, 0.f});
         //smoothVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.
         smoothVase.scale({3.f, 1.5f, 3.f}); // The vase is upside down.
         //smoothVase.m_matrixRotation = ::floating_matrix4(1.f).rotate(::floating_sequence3(1, 0, 0), 180.f_degrees);
         smoothVase.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
         smoothVase.m_strName = "Smooth Vase";

      }

      {

         auto &stoneSphere = scene_renderable("matter://models/StoneSphere.obj", false);
         stoneSphere.translate({ .0f, 0.0f, 0.f });
         stoneSphere.scale({.25f, .25f, .25f });
         //stoneSphere.m_ecoordinatesystem = ::gpu::e_coordinate_system_znf;
         stoneSphere.m_strName = "Stone Sphere";

      }

      {

         auto &woodBarrel = scene_renderable("matter://models/Barrel_OBJ.obj", false);
         woodBarrel.translate({ 1.5f, 0.f, 1.0f });
         woodBarrel.scale({1.f, 1.f, 1.f });
         //woodBarrel.m_ecoordinatesystem = ::gpu::e_coordinate_system_znf;
         woodBarrel.m_strName = "Wood Barrel";

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

       auto orbit_center = floating_sequence3(0.f, 2.0f, 0.0f);

      for (int i = 0; i < lightColors.size(); i++) 
      {
         auto ppointlight = create_point_light(1.0f, 0.1f, lightColors[i]);
         auto rotateLight =
            floating_matrix4(1.f).rotated(
            ::radians((i * _2πf) / lightColors.size()),
            { 0.f, 1.f, 0.f });
          ::floating_sequence3 hand(1.0f, 0.f, 0.f);
          auto rotated_hand = rotateLight * hand;
            auto position = orbit_center + rotated_hand;
         ppointlight->m_sequence3Translation = position;
         ppointlight->m_strName.format("Point Light {}", i);

      }


      constructø(m_pskyboxrendersystem);

      m_pskyboxrendersystem->initialize_render_system(m_pimmersionlayer->m_pengine);

      m_pskyboxrendersystem->prepare(pgpucontext);



      constructø(m_pwavefrontobjrendersystem);

      m_pwavefrontobjrendersystem->initialize_render_system(m_pimmersionlayer->m_pengine);

      m_pwavefrontobjrendersystem->prepare(pgpucontext);
      //m_prenderer->getRenderPass(),
      //globalSetLayout->getDescriptorSetLayout() };

      construct_newø(m_ppointlightrendersystem);

      m_ppointlightrendersystem->initialize_render_system(m_pimmersionlayer->m_pengine);

      m_ppointlightrendersystem->prepare(pgpucontext);


   }


   void main_scene::on_update(::gpu::context* pgpucontext)
   {

      //auto& globalubo = this->global_ubo();

      auto pblockGlobalUbo1 = this->global_ubo1(pgpucontext);

      auto &globalUbo1 = *pblockGlobalUbo1;

      //pgpucontext->clear(::argb(.5f, 0.f, 0.f, 0.5f));

      //::graphics3d::GlobalUbo ubo{};
      ::cast<::app_graphics3d_continuum::camera> pcamera = camera();

      
         //floating_matrix4 matrixImpact;

         ////auto pgpucontext = 
         //matrixImpact = m_pgpucontext->lookAt(pcamera->m_locationPosition,
         //                                pcamera->m_locationPosition + pcamera->m_sequence3Front,
         //                    pcamera->m_sequence3WorldUp);
         ////if (m_pimmersionlayer->m_pengine->m_fYScale < 0)
         ////{
         ////   matrixImpact = glm::lookAtRH(pcamera->m_locationPosition,
         ////                                pcamera->m_locationPosition + pcamera->m_sequence3Front,
         ////                    pcamera->m_sequence3WorldUp);
         ////   // matrixImpact[2][0] = -matrixImpact[2][0];
         ////   // matrixImpact[2][1] = -matrixImpact[2][1];
         ////   // matrixImpact[2][2] = -matrixImpact[2][2];
         ////   // matrixImpact[2][3] = -matrixImpact[2][3];
         ////}
         ////else
         ////{
         ////   matrixImpact =
         ////      glm::lookAtRH(pcamera->m_locationPosition, pcamera->m_locationPosition + pcamera->m_sequence3Front, pcamera->m_sequence3WorldUp);
         ////}
         //pcamera->m_matrixImpact = matrixImpact;
         //}

//         pcamera->m_matrixInversedImpact = pcamera->m_matrixImpact.inversed();


      //auto &globalubo = this->global_ubo();

         auto pimmersion = m_pimmersionlayer;

         auto pscene = pimmersion->m_pscene;

         //auto pgpucamera = pscene->camera();

         //::cast<camera> pcamera = pgpucamera;

         auto dt = m_pimmersionlayer->m_pengine->dt();

         ::cast<input> pinput = m_pimmersionlayer->m_pengine->m_pinput;

         auto &transform = m_pimmersionlayer->m_pengine->m_transform;

         transform.m_sequence3Position = pcamera->m_sequence3Position;

         pinput->_017Update(dt, transform);

         auto positionTransform = transform.m_sequence3Position;

         pcamera->m_sequence3Position = positionTransform;

         pcamera->m_rotation = transform.m_rotation;

         auto aspect = m_pimmersionlayer->m_pengine->m_pusergraphics3d->getAspectRatio();

         pcamera->m_fAspectRatio = aspect;

         //pcamera->m_fNearZ = 0.1f;

         //pcamera->m_fFarZ = 100.0f;

         //pcamera->m_angleFovY = 45.0f_degrees;

         pcamera->update_vectors();

         pcamera->update();

         //float fNearZ = pcamera->m_fNearZ;
         //float fFarZ = pcamera->m_fFarZ;

      auto projection = pcamera->projection();
      globalUbo1["projection"] = projection;

      auto impact = pcamera->impact();
      globalUbo1["view"] = impact;


      ::floating_sequence4 seq4AmbientLightColor(0.2f, 0.2f, 0.2f, 0.2f);
      globalUbo1["ambientLightColor"] = seq4AmbientLightColor;
            

      //auto inversedImpact = pcamera->inversed_impact();
      auto inversedImpact = impact.inversed();
      globalUbo1["invView"] = inversedImpact;

      auto cameraPosition = pcamera->position();
      globalUbo1["cameraPosition"] = cameraPosition;


      if (m_ppointlightrendersystem)
      {

         m_ppointlightrendersystem->update(pgpucontext, this);

      }


   }


   void main_scene::on_render(::gpu::context * pgpucontext)
   {

      auto prenderer = pgpucontext->m_pgpurenderer;

      //auto pgpurendertarget = prenderer->render_target();

      //auto ptexture = pgpurendertarget->current_texture(::gpu::current_layer());

      //pgpucontext->clear(ptexture, ::argb(1.0f, 0.5f, 0.75f, 1.0f)); // Clear with a light blue color

      if (m_pskyboxrendersystem)
      {

         m_pskyboxrendersystem->render(pgpucontext, this);

      }

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



