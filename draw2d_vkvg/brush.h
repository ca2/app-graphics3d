#pragma once


#include "aura/graphics/draw2d/brush.h"


namespace draw2d_vkvg
{


   class CLASS_DECL_DRAW2d_VKVG brush : 
      virtual public ::draw2d::brush
   {
   public:


      brush();
      virtual ~brush();


      // void dump(dump_context & dumpcontext) const override;


   };


} // namespace draw2d_vkvg


