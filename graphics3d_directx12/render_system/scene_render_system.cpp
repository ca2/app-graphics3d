#include "framework.h"
#include "scene_render_system.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/scene_renderable.h"
//#include "gpu_directx11/command_buffer.h"
#include "gpu_directx12/context.h"
#include "gpu_directx12/descriptors.h"
#include "gpu_directx12/gltf/model.h"
//#include "gpu_directx11/pipeline.h"
//#include "gpu_directx11/render_pass.h"
#include "gpu_directx12/renderer.h"
#include "bred/gpu/frame.h"
//#include "gpu_directx11/vk_init.h"
#include "gpu/ibl/_hlsl.h"


namespace graphics3d_directx12
{

   scene_render_system::scene_render_system()
   {


   }


   scene_render_system::~scene_render_system() 
   {


   }


   
   ::memory scene_render_system::pbr_with_ibl_vert_memory() 
   {
      return hlsl_embedded_scene_vert(); 
   }


   ::memory scene_render_system::pbr_with_ibl_frag_memory()
   {
      
      return hlsl_embedded_scene_frag(); 
   
   }


   void scene_render_system::on_prepare(::gpu::context *pgpucontext)
   {
   
      ::graphics3d::scene_render_system::on_prepare(pgpucontext);

   }


   void scene_render_system::on_render(::gpu::context *pgpucontext, ::graphics3d::scene_base *pscenebase)
   {

      ::graphics3d::scene_render_system::on_render(pgpucontext, pscenebase);

   }


} // namespace graphics3d_directx12
