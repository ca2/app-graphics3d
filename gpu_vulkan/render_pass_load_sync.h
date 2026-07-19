#pragma once


#include <vulkan/vulkan.h>


namespace gpu_vulkan
{


   constexpr VkAccessFlags color_attachment_access(bool bLoadExisting) noexcept
   {

      return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
         | (bLoadExisting ? VK_ACCESS_COLOR_ATTACHMENT_READ_BIT : 0);

   }


   constexpr VkAccessFlags depth_attachment_access(bool bLoadExisting) noexcept
   {

      return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
         | (bLoadExisting ? VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT : 0);

   }


} // namespace gpu_vulkan
