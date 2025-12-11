// Created by camilo on 2025-05-12 20:51 <3ThomasBorregaardSorensen!!
#pragma once

namespace opengl
{

   class application;

} // namespace opengl



namespace glc
{


   class CLASS_DECL_OPENGL_GRAPHICS3D_MYPLACE2025 GlContainer :
      virtual public ::particle
   {
   public:


      int m_iWidth;
      int m_iHeight;

      ::function < void(void*, int, int, int)> m_callbackOffscreen;

      double   m_dCursorX;
      double   m_dCursorY;
      bool		m_bFirstMouse;
      bool		m_bLastMouse;


      bool		m_bShouldClose;

      GlContainer();
      ~GlContainer() override;

      virtual bool is_absolute_mouse_position();


      virtual ::pointer < ::opengl::application> start_opengl_application();


   };


} // namespace glc



