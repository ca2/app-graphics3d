#include "framework.h"
#include "approach.h"
#include "context.h"
#include "types.h"
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


   ::pointer < ::gpu::context > approach::allocate_context(::particle* pparticle)
   {

      auto pgpucontext = pparticle->__øcreate< ::gpu::context >();

      return pgpucontext;

   }


   ::pointer < ::gpu::context > approach::start_cpu_buffer_context(::particle* pparticle, const ::image32_callback & callbackImage32CpuBuffer, const ::int_rectangle& rectanglePlacement)
   {

      auto pgpucontext = start_gpu_context(
         start_cpu_buffer_context_t
         {
            pparticle, 
            this,
            callbackImage32CpuBuffer, 
            rectanglePlacement
         });

      return pgpucontext;

   }


   ::pointer < ::gpu::context > approach::start_swap_chain_context(::particle* pparticle, ::windowing::window* pwindow)
   {

      auto pgpucontext = start_gpu_context(
         start_swap_chain_context_t
         {
            pparticle, 
            this,
            pwindow
         });

      return pgpucontext;

   }


   ::pointer < ::gpu::context > approach::start_gpu_context(const start_context_t & startcontext)
   {

      auto pgpucontext = allocate_context(startcontext.m_pparticle);

      if (!pgpucontext)
      {

         throw ::exception(error_resource);

      }

      pgpucontext->start_gpu_context(startcontext);

      return pgpucontext;

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



