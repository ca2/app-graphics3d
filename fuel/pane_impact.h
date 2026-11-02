// Created by camilo on 2026-11-01 00:41 <3ThomasBorregaardSorensen!! Mummi! bilbo!
#pragma once


#include "apex/platform/app_consumer.h"
#include "aura/user/user/form_callback.h"
#include "core/user/userex/pane_tab_impact.h"
#include "app-graphics3d/fuel/application.h"


namespace app_graphics3d_fuel
{
   class CLASS_DECL_APP_GRAPHICS3D_FUEL pane_impact :
      virtual public ::app_consumer < application, ::userex::pane_tab_impact, ::user::form_callback >
   {
   public:
      pane_impact();
      ~pane_impact() override;


      void on_create_impact(::user::impact_data * pimpactdata) override;

      void install_message_routing(::channel * pchannel) override;

      // void assert_ok() const override;

      // void dump(dump_context & dumpgpucontext) const override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;
      void on_change_cur_sel() override;

      //virtual string prepare_menu_impact();

      ///void handle(::topic * ptopic, ::handler_context * phandlercontext) override;


      virtual void _001OnNcDraw(::draw2d::graphics_pointer & pdraw2dgraphics) override;
      virtual void _001OnDraw(::draw2d::graphics_pointer & pdraw2dgraphics) override;

      virtual string get_impact_options_main_body();
      virtual string get_bilbo_html();


      //void create_options_body(::user::interaction * pparent);
   };
}
