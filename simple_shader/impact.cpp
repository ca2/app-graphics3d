#include "framework.h"
#include "impact.h"
#include "frame.h"
#include "application.h"
#include "acme/constant/id.h"
#include "acme/constant/message.h"
#include "acme/constant/user_key.h"
#include "acme/handler/topic.h"
#include "acme/prototype/datetime/datetime.h"
#include "apex/database/_binary_stream.h"
#include "base/user/user/impact_system.h"
#include "base/user/user/split_impact.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/encoding_options.h"
#include "aura/graphics/image/context.h"
#include "aura/message/user.h"
#include "app/shader/render.h"

#include <math.h>


namespace app_simple_shader
{


   impact::impact()
   {

      m_iImpact = -1;

      m_flagNonClient.erase(e_non_client_background);

      m_flagNonClient.erase(e_non_client_focus_rect);

      m_bDefaultClickHandling = true;

   }


   impact::~impact()
   {

   }


   //void impact::assert_ok() const
   //{

   //   user::box::assert_ok();

   //}


   //void impact::dump(dump_context & dumpcontext) const
   //{

   //   user::box::dump(dumpcontext);

   //}


#ifdef _DEBUG


   long long impact::increment_reference_count()
   {

      return  ::user::impact::increment_reference_count();

   }


   long long impact::decrement_reference_count()
   {

      return  ::user::impact::decrement_reference_count();

   }


#endif


   void impact::install_message_routing(::channel * psender)
   {

      ::user::impact::install_message_routing(psender);

      MESSAGE_LINK(e_message_create,psender,this,&impact::on_message_create);
      MESSAGE_LINK(e_message_destroy, psender, this, &impact::on_message_destroy);
      MESSAGE_LINK(e_message_key_down, psender, this, &impact::on_message_key_down);

   }


   void impact::on_message_create(::message::message * pmessage)
   {

      ::pointer<::message::create> pcreate(pmessage);

      pcreate->previous();

      if (pcreate->m_bRet)
      {

         return;

      }

      payload(FONTSEL_IMPACT) = true;

      add_graphical_output_purpose(this,::graphics::e_output_purpose_screen_fps);

//      auto pprocedureRedraw = [this]()
//         {
//
//            set_need_redraw();
//
//            post_redraw();
//
//         };

      //auto papp = get_app();

      //papp->add_procedure("simple_checkbox", pprocedureRedraw);

      //papp->add_procedure("no_client_frame", pprocedureRedraw);

      //auto estatus = 
      
      __construct_new(m_prender);


      m_prender->m_puserinteraction = this;

      //if(!estatus)
      //{

      //   pcreate->set_fail();

      //   return;

      //}

      //m_prender->initialize_application_consumer();

      //m_prender->m_pinteraction = this;

      string strId = get_document()->m_pimpactsystem->id();

      string strText;

      if(get_typed_parent<::user::split_impact>() != nullptr)
      {

         if(get_typed_parent<::user::split_impact>()->get_child_by_id("top_edit_impact") != nullptr)
         {

            auto pinteraction = get_typed_parent<::user::split_impact>()->get_child_by_id("top_edit_impact");

            pinteraction->set_text(strText,::e_source_initialize);

         }

      }

      m_prender->update_shader();

      {

         //::atom atom = "simple_text";

         //auto pproperty = papp->fetch_property(atom);

         //::payload payload;

         m_papp->data_get(m_papp->m_textLoadFragmentPathPrefix);
         //{

         //   pproperty->target_constrained_assign(payload);

         //}

         //auto idRunnable = papp->translate_property_id(atom);
         m_papp->m_textLoadFragmentPathPrefix.property_changed() +=
            [this](auto & change)
         //papp->add_procedure(idRunnable, [this, atom]()
         {

            //auto pproperty = fetch_property(atom);

            m_prender->defer_load_fragment(change.m_payload.as_string());

         };

      }


   }


   void impact::on_message_destroy(::message::message * pmessage)
   {

   }


   void impact::on_message_key_down(::message::message* pmessage)
   {

      auto pkey = pmessage->m_union.m_pkey;

      if (pkey->m_ekey == ::user::e_key_s)
      {

         m_bSaveFrame = true;




      }


   }


   void impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      if (ptopic->id() == "simple_checkbox" || ptopic->id() == "no_client_frame")
      {

         set_need_redraw();

      }

      ::user::impact::handle(ptopic, phandlercontext);

   }


   ::user::document * impact::get_document()
   {

      return ::user::impact::get_document();

   }


   bool impact::keyboard_focus_is_focusable()
   {

      return true;

   }


   void impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      if (m_bSaveFrame)
      {

         m_bSaveFrame = false;

         auto rectangleX = this->rectangle();

         auto pimage = image()->create_image(rectangleX.size());

         ::draw2d::graphics_pointer pgraphics = pimage->get_graphics();

         m_prender->_001OnDraw(pgraphics);

         fork([this, pimage]()
            {

               ::image::encoding_options encodingoptions;

               encodingoptions.m_eformat = ::image::e_format_png;

               string strDate = datetime()->date_time_text_for_file();

               image()->save_image("image://app_simple_shader-" + strDate + ".png", pimage, encodingoptions);

            });


      }

      m_prender->_001OnDraw(pgraphics);

   }


   void impact::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      if(rectangleX.is_empty())
      {

         return;

      }

      m_prender->m_rectangle = rectangleX;

      m_prender->on_layout(pgraphics);

   }


} // namespace simple_shader



