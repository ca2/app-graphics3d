// Created by camilo on 2026-08-04 01:40 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#include "framework.h"
#include "draw2d_window_attachment.h"



namespace gpu_vulkan
{


   draw2d_window_attachment::draw2d_window_attachment()
   {

   }


   draw2d_window_attachment::~draw2d_window_attachment()
   {


   }

   void draw2d_window_attachment::on_end_frame()
   {

      ::gpu::draw2d_window_attachment::on_end_frame();

      //auto procedureaOnTopFrameEnd = ::transfer(m_procedureaOnTopFrameEnd);

      //for (auto& procedure : procedureaOnTopFrameEnd)
      //{

      //   procedure();

      //}

   }

} // namespace gpu_vulkan



