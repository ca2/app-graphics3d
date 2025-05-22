#include "framework.h"
#include "application.h"


__FACTORY_EXPORT void app_simple_shader_factory(::factory::factory* pfactory)
{

   pfactory->add_factory_item < ::app_simple_shader::application, ::platform::application >();

}




