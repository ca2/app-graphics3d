// Created by camilo on 2025-05-25 23:04 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "approach.h"
#include "physical_device.h"
#include "windowing_x11/_.h"
#include "windowing_x11/window.h"


namespace gpu_vulkan
{


   VkResult physical_device::createWindowSurface(::windowing::window* pwindowParam)
   {

      if (m_vksurfacekhr)
      {

         return VK_SUCCESS;

      }

      ::cast <::gpu_vulkan::approach > pgpu = m_pgpuapproach;

      ::cast < ::windowing_x11::window > pwindow = pwindowParam;
      // Surface creation
      VkSurfaceKHR surface;
      VkXlibSurfaceCreateInfoKHR surfaceCreateInfo = {
          .sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
          //.hinstance = (HINSTANCE)::system()->m_hinstanceThis,
          //.hwnd = pwindow->m_hwnd
         .dpy = (Display *) pwindow->__x11_Display(),
         .window = (Window) pwindow->__x11_Window()
      };
      auto result = vkCreateXlibSurfaceKHR(pgpu->m_vkinstance, &surfaceCreateInfo, NULL, &m_vksurfacekhr);

      if (result != VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create vulkan KHR surface");

      }

      return result;

   }


} // namespace gpu_vulkan


