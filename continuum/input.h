// From mnk_controller from V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 <3ThomasBorregaardSorensen!!
#pragma once 
#include "bred/graphics3d/input.h"
#include "acme/constant/user_key.h"
#include "bred/graphics3d/key_map.h"
//


namespace app_graphics3d_continuum
{


   class CLASS_DECL_APP_GRAPHICS3D_CONTINUUM input :
//      virtual public input_t,
      virtual public ::graphics3d::input
   {
   public:



      input();
      ~input() override;


   //   void initialize_input(
   //      const f32_angle & angleCursorPixel = 0.1f_degrees, 
   //      const f32_angle & angleYaw = -90f_degrees,
   //      const f32_angle & anglePitch = 0f_degrees);

   //   //void moveInPlaneXZ(::user::graphics3d * pimpact, float dt, application_object& gameObject);

   //   void prepare_mouse_input() override;
   //   void process_mouse_input() override;
   //   void process_keyboard_input() override;


   //   //void _001OnMouseMove(const ::i32_point & point) override;
   //   //void _001OnMouseOut() override;
   //   void _001PrepareMouseInput();
   //   void process_mouse_input_updateLook();
   //   void _001UpdateLook();

   //   //void processMouseMovement(float xOffset, float yOffset);
   //   void processKeyboardInput(float deltaTime);
   //   //void handleMouseInput();

   //   void process_keyboard_input_updateMovement();
   //   void process_keyboard_input_spaceExplorer();

   //   void reset_mouse_last_position();
   //
   //   floating_sequence3 getCameraDirection() const;
   //   floating_sequence3 getCameraPosition() const;


   //   //::pointer<::user::graphics3d > m_pusergraphics3d;
   //   //::pointer<::graphics3d::engine_instance > m_pgraphics3dengineinstance;
   //   //::pointer<::graphics3d::key_map > m_pkeymap;



   //   //public:
   //       //double lastX = 400.0;
   //       //double lastY = 300.0;
   //       //bool firstMouse = true;

   ////public:



   //   // Existing functions...
   //   //void handleMouseInput(GLFWwindow* window);

   //   //float getX() const { return (float) m_Δx; }
   //   //float getY() const { return (float) m_Δy; }

   //         //::block as_block() override;

   };


}  // namespace app_graphics3d_continuum



