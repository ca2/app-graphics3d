#pragma once


#include "gpu_opengl/_.h"
#include "_opengl.h"


#if defined(_graphics3d_opengl_project)
   #define CLASS_DECL_GRAPHICS3D_OPENGL  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_GRAPHICS3D_OPENGL  CLASS_DECL_IMPORT
#endif


namespace graphics3d_opengl
{


   class engine;

   //class application_handler;

   class renderer;
   //class application_object;


   class model;
   //class context;
   class input;
   //class camera;
   class pipeline;

   
   class descriptor_pool;

   //class shader;
   class texture;

   class mesh;

   class simple_render_system;
   class point_light_system;


} // namespace graphics3d_opengl


//#undef APP_TYPE
//#define APP_TYPE ::vulken::application


//#include "application.h"




