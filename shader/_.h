#pragma once


#include "app-cube/cube/_.h"


#if defined(_app_cube_shader_project)
#define CLASS_DECL_APP_CUBE_SHADER  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_CUBE_SHADER  CLASS_DECL_IMPORT
#endif


namespace app_cube_shader
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
#define APPLICATION_CLASS ::app_cube_shader::application


