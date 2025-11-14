// From mnk_controller from V0idsEmbrace@Twich continuum project
// by camilo on 2025-05-18 <3ThomasBorregaardSorensen!!
#pragma once 
#include "bred/graphics3d/input.h"
#include "acme/constant/user_key.h"
#include "bred/graphics3d/key_map.h"
#include <glm/glm.hpp>


namespace app_graphics3d_continuum
{


   struct input_t 
   {
      //KeyMappings keys{};
      bool   m_bMouseAbsolute;

      double m_dMouseLastX = 0.0;
      double m_dMouseLastY = 0.0;
      double m_Δx = 0.;
      double m_Δy = 0.;

      bool m_bWasLeft = false;


      double   m_dCursorX;
      double   m_dCursorY;
      bool		m_bFirstMouse;
      bool		m_bLastMouse;

      float    m_fMoveSpeed{ 3.f };
      float    m_fLookSpeed{ 1.5f };

      bool  m_b_001AbsoluteMousePosition;
      float m_f_001UpdateLookSensitivity;  // Sensitivity factor for mouse movement
      float _yaw;          // Camera yaw (horizontal rotation)
      float _pitch;        // Camera pitch (vertical rotation)
      floating_sequence3 _cameraDirection; // Camera direction vector
      floating_sequence3 _cameraPosition;
      ::block as_block() { return as_memory_block(*this); }

   };



   class CLASS_DECL_APP_GRAPHICS3D_CONTINUUM input :
      virtual public input_t,
      virtual public ::graphics3d::input
   {
   public:



      input();
      ~input() override;


      void initialize_input(float sensitivity = 0.001f, float yaw = glm::radians(-90.0f),
                            float pitch = glm::radians(0.0f));

      //void moveInPlaneXZ(::user::graphics3d * pimpact, float dt, application_object& gameObject);

      void prepare_mouse_input() override;
      void process_mouse_input() override;
      void process_keyboard_input() override;


      void _001OnMouseMove(const ::int_point & point) override;
      void _001OnMouseOut() override;
      void _001PrepareMouseInput();
      void process_mouse_input_updateLook();
      void _001UpdateLook();

      //void processMouseMovement(float xOffset, float yOffset);
      void processKeyboardInput(float deltaTime);
      //void handleMouseInput();

      void process_keyboard_input_updateMovement();
      void process_keyboard_input_spaceExplorer();

      void reset_mouse_last_position();
   
      floating_sequence3 getCameraDirection() const;
      floating_sequence3 getCameraPosition() const;


      //::pointer<::user::graphics3d > m_pusergraphics3d;
      //::pointer<::graphics3d::engine > m_pengine;
      //::pointer<::graphics3d::key_map > m_pkeymap;



      //public:
          //double lastX = 400.0;
          //double lastY = 300.0;
          //bool firstMouse = true;

   //public:



      // Existing functions...
      //void handleMouseInput(GLFWwindow* window);

      float getX() const { return (float) m_Δx; }
      float getY() const { return (float) m_Δy; }

            ::block as_block() override;

   };


}  // namespace app_graphics3d_continuum



