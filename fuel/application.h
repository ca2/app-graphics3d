#pragma once


#include "core/platform/application.h"


namespace app_core_fuel
{


   class CLASS_DECL_APP_CORE_FUEL application :
      virtual public ::core::application
   {
   public:


      ::user::document *                     m_pdocMenu;
      //::user::single_document_template *     m_ptemplateFuelMain;
      //::user::single_document_template *     m_ptemplateFuelImpact;


      application();
      ~application() override;
      
      __DECLARE_APPLICATION_RELEASE_TIME();



      void init_instance() override;
      void term_instance() override;

      void on_request(::request * prequest) override;

#ifdef _DEBUG

      long long increment_reference_count() override;
      long long decrement_reference_count() override;

#endif

      
      string draw2d_get_default_implementation_name() override;


   };


} // namespace app_core_fuel



