#pragma once


#include "application.h"
#include "acme/prototype/geometry2d/angle.h"
#include "acme/prototype/geometry3d/location.h"
#include "acme/prototype/time/frequency.h"
#include "apex/platform/app_consumer.h"
#include "base/user/user/impact.h"


namespace graphics3d_impact3d
{


   class CLASS_DECL_APP_COMPLEX_IMPACT3D impact :
      virtual public app_consumer < application, ::user::impact >
   {
   public:


      int                     m_iView;
      bool                    m_bSaveFrame;

      class ::time            m_timeLastDraw;
      // Initial position : on +Z
      //::locationd             m_locationglm::vec3 position = glm::vec3(0, 0, 5);
      ::location_double          m_location{0., 0., 5.};
      // Initial horizontal angle : toward -Z
      //float horizontalAngle = 3.14f;
      //::double_angle             m_angleHorizontal{ π };
       ::double_angle             m_angleHorizontal{ radians(MATH_PI) };
      //::angle                 m_angleHorizontal{ π };
      // Initial vertical angle : none
      ::double_angle             m_angleVertical{radians(0.)};
      //::angle                 m_angleImpact{ π / 4.0 };
      //::double_angle             m_angleImpact{ π / 4.0 };
       ::double_angle             m_angleImpact{radians(MATH_PI / 4.0) };
      // Initial Field of View
      //float initialFoV = 45.0f;

      ::frequency             m_frequencySpeed{ 3.0_Hz }; // Hz
      double                  m_doubleRatSpeedHpp{ 0.005 };

      class ::time            m_timeDelta;

      ::double_point             m_pointLastMouse;
      ::double_size              m_sizeMouseDelta;


      impact();
      ~impact() override;


//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;


#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif


      virtual void install_message_routing(::channel * psender) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      document * get_document();


      ::pole_double camera_position();

      ::pole_double right();

      void compute_matrices();
         

      virtual void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);
      DECLARE_MESSAGE_HANDLER(on_message_key_down);
      DECLARE_MESSAGE_HANDLER(on_message_mouse_move);

      virtual bool keyboard_focus_is_focusable() override;


   };


} // namespace graphics3d_impact3d


