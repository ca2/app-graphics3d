// Created by camilo on 2023-04-27 17:06 <3ThomasBorregaardSorensen!!
#pragma once




#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"

#if defined(WINDOWS_DESKTOP)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(LINUX)
#define VK_USE_PLATFORM_XLIB_KHR
#else
#error "wwwwwhat?!?!"
#endif


#include <vulkan/vulkan.h>



#include "graphics.h"


#define VK2D_GRAPHICS(pdraw2dgraphics) (dynamic_cast < ::draw2d_vulkan::graphics * > (pdraw2dgraphics))
#define VK2D_HDC(pdraw2dgraphics) (VK2D_GRAPHICS(pdraw2dgraphics)->m_hdc)


namespace vulkan
{


   //https://community.khronos.org/t/draw-an-arc-in-vulkan/57994/2
   void draw_arc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments);

} // namespace vulkan



namespace draw2d_vulkan
{


   class draw2d;


} // namespace draw2d_vulkan



