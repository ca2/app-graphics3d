#pragma once


#include "cube/frame.h"


namespace graphics3d_MyPlace2025
{


   class CLASS_DECL_GRAPHICS3D_MYPLACE2025 main_frame :
      virtual public ::cube::frame
   {
   public:


      bool                       m_bExplicitTranslucency;


      main_frame();
      ~main_frame() override;


//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;


      ::pointer < ::experience::frame > frame_experience() override;


      //bool has_pending_graphical_update() override;


      void install_message_routing(::channel * pchannel) override;


      DECLARE_MESSAGE_HANDLER(on_message_create);


   };


} // namespace graphics3d_MyPlace2025



