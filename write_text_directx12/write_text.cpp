// Created by camilo 2021-02-10 <3TBS_!!
#include "platform.h"
#include "write_text.h"


namespace write_text_directx12
{

   
   write_text::write_text()
   {


   }


   write_text::~write_text()
   {


   }



   ::write_text::font_pointer write_text::font(const ::write_text::font_family_pointer& pfontfamily, const ::write_text::font_size& fontsize, int iFontWeight)
   {

      auto pwritetextfont = ::write_text::write_text::font(pfontfamily, fontsize, iFontWeight);

      //::cast < ::gpu_directx12::object > pdirectx12object = pwritetextfont;

      //if (pdirectx12object)
      //{

      //   pdirectx12object->initialize_directx12_object(m_pgpudevice);

      //}

      return pwritetextfont;

   }



} // namespace write_text_directx12



