//
// Created by camilo on 2025-08-23 03:27 <3ThomasBorregaardSorensen!!
//
#include "framework.h"
#include "application.h"
#include "camera.h"
#include "immersion.h"
#include "input.h"
#include "main_scene.h"
#include "bred/user/user/graphics3d.h"


namespace app_graphics3d_continuum
{


   immersion::immersion()
   {
      //m_emouse = ::graphics3d::e_mouse_updateLook;
      m_emouse = ::app_graphics3d_continuum::e_mouse_updateLook;
      m_ekeyboard = ::app_graphics3d_continuum::e_keyboard_updateMovement;

   }


   immersion::~immersion()
   {


   }


   ::pointer<::graphics3d::scene_base> immersion::create_main_scene()
   {

      auto pinput = create_newø < ::app_graphics3d_continuum::input>();

      pinput->initialize_input();

      //pinput->m_fMoveSpeed = 3.f;
      //pinput->m_fLookSpeed = 1.5f;

      m_pengine->m_pinput = pinput;

      //m_pengine->m_pinput->m_pusergraphics3d = m_pengine->m_pusergraphics3d;
      pinput->m_pengine = m_pengine;
      //m_pengine->m_pinput->m_pkeymap = m_pengine->m_pusergraphics3d->m_pkeymap;

      auto psceneMain = create_scene<main_scene>("main");

      return psceneMain;

   }


   void immersion::on_pre_update_camera()
   {

      auto pcameraScene = m_pscene->m_pcameraScene;

      if (pcameraScene)
      {

         ::cast<::app_graphics3d_continuum::camera> pcamera = pcameraScene;

         m_pengine->m_transform.m_sequence3Position = pcamera->m_sequence3Position;

         m_pengine->m_transform.m_rotation = pcamera->m_rotation;

         //m_pengine->m_transform.m_quaternionRotation.y = pcamera->m_angleYaw;

      }


   }





} // app_graphics3d_continuum
