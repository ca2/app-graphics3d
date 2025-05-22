#pragma once


#include "base/user/user/document.h"


namespace app_cube_fuel
{


   class CLASS_DECL_APP_CUBE_FUEL document :
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


} // namespace app_cube_fuel















