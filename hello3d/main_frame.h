#pragma once


#include "base/user/simple/main_frame.h"


namespace graphics3d_hello3d
{


   class CLASS_DECL_APP_COMPLEX_HELLO3D main_frame :
      virtual public simple_main_frame
   {
   public:


      bool                       m_bExplicitTranslucency;


      main_frame();
      ~main_frame() override;


//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;


      ::pointer < ::experience::frame > frame_experience() override;

      //bool has_pending_graphical_update() override;

   };


} // namespace graphics3d_hello3d












