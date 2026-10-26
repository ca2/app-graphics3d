#pragma once


#include "acme/_operating_system.h"


#include "bred/gpu/font.h"


namespace draw2d_vulkan
{


   class CLASS_DECL_DRAW2D_VULKAN font : 
      virtual public ::gpu::font
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


} // namespace draw2d_vulkan



