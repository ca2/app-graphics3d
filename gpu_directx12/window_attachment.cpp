// Created by camilo on 2026-08-04 01:42 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#include "platform.h"
#include "window_attachment.h"



namespace gpu_directx12
{


   window_attachment::window_attachment()
   {

   }


   window_attachment::~window_attachment()
   {


   }

   //   void device::on_top_end_frame()
   void window_attachment::on_end_frame()
   {

      ::gpu::window_attachment::on_end_frame();

      //auto procedureaOnTopFrameEnd = ::transfer(m_procedureaOnTopFrameEnd);

      //for (auto& procedure : procedureaOnTopFrameEnd)
      //{

      //   procedure();

      //}

   }

} // namespace gpu_directx12



