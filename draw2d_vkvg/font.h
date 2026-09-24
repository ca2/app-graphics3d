#pragma once


#include "acme/_operating_system.h"


#include "aura/graphics/write_text/font.h"


namespace draw2d_vkvg
{


   class CLASS_DECL_DRAW2D_VKVG font : 
      virtual public ::write_text::font
   {
   public:

#if defined(WINDOWS_DESKTOP)

      HDC                           m_hdcFont;
      HFONT                         m_hfont;

#endif

      //      VKuint	                     m_baseFont;				// Base Display List For The Font Set
//
//#ifdef WINDOWS
//      VKYPHMETRICSFLOAT             m_gmf[256];	// Storage For Information About Our Outline Font Characters
//#endif



      font();
      ~font() override;


      void construct(const ::write_text::font & pfontParam);

      
      // void dump(dump_context & dumpcontext) const override;


      void update(::draw2d::graphics * pdraw2dgraphics) override;
      void destroy() override;


   };


} // namespace draw2d_vkvg



