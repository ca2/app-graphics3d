#include "framework.h"
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
#include "acme/platform/system.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_cube_fuel_memory_vulkan);
IMPLEMENT_APPLICATION_FACTORY(app_cube_fuel_memory_vulkan);


namespace app_cube_fuel_memory_vulkan
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(app_cube_fuel_memory_vulkan));


   application::application()
   {

      m_bUseDraw2dProtoWindow = false;
      //m_ptemplateFuelMain = nullptr;
      //m_ptemplateFuelImpact = nullptr;

      m_strAppId              = "app-cube/fuel-memory-vulkan";
      m_strAppName            = "app-cube/fuel-memory-vulkan";
      m_strBaseSupportId      = "app-cube/fuel-memory-vulkan";
      m_bLicense              = false;

   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      ::app_cube_fuel::application::init_instance();

   }


   void application::term_instance()
   {

      ::app_cube_fuel::application::term_instance();

   }


   void application::on_request(::request * prequest)
   {

      ::app_cube_fuel::application::on_request(prequest);

   }

#ifdef _DEBUG

   long long application::increment_reference_count()
   {

      return ::app_cube_fuel::application::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::app_cube_fuel::application::decrement_reference_count();

   }

#endif

   ::string application::draw2d_get_default_implementation_name()
   {

      return system()->implementation_name("draw2d", "vulkan");

   }


} // namespace app_cube_fuel_memory_vulkan







