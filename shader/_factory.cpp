#include "framework.h"
#include "application.h"


__FACTORY_EXPORT void app_graphics3d_shader_factory(::factory::factory* pfactory)
{

   pfactory->add_factory_item < ::app_graphics3d_shader::application, ::platform::application >();

}




