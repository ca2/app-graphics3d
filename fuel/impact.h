// Created by camilo on 2026-11-01 00:46 <3ThomasBorregaardSorensen!! Mummi! bilbo!
#pragma once


#include "apex/platform/app_consumer.h"
#include "aura/user/user/form_callback.h"
#include "berg/user/user/impact.h"
#include "app-graphics3d/fuel/application.h"

namespace app_graphics3d_fuel
{


   class CLASS_DECL_APP_GRAPHICS3D_FUEL impact :
      virtual public ::app_consumer < application, ::user::impact >
   {
   public:
      ::write_text::font_pointer       m_pfontThomasBS_;
      string                           m_strFont1;
      int                              m_iSequence;



      impact();
      ~impact() override;


#ifdef _DEBUG

      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;

#endif

      //::pointer < ::graphics3d::application > start_graphics3d_application() override;


      virtual void install_message_routing(::channel * psender) override;

      virtual void _001OnClip(::draw2d::graphics_pointer & pdraw2dgraphics) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pdraw2dgraphics) override;

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      document * get_document();


      //s::block global_ubo_block() override;

      void on_layout(::draw2d::graphics_pointer & pdraw2dgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);
      //void on_load_engine() override;

      bool on_click(::item * pitem, ::user::mouse * pmouse) override;

      //bool is_absolute_mouse_position() override;

      //::pointer < ::prodevian::immersion > create_immersion() override;

      //void on_load_engine() override;

      //::pointer < ::graphics3d::key_map > get_default_key_map() override;
   };
}
