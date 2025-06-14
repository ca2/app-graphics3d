#include "framework.h"
#include "approach.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "object.h"
#include "renderer.h"
#include "frame.h"
#include "bred/gpu/layer.h"
#include "texture.h"
#include "direct2d_draw2d_swap_chain.h"


__FACTORY_EXPORT void gpu_directx12_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::gpu_directx12::approach, ::gpu::approach >();
   pfactory->add_factory_item < ::gpu_directx12::context, ::gpu::context >();
   pfactory->add_factory_item < ::gpu_directx12::frame, ::gpu::frame >();
   pfactory->add_factory_item < ::gpu_directx12::shader, ::gpu::shader >();
   pfactory->add_factory_item < ::gpu_directx12::cpu_buffer, ::gpu::cpu_buffer >();
   pfactory->add_factory_item < ::gpu_directx12::renderer, ::gpu::renderer >();

   pfactory->add_factory_item < ::gpu::layer >();

   pfactory->add_factory_item < ::gpu_directx12::texture, ::gpu::texture >();

   pfactory->add_factory_item < ::gpu_directx12::object, ::gpu::object >();

   pfactory->add_factory_item < ::gpu_directx12::device, ::gpu::device >();

   pfactory->add_factory_item < ::gpu_directx12::direct2d_draw2d_swap_chain, ::gpu::swap_chain >();

}



