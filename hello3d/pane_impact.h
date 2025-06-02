#pragma once


#include "core/user/userex/pane_tab_impact.h"
#include "aura/user/user/form_callback.h"
#include "application.h"


namespace graphics3d_hello3d
{


   class CLASS_DECL_APP_COMPLEX_HELLO3D pane_impact :
      virtual public ::app_consumer < application, ::userex::pane_tab_impact, ::user::form_callback >
   {
   public:


      ::user::split_impact *                      m_ptopviewLast;
      string_map <::pointer<impact> >           m_mapimpact;
      string                                    m_strTopicTitle;


      ::pointer<::user::document>               m_pdocMenu;


      pane_impact();
      ~pane_impact() override;


      void on_create_impact(::user::impact_data * pimpactdata) override;

      void install_message_routing(::channel * pchannel) override;

      // void assert_ok() const override;

      // void dump(dump_context & dumpcontext) const override;


      DECLARE_MESSAGE_HANDLER(on_message_create);
      bool BaseOnControlEvent(::user::form_window * pimpact,::user::control_event * phappening);
      void on_change_cur_sel() override;

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      virtual void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      //void on_change_cur_sel() override;


   };


} // namespace graphics3d_hello3d













