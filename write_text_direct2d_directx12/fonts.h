// Created by camilo 2021-02-10 <3TBS_!!
#pragma once


#include "aura/graphics/write_text/fonts.h"


namespace write_text_direct2d_directx12
{


   class CLASS_DECL_WRITE_TEXT_DIRECT2D_DIRECTX12 fonts :
      virtual public ::write_text::fonts
   {
   public:


      fonts();
      ~fonts() override;


      virtual void initialize(::particle * pparticle);


   };


} // namespace write_text_direct2d_directx12



