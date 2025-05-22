#pragma once


#include "apex/platform/app_consumer.h"
#include "aura/user/user/form_callback.h"
#include "base/user/user/tab_impact.h"


namespace app_simple_shader
{


   class CLASS_DECL_APP_SIMPLE_SHADER tab_impact :
      virtual public ::app_consumer < application, ::user::tab_impact, ::user::form_callback >
   {
   public:


      ::user::split_impact *                            m_ptopviewLast;
      ::pointer<impact>                               m_pviewTopic;
      string                                          m_strTopicTitle;


      ::pointer<::user::document>                            m_pdocMenu;


      tab_impact();
      virtual ~tab_impact();


      void on_create_impact(::user::impact_data * pimpactdata) override;

      void install_message_routing(::channel * pchannel) override;

      // void assert_ok() const override;

      // void dump(dump_context & dumpcontext) const override;


      DECLARE_MESSAGE_HANDLER(on_message_create);
      bool BaseOnControlEvent(::user::form_window * pimpact,::user::control_event * phappening);
      void on_change_cur_sel() override;

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      //void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      virtual void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      //void on_change_cur_sel() override;


   };


} // namespace simple_shader













