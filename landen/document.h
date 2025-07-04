#pragma once


#include "base/user/user/document.h"


namespace app_graphics3d_landen
{


   class CLASS_DECL_APP_GRAPHICS3D_LANDEN document :
      virtual public ::user::document
   {
   public:


      document();
	   ~document() override;

      
	   // void assert_ok() const override;
	   // void dump(dump_context & dumpcontext) const override;

      bool on_new_document() override;
      bool on_open_document(const ::payload & payloadFile) override;

      long long increment_reference_count() override;

      long long decrement_reference_count() override;


   };


} // namespace app_core_vulken















