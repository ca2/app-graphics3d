#include "framework.h"
#include "impact.h"
#include "document.h"
#include "application.h"
#include "acme/constant/message.h"
#include "acme/filesystem/filesystem/file_system.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/node.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "apex/database/stream.h"
#include "aura/message/user.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "aura/graphics/graphics/graphics.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/drawing.h"
#include "aura/windowing/window.h"
#include "draw2d_opengl/_.h"
#include "draw2d_opengl/graphics.h"
#include "aura/graphics/graphics3d/types.h"


CLASS_DECL_AURA ::color::color dk_red(); // <3 tbs
//namespace opengl
//{
//
//   ::pointer<::opengl::engine > start_opengl_engine(::glc::GlContainer* pvkcontainer, mouseState* pmousestate);
//
//} // namespace vkc

//namespace glc
//{
//   
//   ::pointer<::opengl::application > start_opengl_application(::glc::GlContainer* pvkcontainer, mouseState* pmousestate);
//
//} // namespace glc


namespace graphics3d_MyPlace2025
{




   impact::impact()
   {

      m_ekeyboard = ::cube::e_keyboard_spaceExplorer;
      m_flagNonClient -= e_non_client_background;
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

      ::user::impact::install_message_routing(psender);

      MESSAGE_LINK(e_message_create,psender,this,&impact::on_message_create);
      MESSAGE_LINK(e_message_destroy, psender, this, &impact::on_message_destroy);
      MESSAGE_LINK(e_message_mouse_move, psender, this, &impact::on_message_mouse_move);
      MESSAGE_LINK(e_message_mouse_leave, psender, this, &impact::on_message_mouse_leave);

   }



   //::pointer < ::opengl::engine > impact::start_opengl_engine()
   //{

   //   auto pengine= ::opengl::start_opengl_engine(this, &m_mousestate);
   //   pengine->initialize_engine(this);
   //   auto pwindow = window();

   //   auto pgraphicsgraphics = pwindow->get_window_graphics();

   //   auto pbufferitem = pgraphicsgraphics->get_screen_item();

   //   auto pgraphics = pbufferitem->g();

   //   ::cast < ::draw2d_opengl::graphics > pgraphicsOpenGL = pgraphics;

   //   pengine->m_pgpucontext = pgraphicsOpenGL->m_pgpucontext;

   //   return pengine;

   //}



   //void impact::on_message_mouse_move(::message::message* pmessage)
   //{

   //   auto pmouse = pmessage->m_union.m_pmouse;

   //   pmessage->m_bRet = true;

   //   if (get_app()->m_popenglengine)
   //   {

   //      auto point = pmouse->m_pointHost;

   //      host_to_client()(point);

   //      //m_mousestate.position.x = point.x();
   //      //m_mousestate.position.y = point.y();
   //      //m_mousestate.buttons.left = true;
   //      //         pmouse->m_p

   //      double w = m_iWidth;
   //      double h = m_iHeight;

   //      if (m_bLastMouse)
   //      {
   //         m_bLastMouse = false;
   //         m_bFirstMouse = true;

   //      }

   //      if (is_absolute_mouse_position())
   //      {
   //         m_dCursorX = ((point.x() - (w / 2.0)) * 2.0);
   //         m_dCursorY = ((point.y() - (h / 2.0)) * 2.0);
   //      }
   //      else
   //      {

   //         m_dCursorX = point.x();
   //         m_dCursorY = point.y();

   //      }

   //      track_mouse_leave();

   //      get_app()->m_popenglengine->handleMouseMove(m_dCursorX, m_dCursorY);

   //   }

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


      m_pparticleImageSynchronization = node()->create_mutex();

      m_pimage = image()->create_image(int_size{ 1920, 1080 });

      get_app()->m_pimpact = this;


      //application()->show_about_box();
    

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


      //pgraphics->defer_add_gpu_render(get_app()->m_popenglengine);
      
      //return;

//      if (pgraphics->payload("set_transparent") == "set_transparent")
//      {
//
//         information() << "set_transparent called";
//
//      }
//      else
//      {
//
//         information() << "set_transparent NOT called!!";
//
//      }

      ::double_rectangle rectangleClipBox;

      //pgraphics->reset_clip();

      //pgraphics->get_clip_box(rectangleClipBox);

      auto matrix = pgraphics->m_matrix;

      //auto origin = pgraphics->origin();

      auto opacity = ::opacity(48);

      pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_blend);

      pgraphics->fill_rectangle(rectangleX, argb(108, 128, 128, 128));

      ::color::color color_dk(dk_red());

#ifdef DEBUG_WORK

      ::int_rectangle rectangleDryProWithLove_Work(5, 5, 1915, 1075);

      pgraphics->fill_rectangle(rectangleDryProWithLove_Work, argb(255, 150, 200, 255));

