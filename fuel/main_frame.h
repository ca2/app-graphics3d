#pragma once


#include "berg/user/simple/frame_window.h"


// Temporary diagnostic window; set to 0 to disable it.
#ifndef APP_GRAPHICS3D_CONTINUUM_DEBUG_CA2_MESSAGE_BOX
#define APP_GRAPHICS3D_CONTINUUM_DEBUG_CA2_MESSAGE_BOX 0
#endif

namespace user_interface
{

   class message_box;

}


namespace app_graphics3d_fuel
{


   class CLASS_DECL_APP_GRAPHICS3D_FUEL main_frame :
      virtual public ::simple_frame_window
   {
   public:


      bool                       m_bExplicitTranslucency;

#if APP_GRAPHICS3D_CONTINUUM_DEBUG_CA2_MESSAGE_BOX
      ::pointer < ::user_interface::message_box > m_pdebugmessagebox;
#endif


      main_frame();
      ~main_frame() override;


//      // void assert_ok() const override;
//      // void dump(dump_context & dumpgpucontext) const override;


      ::pointer < ::experience::frame > frame_experience() override;


      //bool has_pending_graphical_update() override;


      void install_message_routing(::channel * pchannel) override;


      DECLARE_MESSAGE_HANDLER(on_message_create);
#if APP_GRAPHICS3D_CONTINUUM_DEBUG_CA2_MESSAGE_BOX
      DECLARE_MESSAGE_HANDLER(on_message_show_window);
#endif


   };


} // namespace app_graphics3d_fuel



