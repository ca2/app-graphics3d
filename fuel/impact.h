#pragma once
#include "application.h"
#include "app-graphics3d/continuum/impact.h"
namespace app_graphics3d_fuel
{
   class CLASS_DECL_APP_GRAPHICS3D_FUEL impact :
      virtual public ::app_graphics3d_continuum::impact
   {
   public:
      impact();
      ~impact() override;
   };
}
