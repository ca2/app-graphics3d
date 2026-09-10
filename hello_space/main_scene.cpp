// From hello_space (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#include "platform.h"
#include "application.h"
#include "camera.h"
#include "impact.h"
#include "input.h"
#include "main_scene.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "acme/filesystem/filesystem/directory_system.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "aura/windowing/display.h"
#include "aura/windowing/windowing.h"
#include "bred/graphics3d/camera.h"
#include "aura/graphics/image/context.h"
#include "bred/gpu/binding.h"
#include "bred/gpu/block.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/device.h"
#include "bred/gpu/renderer.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/texture.h"
#include "bred/gpu/texture_site.h"
#include "bred/graphics3d/asset_manager.h"
#include "bred/graphics3d/camera.h"
#include "bred/graphics3d/engine_instance.h"
#include "bred/graphics3d/point_light.h"
#include "bred/graphics3d/scene_object.h"
#include "bred/graphics3d/render_system/wavefront_obj_render_system.h"
#include "bred/graphics3d/render_system/point_light_render_system.h"
#include "bred/graphics3d/render_system/skybox_render_system.h"
#include "bred/graphics3d/render_system/texture_render_system.h"


namespace app_graphics3d_hello_space
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

         //floating_sequence3 camera = floating_sequence3(0.0f, 1.0f *m_pgraphics3dengineinstance->m_fYScale, 3.0f);
         floating_sequence3 camera = floating_sequence3(0.0f, 1.0f, 3.0f);
         floating_sequence3 target = floating_sequence3(0.0f, 0.0f, 0.0f); // Look at origin
         //floating_sequence3 direction = glm::normalize(target - cameraPos);
         //camera camera{ floating_sequence3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
         auto pcamera = create_newø<::app_graphics3d_hello_space::camera>();
         pcamera->m_pgraphics3dengineinstance = m_pimmersionlayer->m_pgraphics3dengineinstance;
         pcamera->initialize_camera(target, camera);

         //float aspect = m_pimmersionlayer->m_pgraphics3dengineinstance->m_pusergraphics3d->getAspectRatio();

         pcamera->m_angleFovY = 50_f_degrees;
         //pcamera->m_fAspectRatio = aspect;
         pcamera->m_fNearZ  = 0.1f;
         pcamera->m_fFarZ = 100.f;

         //pcamera->m_pimpact = m_pimpact;
         m_pcameraDefault = pcamera;

      }

      return m_pcameraDefault;

   }
   inline const ::gpu::property *overlay1_properties()
   {

      static ::gpu::property s_propertya[] = {
         {"overlayTopLeft", ::gpu::e_type_seq2}, {"overlayBottomRight", ::gpu::e_type_seq2},
                                              {"overlayOpacity", ::gpu::e_type_f32},
                                              {nullptr, ::gpu::e_type_none}

      };

      return s_propertya;
   }


   void main_scene::on_load_scene(::gpu::context* pgpucontext)
   {

      ::app_graphics3d_continuum::main_scene::on_load_scene(pgpucontext);

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

      //::string strSkybox = m_papp->m_strSkybox;

      //m_strSkybox = strSkybox;

      //for (auto& strSkybox : m_papp->m_straSkybox)
      //{

      //   auto& pskybox = m_mapSkybox[strSkybox];

      //   defer_construct_newø(pskybox);

      //   pskybox->initialize_sky_box(this, strSkybox);

      //}

      //m_pskyboxCurrent2 = m_mapSkybox[m_strSkybox];

      ///float fXScale;

      ///fXScale = m_pimmersionlayer->m_pgraphics3dengineinstance->m_fYScale;

      //{

      //   auto & flatVase = scene_renderable("matter://models/flat_vase.obj", true);
      //   flatVase.translate({ -.5f, 0.f, 0.f });
      //   //flatVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.
      //   flatVase.scale({3.f, 1.5f, 3.f}); // The vase is upside down.
      //   //flatVase.m_matrixRotation = ::floating_matrix4(1.f).rotate(::floating_sequence3(1, 0, 0), 180.f_degrees);
      //   flatVase.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
      //   flatVase.m_strName = "Flat Vase";

      //}

      //{

      //   auto &floor = scene_renderable("matter://models/quad.obj", true);
      //   floor.translate({0.f, 0.f, 0.f});
      //   //floor.scale({5.f, -1.f, 5.f * fXScale });
      //   floor.scale({5.f, 1.f, 5.f});
      //   ///floor.m_matrixRotation = ::floating_matrix4(1.f).rotate(::floating_sequence3(1, 0, 0), 180.f_degrees);
      //   floor.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
      //   floor.m_strName = "Floor";

      //}

      //{

      //   auto &smoothVase = scene_renderable("matter://models/smooth_vase.obj", true);
      //   smoothVase.translate({.5f, .0f, 0.f});
      //   //smoothVase.scale({3.f, -1.5f, 3.f * fXScale }); // The vase is upside down.
      //   smoothVase.scale({3.f, 1.5f, 3.f}); // The vase is upside down.
      //   //smoothVase.m_matrixRotation = ::floating_matrix4(1.f).rotate(::floating_sequence3(1, 0, 0), 180.f_degrees);
      //   smoothVase.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
      //   smoothVase.m_strName = "Smooth Vase";

      //}

      //{

      //   auto &stoneSphere = scene_renderable("matter://models/StoneSphere.obj", false);
      //   stoneSphere.translate({ .0f, 0.0f, 0.f });
      //   stoneSphere.scale({.25f, .25f, .25f });
      //   //stoneSphere.m_ecoordinatesystem = ::gpu::e_coordinate_system_znf;
      //   stoneSphere.m_strName = "Stone Sphere";

      //}

      //{

      //   auto &woodBarrel = scene_renderable("matter://models/Barrel_OBJ.obj", false);
      //   woodBarrel.translate({ 1.5f, 0.f, 1.0f });
      //   woodBarrel.scale({1.f, 1.f, 1.f });
      //   //woodBarrel.m_ecoordinatesystem = ::gpu::e_coordinate_system_znf;
      //   woodBarrel.m_strName = "Wood Barrel";

      //}

      {

         auto &lcdMonitor = scene_renderable("matter://models/MicrosoftMonitor.obj", false);
         lcdMonitor.translate({-1.4f, 0.f, -1.0f});
         lcdMonitor.scale({0.03f, 0.03f, 0.03f});
         lcdMonitor.m_matrixRotation.rotate({0.0f, 1.0f, 0.0f}, 25_f_degrees);
            
         //lcdMonitor.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
         // woodBarrel.m_ecoordinatesystem = ::gpu::e_coordinate_system_znf;
         lcdMonitor.m_strName = "LCD Monitor";
      }

      {
         //f64_angle angleExtra = 80_degrees;
         auto angleExtra = 180_f_degrees;
         double dShift = 0.0;
         auto &screen = scene_renderable("matter://models/quad2.obj", true);
         screen.translate({-1.32,0.72, -0.84 - dShift});
         // floor.scale({5.f, -1.f, 5.f * fXScale });
         screen.scale({9.0 * 0.0418, 1.0, 16.4 * 0.0418});
         screen.m_matrixRotation.rotate({0.0, 1.0, 0.0}, 25_f_degrees + angleExtra);
         screen.m_matrixRotation.rotate({0.0, 0.0, 1.0}, 270_f_degrees);
         screen.m_matrixRotation.rotate({1.0, 0.0, 0.0}, 90_f_degrees);
         m_prenderable = &screen;
         /// floor.m_matrixRotation = ::floating_matrix4(1.f).rotate(::floating_sequence3(1, 0, 0), 180.f_degrees);
         screen.m_ecoordinatesystem = ::gpu::e_coordinate_system_vulkan;
         screen.m_prenderable->m_egpumodel = ::gpu::e_model_wavefront_for_texture;
         screen.m_erendersystem = ::graphics3d::e_render_system_texture;
         screen.m_strName = "Screen";
         //screen.m_prenderable->m_pimageTextureNew = image()->path_image("dropbox://Photos/p.jpg");
         //screen.m_prenderable->m_pimageTextureNew = image()->path_image("dropbox://Photos/tbs8.jpg");
         //screen.m_prenderable->m_pimageTextureNew = image()->path_image("dropbox://Photos/weather/clear/day/ocean.jpg");
      }

      //m_pimageHelloMultiverseScreen = image()->path_image("dropbox://Photos/weather/day/clear/ocean.jpg");
      
      auto pdisplay = system()->windowing()->display();

      ::i32_rectangle rectangleMainMonitor;

      auto iMainMonitor = pdisplay->get_main_monitor(rectangleMainMonitor);

      ::file::path pathBackground = pdisplay->get_wallpaper(iMainMonitor);

      if (pathBackground.is_empty() || !file()->exists(pathBackground))
      {

         pathBackground = directory_system()->roaming() / "app-core/ambient/background.png";

      }

      if (!file()->exists(pathBackground))
      {

         pathBackground = "dropbox://Photos/weather/day/clear/ocean.jpg";

      }

      m_pimageHelloMultiverseScreen = image()->path_image(pathBackground);
      
      construct_newø(m_pbitmapsourcebuffergraphics);

      m_pbitmapsourcebuffergraphics->set_bitmap_source("HelloMultiverse!!", false);

      //float fLo = 0.5f;

      //::array_base < ::color::color > lightColors={
      //     argb(1.f, 1.f, fLo, fLo),
      //     argb(1.f, fLo, fLo, 1.f),
      //     argb(1.f, fLo, 1.f, fLo),
      //     argb(1.f, 1.f, 1.f, fLo),
      //     argb(1.f, fLo, 1.f, 1.f),
      //     argb(1.f, 1.f, 1.f, 1.f)
      //};

      // auto orbit_center = floating_sequence3(0.f, 2.0f, 0.0f);

      //for (int i = 0; i < lightColors.size(); i++) 
      //{
      //   auto ppointlight = create_point_light(1.0f, 0.1f, lightColors[i]);
      //   auto rotateLight =
      //      floating_matrix4(1.f).rotated(
      //      ::radians((i * _2πf) / lightColors.size()),
      //      { 0.f, 1.f, 0.f });
      //    ::floating_sequence3 hand(1.0f, 0.f, 0.f);
      //    auto rotated_hand = rotateLight * hand;
      //      auto position = orbit_center + rotated_hand;
      //   ppointlight->m_sequence3Translation = position;
      //   ppointlight->m_strName.format("Point Light {}", i);

      //}


      //constructø(m_pskyboxrendersystem);

      //m_pskyboxrendersystem->initialize_render_system(m_pimmersionlayer->m_pgraphics3dengineinstance);

      //m_pskyboxrendersystem->prepare(pgpucontext);



      //constructø(m_pwavefrontobjrendersystem);

      //m_pwavefrontobjrendersystem->initialize_render_system(m_pimmersionlayer->m_pgraphics3dengineinstance);

      //m_pwavefrontobjrendersystem->prepare(pgpucontext);
      ////m_prenderer->getRenderPass(),
      ////globalSetLayout->getDescriptorSetLayout() };

      //construct_newø(m_ppointlightrendersystem);

      //m_ppointlightrendersystem->initialize_render_system(m_pimmersionlayer->m_pgraphics3dengineinstance);

      //m_ppointlightrendersystem->prepare(pgpucontext);

      constructø(m_ptexturerendersystem);

      m_ptexturerendersystem->initialize_render_system(m_pimmersionlayer->m_pgraphics3dengineinstance);

      m_ptexturerendersystem->prepare(pgpucontext);


      constructø(m_pgpushaderBlend);
      /*m_pgpushaderBlend->m_propertiesPushShared.set_properties(overlay1_properties());
      pgpucontext->layout_push_constants(m_pgpushaderBlend->m_propertiesPushShared, false);
      m_pgpushaderBlend->m_bEnableBlend = true;
      m_pgpushaderBlend->m_bDisableDepthTest = true;
      auto pbindingTexture1 = m_pgpushaderBlend->binding(1, 0);
      pbindingTexture1->m_strUniform = "backgroundTexture";
      pbindingTexture1->m_ebinding = ::gpu::e_binding_sampler2d;
      pbindingTexture1->m_iTextureUnit = 0;
      auto pbindingTexture2 = m_pgpushaderBlend->binding(2, 0);
      pbindingTexture2->m_strUniform = "overlayTexture";
      pbindingTexture2->m_ebinding = ::gpu::e_binding_sampler2d;
      pbindingTexture2->m_iTextureUnit = 1;*/

      m_pgpushaderBlend->m_bEnableBlend = true;
      //m_pshaderBlend3->m_bindingSampler.set();
      m_pgpushaderBlend->m_bDisableDepthTest = true;

      auto pbindingSampler = m_pgpushaderBlend->binding();
      pbindingSampler->m_ebinding = ::gpu::e_binding_sampler2d;
      pbindingSampler->m_iTextureUnit = 0;
      //m_pgpushaderBlend->m_bT
      //m_pgpushaderBlend->m_pgpurenderer = this;
      //m_pgpushaderBlend->m_setbindingSampler = 0;
      // Image Blend descriptors
      //if (!m_psetdescriptorlayoutImageBlend)
      m_pgpushaderBlend->m_propertiesPushShared.set_properties(::gpu_properties<::gpu::quad>());

      pgpucontext->layout_push_constants(m_pgpushaderBlend->m_propertiesPushShared, false);

      pgpucontext->_001InitializeBlendShader(m_pgpushaderBlend);


   }


   void main_scene::on_update(::gpu::context* pgpucontext)
   {

      ::app_graphics3d_continuum::main_scene::on_update(pgpucontext);

//      //auto& globalubo = this->global_ubo();
//
//      auto pblockGlobalUbo1 = this->global_ubo1(pgpucontext);
//
//      auto &globalUbo1 = *pblockGlobalUbo1;
//
//      //pgpucontext->clear(::argb(.5f, 0.f, 0.f, 0.5f));
//
//      //::graphics3d::GlobalUbo ubo{};
//      ::cast<::app_graphics3d_hello_space::camera> pcamera = camera();
//
//      
//         //floating_matrix4 matrixImpact;
//
//         ////auto pgpucontext = 
//         //matrixImpact = m_pgpucontext->lookAt(pcamera->m_locationPosition,
//         //                                pcamera->m_locationPosition + pcamera->m_sequence3Front,
//         //                    pcamera->m_sequence3WorldUp);
//         ////if (m_pimmersionlayer->m_pgraphics3dengineinstance->m_fYScale < 0)
//         ////{
//         ////   matrixImpact = glm::lookAtRH(pcamera->m_locationPosition,
//         ////                                pcamera->m_locationPosition + pcamera->m_sequence3Front,
//         ////                    pcamera->m_sequence3WorldUp);
//         ////   // matrixImpact[2][0] = -matrixImpact[2][0];
//         ////   // matrixImpact[2][1] = -matrixImpact[2][1];
//         ////   // matrixImpact[2][2] = -matrixImpact[2][2];
//         ////   // matrixImpact[2][3] = -matrixImpact[2][3];
//         ////}
//         ////else
//         ////{
//         ////   matrixImpact =
//         ////      glm::lookAtRH(pcamera->m_locationPosition, pcamera->m_locationPosition + pcamera->m_sequence3Front, pcamera->m_sequence3WorldUp);
//         ////}
//         //pcamera->m_matrixImpact = matrixImpact;
//         //}
//
////         pcamera->m_matrixInversedImpact = pcamera->m_matrixImpact.inversed();
//
//
//      //auto &globalubo = this->global_ubo();
//
//         auto pimmersion = m_pimmersionlayer;
//
//         auto pscene = pimmersion->m_pscene;
//
//         //auto pgpucamera = pscene->camera();
//
//         //::cast<camera> pcamera = pgpucamera;
//
//         auto dt = m_pimmersionlayer->m_pgraphics3dengineinstance->dt();
//
//         ::cast<input> pinput = m_pimmersionlayer->m_pgraphics3dengineinstance->m_pinput;
//
//         auto &transform = m_pimmersionlayer->m_pgraphics3dengineinstance->m_transform;
//
//         transform.m_sequence3Position = pcamera->m_sequence3Position;
//
//         pinput->_017Update(dt, transform);
//
//         auto positionTransform = transform.m_sequence3Position;
//
//         pcamera->m_sequence3Position = positionTransform;
//
//         pcamera->m_rotation = transform.m_rotation;
//
//         auto aspect = m_pimmersionlayer->m_pgraphics3dengineinstance->m_pusergraphics3d->getAspectRatio();
//
//         pcamera->m_fAspectRatio = aspect;
//
//         //pcamera->m_fNearZ = 0.1f;
//
//         //pcamera->m_fFarZ = 100.0f;
//
//         //pcamera->m_angleFovY = 45.0f_degrees;
//
//         pcamera->update_vectors();
//
//         pcamera->update();
//
//         //float fNearZ = pcamera->m_fNearZ;
//         //float fFarZ = pcamera->m_fFarZ;
//
//      auto projection = pcamera->projection();
//      globalUbo1["projection"] = projection;
//
//      auto impact = pcamera->impact();
//      globalUbo1["view"] = impact;
//
//
//      ::floating_sequence4 seq4AmbientLightColor(0.2f, 0.2f, 0.2f, 0.2f);
//      globalUbo1["ambientLightColor"] = seq4AmbientLightColor;
//            
//
//      //auto inversedImpact = pcamera->inversed_impact();
//      auto inversedImpact = impact.inversed();
//      globalUbo1["invView"] = inversedImpact;
//
//      auto cameraPosition = pcamera->position();
//      globalUbo1["cameraPosition"] = cameraPosition;
//
//
//      if (m_ppointlightrendersystem)
//      {
//
//         m_ppointlightrendersystem->update(pgpucontext, this);
//
//      }


   }


   void main_scene::on_before_render(::gpu::context * pgpucontext)
   {

      auto pgpucommandbuffer = pgpucontext->beginSingleTimeCommands(pgpucontext->m_pgpudevice->graphics_queue());

      bool bCommandBufferRecordingSucceeded = true;

            if (!m_pmodelbufferDummy)
      {

         defer_constructø(m_pmodelbufferDummy);

         m_pmodelbufferDummy->initialize_dummy_model(pgpucontext->get_gpu_renderer(), 3);

         // m_pmodelbufferDummy->m_iVertexCount = 3;
      }


      auto pdisplay = system()->windowing()->display();

      ::i32_rectangle rectangleMainMonitor;

      pdisplay->get_main_monitor(rectangleMainMonitor);

      auto sizeMainMonitor = rectangleMainMonitor.size();

      if (sizeMainMonitor.is_empty())
      {

         sizeMainMonitor = pdisplay->get_main_monitor_size();
         rectangleMainMonitor = {0, 0, sizeMainMonitor.cx, sizeMainMonitor.cy};

      }


      defer_construct_newø(m_pgputexturesiteHelloMultiverseScreen);
      defer_construct_newø(m_pgputexturesiteHelloMultiverse);
      defer_construct_newø(m_pgputexturesiteMonitorMultisample);
      defer_construct_newø(m_pgputexturesiteMonitor2);


      if (m_pimageHelloMultiverseScreen.ok())
      {

         if (!m_pgputexturesiteHelloMultiverseScreen->gpu_texture())
         {

            if (defer_constructø(m_pgputexturesiteHelloMultiverseScreen->m_pgputextureSite))
            {

               auto pgputextureHelloMultiverseScreen = m_pgputexturesiteHelloMultiverseScreen->gpu_texture();

               ::gpu::texture_flags flagsHelloMultiverseScreen;
               flagsHelloMultiverseScreen.m_bShaderResource = true;
               flagsHelloMultiverseScreen.m_bTransferTarget = true;

               pgputextureHelloMultiverseScreen->create_texture(pgpucontext, m_pimageHelloMultiverseScreen->size(), flagsHelloMultiverseScreen);

               auto ppixmapImageHelloMultiverseScreen = m_pimageHelloMultiverseScreen->map();

               //pgputextureHelloMultiverseScreen->write_pixels(pgpucommandbuffer, ppixmapImageHelloMultiverseScreen, {});

               pgputextureHelloMultiverseScreen->write_pixels(true, ppixmapImageHelloMultiverseScreen, {});

               constructø(m_pgputexturesiteMonitorMultisample->m_pgputextureSite);

               auto pgputextureMonitorMultisample = m_pgputexturesiteMonitorMultisample->gpu_texture();

               pgputextureMonitorMultisample->m_bMultisample = true;

               ::gpu::texture_flags flagsMonitorMultisample;
               flagsMonitorMultisample.m_bRenderTarget = true;
               flagsMonitorMultisample.m_bTransferSource = true;

               pgputextureMonitorMultisample->create_texture(pgpucontext, sizeMainMonitor, flagsMonitorMultisample);

               constructø(m_pgputexturesiteMonitor2->m_pgputextureSite);
               
               auto pgputextureMonitor2 = m_pgputexturesiteMonitor2->gpu_texture();

               ::gpu::texture_flags flagsMonitor2;
               flagsMonitor2.m_bShaderResource = true;
               flagsMonitor2.m_bTransferTarget = true;

               pgputextureMonitor2->create_texture(pgpucontext, sizeMainMonitor, flagsMonitor2);

            }

         }

      }

      if (m_pbitmapsourcebuffergraphics)
      {

         if (m_pbitmapsourcebuffergraphics->m_pmemorymap)
         {

            void *pdata = m_pbitmapsourcebuffergraphics->m_pmemorymap->get_data();

            if (pdata != nullptr)
            {

               synchronous_lock synchronouslock(m_pbitmapsourcebuffergraphics->m_pmutexBitmapSource,
                                                DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

               try
               {

                  auto pheader = (::graphics::bitmap_source_buffer_header *)pdata;

                  ::i64 x;
                  ::i64 y;
                  ::i64 cx;
                  ::i64 cy;
                  ::i64 cxBitmap;
                  ::i64 cyBitmap;
                  ::i64 iScan;
                  bool bTopLeft;
                  ::i32_rectangle rectangleBitmapSourceMonitor;
                  ::memsize sBitmapSourceHeader;
                  ::image32_t *pimage32;

                  if (pheader->m_iMagic == ::graphics::BITMAP_SOURCE_BUFFER_MAGIC
                     && pheader->m_iVersion == ::graphics::BITMAP_SOURCE_BUFFER_VERSION)
                  {

                     x = pheader->m_xWindow;
                     y = pheader->m_yWindow;
                     cx = pheader->m_cxWindow;
                     cy = pheader->m_cyWindow;
                     cxBitmap = pheader->m_cxBitmap;
                     cyBitmap = pheader->m_cyBitmap;
                     iScan = pheader->m_iScan;
                     bTopLeft = pheader->m_bTopLeft != 0;
                     rectangleBitmapSourceMonitor = {
                        (::i32)pheader->m_xMonitor,
                        (::i32)pheader->m_yMonitor,
                        (::i32)(pheader->m_xMonitor + pheader->m_cxMonitor),
                        (::i32)(pheader->m_yMonitor + pheader->m_cyMonitor)};
                     sBitmapSourceHeader = sizeof(::graphics::bitmap_source_buffer_header);
                     pimage32 = (::image32_t *)(pheader + 1);

                  }
                  else
                  {

                     ::i64 *p = (::i64 *)pdata;

                     x = *p++;
                     y = *p++;
                     cx = *p++;
                     cy = *p++;
                     cxBitmap = *p++;
                     cyBitmap = *p++;
                     iScan = *p++;
                     bTopLeft = true;
                     rectangleBitmapSourceMonitor = rectangleMainMonitor;
                     sBitmapSourceHeader = 7 * sizeof(::i64);
                     pimage32 = (::image32_t *)p;

                  }

                  auto sBitmapSource = m_pbitmapsourcebuffergraphics->m_pmemorymap->m_size;

                  if (cx <= 0 || cy <= 0
                     || cxBitmap <= 0 || cyBitmap <= 0
                     || cxBitmap > 0x7fffffffLL || cyBitmap > 0x7fffffffLL
                     || iScan < cxBitmap * (::i64)sizeof(::image32_t)
                     || sBitmapSource < sBitmapSourceHeader
                     || (::u64)cyBitmap >
                        (::u64)(sBitmapSource - sBitmapSourceHeader) / (::u64)iScan
                     || rectangleMainMonitor.is_empty()
                     || rectangleBitmapSourceMonitor.is_empty())
                  {

                     throw ::exception(error_bad_argument);

                  }

                  //::copy_image32((::color32_t*)p, ppixmap->size(), iScan, ppixmap);

                  auto ppixmap = create_newø<::pixmap>();
                  ppixmap->m_pimage32Raw = pimage32;
                  ppixmap->m_pimage32 = (::image32_t *)nullptr;
                  ppixmap->m_point = {pheader->m_xWindow, pheader->m_yWindow};
                  ppixmap->m_size = { pheader->m_cxWindow, pheader->m_cyWindow };
                  ppixmap->m_sizeRaw = { pheader->m_cxBitmap, pheader->m_cyBitmap };
                  ppixmap->m_iScan = (::i32) iScan;
                  ppixmap->m_bTopLeft = bTopLeft;

                  // m_pimageHelloMultiverse->create({cx, cy},e_flag_success, iScan);

                  // m_pimageHelloMultiverse->m_memoryPixmap.set_size(m_pimageHelloMultiverse->scan_area_in_bytes());
                  // m_pimageHelloMultiverse->pixmap_t::copy(&pixmap);
                  // m_pimageHelloMultiverse->write_pixels(pixmap);

                  defer_constructø(m_pgputexturesiteHelloMultiverse->m_pgputextureSite);
                  //{

                  if (ppixmap->m_sizeRaw != m_pgputexturesiteHelloMultiverse->gpu_texture()->raw_size())
                  {

                     ::gpu::texture_flags flagsHelloMultiverse;
                     flagsHelloMultiverse.m_bShaderResource = true;
                     flagsHelloMultiverse.m_bTransferTarget = true;

                     m_pgputexturesiteHelloMultiverse->gpu_texture()->create_texture(
                        pgpucontext, ppixmap->m_sizeRaw, flagsHelloMultiverse);

                  }

                  m_pgputexturesiteHelloMultiverse->m_pointOutput = {};


                  
                  //ppixmap->pixmap_map();
                     
                     // (pgpucontext, {ppixmap});

                  //}
                  //else
                  //{

                  //   auto iTextureWidth = m_pgputexturesiteHelloMultiverse->width();

                  //   auto iTextureHeight = m_pgputexturesiteHelloMultiverse->height();

                  //   if (iTextureWidth != cx || iTextureHeight != cy)
                  //   {

                  //      m_pgputexturesiteHelloMultiverse->gpu_texture()->create_texture(pgpucontext, { (::i32) cx, (::i32) cy });

                  //   }

                  //}


                  if (m_pgputexturesiteHelloMultiverse)
                  {

                     {

                        auto p = ppixmap->map();

                        //m_pgputexturesiteHelloMultiverse->gpu_texture()->write_pixels(pgpucommandbuffer, ppixmap, {x, y});
                        m_pgputexturesiteHelloMultiverse->gpu_texture()->write_pixels(true, p, {});

                     }


                     // pgpucontext->copy(m_pgputextureMonitor, m_pgputextureHelloMultiverseScreen, nullptr);

                     auto pgputextureMonitorMultisample = m_pgputexturesiteMonitorMultisample->gpu_texture();

                     //m_pgputexturesiteMonitorMultisample->gpu_texture()->set_state(
                       // pgpucommandbuffer, ::gpu::e_texture_state_color_attachment
                     //);

                     m_pgputexturesiteHelloMultiverse->gpu_texture()->set_state(pgpucommandbuffer, ::gpu::e_texture_state_shader_read);
                     m_pgputexturesiteHelloMultiverseScreen->gpu_texture()->set_state(pgpucommandbuffer, ::gpu::e_texture_state_shader_read);

                     pgpucommandbuffer->m_iCommandBufferFrameIndex2 = 0;

                     pgpucommandbuffer->begin_render(m_pgpushaderBlend, m_pgputexturesiteMonitorMultisample);


                     m_pgpushaderBlend->bind_source(pgpucommandbuffer, m_pgputexturesiteHelloMultiverseScreen);

                     //auto pbindingsetTextureScreen = m_pgpushaderBlend->binding_set(1);

                     //auto pgputextureHelloMultiverseScreen = m_pgputexturesiteHelloMultiverseScreen->gpu_texture();

                     //auto pbindingslotsetTextureScreen = pgputextureHelloMultiverseScreen->binding_slot_set(
                     //   pgpucommandbuffer, pbindingsetTextureScreen);

                     //pgpucommandbuffer->bind_slot_set(1, pbindingslotsetTextureScreen);


                     //auto pbindingsetTexture = m_pgpushaderBlend->binding_set(2);

                     //auto pgputextureHelloMultiverse = m_pgputexturesiteHelloMultiverse->gpu_texture();

                     //auto pbindingslotsetTexture =
                       /// pgputextureHelloMultiverse->binding_slot_set(pgpucommandbuffer, pbindingsetTexture);

                     //pgpucommandbuffer->bind_slot_set(2, pbindingslotsetTexture);


                     auto rectangleMonitor = m_pgputexturesiteMonitorMultisample->output_placement();

                     auto sizeMonitorRaw = m_pgputexturesiteMonitorMultisample->raw_size();

                     pgpucommandbuffer->set_viewport(rectangleMonitor, sizeMonitorRaw);
                     pgpucommandbuffer->set_scissor(rectangleMonitor, sizeMonitorRaw);

                     m_pgpushaderBlend->set_impact_quad(
                        m_pgputexturesiteHelloMultiverseScreen->input_placement(),
                        m_pgputexturesiteHelloMultiverseScreen->raw_size());

                     m_pgpushaderBlend->push_properties(pgpucommandbuffer);

                     pgpucommandbuffer->draw(m_pmodelbufferDummy);

                     //pgpucommandbuffer->clear(m_pgputexturesiteHelloMultiverse->gpu_texture(), argb(128, 100, 180, 210));


                     m_pgpushaderBlend->bind_source(pgpucommandbuffer, m_pgputexturesiteHelloMultiverse);

                     //auto screen_x_to_monitor_texture_x =
                     //   [&](::i64 iScreenX)
                     //   {

                     //      return rectangleMonitor.left + (::i32)(
                     //         (iScreenX - rectangleBitmapSourceMonitor.left) *
                     //         rectangleMonitor.width() /
                     //         rectangleBitmapSourceMonitor.width());

                     //   };

                     //auto screen_y_to_monitor_texture_y =
                     //   [&](::i64 iScreenY)
                     //   {

                     //      return rectangleMonitor.top + (::i32)(
                     //         (iScreenY - rectangleBitmapSourceMonitor.top) *
                     //         rectangleMonitor.height() /
                     //         rectangleBitmapSourceMonitor.height());

                     //   };

                     //::i32_rectangle rectangleHelloMultiverseUnclipped(
                     //   screen_x_to_monitor_texture_x(x),
                     //   screen_y_to_monitor_texture_y(y),
                     //   screen_x_to_monitor_texture_x(x + cx),
                     //   screen_y_to_monitor_texture_y(y + cy));

                     //auto rectangleHelloMultiverse = rectangleHelloMultiverseUnclipped;

                     //rectangleHelloMultiverse.intersect(rectangleMonitor);

                     auto rectangleHelloMultiverse = ppixmap->rectangle();

                     if (rectangleHelloMultiverse.has_area())
                     {

                        pgpucommandbuffer->set_viewport(rectangleHelloMultiverse, sizeMonitorRaw);
                        pgpucommandbuffer->set_scissor(rectangleHelloMultiverse, sizeMonitorRaw);

                        //::i32_rectangle rectangleHelloMultiverseSource(
                        //   (::i32)((rectangleHelloMultiverse.left
                        //      - rectangleHelloMultiverseUnclipped.left)
                        //      * cxBitmap / rectangleHelloMultiverseUnclipped.width()),
                        //   (::i32)((rectangleHelloMultiverse.top
                        //      - rectangleHelloMultiverseUnclipped.top)
                        //      * cyBitmap / rectangleHelloMultiverseUnclipped.height()),
                        //   (::i32)((rectangleHelloMultiverse.right
                        //      - rectangleHelloMultiverseUnclipped.left)
                        //      * cxBitmap / rectangleHelloMultiverseUnclipped.width()),
                        //   (::i32)((rectangleHelloMultiverse.bottom
                        //      - rectangleHelloMultiverseUnclipped.top)
                        //      * cyBitmap / rectangleHelloMultiverseUnclipped.height()));
                        //::i32_rectangle rectangleHelloMultiverseSource(
                        //   (::i32)((rectangleHelloMultiverse.left
                        //      )
                        //      * cxBitmap / rectangleHelloMultiverseUnclipped.width()),
                        //   (::i32)((rectangleHelloMultiverse.top
                        //      )
                        //      * cyBitmap / rectangleHelloMultiverseUnclipped.height()),
                        //   (::i32)((rectangleHelloMultiverse.right
                        //      )
                        //      * cxBitmap / rectangleHelloMultiverseUnclipped.width()),
                        //   (::i32)((rectangleHelloMultiverse.bottom
                        //      )
                        //      * cyBitmap / rectangleHelloMultiverseUnclipped.height()));
                        rectangleHelloMultiverse.offset(-rectangleHelloMultiverse.origin());
                        m_pgpushaderBlend->set_impact_quad(
                           rectangleHelloMultiverse,
                           ppixmap->m_sizeRaw);

                        m_pgpushaderBlend->push_properties(pgpucommandbuffer);

                        pgpucommandbuffer->draw(m_pmodelbufferDummy);

                     }


                     //floating_sequence2 seq2TopLeft;

                     //seq2TopLeft.cx = ((double)x / (double)sizeMainMonitor.cx);
                     //seq2TopLeft.cy = ((double)y / (double)sizeMainMonitor.cy);

                     //floating_sequence2 seq2BottomRight;

                     //seq2BottomRight.cx = ((double)(x + cx) / (double)sizeMainMonitor.cx);
                     //seq2BottomRight.cy = ((double)(y + cy) / (double)sizeMainMonitor.cy);

                     //m_pgpushaderBlend->m_propertiesPushShared.seq2("overlayTopLeft") = seq2TopLeft;
                     //m_pgpushaderBlend->m_propertiesPushShared.seq2("overlayBottomRight") = seq2BottomRight;
                     //m_pgpushaderBlend->m_propertiesPushShared.seq2("overlayOpacity") = 1.0;

                     //m_pgpushaderBlend->push_properties(pgpucommandbuffer);
                     //

                     //m_pmodelbufferDummy->bind2(pgpucommandbuffer);

                     //pgpucommandbuffer->draw_vertexes(3);

                     //m_pmodelbufferDummy->unbind(pgpucommandbuffer);

                     m_pgpushaderBlend->unbind(pgpucommandbuffer);

                     pgpucommandbuffer->end_render();

                     pgpucontext->copy(pgpucommandbuffer, m_pgputexturesiteMonitor2, m_pgputexturesiteMonitorMultisample, nullptr, nullptr);

                     if (m_prenderable)
                     {

                        auto prenderableMonitor = m_prenderable->renderable();

                        if (prenderableMonitor)
                        {

                           prenderableMonitor->m_ptextureTexture = m_pgputexturesiteMonitor2->gpu_texture();

                        }

                     }
                  }

                  // memory_copy(p, ppixmap->m_pimage32Raw, ppixmap->height() * iScan);
               }
               catch (...)
               {
                  bCommandBufferRecordingSucceeded = false;
               }
            }
         }
      }


      if (bCommandBufferRecordingSucceeded)
      {

         pgpucommandbuffer.commit();

      }
      else
      {

         pgpucommandbuffer.cancel();

      }
   }


   void main_scene::on_render(::gpu::context * pgpucontext)
   {

      ::app_graphics3d_continuum::main_scene::on_render(pgpucontext);

      auto prenderer = pgpucontext->m_pgpurenderer;

      ////auto pgpurendertarget = prenderer->render_target();

      ////auto ptexture = pgpurendertarget->current_texture(::gpu::current_layer());

      ////pgpucontext->clear(ptexture, ::argb(1.0f, 0.5f, 0.75f, 1.0f)); // Clear with a light blue color

      //if (m_pskyboxrendersystem)
      //{

      //   m_pskyboxrendersystem->render(pgpucontext, this);

      //}

      //if (m_pwavefrontobjrendersystem)
      //{

      //   m_pwavefrontobjrendersystem->render(pgpucontext, this);

      //}

            if (m_ptexturerendersystem)
      {

               

         m_ptexturerendersystem->render(pgpucontext, this);
      }

      //if(m_ppointlightrendersystem)
      //{

      //   m_ppointlightrendersystem->render(pgpucontext, this);

      //}

   }


} // namespace app_graphics3d_hello_space



