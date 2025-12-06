// From graphics3d_opengl/render_systems/skybox_render_system.cpp
// by camilo on 2025-12-04 16:16 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "skybox_render_system.h"
#include "bred/gltf/vertex.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/device.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/shader.h"
#include "bred/gpu/texture.h"
#include "bred/graphics3d/game_object.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/frame.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/shape_factory.h"
#include "bred/graphics3d/skybox.h"
#include "bred/graphics3d/skybox.h"
//#include "shader/skybox.vert.h"
//#include "shader/skybox.frag.h"
//#include "shader/skybox_hdr.frag.h"
//

namespace graphics3d_vulkan
{


	skybox_render_system::skybox_render_system()
	{


	}


	skybox_render_system::~skybox_render_system()
	{

   
   }


   ::memory skybox_render_system::vertex_shader_memory()
   {

    
            static unsigned int pvertexshader[] = {
#include "render_systems/shader/skybox.vert.spv.inl"
      };

      return ::as_memory_block(pvertexshader);


   }


   ::memory skybox_render_system::fragment_shader_memory()
   {

      
    
            static unsigned int pfragmentshader[] = {
#include "render_systems/shader/skybox.frag.spv.inl"
      };

      return ::as_memory_block(pfragmentshader);

   }


   ::memory skybox_render_system::hdr_fragment_shader_memory()
   {


      static unsigned int pfragmentshader[] = {
#include "render_systems/shader/skybox_hdr.frag.spv.inl"
      };

      return ::as_memory_block(pfragmentshader);


   }



   void skybox_render_system::on_prepare(gpu::context *pgpucontext)
   {

      ::graphics3d::skybox_render_system::on_prepare(pgpucontext);

   }



   void skybox_render_system::on_render(::gpu::context* pgpucontext, ::graphics3d::scene_base* pscene)
	{


      ::graphics3d::skybox_render_system::on_render(pgpucontext, pscene);
		
	}



} // namespace graphics3d_vulkan



