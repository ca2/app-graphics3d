#pragma once


#include "app/shader/_.h"
#include "core/_.h"


#if defined(_app_simple_shader_project)
#define CLASS_DECL_APP_SIMPLE_SHADER  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_SIMPLE_SHADER  CLASS_DECL_IMPORT
#endif


namespace app_simple_shader
{

   class application;
   class tab_impact;
   class document;
   class impact;

} // namespace flag


namespace opengl
{


   class opengl;
   class shader;
   class buffer;
   class program;


} // namespace opengl





