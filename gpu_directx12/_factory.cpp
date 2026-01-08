#include "framework.h"
#include "approach.h"
#include "binding.h"
#include "block.h"
#include "command_buffer.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "frame_storage.h"
#include "input_layout.h"
#include "memory_buffer.h"
#include "model_buffer.h"
#include "object.h"
#include "offscreen_render_target_view.h"
#include "renderer.h"
#include "frame.h"
#include "swap_chain.h"
#include "texture.h"
#include "gltf/mesh.h"
#include "gltf/model.h"
#include "bred/gpu/frame_ephemeral.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/pixmap.h"
//#include "direct2d_draw2d_swap_chain.h"



__FACTORY_EXPORT void gpu_directx12_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::gpu_directx12::approach, ::gpu::approach >();
   pfactory->add_factory_item < ::gpu_directx12::context, ::gpu::context >();
   pfactory->add_factory_item < ::gpu_directx12::frame, ::gpu::frame >();
   pfactory->add_factory_item < ::gpu_directx12::shader, ::gpu::shader >();
   pfactory->add_factory_item < ::gpu_directx12::cpu_buffer, ::gpu::cpu_buffer >();
   pfactory->add_factory_item < ::gpu_directx12::renderer, ::gpu::renderer >();

   //pfactory->add_factory_item < ::gpu::layer >();

   pfactory->add_factory_item < ::gpu_directx12::texture, ::gpu::texture >();

   pfactory->add_factory_item < ::gpu_directx12::object, ::gpu::object >();

   pfactory->add_factory_item < ::gpu_directx12::device, ::gpu::device >();

   pfactory->add_factory_item < ::gpu_directx12::offscreen_render_target_view, ::gpu::render_target >();
   pfactory->add_factory_item < ::gpu_directx12::swap_chain, ::gpu::swap_chain >();

   pfactory->add_factory_item < ::gpu_directx12::input_layout, ::gpu::input_layout >();
   pfactory->add_factory_item < ::gpu_directx12::command_buffer   , ::gpu::command_buffer >();
   pfactory->add_factory_item < ::gpu_directx12::memory_buffer, ::gpu::memory_buffer >();
   pfactory->add_factory_item < ::gpu_directx12::model_buffer, ::gpu::model_buffer >();
   pfactory->add_factory_item < ::gpu_directx12::command_buffer, ::gpu::command_buffer >();
   pfactory->add_factory_item < ::gpu::layer >();

   pfactory->add_factory_item < ::gpu::pixmap >();

   pfactory->add_factory_item<::gpu_directx12::gltf::mesh, ::gpu::model::mesh>();
   pfactory->add_factory_item<::gpu_directx12::gltf::model, ::gpu::model::model>();


   pfactory->add_factory_item < ::gpu_directx12::frame_storage, ::gpu::frame_storage >();

   pfactory->add_factory_item < ::gpu::frame_ephemeral >();
   pfactory->add_factory_item<::gpu_directx12::binding, ::gpu::binding>();
   pfactory->add_factory_item<::gpu_directx12::binding_set, ::gpu::binding_set>();
   pfactory->add_factory_item<::gpu_directx12::block, ::gpu::block>();


}



