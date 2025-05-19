#pragma once

//#include <GLFW/glfw3.h>
#include "Core/gl_container.h"


namespace graphics3d_opengl
{

   class Window :
      virtual public ::particle
   {
   public:


      Window(const char* title, int width, int height, glc::GlContainer* pimpact);
      ~Window();

      bool ShouldClose() const;
      void SwapBuffers() const;
      void PollEvents() const;

      glc::GlContainer* GetGlContainer() const { return m_pimpact; }

   //private:
      ::pointer < glc::GlContainer > m_pimpact;
      int m_Width, m_Height;
      bool m_bShouldClose;
   };



} // namespace graphics3d_opengl



