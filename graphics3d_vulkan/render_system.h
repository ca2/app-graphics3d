// Created by camilo on 2025-04-24 21:26 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/graphics3d/render_system.h"


namespace graphics3d_vulkan
{


   class render_system : 
         virtual public ::graphics3d::render_system
   {
   public:


      render_system();
      ~render_system();


      virtual ::gpu_vulkan::render_pass *render_pass2();


   };


} // namespace graphics3d_vulkan
 




