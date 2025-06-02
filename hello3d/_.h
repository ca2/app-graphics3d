#pragma once


#include "core/_.h"
#include "cube/gpu/_.h"
//#include "gpu_opengl/_.h"


#if defined(_graphics3d_hello3d_project)
#define CLASS_DECL_APP_COMPLEX_HELLO3D  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_APP_COMPLEX_HELLO3D  CLASS_DECL_IMPORT
#endif


namespace gpu
{

   
   class object;


} // namespace gpu


namespace graphics3d_hello3d
{

   class application;
   class pane_impact;
   class document;
   class impact;
   class render;


} // namespace graphics3d_hello3d





namespace opengl
{


   class opengl;
   class shader;
   class buffer;
   class program;
   


} // namespace opengl




