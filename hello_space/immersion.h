//
// Created by camilo on 2025-08-23 03:26 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "app-graphics3d/continuum/immersion.h"


namespace app_graphics3d_hello_space
{

   class CLASS_DECL_APP_GRAPHICS3D_HELLO_SPACE immersion : virtual public ::app_graphics3d_continuum::immersion
   {
   public:

      ///::app_graphics3d_hello_space::enum_mouse m_emouse;
      //::app_graphics3d_hello_space::enum_keyboard m_ekeyboard;


      immersion();
      ~immersion() override;

      ::pointer<::graphics3d::scene_base> create_main_scene() override;

      //void on_pre_update_camera() override;


   };

} // namespace app_graphics3d_hello_space
