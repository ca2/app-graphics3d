// Created by camilo on 2026-09-05 03:23 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "impact.h"
#include "statistics_impact.h"
#include "document.h"
#include "application.h"
#include "immersion.h"
#include "main_scene.h"
#include "acme/constant/message.h"
#include "acme/filesystem/filesystem/file_system.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/node.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "aura/message/user.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "apex/database/stream.h"
#include "acme/user/interface/message_box.h"


CLASS_DECL_AURA::color::color dk_red(); // <3 tbs
//namespace vkc
//{
//   ::pointer<::vulkan::application > start_vulkan_application(::vkc::VkContainer* pvkcontainer, mouse_state* pmousestate);
//} // namespace vkc

namespace app_graphics3d_continuum
{


   statistics_impact::statistics_impact()
   {

      m_enonclient -= ::user::e_non_client_background;
      m_iSequence = 0;
      m_bNeedFullRedrawOnResize = true;

      m_bDefaultClickHandling = true;

      m_bEmptyAreaIsClientArea = true;

   }


   statistics_impact::~statistics_impact()
   {

   }


#ifdef _DEBUG


   long long statistics_impact::increment_reference_count()
   {

      return ::user::box::increment_reference_count();

   }


   long long statistics_impact::decrement_reference_count()
   {

      return ::user::box::decrement_reference_count();

   }


#endif



   void statistics_impact::install_message_routing(::channel * psender)
   {

      ::user::show < ::user::box >::install_message_routing(psender);

      USER_MESSAGE_LINK(::user::e_message_create, psender, this, &statistics_impact::on_message_create);
      USER_MESSAGE_LINK(::user::e_message_destroy, psender, this, &statistics_impact::on_message_destroy);

   }




   void statistics_impact::on_message_create(::message::message * pmessage)
   {

      ::pointer<::message::create>pcreate(pmessage);

      pcreate->previous();

      if (pcreate->m_bRet)
      {

         return;

      }

      m_papp->m_pstatisticsimpact = this;

   }


   void statistics_impact::on_message_destroy(::message::message * pmessage)
   {

   }




   void statistics_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      ::user::show < ::user::box >::handle(ptopic, phandlercontext);

   }


   document * statistics_impact::get_document()
   {

      return dynamic_cast <document *>(::user::show <::user::box>::get_document());

   }


   void statistics_impact::_001OnClip(::draw2d::graphics_pointer & pdraw2dgraphics)
   {


   }


   void statistics_impact::_001OnDraw(::draw2d::graphics_pointer & pdraw2dgraphics)
   {

      auto rectangleX = this->rectangle();

      if (rectangleX.is_empty())
      {

         return;

      }

      ::i32 y = 0;

      {

         //::color::color color_dk(dk_red());

#ifdef DEBUG_WORK

         ::i32_rectangle rectangleDryProWithLove_Work(5, 5, 1915, 1075);

         pdraw2dgraphics->fill_rectangle(rectangleDryProWithLove_Work, argb(255, 150, 200, 255));

#endif

         pdraw2dgraphics->set_solid_color(::color::white);

         auto psystem = system();

         auto pnode = psystem->draw2d();

         auto pwritetext = pnode->write_text();

         auto fontsize = ::write_text::font_size(48.0 * rectangleX.height() / 1'080, e_unit_pixel);

         if (!m_pfontThomasBS_)
         {

            m_pfontThomasBS_ = pwritetext->font("Fira Code", fontsize);

         }

         pdraw2dgraphics->set(m_pfontThomasBS_);

         pdraw2dgraphics->set_text_rendering_hint(write_text::e_rendering_anti_alias);

         pdraw2dgraphics->set_alpha_mode(draw2d::e_alpha_mode_blend);

         string_array stra ;

         //bool bWhite = true;

         //::f64 x = 0.;

         ::i32_point point;

         string strText;

         auto pgpuwindowattachment = ::gpu::window_attachment::get(this);

         ::collection::index iFrameSerial = -1;

         iFrameSerial = pgpuwindowattachment->m_iFrameSerial2;

         strText.formatf("øçåJErDgTBS__!!; %lld", (::i64)iFrameSerial);

         stra.atø(0) = strText;

         auto size = pdraw2dgraphics->get_text_extent(strText);

         ::string strGraphicsModeCompletion;

         strGraphicsModeCompletion.format("Graphics Mode Completion: {}/{} {:.3f}%",
            m_papplication->gpu_approach()->graphics3d_modes_step(),
            m_papplication->gpu_approach()->graphics3d_modes_step_count(),
            ((::f64)m_papplication->gpu_approach()->graphics3d_modes_step() /
               (::f64)m_papplication->gpu_approach()->graphics3d_modes_step_count()) * 100.0
         );

         stra.atø(3) = strGraphicsModeCompletion;

         //bool bFixedPosition = true;

         point = { 10, 10 };

         //::color::color color;

         //auto opacity = ::opacity(200);

         for (auto & strItem : stra)
         {

            y += size.cy;

            pdraw2dgraphics->text_out(point.x, point.y + y, strItem);

         }

         pdraw2dgraphics->set_smooth_mode(::draw2d::e_smooth_mode_none);

      }

      ::i32 iHeight = 120;

      ::i32 y = 0;

      if (m_papp->m_pimpact)
      {

         m_papp->m_pimpact->draw_gpu_statistics(y, pdraw2dgraphics);

      }

      y = +iHeight;

      if (m_papp->m_pimpactSwitcher)
      {

         m_papp->m_pimpactSwitcher->draw_gpu_statistics(y, pdraw2dgraphics);

      }

      y = +iHeight;

      if (m_papp->m_pimpactSkybox)
      {

         m_papp->m_pimpactSkybox->draw_gpu_statistics(y, pdraw2dgraphics);

      }

   }


   void statistics_impact::on_layout(::draw2d::graphics_pointer & pdraw2dgraphics)
   {

      auto rectangleX = this->rectangle();

      if (rectangleX.is_empty())
      {

         return;

      }

      ::user::show< ::user::box >::on_layout(pdraw2dgraphics);

      setup_default_client_area_user_item();

   }


   bool statistics_impact::on_click(::item * pitem, ::user::mouse * pmouse)
   {

      if (::is_set(pitem))
      {

         if (pitem->m_item.m_eelement == ::e_element_client)
         {

            print_line("on_click : e_element_client");

            //::file::file_dialog_filter filterdialogfilter;

            //filterdialogfilter.add_item({ "application.txt", "application.txt" });

            //pick_single_file_to_open(filterdialogfilter,
            //   [this](::file::file_dialog * pdialog)
            //                 {

            //                    auto path = pdialog->get_file_path();

            //                    try {
            //                       //auto memory = file()->as_memory(path);

            //                       //auto size = memory.size();

            //                       //informationf("got file with %d bytes", size);

            //                       //file()->put_memory(m_papp->m_pathApplicationText, memory);


            //                    }
            //                    catch (...) {
            //                       auto pmessagebox = message_box("No file loaded...");
            //                       post(pmessagebox);
            //                    }

            //});

            return true;

         }

      }

      return ::user::show < ::user::box >::on_click(pitem, pmouse);

   }




} // namespace app_graphics3d_continuum



