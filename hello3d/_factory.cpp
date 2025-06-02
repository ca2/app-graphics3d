#include "framework.h"
#include "application.h"


__FACTORY_EXPORT void graphics3d_hello3d_factory(::factory::factory* pfactory)
{

   pfactory->add_factory_item < ::graphics3d_hello3d::application, ::platform::application >();

}




