#include "platform.h"
#include "application.h"
#include "acme/handler/request.h"
#include "acme/platform/system.h"
#include "berg/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_graphics3d_fuel_window_directx11);
IMPLEMENT_APPLICATION_FACTORY(app_graphics3d_fuel_window_directx11);


namespace app_graphics3d_fuel_window_directx11
{


   application::application()
   {

      m_strGraphics3DImplementationOverride = "directx11";
      m_strGraphics3DOutputOverride = "on_screen";

      m_strAppId              = "app-graphics3d/fuel-window-directx11";
      m_strAppName            = "app-graphics3d/fuel-window-directx11";
      m_strBaseSupportId      = "app-graphics3d/fuel-window-directx11";
      m_bLicense              = false;

   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      ::app_graphics3d_fuel::application::init_instance();

   }


   void application::term_instance()
   {

      ::app_graphics3d_fuel::application::term_instance();

   }


   void application::on_request(::request * prequest)
   {

      ::app_graphics3d_fuel::application::on_request(prequest);

   }


#ifdef _DEBUG


   long long application::increment_reference_count()
   {

      return ::app_graphics3d_fuel::application::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::app_graphics3d_fuel::application::decrement_reference_count();

   }


#endif


   ::string application::draw2d_get_default_implementation_name()
   {

      return ::app_graphics3d_fuel::application::draw2d_get_default_implementation_name();

   }


} // namespace app_graphics3d_fuel_window_directx11



