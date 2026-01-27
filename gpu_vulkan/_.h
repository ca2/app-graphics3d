// Changed by camilo on 2021-12-10 19:36 PM <3ThomasBorregaardSorensen!!
#pragma once


#include "app/gpu/_.h"


#if defined(WINDOWS_DESKTOP)
#include "windowing_win32/_.h"
#elif defined(WITH_X11)
#include "windowing_x11/_.h"
#endif
#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"


#if defined(_gpu_vulkan_project)
#define CLASS_DECL_GPU_VULKAN  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_GPU_VULKAN  CLASS_DECL_IMPORT
#endif


#include "_gpu_vulkan.h"


namespace gpu_vulkan
{

   
   class program;
   class approach;


   class swap_chain;

   class benchmark;

   class physical_device;

   class context;

   class device;

   class fence;

   class pipeline;

   class renderer;

   class descriptor_pool;

   class memory_buffer;

   class descriptor_set_layout;

   class frame;

   class shader;

   class texture;

   class buffer;

   class command_buffer;

   class layer;

   class memory_buffer;

   class model_buffer;

   class input_layout;

   class render_pass;

   class render_target;

   namespace gltf
   {

      class model;

      class mesh;

      class material;

      //class Model;

   }


   namespace ibl
   {

      
      class cubemap_framebuffer;


   } // namespace ibl


} // namespace gpu_vulkan




 