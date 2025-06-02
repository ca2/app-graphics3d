#pragma once


#include "apex/platform/app_consumer.h"
#include "base/user/user/document.h"
#include "application.h"


namespace graphics3d_impact3d
{


   class CLASS_DECL_APP_COMPLEX_IMPACT3D document :
      virtual public ::app_consumer < application, ::user::document >
   {
   public:


      ::pointer<render>       m_prender;


      document();
	   ~document() override;


	   // void assert_ok() const override;
	   // void dump(dump_context & dumpcontext) const override;

      bool on_new_document() override;
      virtual bool on_open_document(const ::payload & payloadFile) override;

#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif

      ///virtual ::stream& write(::stream& stream) const override;


   };


} // namespace graphics3d_impact3d















