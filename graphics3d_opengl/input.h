#pragma once

//#include "GLFW/glfw3.h"
//#include "Core/gl_container.h"
//#include "opengl-graphics3d/graphics3d_opengl/types.h"
//#include "acme/constant/user_key.h"
#include "aura/graphics/graphics3d/input.h"
//#include <unordered_map>

//
//
//namespace graphics3d_opengl
//{
//
//
//   class input :
//      virtual public ::particle
//   {
//   public:
//
//      ::pointer < ::user::graphics3d> m_pimpact;
//      bool m_IsFullscreen;
//
//      //::map<::user::e_key, bool> m_KeyPressed; // Store the state of each key
//      //static int m_WindowedWidth, m_WindowedHeight;
//      //static int m_WindowedPosX, m_WindowedPosY;
//
//      input(::user::graphics3d* pimpact);
//      ~input() override;
//
//      ::user::enum_key_state  get_key_state(::user::e_key ekey);
//      bool IsKeyPressed(::user::e_key ekey); // New method for released keys
//      bool IsKeyReleased(::user::e_key ekey); // New method for released keys
//      //void SetGLFWWindow(::user::graphics3d * pimpact);
//      void ToggleFullscreen();  // Fullscreen toggle logic
//      void Update(); // Update key states each frame
//      bool IsAnyKeyPressed(); // New method to check if any keys are pressed
//   //private:
//
//      bool WasKeyPressed(::user::e_key ekey); // Helper to check if key was pressed once
//   };
//
//
//
//
//} // namespace graphics3d_opengl
//
//
