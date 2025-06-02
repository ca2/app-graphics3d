#pragma once


#include "cube/_.h"


#if defined(_app_graphics3d_shader_project)
#define CLASS_DECL_APP_GRAPHICS3D_SHADER  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_GRAPHICS3D_SHADER  CLASS_DECL_IMPORT
#endif


namespace app_graphics3d_shader
{


   class application;
   class main_window;


   class render;


} // namespace flag


namespace opengl
{


   class opengl;
   class shader;
   class buffer;
   class program;


} // namespace opengl



#undef APPLICATION_CLASS
#define APPLICATION_CLASS ::app_graphics3d_shader::application


