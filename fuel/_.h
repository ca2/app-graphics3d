#pragma once


#include "bred/_.h"


#if defined(_app_graphics3d_fuel_project)
   #define CLASS_DECL_APP_GRAPHICS3D_FUEL  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_APP_GRAPHICS3D_FUEL  CLASS_DECL_IMPORT
#endif


namespace app_graphics3d_fuel
{


   class application;
   class main_frame;
   class document;
   class impact;
   class pane_impact;


} // namespace flag


//#undef APP_TYPE
//#define APP_TYPE ::fuel::application


//#include "application.h"




