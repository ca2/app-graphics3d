#pragma once


#include "apex/platform/app_consumer.h"
#include "base/user/user/impact.h"


namespace app_simple_shader
{


   class CLASS_DECL_APP_SIMPLE_SHADER impact :
      virtual public ::app_consumer < application, ::user::impact >
   {
   public:


      int                                 m_iImpact;
      ::pointer<::app_shader::render>     m_prender;
      bool                                m_bSaveFrame;


      impact();
      virtual ~impact();

//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;


#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif


      virtual void install_message_routing(::channel * psender) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      ::user::document * get_document();

      virtual void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);
      DECLARE_MESSAGE_HANDLER(on_message_key_down);

      virtual bool keyboard_focus_is_focusable() override;

   };


} // namespace simple_shader


