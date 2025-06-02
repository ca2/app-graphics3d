#pragma once


#include "cube/frame.h"


namespace app_graphics3d_continuum
{


   class CLASS_DECL_APP_GRAPHICS3D_CONTINUUM main_frame :
      virtual public ::cube::frame
   {
   public:


      bool                       m_bExplicitTranslucency;


      main_frame();
      ~main_frame() override;


//      // void assert_ok() const override;
//      // void dump(dump_context & dumpgpucontext) const override;


      ::pointer < ::experience::frame > frame_experience() override;


      //bool has_pending_graphical_update() override;


      void install_message_routing(::channel * pchannel) override;


      DECLARE_MESSAGE_HANDLER(on_message_create);


   };


} // namespace app_graphics3d_continuum



