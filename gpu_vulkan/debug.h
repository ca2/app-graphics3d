/*
 * Vulkan examples debug wrapper
 *
 * Copyright (C) 2016-2023 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#include "vulkan/vulkan.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#define GLM_FORCE_LEFT_HANDED  // Optional — depends on your conventions
#include <glm/glm.hpp>

namespace vulkan
{
   namespace debug
   {
      // Default debug callback
      VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsMessageCallback(
         VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
         VkDebugUtilsMessageTypeFlagsEXT messageType,
         const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
         void * pUserData);

      // Load debug function pointers and set debug callback
      void setupDebugging(VkInstance instance);
      // Clear debug callback
      void freeDebugCallback(VkInstance instance);
      // Used to populate a VkDebugUtilsMessengerCreateInfoEXT with our example messenger function and desired flags
      void setupDebugingMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT & debugUtilsMessengerCI);
   }

   // Wrapper for the VK_EXT_debug_utils extension
   // These can be used to name Vulkan objects for debugging tools like RenderDoc
   namespace debugutils
   {
      void setup(VkInstance instance);
      void cmdBeginLabel(VkCommandBuffer cmdbuffer, ::string caption, glm::vec4 color);
      void cmdEndLabel(VkCommandBuffer cmdbuffer);
   }
} // namespace debug