#endif

      pgraphics->set_text_color(color_dk);

      auto psystem = system();

      auto pnode = psystem->draw2d();

      auto pwritetext = pnode->write_text();

      auto fontsize = ::write_text::font_size(48.0 * rectangleX.height() / 1'080, e_unit_pixel);

      m_pfontThomasBS_ = pwritetext->font("Fira Code", fontsize);

      pgraphics->set(m_pfontThomasBS_);

      pgraphics->set_text_rendering_hint(write_text::e_rendering_anti_alias);

      pgraphics->set_alpha_mode(draw2d::e_alpha_mode_blend);

      _synchronous_lock synchronouslockDocument(get_document()->synchronization());
      
      string_array & stra = get_document()->m_straLine;
      
      bool bWhite  = true;
      
      double x = 0.;
      
      double y = 0.;
      
      ::int_point point;

      string strText;

      m_iSequence++;
      
      strText.formatf("psimpleapplication->m_pimpact->_001OnDraw(pgraphics); %d", m_iSequence);

      auto size = pgraphics->get_text_extent(strText);

      bool bFixedPosition = true;

      if (bFixedPosition)
      {

         point = { 10, 10 };

      }
      else
      {
       
         point.x() = mathematics()->random(0, (int)(rectangleX.width() - size.cx()));
         point.y() = mathematics()->random(0, (int)(rectangleX.height() - size.cy() * 2));

      }

      ::color::color color;

      opacity = ::opacity(96);

      for(auto & strItem : stra)
      {
         
         if(strItem.is_empty())
         {
            
            x = 0;
            
            y += size.cy();
            
            bWhite = true;
         
            continue;
         
         }

         if(bWhite)
         {
            
            color = color::white;
            
         }
         else
         {
            
            color = color_dk;
            
         }

         color &= opacity;

         pgraphics->set_text_color(color);

         pgraphics->text_out(point.x() + x, point.y() + y, strItem);
         
         auto s = pgraphics->get_text_extent(strItem);

         x += s.cx();
         
         bWhite = !bWhite;
         
      }

      color = color_dk;
      
      color &= opacity;

      pgraphics->set_text_color(color);

      pgraphics->text_out(point.x(), point.y() + y + size.cy(), strText);

      pgraphics->set_smooth_mode(::draw2d::e_smooth_mode_none);
      //::user::impact::_001OnDraw(pgraphics);


      if (::is_ok(m_pimage))
      {

         pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_blend);

         _synchronous_lock synchronouslock(m_pparticleImageSynchronization);

         ::image::image_source imagesource(m_pimage, m_pimage->rectangle());

         ::image::image_drawing_options imagedrawingoptions(m_pimage->rectangle());

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pgraphics->draw(imagedrawing);

      }

      //auto papp = get_app();

      //if (papp)
      //{

      //    auto pengine = get_app()->m_popenglengine;

      //    if(pengine)
      //    { 

      //        auto r = host_rectangle();

      //        pengine->m_rectangle = r;

      //        pengine->m_sizeHost = top_level()->raw_rectangle().size();
      //    
      //        pengine->render_step();
      //    }
      //}

      ////get_app()->m_popenglengine->render_step();

      ::user::graphics3d::_001OnDraw(pgraphics);

   }


   //void impact::on_message_mouse_leave(::message::message* pmessage)
   //{

   //   //if (is_absolute_mouse_position())
   //   {

   //      reset_mouse_last_position();

   //   }

   //}


   //void impact::reset_mouse_last_position()
   //{

   //   if (is_absolute_mouse_position())
   //   {
   //      m_dMouseLastX = 0.;
   //      m_dMouseLastY = 0.;
   //   }
   //   m_bLastMouse = true;

   //}

   void impact::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      if(rectangleX.is_empty())
      {

         return;

      }

      ::user::graphics3d::on_layout(pgraphics);

      //auto iWidth = rectangleX.width();
      //auto iHeight = rectangleX.height();

      //get_app()->update_3d_application(iWidth, iHeight);

      ////m_iWidth = rectangleX.width();
      ////m_iHeight = rectangleX.height();

      //reset_mouse_last_position();

      //::user::impact::on_layout(pgraphics);
      //
      //setup_default_client_area_user_item();

   }


   //::pointer < ::opengl::application> impact::start_opengl_application()
   //{

   //   return ::glc::start_opengl_application(this, &m_mousestate);

   //}


   //void impact::reset_mouse_last_position()
   //{

   //   if (is_absolute_mouse_position())
   //   {
   //      
   //      m_dCursorX = 0.;
   //      m_dCursorY = 0.;

   //   }

   //   m_bLastMouse = true;

   //}



   bool impact::on_click(::item * pitem, ::user::mouse * pmouse)
   {

      if (::is_set(pitem))
      {

         if (pitem->m_item.m_eelement == ::e_element_client)
         {

            print_line("on_click : e_element_client");
            
            ::file::file_dialog_filter filedialogfilter;
            
            filedialogfilter.add({"application.txt", "application.txt"});
            
            pick_single_file_to_open(filedialogfilter,
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
                  pmessagebox->async();
               }
               
            });

            return true;

         }

      }

      return ::user::impact::on_click(pitem, pmouse);

   }


   //bool impact::is_absolute_mouse_position()
   //{

   //   return m_bAbsoluteMousePosition;

   //}

   ::pointer < ::graphics3d::key_map > impact::get_default_key_map()
   {

      auto pmap = __create_new < ::graphics3d::key_map>();
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


      //)->IsKeyPressed(::user::e_key_w


} // namespace graphics3d_MyPlace2025



