#include "framework.h"
#include "approach.h"
#include "command_buffer.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "input_layout.h"
#include "object.h"
#include "offscreen_render_target_view.h"
#include "renderer.h"
#include "frame.h"
#include "swap_chain.h"
#include "texture.h"
#include "bred/gpu/layer.h"
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
   pfactory->add_factory_item < ::gpu::layer >();
}



