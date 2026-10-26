#include "platform.h"
#include "main_frame.h"
#include "acme/constant/message.h"
#include "aura/message/user.h"
#if APP_GRAPHICS3D_CONTINUUM_DEBUG_CA2_MESSAGE_BOX
#include "acme/user/interface/message_box.h"
#include <atomic>
#endif


namespace app_graphics3d_continuum
{


   main_frame::main_frame()
   {

      window_enable_full_screen();

      m_etranslucencyFrame = ::user::e_translucency_present;

   }


   main_frame::~main_frame()
   {

   }


   ::pointer < ::experience::frame > main_frame::frame_experience()
   {

      auto pframe = ::simple_frame_window::frame_experience(nullptr, "022", "LightBlue");

      return pframe;

   }


//   bool main_frame::has_pending_graphical_update()
//   {
//
//      return ::simple_main_frame::has_pending_graphical_update();
//
//   }


   void main_frame::install_message_routing(::channel * pchannel)
   {

      ::simple_frame_window::install_message_routing(pchannel);
       
      USER_MESSAGE_LINK(::user::e_message_create, pchannel, this, &main_frame::on_message_create);
#if APP_GRAPHICS3D_CONTINUUM_DEBUG_CA2_MESSAGE_BOX
      USER_MESSAGE_LINK(::user::e_message_show_window, pchannel, this, &main_frame::on_message_show_window);
#endif

   }


   void main_frame::on_message_create(::message::message * pmessage)
   {

      //set_bitmap_source("Simple papp!!");

         set_timer(e_timer_update_main_frame_caption, 1_s,
                   [this](::timer * ptimer)
                   {
                      ::string strTitle;
                      strTitle = "continuum";

                      strTitle += " draw2d_" + system()->component_factory_implementation_name("draw2d");
                      strTitle += " gpu_" + system()->component_factory_implementation_name("gpu");

                      set_window_text(strTitle);
                   });

   }


#if APP_GRAPHICS3D_CONTINUUM_DEBUG_CA2_MESSAGE_BOX
   void main_frame::on_message_show_window(::message::message * pmessage)
   {

      ::simple_frame_window::on_message_show_window(pmessage);

      ::pointer<::message::show_window> pshowwindow(pmessage);

      if (!pshowwindow->m_bShow)
      {

         return;

      }

      static std::atomic_bool s_bDebugMessageBoxShown{false};

      if (s_bDebugMessageBoxShown.exchange(true))
      {

         return;

      }

      information("Main frame shown; queueing temporary CA2 message-box diagnostic after show dispatch");

      // Wait until the main window's X11 MapNotify/show handling has returned.
      // Creating and activating another top-level window inside that dispatch
      // can let the remaining main-window activation work cover the dialog.
      post([this]()
      {

         // particle::message_box currently expects a non-null icon array.
         ::string_array_base straIconUrls;
         m_pdebugmessagebox = message_box(
            "CA2 message-box diagnostic.\n"
            "This is a separate top-level window; it does not use the main window's swap chain.",
            "CA2 draw-path diagnostic",
            ::user::e_message_box_ok,
            nullptr,
            &straIconUrls);

         if (::is_set(m_pdebugmessagebox))
         {

            m_pdebugmessagebox->display(e_display_normal, {});

            information("CA2 message-box diagnostic display returned");

         }

      });

   }
#endif


} // namespace app_graphics3d_continuum



