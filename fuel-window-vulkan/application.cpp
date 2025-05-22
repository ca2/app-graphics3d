#include "framework.h"
#include "application.h"
#include "acme/handler/request.h"
#include "acme/platform/system.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_core_fuel_window_vulkan);
IMPLEMENT_APPLICATION_FACTORY(app_core_fuel_window_vulkan);


namespace app_core_fuel_window_vulkan
{


   application::application()
   {

      m_bUseDraw2dProtoWindow = true;

      m_strAppId              = "app-core/fuel-window-vulkan";
      m_strAppName            = "app-core/fuel-window-vulkan";
      m_strBaseSupportId      = "app-core/fuel-window-vulkan";
      m_bLicense              = false;

   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      ::app_core_fuel::application::init_instance();

   }


   void application::term_instance()
   {

      ::app_core_fuel::application::term_instance();

   }


   void application::on_request(::request * prequest)
   {

      ::app_core_fuel::application::on_request(prequest);

   }


#ifdef _DEBUG


   long long application::increment_reference_count()
   {

      return ::app_core_fuel::application::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::app_core_fuel::application::decrement_reference_count();

   }


#endif


   ::string application::draw2d_get_default_implementation_name()
   {

      return system()->implementation_name("draw2d", "vulkan");

   }


} // namespace app_core_fuel_window_vulkan



