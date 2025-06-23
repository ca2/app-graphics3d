// Changed by camilo on 2021-12-10 19:36 PM <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/_.h"


#if defined(_gpu_directx12_project)
#define CLASS_DECL_GPU_DIRECTX12  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_GPU_DIRECTX12  CLASS_DECL_IMPORT
#endif


#include "_gpu_directx12.h"


//#define HELLO_TRIANGLE_DEBUG

namespace gpu_directx12
{

   
   class program;
   class approach;


   class swap_chain;

   class benchmark;

   class physical_device;

   class context;

   class device;

   class pipeline;

   class renderer;

   class descriptor_pool;

   class buffer;

   class set_descriptor_layout;

   class frame;

   class render_target_view;

   class offscreen_render_target_view;

   class swap_chain_render_target_view;

   class texture;

   class depth_stencil;

   class shader;

   class command_buffer;

   class direct2d_draw2d_swap_chain;


} // namespace gpu_directx12




 