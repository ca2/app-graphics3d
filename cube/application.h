// Created by camilo on 2025-05-16 23:15 <3ThomasBorregaardSorensen!!
#pragma once


#include "core/platform/application.h"
#include "app-cube/cube/camera.h"


namespace cube
{


   class CLASS_DECL_CUBE application :
      virtual public ::core::application
   {
   public:

      
      int m_iWidth;

      int m_iHeight;

      ::pointer<impact> m_pimpact;

      ::task_pointer m_ptask3dApp;


      ::function < void(void*, int, int, int)> m_callbackOffscreen;


      application();
      ~application();


      virtual ::cube::camera get_default_camera();


      virtual ::file::path assets_folder();

      virtual ::file::path shaders_folder();


      //virtual void initialize_cube(::cube::impact* pimpact);

      //virtual void run_application();

      //virtual void on_layout_cube(int cx, int cy);


   };


} // namespace cube



