#include "framework.h"
#include "approach.h"
#include "command_buffer.h"
#include "frame.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "input_layout.h"
#include "layer.h"
#include "model_buffer.h"
#include "object.h"
#include "pipeline.h"
#include "render_target.h"
#include "renderer.h"
#include "swap_chain.h"
#include "texture.h"
#include "bred/gpu/pixmap.h"
#include "bred/gpu/frame_ephemeral.h"
#include "bred/gpu/frame_storage.h"
//#include "app-graphics3d/gpu_vulkan/ibl/cubemap_framebuffer.h"
#include "app-graphics3d/gpu_vulkan/ibl/diffuse_irradiance_map.h"
#include "app-graphics3d/gpu_vulkan/ibl/equirectangular_cubemap.h"
//#include "app-graphics3d/gpu_vulkan/ibl/hdri_cube.h"
#include "app-graphics3d/gpu_vulkan/ibl/specular_map.h"
#include "gpu/full_screen_quad.h"
//#include "app-graphics3d/gpu_vulkan/cube.h"



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
   pfactory->add_factory_item < ::gpu_vulkan::render_target, ::gpu::render_target >();
   pfactory->add_factory_item < ::gpu_vulkan::swap_chain, ::gpu::swap_chain >();

   pfactory->add_factory_item < ::gpu_vulkan::input_layout, ::gpu::input_layout >();
   pfactory->add_factory_item < ::gpu_vulkan::layer, ::gpu::layer >();
   pfactory->add_factory_item < ::gpu_vulkan::command_buffer, ::gpu::command_buffer >();

   pfactory->add_factory_item < ::gpu_vulkan::memory_buffer, ::gpu::memory_buffer >();
   pfactory->add_factory_item < ::gpu_vulkan::model_buffer, ::gpu::model_buffer > ();

   pfactory->add_factory_item < ::gpu_vulkan::frame, ::gpu::frame >();
   pfactory->add_factory_item < ::gpu_vulkan::pipeline>();
   pfactory->add_factory_item < ::gpu::pixmap >();

   pfactory->add_factory_item < ::gpu::frame_ephemeral >();
   pfactory->add_factory_item < ::gpu::frame_storage >();
   pfactory->add_factory_item<::gpu_vulkan::render_pass>();

   pfactory->add_factory_item<::gpu_vulkan::ibl::diffuse_irradiance_map, ::gpu::ibl::diffuse_irradiance_map>();
   pfactory->add_factory_item<::gpu_vulkan::ibl::specular_map, ::gpu::ibl::specular_map>();
   //pfactory->add_factory_item<::gpu_vulkan::ibl::cubemap_framebuffer, ::gpu::ibl::cubemap_framebuffer>();
   pfactory->add_factory_item<::gpu_vulkan::ibl::equirectangular_cubemap, ::gpu::ibl::equirectangular_cubemap>();
   //spfactory->add_factory_item<::gpu::ibl::mipmap_cubemap_framebuffer>();
   //pfactory->add_factory_item<::gpu::ibl::brdf_convolution_framebuffer>();
   pfactory->add_factory_item<::gpu::full_screen_quad>();
   //pfactory->add_factory_item<::gpu_vulkan::ibl::hdri_cube, ::gpu::ibl::hdri_cube>();
   //pfactory->add_factory_item<::gpu_vulkan::cube, ::gpu::cube>();
   

}




