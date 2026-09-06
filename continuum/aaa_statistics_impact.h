// Created by camilo on 2026-09-05 03:22 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "bred/user/user/graphics3d.h"
#include "berg/user/user/show.h"
#include "bred/graphics3d/global_ubo1.h"


namespace app_graphics3d_continuum
{


   class CLASS_DECL_APP_GRAPHICS3D_CONTINUUM statistics_impact :
      virtual public ::app_consumer < application, ::user::show <::user::box> >
   {
   public:


      ::write_text::font_pointer       m_pfontThomasBS_;
      string                           m_strFont1;
      int                              m_iSequence;



      statistics_impact();
      ~statistics_impact() override;


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

      bool on_click(::item * pitem, ::user::mouse * pmouse) override;


   };




} // namespace app_graphics3d_continuum


