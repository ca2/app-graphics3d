#include "framework.h"
#include "impact.h"
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


CLASS_DECL_AURA ::color::color dk_red(); // <3 tbs
//namespace vkc
//{
//   ::pointer<::vulkan::application > start_vulkan_application(::vkc::VkContainer* pvkcontainer, mouse_state* pmousestate);
//} // namespace vkc

namespace app_graphics3d_hello_space
{




   impact::impact()
   {

      
      m_enonclient -= ::user::e_non_client_background;
      m_iSequence = 0;
      m_bNeedFullRedrawOnResize = true;
      
      m_bDefaultClickHandling = true;
      
      m_bEmptyAreaIsClientArea = true;

   }


   impact::~impact()
   {

   }


#ifdef _DEBUG


   long long impact::increment_reference_count()
   {

      return ::user::impact::increment_reference_count();

   }


   long long impact::decrement_reference_count()
   {

      return ::user::impact::decrement_reference_count();

   }


#endif



   void impact::install_message_routing(::channel * psender)
   {

      ::user::show < ::user::graphics3d >::install_message_routing(psender);

      USER_MESSAGE_LINK(::user::e_message_create,psender,this,&impact::on_message_create);
      USER_MESSAGE_LINK(::user::e_message_destroy, psender, this, &impact::on_message_destroy);

   }



   //::pointer < ::graphics3d::application > impact::start_graphics3d_application()
   //{

   //   return ::graphics3d_landen::impact::start_graphics3d_application();

   //}



   void impact::on_message_create(::message::message * pmessage)
   {

      ::pointer<::message::create>pcreate(pmessage);

      //display();

      pcreate->previous();

      if (pcreate->m_bRet)
      {

         return;

      }


      //application()->show_about_box();
    
      //m_propertiesGlobalUbo.set(global_ubo_properties());

   }


   void impact::on_message_destroy(::message::message * pmessage)
   {

   }




   void impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      ::user::impact::handle(ptopic, phandlercontext);

   }


   document * impact::get_document()
   {

      return dynamic_cast < document * >(::user::impact::get_document());

   }


   void impact::_001OnClip(::draw2d::graphics_pointer & pgraphics)
   {


   }


   void impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {
      
      auto rectangleX = this->rectangle();

      if (rectangleX.is_empty())
      {

         return;
         
      }
      
#if 1

      ::f64_rectangle rectangleClipBox;

      // pgraphics->reset_clip();

      // pgraphics->get_clip_box(rectangleClipBox);

      auto matrix = pgraphics->m_matrix;

      // auto origin = pgraphics->origin();

      pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_blend);
      ::get_task()->payload("debug") = 123;
      pgraphics->fill_rectangle(rectangleX, argb(108, 128, 128, 128));
      ::get_task()->payload("debug") = 0;

      ::user::graphics3d::_001OnDraw(pgraphics);

#endif
      
      draw_gpu_statistics(pgraphics);

   }


   void impact::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      if(rectangleX.is_empty())
      {

         return;

      }

      ::user::graphics3d::on_layout(pgraphics);
      
      setup_default_client_area_user_item();

   }


   bool impact::on_click(::item * pitem, ::user::mouse * pmouse)
   {

      if (::is_set(pitem))
      {

         if (pitem->m_item.m_eelement == ::e_element_client)
         {

            print_line("on_click : e_element_client");
            
            ::file::file_dialog_filter filterdialogfilter;
            
            filterdialogfilter.add_item({"application.txt", "application.txt"});
            
            pick_single_file_to_open(filterdialogfilter, 
               [ this ] (::file::file_dialog * pdialog)
                             {

                  auto path = pdialog->get_file_path();
               
               try {
                  auto memory = file()->as_memory(path);
                  
                  auto size = memory.size();
                  
                  informationf("got file with %d bytes", size);
                  
                  file()->put_memory(m_papp->m_pathApplicationText, memory);
                  

               } catch (...) {
                  auto pmessagebox = message_box("No file loaded...");
                  post(pmessagebox);
               }
               
            });

            return true;

         }

      }

      return ::user::impact::on_click(pitem, pmouse);

   }


   ::pointer < ::prodevian::immersion > impact::create_immersion()
   {

      auto pimmersion = create_newø<::app_graphics3d_hello_space::immersion>();

      return pimmersion;

   }


   ::pointer < ::graphics3d::key_map > impact::get_default_key_map()
   {

      auto pmap = create_newø < ::graphics3d::key_map>();
      using namespace ::graphics3d;

      pmap->map(e_key_moveLeft, ::user::e_key_a);
      pmap->map(e_key_moveRight, ::user::e_key_d);
      pmap->map(e_key_moveForward, ::user::e_key_w);
      pmap->map(e_key_moveBackward, ::user::e_key_s);
      pmap->map(e_key_moveUp, ::user::e_key_e);
      pmap->map(e_key_moveDown, ::user::e_key_q);
      pmap->map(e_key_lookLeft, ::user::e_key_left);
      pmap->map(e_key_lookRight, ::user::e_key_right);
      pmap->map(e_key_lookUp, ::user::e_key_up);
      pmap->map(e_key_lookDown, ::user::e_key_down);
      pmap->map(e_key_Exit, ::user::e_key_escape);

      return pmap;

   }



} // namespace app_graphics3d_hello_space



