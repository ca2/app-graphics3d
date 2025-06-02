#pragma once


#include "app-graphics3d/gpu_directx/_.h"
#include "_directx.h"


#if defined(_graphics3d_directx_project)
   #define CLASS_DECL_GRAPHICS3D_DIRECTX  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_GRAPHICS3D_DIRECTX  CLASS_DECL_IMPORT
#endif


namespace graphics3d_directx
{


   class application_handler;

   //class renderer;
   class application_object;


   class model;
   //class context;
   class input;
   //class camera;
   class pipeline;

   class engine;

   //class descriptor_pool;

} // namespace graphics3d_directx


//#undef APP_TYPE
//#define APP_TYPE ::vulken::application


//#include "application.h"




