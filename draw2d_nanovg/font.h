#pragma once


#include "acme/_operating_system.h"


#include "aura/graphics/write_text/font.h"


namespace draw2d_nanovg
{


   class CLASS_DECL_DRAW2D_NANOVG font : 
      virtual public ::write_text::font
   {
   public:


      font();
      ~font() override;


      void construct(const ::write_text::font & pfontParam);

      
      // void dump(dump_context & dumpcontext) const override;


      void update(::draw2d::graphics * pdraw2dgraphics) override;
      void destroy() override;


   };


} // namespace draw2d_nanovg



