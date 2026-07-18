#include "framework.h"
#include "skybox_render_system.h"
#include "bred/gltf/vertex.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/device.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/shader.h"
#include "bred/gpu/texture.h"
///#include "bred/graphics3d/model.h"
#include "bred/graphics3d/game_object.h"
#include "bred/graphics3d/engine.h"
//#include "bred/graphics3d/frame.h"
#include "bred/graphics3d/scene_base.h"
#include "bred/graphics3d/shape_factory.h"
#include "bred/graphics3d/skybox.h"
#include "bred/graphics3d/skybox.h"
#include "gpu/ibl/hlsl/skybox.vert.h"
#include "gpu/ibl/hlsl/skybox.frag.h"
#include "gpu/ibl/hlsl/skybox_hdr.frag.h"


namespace graphics3d_directx12
{


	skybox_render_system::skybox_render_system()
	{


	}


	skybox_render_system::~skybox_render_system()
	{

   
   }


   ::memory skybox_render_system::vertex_shader_memory()
   {

      return {g_psz_skybox_vert};

   }


   ::memory skybox_render_system::fragment_shader_memory()
   {

      return {g_psz_skybox_frag};
   }


   ::memory skybox_render_system::hdr_fragment_shader_memory()
   {

      return {g_psz_skybox_hdr_frag};

   }


   void skybox_render_system::on_prepare(gpu::context *pgpucontext)
   {

      ::graphics3d::skybox_render_system::on_prepare(pgpucontext);

   }



   void skybox_render_system::on_render(::gpu::context* pgpucontext, ::graphics3d::scene_base* pscene)
	{


      ::graphics3d::skybox_render_system::on_render(pgpucontext, pscene);
		
	}



} // namespace graphics3d_directx12



