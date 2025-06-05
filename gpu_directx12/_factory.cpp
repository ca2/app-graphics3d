#include "framework.h"
#include "approach.h"
#include "program.h"
#include "shader.h"
#include "cpu_buffer.h"
#include "object.h"
#include "renderer.h"
#include "frame.h"

//BEGIN_FACTORY(gpu_directx12)
//FACTORY_ITEM(::directx12::directx12)
//FACTORY_ITEM(::directx12::program)
//FACTORY_ITEM(::directx12::shader)
//FACTORY_ITEM(::directx12::buffer)
//END_FACTORY()


__FACTORY_EXPORT void gpu_directx12_factory(::factory::factory * pfactory)
{

   pfactory->add_factory_item < ::gpu_directx12::approach, ::gpu::approach >();
   pfactory->add_factory_item < ::gpu_directx12::context, ::gpu::context >();
   pfactory->add_factory_item < ::gpu_directx12::frame, ::gpu::frame >();
   pfactory->add_factory_item < ::gpu_directx12::shader, ::gpu::shader >();
   pfactory->add_factory_item < ::gpu_directx12::cpu_buffer, ::gpu::cpu_buffer >();
   pfactory->add_factory_item < ::gpu_directx12::renderer, ::gpu::renderer >();

   pfactory->add_factory_item < ::gpu_directx12::object, ::gpu::object >();

   pfactory->add_factory_item < ::gpu_directx12::device, ::gpu::device >();

}



