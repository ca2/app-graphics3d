#include "platform.h"
#include "application.h"
//#include "child_frame.h"
//#include "document.h"
//#include "edit_impact.h"
//#include "frame.h"
//#include "impact.h"
//#include "main_impact.h"
//#include "pane_impact.h"
//#include "toggle_impact.h"
//#include "top_impact.h"
//#include "window.h"
#include "acme/handler/request.h"
#include "acme/constant/impact.h"
#include "acme/platform/system.h"
#include "berg/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_graphics3d_fuel);
IMPLEMENT_APPLICATION_FACTORY(app_graphics3d_fuel);


namespace app_graphics3d_fuel
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(app_graphics3d_fuel_memory_opengl));


   application::application()
   {

      //m_bUseSwapChainWindow = false;
      //m_ptemplateFuelMain = nullptr;
      //m_ptemplateFuelImpact = nullptr;

      m_strAppId              = "app-graphics3d/fuel";
      m_strAppName            = "Fuel";
      m_strBaseSupportId      = "app-graphics3d/fuel";
      m_bLicense              = false;
      m_bGpu = true;

   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      ::app_graphics3d_continuum::application::init_instance();

   }


   void application::term_instance()
   {

      ::app_graphics3d_continuum::application::term_instance();

   }


   void application::on_request(::request * prequest)
   {

      ::app_graphics3d_continuum::application::on_request(prequest);

   }

#ifdef _DEBUG

   long long application::increment_reference_count()
   {

      return ::bred::application::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::bred::application::decrement_reference_count();

   }

#endif

   ::string application::draw2d_get_default_implementation_name()
   {

      return ::bred::application::draw2d_get_default_implementation_name();

   }


   void application::create_options_impact(const ::atom & atom, ::user::interaction * pparent)
   {

      if (atom == APP_OPTIONS_IMPACT)
         ::bred::application::create_options_impact("options_impact_handler://gpu", pparent);
      else
         ::bred::application::create_options_impact(atom, pparent);

   }


   void application::on_after_create_form(const ::atom & atom, ::user::interaction * pparent)
   {

      ::bred::application::on_after_create_form(
         atom == APP_OPTIONS_IMPACT ? ::atom("options_impact_handler://gpu") : atom, pparent);

   }


} // namespace app_graphics3d_fuel







