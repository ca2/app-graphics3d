#include "framework.h"
#include "approach.h"
#include "acme/exception/interface_only.h"
#include "acme/filesystem/filesystem/file_context.h"
//#include "_.h"
//#include "_gpu.h"


namespace gpu
{


   approach::approach()
   {

   }


   approach::~approach()
   {

   }


   void approach::initialize(::particle * pparticle)
   {

      ::object::initialize(pparticle);

   }


   void approach::initialize_gpu_approach()
   {


   }
   
   
   ::file::path approach::shader_path(const ::file::path& pathShader)
   {

      return pathShader;

   }


   void approach::defer_shader_memory(::memory& memory, const ::file::path& pathShader)
   {

      if (memory.is_empty())
      {

         auto path = shader_path(pathShader);

         memory = file()->as_memory(path);

      }

   }


   ::pointer < ::gpu::context > approach::create_offscreen_context(::particle* pparticle, const ::int_rectangle& rectanglePlacement)
   {

      auto pgpucontext = _create_context(pparticle, ::gpu::e_output_cpu_buffer, nullptr, rectanglePlacement);

      return pgpucontext;

   }


   ::pointer < ::gpu::context > approach::create_window_context(::particle* pparticle, ::windowing::window* pwindow)
   {

      auto pgpucontext = _create_context(pparticle, ::gpu::e_output_swap_chain, pwindow, {});

      return pgpucontext;

   }


   ::pointer < ::gpu::context > approach::_create_context(::particle * pparticle, ::gpu::enum_output eoutput, ::windowing::window * pwindow, const ::int_rectangle & rectanglePlacement)
   {

      throw ::interface_only();

      return nullptr;

   }


   void approach::defer_init_gpu_library()
   {

      //return ::success;

   }


   ::gpu::payload approach::load_dds(const ::scoped_string & scopedstrImagePath)
   {

      throw interface_only();

      return {};

   }

   
   void approach::create_global_ubo(::gpu::context* pgpucontext, int iSize, int iFrameCount)
   {


   }


   void approach::update_global_ubo(::gpu::context* pgpucontext, int iFrameIndex, const ::block& block)
   {


   }


} // namespace gpu



