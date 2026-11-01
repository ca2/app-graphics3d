#pragma once
#include "application.h"
#include "app-graphics3d/continuum/pane_impact.h"
namespace app_graphics3d_fuel
{
   class CLASS_DECL_APP_GRAPHICS3D_FUEL pane_impact :
      virtual public ::app_graphics3d_continuum::pane_impact
   {
   public:
      pane_impact();
      ~pane_impact() override;
   };
}
