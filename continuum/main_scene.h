// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#pragma once


#include "apex/platform/app_consumer.h"
#include "bred/graphics3d/skybox.h"
#include "bred/graphics3d/scene.h"


namespace app_graphics3d_continuum
{


   class main_scene :
      virtual public ::app_consumer < application, ::graphics3d::scene >
   {
   public:


      //::pointer< ::graphics3d::sky_box> m_Skybox;


      //::pointer < simple_render_system >		m_psimplerendersystem;
      //::pointer < point_light_system >			m_ppointlightsystem;

      ::pointer < ::graphics3d::object_render_system >		m_pobjectrendersystem;
      ::pointer < ::graphics3d::point_light_render_system >		m_ppointlightrendersystem;
      ::pointer < ::graphics3d::skybox_render_system >		m_pskyboxrendersystem;

      string_map < ::pointer < ::graphics3d::sky_box > > m_mapSkybox;

      main_scene();
      ~main_scene() override;

      
      void on_initialize_scene() override;

      virtual ::graphics3d::sky_box * get_skybox();

      ::graphics3d::camera* get_default_camera() override;


      void on_load_scene(::gpu::context* pgpucontext) override;


      void on_update_global_ubo(::gpu::context* pgpucontext) override;
      void on_render(::gpu::context * pgpucontext) override;


   };


} // namespace app_graphics3d_continuum



