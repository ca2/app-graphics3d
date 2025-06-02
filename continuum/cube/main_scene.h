// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#pragma once


#include "aura/graphics/graphics3d/scene.h"


namespace app_graphics3d_continuum
{




   class main_scene :
      virtual public ::graphics3d::scene
   {
   public:



      ::pointer < impact >                   m_pimpact;
      ::pointer < simple_render_system >		m_psimplerendersystem;
      ::pointer < point_light_system >			m_ppointlightsystem;



      main_scene();
      ~main_scene() override;

      
      void on_initialize_scene() override;


      ::pointer < ::graphics3d::camera > get_default_camera() override;


      void on_load_scene(::gpu::context* pgpucontext) override;


      void on_update_global_ubo(::gpu::context* pgpucontext) override;
      void on_render(::gpu::context * pgpucontext) override;


   };


} // namespace app_graphics3d_continuum



