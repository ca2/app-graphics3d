#pragma once


#include "aura/user/user/graphics3d.h"
#include "base/user/user/show.h"
#include "app-graphics3d/continuum/global_ubo.h"


namespace app_graphics3d_continuum
{


   class CLASS_DECL_APP_GRAPHICS3D_CONTINUUM impact :
      virtual public ::app_consumer < application, ::user::show <::user::graphics3d> >
   {
   public:


      ::write_text::font_pointer       m_pfontThomasBS_;
      string                           m_strFont1;
      int                              m_iSequence;
      GlobalUbo                        m_globalubo;


      impact();
      ~impact() override;


#ifdef _DEBUG

      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;

#endif

      //::pointer < ::graphics3d::application > start_graphics3d_application() override;


      virtual void install_message_routing(::channel * psender) override;

      virtual void _001OnClip(::draw2d::graphics_pointer & pgraphics) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      document * get_document();


      ::block global_ubo_block() override;

      void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);
      void on_load_engine() override;
      
      bool on_click(::item * pitem, ::user::mouse * pmouse) override;
      
      //bool is_absolute_mouse_position() override;

      //void on_load_engine() override;

      ::pointer < ::graphics3d::key_map > get_default_key_map() override;


   };


} // namespace app_graphics3d_continuum


