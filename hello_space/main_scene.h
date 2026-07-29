// From hello_space (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#pragma once


#include "apex/platform/app_consumer.h"
#include "bred/graphics3d/skybox.h"
#include "graphics3d/scene.h"
#include "aura/graphics/graphics/bitmap_source_buffer.h"
#include "app-graphics3d/continuum/main_scene.h"


namespace app_graphics3d_hello_space
{


   class main_scene : virtual public ::app_consumer<application, ::app_graphics3d_continuum::main_scene>
   {
   public:


      //::pointer< ::graphics3d::sky_box> m_Skybox;


      //::pointer < simple_render_system >		m_psimplerendersystem;
      //::pointer < point_light_system >			m_ppointlightsystem;

      //::pointer < ::graphics3d::wavefront_obj_render_system >	m_pwavefrontobjrendersystem;
      //::pointer < ::graphics3d::point_light_render_system >		m_ppointlightrendersystem;
      ///::pointer < ::graphics3d::skybox_render_system >		   m_pskyboxrendersystem;
      ::pointer<::graphics3d::texture_render_system> m_ptexturerendersystem;
      ::image::image_pointer m_pimageHelloMultiverseScreen;
      ::pointer<::gpu::texture> m_pgputextureHelloMultiverseScreen;
      ::pointer<::gpu::texture> m_pgputextureHelloMultiverse;
      ::pointer<::gpu::texture> m_pgputextureMonitorMultisample;
      ::pointer<::gpu::texture> m_pgputextureMonitor2;
      ::pointer<::gpu::shader> m_pgpushaderBlend;
      ::pointer<::gpu::model_buffer> m_pmodelbufferDummy;
      ::graphics3d::scene_renderable *m_prenderable = nullptr;
      ::pointer<::graphics::bitmap_source_buffer> m_pbitmapsourcebuffer;

      //floating_sequence3 m_initialCameraPosition{0.f};
      //floating_sequence3 m_initialCameraRotation{0.f};
      //::pointer<::graphics3d::skybox>                          m_pskybox;

      main_scene();
      ~main_scene() override;

      
      void on_initialize_scene() override;

//      virtual ::graphics3d::skybox * get_skybox();

      ::graphics3d::camera* get_default_camera() override;


      void on_load_scene(::gpu::context* pgpucontext) override;


      void on_update(::gpu::context* pgpucontext) override;
      void on_before_render(::gpu::context *pgpucontext) override;
      void on_render(::gpu::context * pgpucontext) override;


   };


} // namespace app_graphics3d_hello_space



