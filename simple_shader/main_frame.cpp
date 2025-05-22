#include "framework.h"
#include "main_frame.h"


namespace app_simple_shader
{


   main_frame::main_frame()
   {

      //m_bDefaultCreateToolbar = false;

      window_enable_full_screen();

   }


   main_frame::~main_frame()
   {

   }


   //void main_frame::assert_ok() const
   //{

   //   simple_frame_window::assert_ok();

   //}


   //void main_frame::dump(dump_context & dumpcontext) const
   //{

   //   simple_frame_window::dump(dumpcontext);

   //}


   ::pointer < ::experience::frame > main_frame::frame_experience()
   {

      auto pframe = ::simple_frame_window::frame_experience(nullptr, "022", "LightBlue");

      return pframe;

   }


   //bool main_frame::has_pending_graphical_update()
   //{

   //   return ::simple_frame_window::has_pending_graphical_update();

   //}


   void main_frame::install_message_routing(::channel * pchannel)
   {

      simple_frame_window::install_message_routing(pchannel);

      MESSAGE_LINK(e_message_create,pchannel,this,&main_frame::on_message_create);

   }


   void main_frame::on_message_create(::message::message * pmessage)
   {

      set_bitmap_source("Simple Shader!!");

   }


} // namespace simple_shader



