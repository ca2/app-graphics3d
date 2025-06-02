#pragma once


#include "base/user/simple/frame_window.h"


namespace graphics3d_hello3d
{


   class CLASS_DECL_APP_COMPLEX_HELLO3D frame :
      virtual public simple_frame_window
   {
   public:


      frame();
      virtual ~frame();


	   // void assert_ok() const override;
	   // void dump(dump_context & dumpcontext) const override;


      //::user::enum_translucency get_translucency(::user::style * pstyle) override;


   };


} // namespace graphics3d_hello3d












