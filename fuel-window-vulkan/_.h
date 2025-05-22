#pragma once


#include "app-core/fuel/_.h"


#if defined(_app_core_fuel_window_vulkan_project)
   #define CLASS_DECL_APP_CORE_FUEL_WINDOW_VULKAN  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_APP_CORE_FUEL_WINDOW_VULKAN  CLASS_DECL_IMPORT
#endif


namespace app_core_fuel_window_vulkan
{


   class application;


} // namespace app_core_fuel_window_vulkan



