//
// Created by camilo on 2025-08-23 03:26 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "bred/prodevian/immersion.h"


namespace app_graphics3d_continuum
{

class CLASS_DECL_APP_GRAPHICS3D_CONTINUUM immersion :
   virtual public ::prodevian::immersion
{
public:
immersion();
   ~immersion() override;

   ::pointer < ::graphics3d::scene>create_main_scene() override;

};

} // app_graphics3d_continuum

