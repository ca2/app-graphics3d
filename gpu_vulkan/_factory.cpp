#include "framework.h"
#include "approach.h"
#include "command_buffer.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "input_layout.h"
#include "layer.h"
#include "model_buffer.h"
#include "object.h"
#include "offscreen_render_pass.h"
#include "renderer.h"
#include "swap_chain.h"
#include "texture.h"


//BEGIN_FACTORY(gpu_vulkan)
//FACTORY_ITEM(::vulkan::vulkan)
//FACTORY_ITEM(::vulkan::program)
//FACTORY_ITEM(::vulkan::shader)
//FACTORY_ITEM(::vulkan::buffer)
//END_FACTORY()


__FACTORY_EXPORT void gpu_vulkan_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::gpu_vulkan::approach, ::gpu::approach >();
   pfactory->add_factory_item < ::gpu_vulkan::context, ::gpu::context >();
   //pfactory->add_factory_item < ::gpu_vulkan::program, ::gpu::program >();
   pfactory->add_factory_item < ::gpu_vulkan::shader, ::gpu::shader >();
   pfactory->add_factory_item < ::gpu_vulkan::cpu_buffer, ::gpu::cpu_buffer >();
   pfactory->add_factory_item < ::gpu_vulkan::renderer, ::gpu::renderer >();

   pfactory->add_factory_item < ::gpu_vulkan::object, ::gpu::object >();
   pfactory->add_factory_item < ::gpu_vulkan::texture, ::gpu::texture >();

   pfactory->add_factory_item < ::gpu_vulkan::device, ::gpu::device >();
   pfactory->add_factory_item < ::gpu_vulkan::offscreen_render_pass, ::gpu::render_target >();
   pfactory->add_factory_item < ::gpu_vulkan::swap_chain, ::gpu::swap_chain >();

   pfactory->add_factory_item < ::gpu_vulkan::input_layout, ::gpu::input_layout >();
   pfactory->add_factory_item < ::gpu_vulkan::layer, ::gpu::layer >();
   pfactory->add_factory_item < ::gpu_vulkan::command_buffer, ::gpu::command_buffer >();

   pfactory->add_factory_item < ::gpu_vulkan::memory_buffer, ::gpu::memory_buffer >();
   pfactory->add_factory_item < ::gpu_vulkan::model_buffer, ::gpu::model_buffer > ();

   

}




