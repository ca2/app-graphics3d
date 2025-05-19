// Created by camilo on 2025-05-12 20:53 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "gl_container.h"


namespace glc
{


   GlContainer::GlContainer()
   {

      m_iWidth = 0;
      m_iHeight = 0;

   }


   GlContainer::~GlContainer()
   {


   }


   bool GlContainer::is_absolute_mouse_position()
   {


      return true;

   }

   ::pointer < ::opengl::application> GlContainer::start_opengl_application()
   {

      return {};

   }




} // namespace glc



