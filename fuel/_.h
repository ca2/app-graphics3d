#pragma once


#include "core/_.h"



#if defined(_app_core_fuel_project)
   #define CLASS_DECL_APP_CORE_FUEL  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_APP_CORE_FUEL  CLASS_DECL_IMPORT
#endif


namespace app_core_fuel
{


   class application;
   //class top_impact;
   //class main_impact;
   class document;
   class impact;


} // namespace flag


//#undef APP_TYPE
//#define APP_TYPE ::fuel::application


//#include "application.h"




