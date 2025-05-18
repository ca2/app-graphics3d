#pragma once


#include "base/user/user/impact.h"
#include "app-cube/cube/application.h"
#include "app-cube/cube/graphics3d/engine.h"
#include "app-cube/cube/graphics3d/key_map.h"
#include "app-cube/cube/graphics3d/types.h"
#include "apex/platform/app_consumer.h"



namespace cube
{


   class CLASS_DECL_CUBE impact :
      virtual public ::app_consumer < application, ::user::impact>
   {
   public:

      //memory m_memory;
      //int m_i;

      ::pointer < ::graphics3d::engine >			m_pengine;

      ::pointer < ::graphics3d::key_map >			m_pkeymap;
      ::task_pointer                            m_ptaskEngine;


      ::function < void(void*, int, int, int)>  m_callbackOffscreen;


      ::graphics3d::mouse_state                 m_mousestate;
      ::image::image_pointer                    m_pimage;

      ::particle_pointer                        m_pparticleImageSynchronization;

      //::function < void(void*, int, int, int)> m_callbackOffscreen;

      double   m_dCursorX;
      double   m_dCursorY;
      bool		m_bFirstMouse;
      bool		m_bLastMouse;


      bool		m_bShouldClose;
      int		m_iWidth;
      int		m_iHeight;
      bool		m_bFrameBufferResized;




      impact();
      ~impact() override;

//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;
      //::int_size size() override;



      void install_message_routing(::channel * pchannel) override;

      virtual ::user::enum_key_state get_key_state(::user::e_key ekey) ;
      void _001OnNcClip(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnClip(::draw2d::graphics_pointer & pgraphics) override;


      void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnNcPostDraw(::draw2d::graphics_pointer & pgraphics) override;


      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      ::user::document * get_document();


      //virtual ::pointer < ::cube::application > start_graphics3d_application();


      void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);
      DECLARE_MESSAGE_HANDLER(on_message_key_down);
      DECLARE_MESSAGE_HANDLER(on_message_key_up);
      DECLARE_MESSAGE_HANDLER(on_message_mouse_move);
      DECLARE_MESSAGE_HANDLER(on_message_left_button_down);
      DECLARE_MESSAGE_HANDLER(on_message_left_button_up);
      DECLARE_MESSAGE_HANDLER(on_message_mouse_leave);

      void on_timer(::timer * ptimer) override;

      void reset_mouse_last_position();


      long long increment_reference_count() override
      {
         return ::object::increment_reference_count();
      }
      long long decrement_reference_count() override
      {
         return ::object::decrement_reference_count();
      }
      bool keyboard_focus_is_focusable() override;


      virtual bool is_absolute_mouse_position();

      virtual bool shouldClose();
      //virtual ::int_size size();
      virtual bool wasWindowResized();
      virtual void resetWindowResizedFlag();
//
  //    virtual ::user::enum_key_state get_key_state(::user::e_key ekey);

      //virtual void initWindow();


      virtual void defer_initialize_engine();

      virtual void on_load_engine();


      template < typename SCENE >
      ::pointer < SCENE> add_scene(const ::scoped_string& scopedstrName)
      {

         auto pscene = __create_new < SCENE>();

         pscene->m_strName = scopedstrName;

         pscene->m_pimpact = this;

         m_pengine->add_scene(pscene);

         return pscene;

      }


      virtual ::pointer < ::graphics3d::key_map > get_default_key_map();

   };


} // namespace cube



