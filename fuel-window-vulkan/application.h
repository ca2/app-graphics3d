#pragma once


#include "app-cube/fuel/application.h"


namespace app_cube_fuel_window_vulkan
{


   class CLASS_DECL_APP_CUBE_FUEL_WINDOW_VULKAN application :
      virtual public ::app_cube_fuel::application
   {
   public:




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


} // namespace app_cube_fuel_window_vulkan



