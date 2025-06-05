#pragma once


#include "gpu_directx11/_.h"
#include "_directx11.h"


#if defined(_graphics3d_directx11_project)
   #define CLASS_DECL_GRAPHICS3D_DIRECTX11  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_GRAPHICS3D_DIRECTX11  CLASS_DECL_IMPORT
#endif


namespace graphics3d_directx11
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

} // namespace graphics3d_directx11


//#undef APP_TYPE
//#define APP_TYPE ::vulken::application


//#include "application.h"




