#pragma once


#include "aura/graphics/draw2d/palette.h"
#include "object.h"


namespace draw2d_direct2d_directx12
{


   class CLASS_DECL_DRAW2D_DIRECT2D_DIRECTX12 palette :
      virtual public ::draw2d_direct2d_directx12::object,
      virtual public ::draw2d::palette
   {
   public:


      palette();
      ~palette() override;


   };


} // namespace draw2d_direct2d_directx12




